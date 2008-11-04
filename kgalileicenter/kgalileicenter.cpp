/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocxml.h>
#include <gstorage.h>
#include <gprofile.h>
#include <ggroup.h>
#include <gsubject.h>
#include <gsubjects.h>
#include <glang.h>
#include <gweightinfo.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qprinter.h>
#include <qmessagebox.h>
#include <qworkspace.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <kdatepicker.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kapp.h>
#include <kmessagebox.h>
#include <kmimetype.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <kpopupmenu.h>
#include <kstatusbar.h>
#include <kurlrequester.h>
#include <kprogress.h>
#include <kprocess.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewdocs.h"
#include "kviewdoc.h"
#include "kviewmetaengine.h"
#include "kviewusers.h"
#include "kviewstats.h"
#include "kviewthcommunities.h"
#include "kviewcommunities.h"
#include "kviewcommunity.h"
#include "kviewtopics.h"
#include "kviewtopic.h"
#include "kviewthtopics.h"
#include "kviewprg.h"
#include "kviewprofile.h"
#include "kviewdicts.h"
#include "qsessionprogress.h"
#include "qcreatedatabase.h"
#include "qfilldatabase.h"
#include "qhistorydlg.h"
#include "qexportmatrixdlg.h"
#include "qsimulationdlg.h"
#include "configure.h"
#include "kprgconsole.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDoc* KGALILEICenterApp::getDocument(void) const
{
	return(Doc);
}


//-----------------------------------------------------------------------------
GSession* KGALILEICenterApp::getSession(void) const
{
	if(!Doc)
		return(0);
	return(Doc->GetSession());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionConnect(void)
{
	QString method;
	GSession* Sess;
	bool DestroyDoc(false);

	slotStatusMsg(i18n("Connecting..."));
	Doc=0;
	try
	{
		Doc=new KDoc(this);
		DestroyDoc=true;
		Sess = CreateSession();
		Doc->SetSession(Sess);
		slotSaveModifier();
		QSessionProgressDlg dlg(this,Sess,"Loading from Database");
		if(dlg.Run(new QLoadSession()))
		{
			sessionConnect->setEnabled(false);
			UpdateMenusEntries();
			dbStatus->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("connect_established",KIcon::Small)));
		}
		DestroyDoc=false;
		slotStatusMsg(i18n("Connected"));
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
	if(DestroyDoc)
	{
		delete Doc;
		Doc=0;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDebugInfo(void)
{
	createClient(Doc,new KPrgConsole(Doc,pWorkspace,"Console",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Complete Session");
	if(!Dlg.Run(new QComputeAll()))
		return;
	Doc->updateAllViews(otDoc);
	Doc->updateAllViews(otProfile);
	Doc->updateAllViews(otCommunity);

	if(GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
	{
		Doc->updateAllViews(otNoClass);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDisconnect(void)
{
	try
	{
		if(Doc)
		{
			Doc->closeDocument();
			delete Doc;
			Doc=0;
		}
		DisableAllActions();
		sessionConnect->setEnabled(true);
		statusBar()->changeItem("Not Connected !",1);
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionStats(void)
{
	createClient(Doc,new KViewStats(Doc,pWorkspace,"View statistics",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionQuit(void)
{
	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	slotStatusMsg(i18n("Ready."));
	close();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCreateDatabase(void)
{
	QCreateDatabase dlg(this,0,true);
	dlg.BasicURL->setText(ToQString(StructuresPath));
	if(!dlg.exec())
		return;
	StructuresPath=FromQString(dlg.BasicURL->text());

	try
	{
		QSessionProgressDlg d(this,0,"Create Database");
		d.Run(new QCreateDB(FromQString(dlg.DbName->text()),FromQString(dlg.hostName->text()),
			FromQString(dlg.userName->text()),FromQString(dlg.password->text()),FromQString(dlg.BasicURL->text())));
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotImportUsersData(void)
{
	try
	{
		//Create de DB
		QString name=KFileDialog::getOpenFileName("~",QString::null,this,"Users' Data XML file");
		if(name.isEmpty())
			return;
		QSessionProgressDlg d(this,Doc->GetSession(),"Import Users' Data");
		d.Run(new QImportUsersData(FromQString(name)));
		Doc->updateAllViews(otDoc);
		Doc->updateAllViews(otProfile);
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotImportDocs(void)
{
	QFillEmptyDatabase dlg(this,0,true);
	dlg.KUDirectory->setMode(KFile::Directory);

	if(dlg.exec())
	{
		RString catDirectory  = FromQString(dlg.KUDirectory->url());
		int depth = dlg.Depth->value();
		RString parentName = FromQString(dlg.ParentName->text());

		// if the database name field is empty -> ERROR
		if( catDirectory.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a directory containing all the categories! "));
			return;
		}
		if((dlg.HasParent->isChecked())&& parentName.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must insert a NAME for the parent or unchecked the \"Has Parent\" option! "));
			return;
		}

		QSessionProgressDlg Dlg(this,Doc->GetSession(),"Fill Database");
		Dlg.Run(new QImportDocs(catDirectory,depth,parentName,FromQString(dlg.DefaultMIMEType->text())));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowUsers(void)
{
	createClient(Doc,new KViewUsers(Doc,pWorkspace,"View users",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Profiles");
	if(!Dlg.Run(new QComputeProfiles()))
		return;
	Doc->updateAllViews(otProfile);
	//test whether a linking method has been used during Profile computation.
	//if true -->refresh Links
	if(GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
	{
		Doc->updateAllViews(otNoClass);
	}

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPostProfilesCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Post-Profiles Analyze");
	if(!Dlg.Run(new QPostComputeProfiles()))
		return;
	Doc->updateAllViews(otProfile);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfileCalc(void)
{
	KViewProfile* m = dynamic_cast<KViewProfile*>(pWorkspace->activeWindow());
	if(!m) return;
	((KViewProfile*)m)->ComputeProfile();
	slotWindowActivated(m);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowCommunities(void)
{
	createClient(Doc,new KViewCommunities(Doc,pWorkspace,"View Communities",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCommunitiesCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Communities");
	if(!Dlg.Run(new QGroupProfiles()))
		return;
	Doc->updateAllViews(otCommunity);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPostCommunities(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Post-Communities Analyze");
	if(!Dlg.Run(new QPostGroupProfiles()))
		return;
	Doc->updateAllViews(otCommunity);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowTopics(void)
{
	createClient(Doc,new KViewTopics(Doc,pWorkspace,"View Topics",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTopicsCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Topics");
	if(!Dlg.Run(new QGroupDocs()))
		return;
	Doc->updateAllViews(otTopic);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPostTopics(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Post-Topics Analyze");
	if(!Dlg.Run(new QPostGroupDocs()))
		return;
	Doc->updateAllViews(otTopic);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSimulationDlg(void)
{
	QSimulationDlg dlg(this);

	dlg.NbOK->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbOK","Subjects"));
	dlg.RelOK->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelOK","Subjects"));
	dlg.NbKO->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbKO","Subjects"));
	dlg.RelKO->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelKO","Subjects"));
	dlg.NbH->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbH","Subjects"));
	dlg.RelH->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelH","Subjects"));
	dlg.PercErr->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("PercErr","Subjects"));
	dlg.NbProfMin->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbProfMin","Subjects"));
	dlg.NbProfMax->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbProfMax","Subjects"));
	dlg.PercSocial->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("PercSocial","Subjects"));
	dlg.NbSubjects->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbSubjects","Subjects"));
	dlg.RelSubjects->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelSubjects","Subjects"));
	dlg.NbMinDocsSubject->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbMinDocsSubject","Subjects"));
	dlg.NbDocsAssess->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbDocsAssess","Subjects"));
	dlg.SwitchPerc->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("SwitchPerc","Subjects"));
	if(dlg.exec())
	{
		GALILEIApp->GetGALILEIConfig()->SetDouble("NbOK",dlg.NbOK->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetBool("RelOK",dlg.RelOK->isChecked(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("NbKO",dlg.NbKO->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetBool("RelKO",dlg.RelKO->isChecked(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("NbH",dlg.NbH->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetBool("RelH",dlg.RelH->isChecked(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("PercErr",dlg.PercErr->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMin",dlg.NbProfMin->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMax",dlg.NbProfMax->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("PercSocial",dlg.PercSocial->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("NbSubjects",dlg.NbSubjects->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetBool("RelSubjects",dlg.RelSubjects->isChecked(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbMinDocsSubject",dlg.NbMinDocsSubject->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbDocsAssess",dlg.NbDocsAssess->value(),"Subjects");
		GALILEIApp->GetGALILEIConfig()->SetDouble("SwitchPerc",dlg.SwitchPerc->value(),"Subjects");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSubjectsCreate(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Create Ideal Subjects");
	Dlg.Run(new QCreateIdealSubjects());
	Doc->updateAllViews(otCommunity);
	Doc->updateAllViews(otUser);
	Doc->updateAllViews(otDoc);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCommunitiesCreate(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Create Ideal Communities");
	Dlg.Run(new QCreateIdealCommunities());
	Doc->updateAllViews(otCommunity);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTopicsCreate(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Create Ideal Topics");
	Dlg.Run(new QCreateIdealTopics());
	Doc->updateAllViews(otTopic);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDoFdbks(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Feedback Cycle");
	Dlg.Run(new QMakeFdbks());
	Doc->updateAllViews(otProfile);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDoAssessments(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Assessments Cycle");
	Dlg.Run(new QMakeAssessments());
	Doc->updateAllViews(otProfile);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCommunitiesCompare(void)
{
	if(Doc->GetSession()->GetSubjects())
		createClient(Doc,new KViewThCommunities(Doc,Doc->GetSession()->GetSubjects(),pWorkspace,"View Theoretical Communities",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTopicsCompare(void)
{
	if(Doc->GetSession()->GetSubjects())
		createClient(Doc,new KViewThTopics(Doc,Doc->GetSession()->GetSubjects(),pWorkspace,"View Theoretical Topics",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowDocs(void)
{
	createClient(Doc,new KViewDocs(Doc,pWorkspace,"View Documents",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocAnalyse(void)
{
	KViewDoc* m;

	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.*"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
//		try
		{
			createClient(Doc,m=new KViewDoc(url.path(),KMimeType::findByURL(url)->name(),Doc,pWorkspace,"View Document",0));
			m->AnalyseDocXML();
			slotWindowActivated(m);
		}
//		catch(GException& e)
//		{
//			QMessageBox::critical(this,"KGALILEICenter",e.GetMsg());
//		}
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsAnalyse(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Analyse Documents");
	if(!Dlg.Run(new QAnalyzeDocs()))
		return;
	Doc->updateAllViews(otDoc);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPostDocsAnalyse(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Post-Documents Analyse");
	if(!Dlg.Run(new QPostAnalyzeDocs()))
		return;
	Doc->updateAllViews(otDoc);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsIndexer(void)
{
	QString dir=KFileDialog::getExistingDirectory(QString::null,this,"Choose directory where to export the files");
	if(!dir.isEmpty())
	{
	 	KProgressDialog dlg(this,"Progress","Export documents");
		dlg.show();
		KApplication::kApplication()->processEvents();
		RString pre(dir.latin1());
		pre+="/doc";
		RCursor<GDoc> Docs=Doc->GetSession()->GetDocs();
		size_t nb;
		for(Docs.Start(),nb=0;!Docs.End();Docs.Next(),nb++)
		{
			if(dlg.wasCancelled())
				break;
			dlg.progressBar()->setProgress(static_cast<int>((nb*100)/Docs.GetNb()));
			KApplication::kApplication()->processEvents();
			if(!Docs()->GetLang())
				continue;
			RString name=pre+RString::Number(Docs()->GetId())+".txt";
			dlg.setLabel(name.Latin1());
			KApplication::kApplication()->processEvents();
			RTextFile file(name.Latin1());
			file.Open(RIO::Create);
			RCursor<GWeightInfo> Words(Docs()->GetInfos());
			for(Words.Start();!Words.End();Words.Next())
			{
				for(size_t i=lround(Words()->GetWeight())+1;--i;)
				{
					file<<Doc->GetSession()->GetStorage()->LoadConcept(Words()->GetId(),Words()->GetType());
				}
				file<<endl;
			}
			file.Close();
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCreateXML(void)
{
	KViewDoc* m = dynamic_cast<KViewDoc*>(pWorkspace->activeWindow());
	if(!m) return;
	m->CreateDocXML();
	slotWindowActivated(m);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSaveXML(void)
{
	int dlg;
	KURL url;

	KViewDoc* m = dynamic_cast<KViewDoc*>(pWorkspace->activeWindow());
	if(!m) return;
	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.docxml"), this, i18n("Save DocXML File..."));
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
	m->SaveDocXML(url.path());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAnalyseXML(void)
{
	KViewDoc* m = dynamic_cast<KViewDoc*>(pWorkspace->activeWindow());
	if(!m) return;
	m->AnalyseDocXML();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotQueryMetaEngine(void)
{
	if(!GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod(false))
	{
		QMessageBox::information(this," Error "," No Meta Engine Method selected!!");
		return;
	}
	createClient(Doc,new KViewMetaEngine(Doc,pWorkspace,"View Meta Engine Results",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotViewStatusBar(void)
{
	slotStatusMsg(i18n("Toggle the statusbar..."));
	if(!viewStatusBar->isChecked())
	{
		statusBar()->hide();
	}
	else
	{
		statusBar()->show();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::windowMenuAboutToShow(void)
{
	windowMenu->popupMenu()->clear();
	windowMenu->insert(windowCascade);
	windowMenu->insert(windowTile);
	if(pWorkspace->windowList().isEmpty())
	{
		windowCascade->setEnabled(false);
		windowTile->setEnabled(false);
	}
	else
	{
		windowCascade->setEnabled(true);
		windowTile->setEnabled(true);
	}
	windowMenu->popupMenu()->insertSeparator();

	QWidgetList windows = pWorkspace->windowList();
	for(int i=0;i<int(windows.count());++i)
	{
		int id = windowMenu->popupMenu()->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(),this,SLOT(windowMenuActivated(int)));
		windowMenu->popupMenu()->setItemParameter(id,i);
		windowMenu->popupMenu()->setItemChecked(id,pWorkspace->activeWindow()==windows.at(i));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::windowMenuActivated(int id)
{
	QWidget* w = pWorkspace->windowList().at( id );
	if(w)
		w->setFocus();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowTile(void)
{
	pWorkspace->tile();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowCascade(void)
{
	pWorkspace->cascade();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotStatusMsg(const QString& text)
{
	statusBar()->clear();
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSaveModifier(void)
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->SetSaveResults(sessionSave->isChecked());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otDoc);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otProfile);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCommunitiesClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otCommunity);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTopicsClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otTopic);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotConfigure(void)
{
	ConfigureDlg Dlg;
	Dlg.Init(this);
	if(Dlg.exec())
	{
		Dlg.Done(this);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRunProgram(void)
{
	QString tmpfile;
	char tmp[100];
	KViewPrg* o;

	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(ToQString(PrgPath),i18n("*.kprg|KGALILEICenter Programs"), this, i18n("Open File..."));
	if(url.isEmpty())
	{
		QMessageBox::critical(this,"KGALILEICenter","File could not be found");
		return;
	}
	KIO::NetAccess::download(url,tmpfile,this);
	strcpy(tmp,tmpfile);
	try
	{
		createClient(Doc,o=new KViewPrg(Doc,pWorkspace,tmp,0));
		o->run();
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
	KIO::NetAccess::removeTempFile( tmpfile );
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotHelpProgram(void)
{
	KViewPrg* o;

	KApplication::kApplication()->processEvents();
	try
	{
		createClient(Doc,o=new KViewPrg(Doc,pWorkspace,"",0));
		o->run();
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
		return;
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
		return;
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
		return;
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
		return;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowActivated(QWidget*)
{
	if(dynamic_cast<KView*>(pWorkspace->activeWindow()))
		setCaption(pWorkspace->activeWindow()->caption());
	else
		setCaption("KGALILEICenter");

	// Update menu
	if(dynamic_cast<KViewUsers*>(pWorkspace->activeWindow()))
	{
		createXML->setEnabled(false);
		saveXML->setEnabled(false);
		analyseXML->setEnabled(false);
	}
	else if(dynamic_cast<KViewDocs*>(pWorkspace->activeWindow()))
	{
		createXML->setEnabled(false);
		saveXML->setEnabled(false);
		analyseXML->setEnabled(false);
	}
	else if(dynamic_cast<KViewDoc*>(pWorkspace->activeWindow()))
	{
		createXML->setEnabled(!(((KViewDoc*)pWorkspace->activeWindow())->IsDocXML()));
		saveXML->setEnabled(((KViewDoc*)pWorkspace->activeWindow())->IsDocXML());
		analyseXML->setEnabled(((KViewDoc*)pWorkspace->activeWindow())->IsDocXML());
	}
	else if(dynamic_cast<KViewProfile*>(pWorkspace->activeWindow()))
	{
		createXML->setEnabled(false);
		saveXML->setEnabled(false);
		analyseXML->setEnabled(false);
	}
	else if(dynamic_cast<KViewCommunities*>(pWorkspace->activeWindow()))
	{
		saveXML->setEnabled(false);
		createXML->setEnabled(false);
		analyseXML->setEnabled(false);
	}
	else
	{
		createXML->setEnabled(false);
		saveXML->setEnabled(false);
		analyseXML->setEnabled(false);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotHandleItem(QListViewItem* item)
{
	QListViewItemType* obj=(QListViewItemType*)item;

	switch(obj->Type)
	{
		case otProfile:
			createClient(Doc,new KViewProfile(obj->Obj.Profile,Doc,pWorkspace,"View Profile",0));
			break;

		case otDoc:
			createClient(Doc,new KViewDoc(obj->Obj.Doc,Doc,pWorkspace,"View Document",0));
			break;

		case otCommunity:
			createClient(Doc,new KViewCommunity(obj->Obj.Community,Doc,pWorkspace,"View Community",0));
			break;

		case otTopic:
			createClient(Doc,new KViewTopic(obj->Obj.Topic,Doc,pWorkspace,"View Topic",0));
			break;

		case otNoClass:
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSeeDicts(void)
{
	createClient(Doc,new KViewDicts(Doc,pWorkspace,"View Dictionaries",0));
}


//-----------------------------------------------------------------------------
KGALILEICenterApp::~KGALILEICenterApp(void)
{
	try
	{
		delete Debug;
		if(Doc)
			delete Doc;
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter - Undefined Error","Problem");
	}
}
