/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	(C) 2001-2002 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
// include files for R Project
#include <rxml/rxmlfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <docs/gdocoptions.h>
#include <sessions/gsessionmysql.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofilecalcvector.h>
#include <profiles/gprofilecalcfeedback.h>
#include <profiles/gprofilecalcreweighting.h>
#include <groups/ggroupingsim.h>
#include <groups/ggroupinggga.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qprinter.h>
#include <qmessagebox.h>
#include <qworkspace.h>
#include <qlineedit.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kapp.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kio/job.h>
#include <kio/netaccess.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewdocs.h"
#include "kviewdoc.h"
#include "kviewusers.h"
#include "kviewthgroups.h"
#include "kviewgroups.h"
#include "kviewgroup.h"
#include "kviewstat.h"
#include "kviewprg.h"
#include "kviewstems.h"
#include "kviewprofile.h"
#include "kviewga.h"
#include "qconnectmysql.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionConnect(void)
{
	QConnectMySQL dlg(this,0,true);
	QString method;
	GSessionMySQL* Sess;

	dlg.txtDb->setText(dbName());
	dlg.txtLogin->setText(dbUser());
	dlg.txtPwd->setText(dbPwd());
	dlg.txtHost->setText(dbHost());
	if(dlg.exec())
	{
		slotStatusMsg(i18n("Connecting..."));
		dbName=dlg.txtDb->text().latin1();
		dbHost=dlg.txtHost->text().latin1();
		dbUser=dlg.txtLogin->text().latin1();
		dbPwd=dlg.txtPwd->text().latin1();
		try
		{
			Sess = new GSessionMySQL(dbHost,dbUser,dbPwd,dbName,this);
			unsigned int cmd=dlg.cbLoad->currentItem();
			QSessionProgressDlg* d=new QSessionProgressDlg(this,Sess,"Loading from Database");
			d->LoadSession(cmd);
			Doc=new KDoc(this,Sess);
			Sess->RegisterProfileDesc(new GSubProfileDesc("Vector space",GSubProfileVector::NewSubProfile));
			Sess->RegisterComputingMethod(new GProfileCalcVector(Sess));
			Sess->RegisterComputingMethod(new GProfileCalcFeedback(Sess));
			Sess->RegisterComputingMethod(new GProfileCalcReWeighting(Sess));
			Sess->RegisterGroupingMethod(new GGroupingSim(Sess));
			Sess->RegisterGroupingMethod(new GGroupingGGA(Sess));
			Config->setGroup("Session Options");
			method=Config->readEntry("Description Method","Vector space");
			Sess->SetCurrentProfileDesc(method);
			method=Config->readEntry("Grouping Method","First-Fit Heuristic");
			Sess->SetCurrentGroupingMethod(method);
			method=Config->readEntry("Computing Method","Statistical");
			Sess->SetCurrentComputingMethod(method);
			Config->setGroup("Computing Options");
			GProfileCalcCursor Computings=Sess->GetComputingsCursor();
			for(Computings.Start();!Computings.End();Computings.Next())
				Computings()->SetSettings(Config->readEntry(Computings()->GetComputingName(),""));
			Config->setGroup("Grouping Options");
			GGroupingCursor Groupings=Sess->GetGroupingsCursor();
			for(Groupings.Start();!Groupings.End();Groupings.Next())
				Groupings()->SetSettings(Config->readEntry(Groupings()->GetGroupingName(),""));
			sessionDisconnect->setEnabled(true);
			sessionCompute->setEnabled(true);
			sessionConnect->setEnabled(false);
			RImportDC->setEnabled(true);
			RImportG->setEnabled(true);
			RExportDM->setEnabled(true);
			RExportP->setEnabled(true);
			textFrench->setEnabled(true);
			textEnglish->setEnabled(true);
			plugins->setEnabled(true);
			runProgram->setEnabled(true);
			UpdateMenusEntries();
			dbStatus->setPixmap(QPixmap("/usr/share/icons/hicolor/16x16/actions/connect_established.png"));
		}
		catch(GException& e)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
			if(Doc)
			{
				delete Doc;
				Doc=0;
			}
		}
		catch(RMySQL::RMySQLError& e)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString(e.GetError()));
			if(Doc)
			{
				delete Doc;
				Doc=0;
			}
		}
		slotStatusMsg(i18n("Ready"));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Complete Session");
	d->ComputeAll(!sessionAlwaysCalc->isChecked());
	Doc->updateAllViews(0);
	Doc->updateAllViews(1);
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDisconnect(void)
{
	if(Doc)
	{
		Config->setGroup("Session Options");
		Config->writeEntry("Description Method",Doc->GetSession()->GetCurrentProfileDesc()->GetProfDescName());
		Config->writeEntry("Grouping Method",Doc->GetSession()->GetCurrentGroupingMethod()->GetGroupingName());
		Config->writeEntry("Computing Method",Doc->GetSession()->GetCurrentComputingMethod()->GetComputingName());
		Config->setGroup("Computing Options");
		GProfileCalcCursor Computings=Doc->GetSession()->GetComputingsCursor();
		for(Computings.Start();!Computings.End();Computings.Next())
			Config->writeEntry(Computings()->GetComputingName(),Computings()->GetSettings());
		Config->setGroup("Grouping Options");
		GGroupingCursor Groupings=Doc->GetSession()->GetGroupingsCursor();
		for(Groupings.Start();!Groupings.End();Groupings.Next())
			Config->writeEntry(Groupings()->GetGroupingName(),Groupings()->GetSettings());
		Doc->closeDocument();
		delete Doc;
		Doc=0;
	}
	DisableAllActions();
	sessionConnect->setEnabled(true);
	sessionDisconnect->setEnabled(false);
	sessionCompute->setEnabled(false);
	runProgram->setEnabled(false);
	textFrench->setEnabled(false);
	textEnglish->setEnabled(false);
	plugins->setEnabled(false);
	statusBar()->changeItem("Not Connected !",1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionTest(void)
{
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionQuit(void)
{
	slotStatusMsg(i18n("Exiting..."));
	if(Doc)
	{
		Config->setGroup("Session Options");
		Config->writeEntry("Description Method",Doc->GetSession()->GetCurrentProfileDesc()->GetProfDescName());
		Config->writeEntry("Grouping Method",Doc->GetSession()->GetCurrentGroupingMethod()->GetGroupingName());
		Config->writeEntry("Computing Method",Doc->GetSession()->GetCurrentComputingMethod()->GetComputingName());
		Config->setGroup("Computing Options");
		GProfileCalcCursor Computings=Doc->GetSession()->GetComputingsCursor();
		for(Computings.Start();!Computings.End();Computings.Next())
			Config->writeEntry(Computings()->GetComputingName(),Computings()->GetSettings());
		Config->setGroup("Grouping Options");
		GGroupingCursor Groupings=Doc->GetSession()->GetGroupingsCursor();
		for(Groupings.Start();!Groupings.End();Groupings.Next())
			Config->writeEntry(Groupings()->GetGroupingName(),Groupings()->GetSettings());
		Doc->closeDocument();
		delete Doc;
		Doc=0;
	}
	saveOptions();
	close();
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowUsers(void)
{
	createClient(Doc,new KViewUsers(Doc,pWorkspace,"View users",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfileCalc(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gProfile) return;
	setDocParams(Doc);
	((KViewProfile*)m)->ComputeProfile();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesCalc(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Documents");
	d->ComputeProfiles(!profileAlwaysCalc->isChecked());
	Doc->updateAllViews(1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowGroups(void)
{
	createClient(Doc,new KViewGroups(Doc,pWorkspace,"View Groups",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsCalc(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Make Groups");
	d->GroupProfiles(!groupAlwaysCalc->isChecked());
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupingCompare(void)
{
	slotStatusMsg(i18n("Opening file..."));
	setDocParams(Doc);
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.grp|Ideal Groupement files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewThGroups(Doc,url.path(),pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowDocs(void)
{
	createClient(Doc,new KViewDocs(Doc,pWorkspace,"View Docs",0));
	addJudgement->setEnabled(true);
	editJudgement->setEnabled(true);
}



//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsAnalyse(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Documents");
	d->AnalyseDocs(!docAlwaysCalc->isChecked());
	Doc->updateAllViews(0);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsStats(void)
{
	setDocParams(Doc);
//	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Documents");
//	d->AnalyseDocs(!docAlwaysCalc->isChecked());
//	Doc->updateAllViews(0);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCreateXML(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	setDocParams(Doc);
	((KViewDoc*)m)->CreateDocXML();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAnalyseXML(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	setDocParams(Doc);
	((KViewDoc*)m)->AnalyseDocXML();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTextFrench(void)
{
	slotStatusMsg(i18n("Opening file..."));
	setDocParams(Doc);
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.mm|MMorph dictionnary files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewStems("fr",url.path(),Doc,pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTextEnglish(void)
{
	slotStatusMsg(i18n("Opening file..."));
	setDocParams(Doc);
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.mm|MMorph dictionnary files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewStems("en",url.path(),Doc,pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGAInit(void)
{
	KApplication::kApplication()->processEvents();
	setDocParams(Doc);
	createClient(Doc,new KViewGA(Doc,"en",16,pWorkspace,"Genetic Algorithm",0));
	gaPause->setEnabled(true);
	gaStart->setEnabled(true);
	gaStop->setEnabled(true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGAStart(void)
{
	KApplication::kApplication()->processEvents();
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==gGA))
	{
		((KViewGA*)m)->RunGA();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGAPause(void)
{
	KApplication::kApplication()->processEvents();
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==gGA))
	{
		((KViewGA*)m)->PauseGA();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGAStop(void)
{
	KApplication::kApplication()->processEvents(1000);
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==gGA))
	{
		((KViewGA*)m)->StopGA();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotViewToolBar(void)
{
	slotStatusMsg(i18n("Toggle the toolbar..."));
	if(!viewToolBar->isChecked())
	{
		toolBar("mainToolBar")->hide();
	}
	else
	{
		toolBar("mainToolBar")->show();
	}
	slotStatusMsg(i18n("Ready."));
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
void KGALILEICenterApp::slotRunProgram()
{
 	QString tmpfile;
	char tmp[100];
	KViewPrg* o;

	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL("/home/pfrancq",i18n("*.kprg|KGALILEICenter Programs"), this, i18n("Open File..."));
	if(url.isEmpty())
	{
		QMessageBox::critical(this,"KGALILEICenter","File could not be found");
		return;
	}
	KIO::NetAccess::download(url,tmpfile);
	strcpy(tmp,tmpfile);
	try
	{
		createClient(Doc,o=new KViewPrg(Doc,pWorkspace,tmp,0));
		KApplication::kApplication()->processEvents();
		Doc->GetSession()->RunPrg(o,tmp);
 		QMessageBox::information(this,"KGALILEICenter","Program Executed");
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
	}
	catch(RMySQL::RMySQLError& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString(e.GetError()));
	}
	KIO::NetAccess::removeTempFile( tmpfile );
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowActivated(QWidget*)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m)
	{
		// Update caption
		setCaption(m->caption());

		// Update menu
		switch(m->getType())
		{
			case gUsers:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(true);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;

			case gDocs:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;

			case gDoc:
				createXML->setEnabled(true);
				analyseXML->setEnabled(true);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(true);
				editJudgement->setEnabled(true);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;

			case gProfile:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(true);
				addJudgement->setEnabled(true);
				editJudgement->setEnabled(true);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;

			case gGroups:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;

			case gGA:
				addProfile->setEnabled(false);
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				gaStart->setEnabled(true);
				gaPause->setEnabled(true);
				gaStop->setEnabled(true);

			case gNothing:
			default:
				addProfile->setEnabled(false);
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				gaStart->setEnabled(false);
				gaPause->setEnabled(false);
				gaStop->setEnabled(false);
				break;
		}
	}
	else
	{
		setCaption("");
		addProfile->setEnabled(false);
		createXML->setEnabled(false);
		analyseXML->setEnabled(false);
		profileCalc->setEnabled(false);
		addJudgement->setEnabled(false);
		editJudgement->setEnabled(false);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotHandleItem(QListViewItem* item)
{
	QListViewItemType* obj=(QListViewItemType*)item;

	switch(obj->Type)
	{
		case QListViewItemType::tProfile:
			createClient(Doc,new KViewProfile(obj->Obj.Profile,Doc,pWorkspace,"View Profile",0));
			break;

		case QListViewItemType::tDocument:
			createClient(Doc,new KViewDoc(obj->Obj.Doc,Doc,pWorkspace,"View Document",0));
			break;

		case QListViewItemType::tGroup:
			createClient(Doc,new KViewGroup(obj->Obj.Group,Doc,pWorkspace,"View Group",0));
			break;

		case QListViewItemType::tNothing:
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRImportDC(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.dc|Document Concept"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		Doc->GetSession()->ImportDC(url.path().latin1());	
	}
	slotStatusMsg(i18n("Ready."));
	slotSessionDisconnect();
	slotSessionConnect();

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRImportG(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.g|Groupment"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		Doc->GetSession()->ImportG(url.path().latin1());	
	}
	slotStatusMsg(i18n("Ready."));
	slotSessionDisconnect();
	slotSessionConnect();
}

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRExportDM(void)
{
	slotStatusMsg(i18n("Saving file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getSaveURL(QString::null,i18n("*.dm|Document Mot"), this, i18n("Save File..."));
	if(!url.isEmpty())
	{
		Doc->GetSession()->ExportDM(url.path().latin1());
	}
}

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRExportP(void)
{
	slotStatusMsg(i18n("Saving file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getSaveURL(QString::null,i18n("*.p|Profile"), this, i18n("Save File..."));
	if(!url.isEmpty())
	{
		Doc->GetSession()->ExportP(url.path().latin1());
	}
	
}

//-----------------------------------------------------------------------------
KGALILEICenterApp::~KGALILEICenterApp(void)
{
	delete Printer;
	if(Doc)
		delete Doc;
	if(DocOptions)
		delete(DocOptions);
}
