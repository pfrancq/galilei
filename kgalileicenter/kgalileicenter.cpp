/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <pwd.h>
#include <unistd.h>
#include <math.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <glang.h>
#include <gmetaengine.h>
#include <gtool.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QMdiSubWindow>
#include <QInputDialog>
#include <kstatusbar.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <KDE/KLocale>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kactionmenu.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kgalileicenter.h>
#include <qsessionprogress.h>
#include <configure.h>
#include <preferences.h>
#include <kviewdoc.h>
#include <kviewmetaengine.h>
#include <kviewprg.h>
#include <qfilldatabase.h>
#include <kviewstats.h>
#include <kviewgroup.h>
#include <kviewgroups.h>
#include <kviewusers.h>
#include <kviewclasses.h>
#include <kviewidealgroups.h>
#include <kviewprofile.h>
#include <kviewdicts.h>
#include <qcreatedatabase.h>
#include <kprgconsole.h>
#include <ui_qchooselist.h>
//#include <ui_mainconfigure.h>



//-----------------------------------------------------------------------------
//
// class KGALILEICenter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KGALILEICenter* KGALILEICenter::App(0);

//-----------------------------------------------------------------------------
KGALILEICenter::KGALILEICenter(int argc, char *argv[])
	: KXmlGuiWindow(0), GGALILEIApp("KGALILEICenter",argc,argv,true),
	  Desktop(new QMdiArea(this)), Status(new QLabel(statusBar())), Doc(0), Debug(0)
{
    setAcceptDrops(true);
    setCentralWidget(Desktop);

    initActions();

	Status->setPixmap(KIconLoader::global()->loadIcon("network-disconnect",KIconLoader::Small));
	statusBar()->insertWidget(0,Status);
	statusBar()->insertItem(i18n("Ready."),1);
    statusBar()->show();
    setupGUI();
    readOptions();
	sessionConnected(false);
	App=this;
}


//-----------------------------------------------------------------------------
void KGALILEICenter::Init(void)
{
	GGALILEIApp::Init();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::Run(void)
{
	show();
}


//-----------------------------------------------------------------------------
KAction* KGALILEICenter::addAction(const char* title,const char* name,const char* slot,const char* icon,const char* key)
{
	KAction* ptr(new KAction(i18n(title),this));
	if(icon)
		ptr->setIcon(KIcon(icon));
	if(key)
		ptr->setShortcut(QKeySequence(tr(key)));
	actionCollection()->addAction(QLatin1String(name),ptr);
	connect(ptr,SIGNAL(triggered(bool)),this,slot);
	return(ptr);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::initActions(void)
{
	// Menu "Connect"
	aSessionSave=new KToggleAction(i18n("Save Results"),this);
	actionCollection()->addAction(QLatin1String("sessionSave"),aSessionSave);
	connect(aSessionSave,SIGNAL(toggled(bool)),this,SLOT(saveModifier()));
	aSessionConnect=addAction("&Connect","sessionConnect",SLOT(sessionConnect()),"network-connect","Ctrl+C");
	Actions.insert(Actions.size(),addAction("&Disconnect","sessionDisconnect",SLOT(sessionDisconnect()),"network-disconnect"));
	Actions.insert(Actions.size(),addAction("Compute Session","sessionCompute",SLOT(sessionCompute()),"system-run"));
	addAction("&Console","programConsole",SLOT(programConsole()),"terminal","Ctrl+K");
	addAction("Create &Session","createDatabase",SLOT(createDatabase()));
	Actions.insert(Actions.size(),addAction("Import Documents","importDocs",SLOT(importDocs()),"tab-new"));
	Actions.insert(Actions.size(),addAction("&Run Script","runProgram",SLOT(runProgram()),"fork","Ctrl+R"));
	Actions.insert(Actions.size(),addAction("&Statistics","sessionStats",SLOT(sessionStats()),"view-statistics"));
	Actions.insert(Actions.size(),addAction("Run &Tool","runTool",SLOT(runTool()),"","Ctrl+T"));
	addAction("E&xit","sessionQuit",SLOT(sessionQuit()),"window-close","Ctrl+Q");

	// Menu "Knowledge"
	Actions.insert(Actions.size(),addAction("See &Dictionaries","showDicts",SLOT(showDicts())));

	// Menu "Document"
	Actions.insert(Actions.size(),addAction("&Force Re-computing Documents","docsClear",SLOT(docsClear())));
	Actions.insert(Actions.size(),addAction("Show &Documents","showDocs",SLOT(showDocs()),"text-xml"));
	Actions.insert(Actions.size(),addAction("Export Documents Description","exportDocs",SLOT(exportDocs())));
	Actions.insert(Actions.size(),addAction("Index Documents","indexDocs",SLOT(indexDocs())));
	Actions.insert(Actions.size(),addAction("Load and Analyze a Document","docAnalyze",SLOT(docAnalyze()),"","Ctrl+O"));
	Actions.insert(Actions.size(),addAction("&Analyze Documents","docsAnalyze",SLOT(docsAnalyze()),"kfind"));
	Actions.insert(Actions.size(),addAction("&Export Documents","docsIndexer",SLOT(docsIndexer())));
	Actions.insert(Actions.size(),addAction("&Query Meta Engine","queryMetaEngine",SLOT(queryMetaEngine()),"edit-find"));
	Actions.insert(Actions.size(),addAction("Compute Suggestions","computeSugs",SLOT(computeSugs())));
	Actions.insert(Actions.size(),addAction("Compute Trust","computeTrust",SLOT(computeTrust())));

	// Menu "Topics"
	Actions.insert(Actions.size(),addAction("Force Re-computing Topics","topicsClear",SLOT(topicsClear())));
	Actions.insert(Actions.size(),addAction("Group Documents","topicsCalc",SLOT(topicsCalc())));
	Actions.insert(Actions.size(),addAction("Show &Topics","showTopics",SLOT(showTopics()),"window_new"));
	Actions.insert(Actions.size(),addAction("Show &Classes","showClasses",SLOT(showClasses()),"view-list-tree"));

	// Menu "Users"
	Actions.insert(Actions.size(),addAction("&Force Re-computing profiles","profilesClear",SLOT(profilesClear())));
	Actions.insert(Actions.size(),addAction("&Show Users","showUsers",SLOT(showUsers()),"meeting-attending"));
	Actions.insert(Actions.size(),addAction("&Compute Profiles","profilesCalc",SLOT(profilesCalc()),"media-playback-start"));
	Actions.insert(Actions.size(),addAction("Compute &Profile","profileCalc",SLOT(profileCalc())));

	// Menu "Communities"
	Actions.insert(Actions.size(),addAction("&Force Re-computing Communities","communitiesClear",SLOT(communitiesClear())));
	Actions.insert(Actions.size(),addAction("&Show Communities","showCommunities",SLOT(showCommunities()),"user-group-new"));
	Actions.insert(Actions.size(),addAction("&Group Profiles","communitiesCalc",SLOT(communitiesCalc()),"media-seek-forward"));

	// Menu "Debug"
	Actions.insert(Actions.size(),addAction("Test Subjects","testSubjects",SLOT(testSubjects())));
	Actions.insert(Actions.size(),addAction("Initialize the simulation","subjectsCreate",SLOT(subjectsCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal &Communities","communitiesCreate",SLOT(communitiesCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal &Topics","topicsCreate",SLOT(topicsCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal T&opics (using the classes)","topicsClassesCreate",SLOT(topicsClassesCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal C&lasses","classesCreate",SLOT(classesCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal C&lasses (using documents)","classesDocsCreate",SLOT(classesDocsCreate())));
	Actions.insert(Actions.size(),addAction("&Feedback Cycle","doFdbks",SLOT(doFdbks())));
	Actions.insert(Actions.size(),addAction("&Assessments Cycle","doAssessments",SLOT(doAssessments())));
	Actions.insert(Actions.size(),addAction("Compare Ideal Communities","communitiesCompare",SLOT(communitiesCompare())));
	Actions.insert(Actions.size(),addAction("Compare Ideal Topics","topicsCompare",SLOT(topicsCompare())));
	Actions.insert(Actions.size(),addAction("Compare Ideal Classes","classesCompare",SLOT(classesCompare())));

	// Menu "options"
	Actions.insert(Actions.size(),addAction("Configure Session","configureSession",SLOT(optionsSession())));
	KStandardAction::preferences(this,SLOT(optionsPreferences()),actionCollection());

	// Menu "Window"
	KAction* windowCloseAll(new KAction(i18n("&Close All"),this));
	actionCollection()->addAction(QLatin1String("window_closeall"),windowCloseAll);
	connect(windowCloseAll,SIGNAL(triggered(bool)),Desktop,SLOT(closeAllSubWindows()));
	KAction* windowTile(new KAction(i18n("&Tile"),this));
	actionCollection()->addAction(QLatin1String("window_tile"),windowTile);
	connect(windowTile,SIGNAL(triggered(bool)),Desktop,SLOT(tileSubWindows()));
	KAction* windowCascade(new KAction(i18n("&Cascade"),this));
	actionCollection()->addAction(QLatin1String("window_cascade"),windowCascade);
	connect(windowCascade,SIGNAL(triggered(bool)),Desktop,SLOT(cascadeSubWindows()));

	// Help Menu
//	helpProgram = new KAction(i18n("List of all classes"), 0, 0, this, SLOT(slotHelpProgram()), actionCollection(),"helpProgram");
//	helpProgram=new KAction("List of all classes",0,0,this,SLOT(slotHelpProgram()),this);
//	menuBar()->insertItem ("&Help",helpProgram);*/
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionConnected(bool connected)
{
	aSessionConnect->setEnabled(!connected);
	aSessionSave->setEnabled(connected);
	for(int i=0;i<Actions.count();i++)
		Actions.at(i)->setEnabled(connected);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KGALILEICenter");

	aSessionSave->setChecked(General.readEntry("Save Results",false));
	PrgPath=FromQString(General.readEntry("PrgPath",""));
	ToolCat=FromQString(General.readEntry("ToolCat",""));
	Tool=FromQString(General.readEntry("Tool",""));
	SessionName=FromQString(General.readEntry("SessionName",""));

	Configure::readOptions();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KGALILEICenter");

	General.writeEntry("Save Results",aSessionSave->isChecked());
	General.writeEntry("PrgPath",ToQString(PrgPath));
	General.writeEntry("ToolCat",ToQString(ToolCat));
	General.writeEntry("Tool",ToQString(Tool));
	General.writeEntry("SessionName",ToQString(SessionName));

	Configure::saveOptions();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::emitDocsChanged(void)
{
	emit docsChanged();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::emitTopicsChanged(void)
{
	emit topicsChanged();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::emitProfilesChanged(void)
{
	emit profilesChanged();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::emitCommunitiesChanged(void)
{
	emit communitiesChanged();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionQuit(void)
{
	statusMsg(i18n("Exiting..."));
	sessionDisconnect();
	saveOptions();
	statusMsg(i18n("Ready."));
	close();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionConnect(void)
{
	bool DestroyDoc(false);

	statusMsg(i18n("Connecting..."));
	try
	{
	    bool ok;
	    SessionName=FromQString(QInputDialog::getText(this,tr("Connect to a session"),tr("Session name:"),QLineEdit::Normal,ToQString(SessionName),&ok));
	    if((!ok)||(SessionName.IsEmpty()))
	    	return;
		DestroyDoc=true;
		QSessionProgressDlg dlg(this,"Loading from Database");
		if(dlg.Run(new QCreateSession(this,Doc,SessionName)))
		{
			sessionConnected(true);
			Status->setPixmap(QPixmap(KIconLoader::global()->loadIcon("network-connect",KIconLoader::Small)));
			DestroyDoc=false;
			statusMsg(i18n("Connected"));
			saveModifier();
		}
		else
			statusMsg(i18n("Ready"));
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
	if(DestroyDoc)
	{
		Doc=0;
	}
}

//-----------------------------------------------------------------------------
void KGALILEICenter::sessionDisconnect(void)
{
	if(Doc)
	{
		Desktop->closeAllSubWindows();
		DeleteSession(Doc);
		Doc=0;
		sessionConnected(false);
		statusMsg(i18n("Not Connected !"));
		Status->setPixmap(KIconLoader::global()->loadIcon("network-disconnect",KIconLoader::Small));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionCompute(void)
{
	QSessionProgressDlg Dlg(this,"Compute Complete Session");
	QComputeAll* Task(new QComputeAll(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::programConsole(void)
{
	KPrgConsole* ptr(new KPrgConsole());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::createDatabase(void)
{
	QCreateDatabase(this).run();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::importDocs(void)
{
	QFillDatabase(this).run();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::runProgram(void)
{
	QString url(KFileDialog::getOpenFileName(KUrl(ToQString(PrgPath)),"*.kprg|KGALILEICenter Programs",Desktop,"Open Program File..."));
	if(url.isEmpty())
	{
		QMessageBox::critical(this,"KGALILEICenter","File could not be found");
		return;
	}
	KViewPrg* ptr(new KViewPrg(FromQString(url)));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	try
	{
		ptr->run();
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionStats(void)
{
	KViewStats* ptr(new KViewStats());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::statusMsg(const QString& text)
{
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::saveModifier(void)
{
	if(Doc)
		Doc->SetSaveResults(aSessionSave->isChecked());
}


//-----------------------------------------------------------------------------
void KGALILEICenter::optionsPreferences(void)
{
	Preferences Dlg(this);
	Dlg.exec();
}

//-----------------------------------------------------------------------------
void KGALILEICenter::optionsSession(void)
{
	Configure Dlg(this);
	Dlg.exec();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showDicts(void)
{
	KViewDicts* ptr(new KViewDicts(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->create();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docsClear()
{
	if(Doc)
		Doc->ForceReCompute(otDoc);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showDocs(void)
{
	QGObjectsList* ptr(new QGObjectsList());
	connect(ptr,SIGNAL(Show(GDoc*)),this,SLOT(showDoc(GDoc*)));
	ptr->setAttribute(Qt::WA_DeleteOnClose);
	Desktop->addSubWindow(ptr)->setWindowTitle("Documents");
	ptr->Set(Doc,QGObjectsList::Docs);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::exportDocs(void)
{
	mThrowGException("Export documents is not implemented");
//	QString file(KFileDialog::getSaveFileName(KUrl(),"*.*",this,"Choose file where to export the files descriptions"));
//	if(!file.isEmpty())
//	{
//		RTextFile Export(FromQString(file),"utf-8");
//		Export.Open(RIO::Create);
//		Export<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
//		Export<<"<!DOCTYPE documents>"<<endl;
//		Export<<"<documents>"<<endl;
//		RCursor<GDoc> Docs(Doc->GetObjs(static_cast<GDoc*>(0)));
//		for(Docs.Start();!Docs.End();Docs.Next())
//		{
//			Export<<"\t<document id=\""+RString::Number(Docs()->GetId())+"\" url=\""+Docs()->GetURL()()+"\">"<<endl;
//			R::RCursor<GWeightInfo> Infos(Docs()->GetVector().GetInfos());
//			GConceptType* type(0);
//			double norm(0.0),max(0.0);
//			for(Infos.Start();!Infos.End();Infos.Next())
//			{
//				if(type!=Infos()->GetType())
//				{
//					if(type)
//						Export<<"\t\t\t<norm>"<<RString::Number(sqrt(norm))<<"</norm>"<<endl<<"\t\t</vector>"<<endl;
//					type=Infos()->GetType();
//					norm=0.0;
//					max=Docs()->GetVector().GetMaxAbsWeight(type);
//					Export<<"\t\t<vector type=\""<<type->GetName()<<"\">"<<endl;
//				}
//				Export<<"\t\t\t"<<"\t"<<RString::Number(Infos()->GetId())<<"\t";
//				double w(Infos()->GetWeight());
//				double tfidf(Infos()->GetConcept()->GetIF(otDoc)*w/max);
//				Export<<RString::Number(w)<<"\t"<<RString::Number(tfidf);
//				norm+=tfidf*tfidf;
//				Export<<endl;
//			}
//			if(type)
//				Export<<"\t\t\t<norm>"<<RString::Number(sqrt(norm))<<"</norm>"<<endl<<"\t\t</vector>"<<endl;
//			Export<<"\t</document>"<<endl;
//		}
//		Export<<"</documents>"<<endl;
//	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::indexDocs(void)
{
	QSessionProgressDlg Dlg(this,"Index documents");
	QIndexDocs* Task(new QIndexDocs(this));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showDoc(GDoc* doc)
{
	KViewDoc* ptr(new KViewDoc(this,doc));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docAnalyze(void)
{
	statusMsg(i18n("Opening file..."));
	QString url(KFileDialog::getOpenFileName(KUrl("~"),"*.*",Desktop,"Document to Analyze"));
	if(!url.isEmpty())
	{
		KViewDoc* ptr(new KViewDoc(this,FromQString(url),FromQString(KMimeType::findByPath(url)->name())));
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		try
		{
			ptr->AnalyzeDoc();
		}
		catch(GException& e)
		{
			KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
		}
		catch(RException& e)
		{
			KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
		}
		catch(std::exception& e)
		{
			KMessageBox::error(this,e.what(),"std::exception");
		}
		catch(...)
		{
			KMessageBox::error(this,"Undefined Error");
		}
	}
	statusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docsAnalyze(void)
{
	QSessionProgressDlg Dlg(this,"Analyze Documents");
	QAnalyzeDocs* Task(new QAnalyzeDocs(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docsIndexer(void)
{
	mThrowGException("docsIndexer not implemented");
//	QString dir(KFileDialog::getExistingDirectory(KUrl(),this,"Choose directory where to export the files"));
//	if(!dir.isEmpty())
//	{
//	 	KProgressDialog dlg(this,"Progress","Export documents");
//		dlg.show();
//		KApplication::kApplication()->processEvents();
//		RString pre(FromQString(dir));
//		pre+="/doc";
//		RCursor<GDoc> Docs(Doc->GetDocs());
//		size_t nb;
//		for(Docs.Start(),nb=0;!Docs.End();Docs.Next(),nb++)
//		{
//			if(dlg.wasCancelled())
//				break;
//			dlg.progressBar()->setValue(static_cast<int>((nb*100)/Docs.GetNb()));
//			KApplication::kApplication()->processEvents();
//			if(!Docs()->GetLang())
//				continue;
//			RString name(pre+RString::Number(Docs()->GetId())+".txt");
//			dlg.setLabelText(ToQString(name));
//			KApplication::kApplication()->processEvents();
//			RTextFile file(name);
//			file.Open(RIO::Create);
//			RCursor<GWeightInfo> Words(Docs()->GetVector().GetInfos());
//			for(Words.Start();!Words.End();Words.Next())
//			{
//				for(size_t i=lround(Words()->GetWeight())+1;--i;)
//				{
//					file<<Doc->GetStorage()->LoadConcept(Words()->GetId());
//				}
//				file<<endl;
//			}
//			file.Close();
//		}
//	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::queryMetaEngine(void)
{
	if(!GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine",false))
	{
		QMessageBox::information(this," Error "," No Meta Engine Method selected!!");
		return;
	}
	KViewMetaEngine* ptr(new KViewMetaEngine(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsClear(void)
{
	if(Doc)
		Doc->ForceReCompute(otTopic);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsCalc(void)
{
	QSessionProgressDlg Dlg(this,"Compute Topics");
	QGroupDocs* Task(new QGroupDocs(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showTopics(void)
{
	KViewTopics* ptr(new KViewTopics());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showTopic(GTopic* topic)
{
	KViewTopic* ptr(new KViewTopic(topic));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::profilesClear()
{
	if(Doc)
		Doc->ForceReCompute(otProfile);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showUsers(void)
{
	KViewUsers* ptr(new KViewUsers());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showProfile(GProfile* profile)
{
	KViewProfile* ptr(new KViewProfile(this,profile));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::profilesCalc(void)
{
	QSessionProgressDlg Dlg(this,"Compute Profiles");
	QComputeProfiles* Task(new QComputeProfiles(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::profileCalc(void)
{
	KViewProfile* win(dynamic_cast<KViewProfile*>(Desktop->activeSubWindow()));
	if(!win) return;
	win->ComputeProfile();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesClear()
{
	if(Doc)
		Doc->ForceReCompute(otCommunity);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showCommunities(void)
{
	KViewCommunities* ptr(new KViewCommunities());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();

}


//-----------------------------------------------------------------------------
void KGALILEICenter::showCommunity(GCommunity* community)
{
	KViewCommunity* ptr(new KViewCommunity(community));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}

//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesCalc(void)
{
	QSessionProgressDlg Dlg(this,"Compute Communities");
	QGroupProfiles* Task(new QGroupProfiles(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::subjectsCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Subjects");
	QCreateIdealSubjects* Task(new QCreateIdealSubjects(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Topics");
	QCreateIdealTopics* Task(new QCreateIdealTopics(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsClassesCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Topics");
	QCreateIdealTopicsFromClasses* Task(new QCreateIdealTopicsFromClasses(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::classesCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Classes");
	QCreateIdealClasses* Task(new QCreateIdealClasses(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::classesDocsCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Classes");
	QCreateIdealDocsClasses* Task(new QCreateIdealDocsClasses(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Communities");
	QCreateIdealCommunities* Task(new QCreateIdealCommunities(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::testSubjects(void)
{
	QSessionProgressDlg Dlg(this,"Test Subjects");
	QTestSubjects* Task(new QTestSubjects(this));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::doFdbks(void)
{
	QSessionProgressDlg Dlg(this,"Feedback Cycle");
	QMakeFdbks* Task(new QMakeFdbks(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::doAssessments(void)
{
	QSessionProgressDlg Dlg(this,"Assessments Cycle");
	QMakeAssessments* Task(new QMakeAssessments(this));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::computeTrust(void)
{
	QSessionProgressDlg Dlg(this,"Compute Trust");
	QComputeTrust* Task(new QComputeTrust(this));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::computeSugs(void)
{
	QSessionProgressDlg Dlg(this,"Compute Suggestions");
	QComputeSugs* Task(new QComputeSugs(this));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::runTool(void)
{
	GPlugInManager* Manager(GALILEIApp->GetManager("Tools"));

	// Create the dialog box
	KDialog Choose(this);
	Ui_QChooseList Ui;
	QWidget* widget(new QWidget(&Choose));
	Ui.setupUi(widget);
	Choose.setMainWidget(widget);

	// Init the dialog box with the lists
	Ui.Desc->setText("Choose the tool to run");
	RCursor<GPlugInList> Lists(Manager->GetPlugInLists());
	int Row(0),i;
	for(Lists.Start(),i=0;!Lists.End();Lists.Next(),i++)
	{
		if(Lists()->GetName()==ToolCat)
			Row=i;
		Ui.List->addItem(ToQString(Lists()->GetName()));
	}
	Ui.List->setCurrentRow(Row);
	if(!Choose.exec())
		return;
	ToolCat=FromQString(Ui.List->item(Ui.List->currentRow())->text());

	// Init the dialog box with the plug-ins
	RCastCursor<GPlugIn,GTool> Tools(Manager->GetPlugIns<GTool>(ToolCat));
	Ui.List->clear();
	for(Tools.Start(),i=0,Row=0;!Tools.End();Tools.Next(),i++)
	{
		if(Tools()->GetName()==Tool)
			Row=i;
		Ui.List->addItem(ToQString(Tools()->GetName()));
	}
	Ui.List->setCurrentRow(Row);
	if(!Choose.exec())
		return;

	QSessionProgressDlg Dlg(this,"Tool");
	Tool=FromQString(Ui.List->item(Ui.List->currentRow())->text());
	QRunTool* Task(new QRunTool(this,Tool,ToolCat));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesCompare(void)
{
	try
	{
		KViewIdealCommunities* ptr(new KViewIdealCommunities());
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsCompare(void)
{
	try
	{
		KViewIdealTopics* ptr(new KViewIdealTopics());
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::classesCompare(void)
{
	try
	{
		KViewIdealClasses* ptr(new KViewIdealClasses());
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showClasses(void)
{
	KViewClasses* ptr(new KViewClasses());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showClass(GClass* /*theclass*/)
{
/*	KViewCommunity* ptr(new KViewCommunity(community));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();*/
}


////-----------------------------------------------------------------------------
//void KGALILEICenterApp::slotHelpProgram(void)
//{
//	KViewPrg* o;
//
//	KApplication::kApplication()->processEvents();
//	try
//	{
//		createClient(Doc,o=new KViewPrg(Doc,pWorkspace,"",0));
//		o->run();
//	}
///	catch(GException& e)
//{
//	KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
//}
//catch(RException& e)
//{
//	KMessageBox::error(this,e.GetMsg(),"R Exception");
//}
//catch(std::exception& e)
//{
//	KMessageBox::error(this,e.what(),"std::exception");
//}
//catch(...)
//{
//	KMessageBox::error(this,"Undefined Error");
//}
//}


//-----------------------------------------------------------------------------
KGALILEICenter::~KGALILEICenter(void)
{
	App=0;
	try
	{
		delete Status;
		delete Debug;
	}
	catch(GException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,ToQString(e.GetMsg()),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}
