/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QMdiSubWindow>
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
#include <kviewdoc.h>
#include <kviewmetaengine.h>
#include <kviewprg.h>
#include <qfilldatabase.h>
#include <kviewstats.h>
#include <kviewgroup.h>
#include <kviewgroups.h>
#include <kviewusers.h>
#include <kviewidealgroups.h>
#include <kviewprofile.h>
#include <kviewdicts.h>
#include <qcreatedatabase.h>
#include <ui_qdebuginfo.h>
#include <kprgconsole.h>



//-----------------------------------------------------------------------------
//
// class KGALILEICenter
//
//-----------------------------------------------------------------------------

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
    Init();
    readOptions();
	sessionConnected(false);
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
	aSessionConnect=addAction("&Connect","sessionConnect",SLOT(sessionConnect()),"network-connect");
	Actions.insert(Actions.size(),addAction("&Disconnect","sessionDisconnect",SLOT(sessionDisconnect()),"network-disconnect"));
	Actions.insert(Actions.size(),addAction("Compute &Session","sessionCompute",SLOT(sessionCompute()),"system-run"));
	addAction("&Console","programConsole",SLOT(programConsole()),"terminal","Ctrl+K");
	addAction("Create &MySQL Database","createDatabase",SLOT(createDatabase()));
	Actions.insert(Actions.size(),addAction("Import Documents","importDocs",SLOT(importDocs()),"tab-new"));
	Actions.insert(Actions.size(),addAction("&Run Program","runProgram",SLOT(runProgram()),"fork","Ctrl+R"));
	Actions.insert(Actions.size(),addAction("&Statistics","sessionStats",SLOT(sessionStats()),"view-statistics"));
	Actions.insert(Actions.size(),addAction("&Debug Information","sessionDebug",SLOT(sessionDebug()),"help"));
	addAction("E&xit","sessionQuit",SLOT(sessionQuit()),"window-close","Ctrl+Q");

	// Menu "Knowledge"
	Actions.insert(Actions.size(),addAction("See &Dictionaries","showDicts",SLOT(showDicts())));

	// Menu "Document"
	Actions.insert(Actions.size(),addAction("&Force Re-computing Documents","docsClear",SLOT(docsClear())));
	Actions.insert(Actions.size(),addAction("Show &Documents","showDocs",SLOT(showDocs()),"text-xml"));
	Actions.insert(Actions.size(),addAction("Export Documents Description","exportDocs",SLOT(exportDocs())));
	Actions.insert(Actions.size(),addAction("Load and Analyze a Document","docAnalyse",SLOT(docAnalyse())));
	Actions.insert(Actions.size(),addAction("&Analyze Documents","docsAnalyse",SLOT(docsAnalyse()),"kfind"));
	Actions.insert(Actions.size(),addAction("Execute &Post-Documents Methods","postDocsAnalyse",SLOT(postDocsAnalyse())));
	Actions.insert(Actions.size(),addAction("&Export Documents","docsIndexer",SLOT(docsIndexer())));
	Actions.insert(Actions.size(),addAction("&Create XML Structure","createXML",SLOT(createXML())));
	Actions.insert(Actions.size(),addAction("&Save XML Structure","saveXML",SLOT(saveXML())));
	Actions.insert(Actions.size(),addAction("&Query Meta Engine","queryMetaEngine",SLOT(queryMetaEngine()),"edit-find"));

	// Menu "Topics"
	Actions.insert(Actions.size(),addAction("Force Re-computing Topics","topicsClear",SLOT(topicsClear())));
	Actions.insert(Actions.size(),addAction("Group Documents","topicsCalc",SLOT(topicsCalc())));
	Actions.insert(Actions.size(),addAction("Execute Post-Topic Methods","postTopicsCalc",SLOT(postTopicsCalc())));
	Actions.insert(Actions.size(),addAction("Show &Topics","showTopics",SLOT(showTopics()),"window_new"));

	// Menu "Users"
	Actions.insert(Actions.size(),addAction("&Force Re-computing profiles","profilesClear",SLOT(profilesClear())));
	Actions.insert(Actions.size(),addAction("&Show Users","showUsers",SLOT(showUsers()),"meeting-attending"));
	Actions.insert(Actions.size(),addAction("&Compute Profiles","profilesCalc",SLOT(profilesCalc()),"media-playback-start"));
	Actions.insert(Actions.size(),addAction("Execute &Post-Profiles Methods","postProfilesCalc",SLOT(postProfilesCalc())));
	Actions.insert(Actions.size(),addAction("Compute &Profile","profileCalc",SLOT(profileCalc())));

	// Menu "Communities"
	Actions.insert(Actions.size(),addAction("&Force Re-computing Communities","communitiesClear",SLOT(communitiesClear())));
	Actions.insert(Actions.size(),addAction("&Show Communities","showCommunities",SLOT(showCommunities()),"user-group-new"));
	Actions.insert(Actions.size(),addAction("&Group Profiles","communitiesCalc",SLOT(communitiesCalc()),"media-seek-forward"));
	Actions.insert(Actions.size(),addAction("Execute &Post-Communities Methods","postCommunitiesCalc",SLOT(postCommunities())));

	// Menu "Debug"
	Actions.insert(Actions.size(),addAction("Initialize the simulation","subjectsCreate",SLOT(subjectsCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal &Communities","communitiesCreate",SLOT(communitiesCreate())));
	Actions.insert(Actions.size(),addAction("Create Ideal &Topics","topicsCreate",SLOT(topicsCreate())));
	Actions.insert(Actions.size(),addAction("&Feedback Cycle","doFdbks",SLOT(doFdbks())));
	Actions.insert(Actions.size(),addAction("&Assessments Cycle","doAssessments",SLOT(doAssessments())));
	Actions.insert(Actions.size(),addAction("Compare Communities Topics","communitiesCompare",SLOT(communitiesCompare())));
	Actions.insert(Actions.size(),addAction("Compare Ideal Topics","topicsCompare",SLOT(topicsCompare())));

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

    KStandardAction::preferences(this,SLOT(optionsPreferences()),actionCollection());
}


//-----------------------------------------------------------------------------
void KGALILEICenter::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KGALILEICenter");

	aSessionSave->setChecked(General.readEntry("Save Results",false));
	PrgPath=FromQString(General.readEntry("PrgPath",""));

	Configure::readOptions();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"KGALILEICenter");

	General.writeEntry("Save Results",aSessionSave->isChecked());
	General.writeEntry("PrgPath",ToQString(PrgPath));

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
void KGALILEICenter::sessionConnected(bool connected)
{
	aSessionConnect->setEnabled(!connected);
	aSessionSave->setEnabled(connected);
	for(int i=0;i<Actions.count();i++)
		Actions.at(i)->setEnabled(connected);
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
		DestroyDoc=true;
		Doc=CreateSession();
		saveModifier();
		QSessionProgressDlg dlg(this,"Loading from Database");
		if(dlg.Run(new QLoadSession()))
		{
			sessionConnected(true);
			Status->setPixmap(QPixmap(KIconLoader::global()->loadIcon("network-connect",KIconLoader::Small)));
			DestroyDoc=false;
			statusMsg(i18n("Connected"));
		}
		else
			statusMsg(i18n("Ready"));
	}
	catch(GException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"R Exception");
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
		DeleteSession();
		Doc=0;
	}
}

//-----------------------------------------------------------------------------
void KGALILEICenter::sessionDisconnect(void)
{
	if(Doc)
	{
		Desktop->closeAllSubWindows();
		DeleteSession();
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
	QComputeAll* Task(new QComputeAll());
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::sessionDebug(void)
{
	KDialog dlg(this);
	dlg.setCaption("Choose debug information");
	Ui_QDebugInfo Ui;
	QWidget* widget=new QWidget(&dlg);
	Ui.setupUi(widget);
	dlg.setMainWidget(widget);
	RCursor<GDebugObject> Objs(Doc->GetDebugObjects());
	for(Objs.Start();!Objs.End();Objs.Next())
		Ui.Object->addItem(ToQString(Objs()->GetName()));
	if(dlg.exec())
		Doc->DebugInfo(FromQString(Ui.Object->currentText()),FromQString(Ui.Info->text()));
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
		KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"R Exception");
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
	Configure Dlg(this);
	Dlg.exec(Doc);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showDicts(void)
{
	KViewDicts* ptr(new KViewDicts());
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
	ptr->Set(QGObjectsList::Docs);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::exportDocs(void)
{
	QString file(KFileDialog::getSaveFileName(KUrl(),"*.*",this,"Choose file where to export the files descriptions"));
	if(!file.isEmpty())
	{
		RTextFile Export(FromQString(file),"utf-8");
		Export.Open(RIO::Create);
		Export<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
		Export<<"<!DOCTYPE documents>"<<endl;
		Export<<"<documents>"<<endl;
		RCursor<GDoc> Docs(Doc->GetDocs());
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			Export<<"\t<document id=\""+RString::Number(Docs()->GetId())+"\" url=\""+Docs()->GetURL()()+"\">"<<endl;
			R::RCursor<GWeightInfo> Infos(Docs()->GetInfos());
			GConceptType* type(0);
			double norm(0.0),max(0.0);
			for(Infos.Start();!Infos.End();Infos.Next())
			{
				if(type!=Infos()->GetType())
				{
					if(type)
						Export<<"\t\t\t<norm>"<<RString::Number(sqrt(norm))<<"</norm>"<<endl<<"\t\t</vector>"<<endl;
					type=Infos()->GetType();
					norm=0.0;
					max=Docs()->GetMaxAbsWeight(type);
					Export<<"\t\t<vector type=\""<<type->GetName()<<"\">"<<endl;
				}
				Export<<"\t\t\t"<<"\t"<<RString::Number(Infos()->GetId())<<"\t";
				double w(Infos()->GetWeight());
				double tfidf(Infos()->GetConcept()->GetType()->GetIF(Infos()->GetConcept()->GetId(),otDoc)*w/max);
				Export<<RString::Number(w)<<"\t"<<RString::Number(tfidf);
				norm+=tfidf*tfidf;
				Export<<endl;
			}
			if(type)
				Export<<"\t\t\t<norm>"<<RString::Number(sqrt(norm))<<"</norm>"<<endl<<"\t\t</vector>"<<endl;
			Export<<"\t</document>"<<endl;
		}
		Export<<"</documents>"<<endl;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::showDoc(GDoc* doc)
{
	KViewDoc* ptr(new KViewDoc(doc));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docAnalyse(void)
{
	statusMsg(i18n("Opening file..."));
	QString url(KFileDialog::getOpenFileName(KUrl("~"),"*.*",Desktop,"Document to Analyze"));
	if(!url.isEmpty())
	{
		KViewDoc* ptr(new KViewDoc(FromQString(url),FromQString(KMimeType::findByPath(url)->name())));
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		try
		{
			ptr->AnalyseDocXML();
		}
		catch(GException& e)
		{
			KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
		}
		catch(RException& e)
		{
			KMessageBox::error(this,e.GetMsg(),"R Exception");
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
void KGALILEICenter::docsAnalyse(void)
{
	QSessionProgressDlg Dlg(this,"Analyze Documents");
	QAnalyzeDocs* Task(new QAnalyzeDocs());
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::postDocsAnalyse(void)
{
	QSessionProgressDlg Dlg(this,"Post-Documents Analyse");
	QPostAnalyzeDocs* Task(new QPostAnalyzeDocs());
	connect(Task,SIGNAL(finish()),this,SLOT(emitDocsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::docsIndexer(void)
{
	QString dir(KFileDialog::getExistingDirectory(KUrl(),this,"Choose directory where to export the files"));
	if(!dir.isEmpty())
	{
	 	KProgressDialog dlg(this,"Progress","Export documents");
		dlg.show();
		KApplication::kApplication()->processEvents();
		RString pre(FromQString(dir));
		pre+="/doc";
		RCursor<GDoc> Docs(Doc->GetDocs());
		size_t nb;
		for(Docs.Start(),nb=0;!Docs.End();Docs.Next(),nb++)
		{
			if(dlg.wasCancelled())
				break;
			dlg.progressBar()->setValue(static_cast<int>((nb*100)/Docs.GetNb()));
			KApplication::kApplication()->processEvents();
			if(!Docs()->GetLang())
				continue;
			RString name(pre+RString::Number(Docs()->GetId())+".txt");
			dlg.setLabelText(ToQString(name));
			KApplication::kApplication()->processEvents();
			RTextFile file(name);
			file.Open(RIO::Create);
			RCursor<GWeightInfo> Words(Docs()->GetInfos());
			for(Words.Start();!Words.End();Words.Next())
			{
				for(size_t i=lround(Words()->GetWeight())+1;--i;)
				{
					file<<Doc->GetStorage()->LoadConcept(Words()->GetId(),Words()->GetType());
				}
				file<<endl;
			}
			file.Close();
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenter::createXML(void)
{
	KViewDoc* win(dynamic_cast<KViewDoc*>(Desktop->activeSubWindow()));
	if(!win) return;
	win->CreateDocXML();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::saveXML(void)
{
	KViewDoc* win(dynamic_cast<KViewDoc*>(Desktop->activeSubWindow()));
	if(!win) return;
	int dlg (KMessageBox::No);
	KUrl url;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveUrl(KUrl("~"),i18n("*.docxml"), this, i18n("Save DocXML File..."));
		if(url.isEmpty()) return;
		QFile Test(url.path());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}
	win->SaveDocXML(FromQString(url.path()));
}


//-----------------------------------------------------------------------------
void KGALILEICenter::queryMetaEngine(void)
{
	if(!GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod(false))
	{
		QMessageBox::information(this," Error "," No Meta Engine Method selected!!");
		return;
	}
	KViewMetaEngine* ptr(new KViewMetaEngine());
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
	QGroupDocs* Task(new QGroupDocs());
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::postTopicsCalc(void)
{
	QSessionProgressDlg Dlg(this,"Post-Topics Analyze");
	QPostGroupDocs* Task(new QPostGroupDocs());
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
	KViewProfile* ptr(new KViewProfile(profile));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::profilesCalc(void)
{
	QSessionProgressDlg Dlg(this,"Compute Profiles");
	QComputeProfiles* Task(new QComputeProfiles());
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::postProfilesCalc(void)
{
	QSessionProgressDlg Dlg(this,"Post-Profiles Analyze");
	QPostComputeProfiles* Task(new QPostComputeProfiles());
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
	QGroupProfiles* Task(new QGroupProfiles());
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::postCommunities(void)
{
	QSessionProgressDlg Dlg(this,"Post-Communities Analyze");
	QPostGroupProfiles* Task(new QPostGroupProfiles());
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::subjectsCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Subjects");
	QCreateIdealSubjects* Task(new QCreateIdealSubjects());
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
	QCreateIdealTopics* Task(new QCreateIdealTopics());
	connect(Task,SIGNAL(finish()),this,SLOT(emitTopicsChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesCreate(void)
{
	QSessionProgressDlg Dlg(this,"Create Ideal Communities");
	QCreateIdealCommunities* Task(new QCreateIdealCommunities());
	connect(Task,SIGNAL(finish()),this,SLOT(emitCommunitiesChanged()));
	Dlg.Run(Task);
}

//-----------------------------------------------------------------------------
void KGALILEICenter::doFdbks(void)
{
	QSessionProgressDlg Dlg(this,"Feedback Cycle");
	QMakeFdbks* Task(new QMakeFdbks());
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::doAssessments(void)
{
	QSessionProgressDlg Dlg(this,"Assessments Cycle");
	QMakeAssessments* Task(new QMakeAssessments());
	connect(Task,SIGNAL(finish()),this,SLOT(emitProfilesChanged()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KGALILEICenter::communitiesCompare(void)
{
	KViewIdealCommunities* ptr(new KViewIdealCommunities());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void KGALILEICenter::topicsCompare(void)
{
	KViewIdealTopics* ptr(new KViewIdealTopics());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
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
	try
	{
		delete Debug;
	}
	catch(GException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"R Exception");
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
