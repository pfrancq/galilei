/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <pwd.h>
#include <unistd.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxml/rxmlfile.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
using namespace RIO;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <docs/gdocoptions.h>
#include <sessions/gsessionmysql.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofilecalcvector.h>
#include <profiles/gprofilecalcfeedback.h>
#include <profiles/gprofilecalcreweighting.h>
#include <groups/ggroupingsim.h>
#include <groups/ggroupinggga.h>
#include <groups/ggroupingkcos.h>
#include <groups/ggroupingkprotos.h>
#include <groups/ggroup.h>
#include <groups/gidealgroup.h>
#include <groups/ggroupcalcgravitation.h>
#include <groups/ggroupcalcrelevant.h>
#include <groups/gsubject.h>
#include <groups/gsubjecttree.h>
#include <tests/gmixidealgroups.h>
#include <langs/glang.h>
#include <langs/gdict.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qprinter.h>
#include <qmessagebox.h>
#include <qworkspace.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>


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
#include "kviewgroupsevaluation.h"
#include "kviewevaluategroup.h"
#include "kviewstat.h"
#include "kviewquery.h"
#include "kviewprg.h"
#include "kviewstems.h"
#include "kviewprofile.h"
#include "kviewga.h"
#include "kviewr.h"
#include "kviewchromos.h"
#include "qconnectmysql.h"
#include "qsessionprogress.h"
#include "qlanguages.h"
#include "qviewchromos.h"
#include "qmixidealconfig.h"



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
			Sess->RegisterGroupingMethod(new GGroupingKCos(Sess));
//			Sess->RegisterGroupingMethod(new GGroupingKProtos(Sess));
			Sess->RegisterGroupCalcMethod(new GGroupCalcGravitation(Sess));
			Sess->RegisterGroupCalcMethod(new GGroupCalcRelevant(Sess));
			Config->setGroup("Session Options");
			method=Config->readEntry("Description Method","Vector space");
			Sess->SetCurrentProfileDesc(method);
			method=Config->readEntry("Grouping Method","First-Fit Heuristic");
			Sess->SetCurrentGroupingMethod(method);
			method=Config->readEntry("Computing Method","Statistical");
			Sess->SetCurrentComputingMethod(method);
			method=Config->readEntry("Group Description Method","Relevant SubProfile");
			Sess->SetCurrentGroupCalcMethod(method);
			Config->setGroup("Computing Options");
			GProfileCalcCursor Computings=Sess->GetComputingsCursor();
			for(Computings.Start();!Computings.End();Computings.Next())
				Computings()->SetSettings(Config->readEntry(Computings()->GetComputingName(),""));
			Config->setGroup("Grouping Options");
			GGroupingCursor Groupings=Sess->GetGroupingsCursor();
			for(Groupings.Start();!Groupings.End();Groupings.Next())
				Groupings()->SetSettings(Config->readEntry(Groupings()->GetGroupingName(),""));
			Config->setGroup("Group Description Options");
			GGroupCalcCursor GroupCalcs=Sess->GetGroupCalcsCursor();
			for(GroupCalcs.Start();!GroupCalcs.End();GroupCalcs.Next())
				GroupCalcs()->SetSettings(Config->readEntry(GroupCalcs()->GetComputingName(),""));
			sessionDisconnect->setEnabled(true);
			sessionCompute->setEnabled(true);
			sessionConnect->setEnabled(false);
 			rRunR->setEnabled(true);
			textFrench->setEnabled(true);
			textEnglish->setEnabled(true);
			plugins->setEnabled(true);
			gaAnalyse->setEnabled(true);
			runProgram->setEnabled(true);
			sessionLoadIdealGroupInGroups->setEnabled(true);
			runInsts->setEnabled(true);
			runQuery->setEnabled(true);
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
void KGALILEICenterApp::slotSessionAutoConnect(const char* host,const char* user,const char* passwd,const char* db)
{
	QConnectMySQL dlg(this,0,true);
	QString method;
	GSessionMySQL* Sess;
	Sess = new GSessionMySQL(host,user,passwd,db,this);
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
	Sess->RegisterGroupingMethod(new GGroupingKCos(Sess));
//	Sess->RegisterGroupingMethod(new GGroupingKProtos(Sess));
	Sess->RegisterGroupCalcMethod(new GGroupCalcGravitation(Sess));
	Sess->RegisterGroupCalcMethod(new GGroupCalcRelevant(Sess));
	Config->setGroup("Session Options");
	method=Config->readEntry("Description Method","Vector space");
	Sess->SetCurrentProfileDesc(method);
	method=Config->readEntry("Grouping Method","First-Fit Heuristic");
	Sess->SetCurrentGroupingMethod(method);
	method=Config->readEntry("Computing Method","Statistical");
	Sess->SetCurrentComputingMethod(method);
	method=Config->readEntry("Group Description Method","Relevant SubProfile");
	Sess->SetCurrentGroupCalcMethod(method);
	Config->setGroup("Computing Options");
	GProfileCalcCursor Computings=Sess->GetComputingsCursor();
	for(Computings.Start();!Computings.End();Computings.Next())
		Computings()->SetSettings(Config->readEntry(Computings()->GetComputingName(),""));
	Config->setGroup("Grouping Options");
	GGroupingCursor Groupings=Sess->GetGroupingsCursor();
	for(Groupings.Start();!Groupings.End();Groupings.Next())
		Groupings()->SetSettings(Config->readEntry(Groupings()->GetGroupingName(),""));
	Config->setGroup("Group Description Options");
	GGroupCalcCursor GroupCalcs=Sess->GetGroupCalcsCursor();
	for(GroupCalcs.Start();!GroupCalcs.End();GroupCalcs.Next())
		GroupCalcs()->SetSettings(Config->readEntry(GroupCalcs()->GetComputingName(),""));
	sessionDisconnect->setEnabled(true);
	sessionCompute->setEnabled(true);
	sessionConnect->setEnabled(false);
 	rRunR->setEnabled(true);
	textFrench->setEnabled(true);
	textEnglish->setEnabled(true);
	plugins->setEnabled(true);
	gaAnalyse->setEnabled(true);
	runProgram->setEnabled(true);
	runInsts->setEnabled(true);
	runQuery->setEnabled(true);
	UpdateMenusEntries();
	dbStatus->setPixmap(QPixmap("/usr/share/icons/hicolor/16x16/actions/connect_established.png"));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Complete Session");
	d->ComputeAll(!sessionAlwaysCalc->isChecked(),profileAlwaysSave->isChecked()||groupAlwaysSave->isChecked());
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
		Config->writeEntry("Group Description Method",Doc->GetSession()->GetCurrentGroupCalcMethod()->GetComputingName());
		Config->setGroup("Computing Options");
		GProfileCalcCursor Computings=Doc->GetSession()->GetComputingsCursor();
		for(Computings.Start();!Computings.End();Computings.Next())
			Config->writeEntry(Computings()->GetComputingName(),Computings()->GetSettings());
		Config->setGroup("Grouping Options");
		GGroupingCursor Groupings=Doc->GetSession()->GetGroupingsCursor();
		for(Groupings.Start();!Groupings.End();Groupings.Next())
			Config->writeEntry(Groupings()->GetGroupingName(),Groupings()->GetSettings());
		Config->setGroup("Group Description Options");
		GGroupCalcCursor GroupCalcs=Doc->GetSession()->GetGroupCalcsCursor();
		for(GroupCalcs.Start();!GroupCalcs.End();GroupCalcs.Next())
			Config->writeEntry(GroupCalcs()->GetComputingName(),GroupCalcs()->GetSettings());
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
	gaAnalyse->setEnabled(false);
	statusBar()->changeItem("Not Connected !",1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionTest(void)
{
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotLoadIdealGroupInGroups(void)
{
	Doc->GetSession()->LoadIdealGroupmentInGroups();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionStat(void)
{
//	slotSessionAutoConnect("127.0.0.1","root","","lemonde");
//	cout<<"hplemonde 1 doc 1 occur max"<<endl;
//	Doc->GetSession()->DocsFilter(1,1);
//	GIdealGroup judgmentlemonde (Doc->GetSession());
//	slotProfilesCalc();
//	createClient(Doc,new KViewEvaluateGroup(Doc,pWorkspace,"View Stats About Groups",0));
//	slotSessionDisconnect();
//
//	slotSessionAutoConnect("127.0.0.1","root","","lemonde");
//	cout<<"hplemonde 1 doc 2 occur max"<<endl;
//	Doc->GetSession()->DocsFilter(1,2);
//	slotProfilesCalc();
//	createClient(Doc,new KViewEvaluateGroup(Doc,pWorkspace,"View Stats About Groups",0));
//	slotSessionDisconnect();

//	slotSessionAutoConnect("127.0.0.1","root","","hp1500");
//	cout<<"hp1500 1 doc 1 occur max"<<endl;
//	Doc->GetSession()->DocsFilter(1,1);
//	GIdealGroup judgment1500 (Doc->GetSession());
//	judgment1500.SetSettings("10 10 12345 2 0 100");
//	judgment1500.CreateJudgement(parent,idealgroup,true);

//	slotSessionAutoConnect("127.0.0.1","root","","lemonde");
//	cout<<"hplemonde 1 doc 1 occur max"<<endl;
//	Doc->GetSession()->DocsFilter(1,1);
//	GIdealGroup judgmentlemonde (Doc->GetSession());
//	judgmentlemonde.SetSettings("10 10 12345 2 0 100");
//	judgmentlemonde.CreateJudgement(parent,idealgroup,true);
//	slotProfilesCalc();
//	createClient(Doc,new KViewEvaluateGroup(Doc,pWorkspace,"View Stats About Groups",0));
//	slotSessionDisconnect();
//
//	slotSessionAutoConnect("127.0.0.1","root","","lemonde");
//	cout<<"hplemonde 1 doc 2 occur max"<<endl;
//	Doc->GetSession()->DocsFilter(1,2);
//	slotProfilesCalc();
//	createClient(Doc,new KViewEvaluateGroup(Doc,pWorkspace,"View Stats About Groups",0));
//	slotSessionDisconnect();
////
//	slotSessionAutoConnect("127.0.0.1","root","","hp20000");
//	cout<<"hp20000 1 doc 1 occur max"<<endl;
////	Doc->GetSession()->DocsFilter(1,1);
//	GIdealGroup judgment20000 (Doc->GetSession());
//	judgment20000.SetSettings("10 10 12345 2 0 100");
//	judgment20000.CreateJudgement(parent,idealgroup,true);
//	slotProfilesCalc();
//	createClient(Doc,new KViewEvaluateGroup(Doc,pWorkspace,"View Stats About Groups",0));
//	slotSessionDisconnect();

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
		Config->writeEntry("Group Description Method",Doc->GetSession()->GetCurrentGroupCalcMethod()->GetComputingName());
		Config->setGroup("Computing Options");
		GProfileCalcCursor Computings=Doc->GetSession()->GetComputingsCursor();
		for(Computings.Start();!Computings.End();Computings.Next())
			Config->writeEntry(Computings()->GetComputingName(),Computings()->GetSettings());
		Config->setGroup("Grouping Options");
		GGroupingCursor Groupings=Doc->GetSession()->GetGroupingsCursor();
		for(Groupings.Start();!Groupings.End();Groupings.Next())
			Config->writeEntry(Groupings()->GetGroupingName(),Groupings()->GetSettings());
		Config->setGroup("Group Description Options");
		GGroupCalcCursor GroupCalcs=Doc->GetSession()->GetGroupCalcsCursor();
		for(GroupCalcs.Start();!GroupCalcs.End();GroupCalcs.Next())
			Config->writeEntry(GroupCalcs()->GetComputingName(),GroupCalcs()->GetSettings());
		Doc->closeDocument();
		delete Doc;
		Doc=0;
	}
	saveOptions();
	slotStatusMsg(i18n("Ready."));
	close();
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
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Profiles");
	d->ComputeProfiles(!profileAlwaysCalc->isChecked(),profileAlwaysSave->isChecked());
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
	d->GroupProfiles(!groupAlwaysCalc->isChecked(),groupAlwaysSave->isChecked());
	Doc->GetSession()->SaveGroups();
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
void KGALILEICenterApp::slotGroupsEvaluation(void)
{
	createClient(Doc,new KViewGroupsEvaluation(Doc,pWorkspace,"View Docs",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowDocs(void)
{
	createClient(Doc,new KViewDocs(Doc,pWorkspace,"View Docs",0));
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
		createClient(Doc,m=new KViewDoc(url.path(),KMimeType::findByURL(url)->name(),Doc,pWorkspace,"View Document",0));
		m->CreateDocXML();
		slotWindowActivated(m);
	}
	slotStatusMsg(i18n("Ready."));
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
	slotWindowActivated(m);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSaveXML(void)
{
	int dlg;
	KURL url;

	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.docxml"), this, i18n("Save DocXML File..."));
		if(url.isEmpty()) return;
		QFile Test(url.path().latin1());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}
	((KViewDoc*)m)->SaveDocXML(url.path());
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
	QLanguages dlg(this,0,true);
	char tmp[3];

	KApplication::kApplication()->processEvents();
	if(dlg.exec())
	{
		if(dlg.cbLangs->currentItem()==0)
			strcpy(tmp,"en");
		else
			strcpy(tmp,"fr");
		setDocParams(Doc);
		createClient(Doc,new KViewGA(Doc,tmp,dlg.cbGlobal->isChecked(),dlg.cbScratch->isChecked(),pWorkspace,"Genetic Algorithm",0));
		gaPause->setEnabled(true);
		gaStart->setEnabled(true);
		gaStop->setEnabled(true);
	}
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
void KGALILEICenterApp::slotGASave(void)
{
	KApplication::kApplication()->processEvents(1000);
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m&&(m->getType()==gGA))
	{
		((KViewGA*)m)->SaveGA();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGAAnalyse(void)
{
	QViewChromos dlg(this,0,true);
	char tmp[3];

	KApplication::kApplication()->processEvents();
	if(dlg.exec())
	{
		if(dlg.cbLangs->currentItem()==0)
			strcpy(tmp,"en");
		else
			strcpy(tmp,"fr");
		setDocParams(Doc);
		createClient(Doc,new KViewChromos(Doc,tmp,dlg.cbGlobal->isChecked(),dlg.cbSim->isChecked(),pWorkspace,"View Chromosomes",0));
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
void KGALILEICenterApp::slotRunProgram(void)
{
	QString tmpfile;
	char tmp[100];
	KViewPrg* o;

	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString(getpwuid(getuid())->pw_dir)+QString("/galilei/prg"),i18n("*.kprg|KGALILEICenter Programs"), this, i18n("Open File..."));
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
void KGALILEICenterApp::slotRunQuery(void)
{
	createClient(Doc,new KViewQuery(Doc,pWorkspace,"Run Query",0));
}


//-----------------------------------------------------------------------------
//void KGALILEICenterApp::slotMixIdealGroups(void)
//{
//	GMixIdealGroups* mix;
//	GSubjectTree* subjecttree=new GSubjectTree(0, 0, 0);
//	RContainer<GGroups,unsigned int,true,true>* idealgroups;
//	idealgroups= new RContainer<GGroups,unsigned int,true,true>(5,2);
//	RContainer<GGroupIdParentId,unsigned int,true,true>* parent;
//	parent=new RContainer<GGroupIdParentId,unsigned int,true,true>(10,5);
//
//	Doc->GetSession()->LoadIdealGroupment(idealgroups);
//	Doc->GetSession()->LoadSubjectTree(subjecttree);
//	subjecttree->CreateParent(parent);
//	mix = new GMixIdealGroups(Doc->GetSession(), parent, idealgroups);
//	mix->Run();
//}

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotMixIdealGroups(void)
 {
	GMixIdealGroups* mix;
	const char *nbgroups, *level;
	bool random, mergesame, mergediff, split;
	GSubjectTree* subjecttree=new GSubjectTree(0, 0, 0);
	RContainer<GGroups,unsigned int,true,true>* idealgroups;
	RContainer<GGroupIdParentId,unsigned int,true,true>* parent;
	idealgroups= new RContainer<GGroups,unsigned int,true,true>(5,2);

	parent=new RContainer<GGroupIdParentId,unsigned int,true,true>(10,5);
	Doc->GetSession()->LoadIdealGroupment(idealgroups);
	Doc->GetSession()->LoadSubjectTree(subjecttree);
	subjecttree->CreateParent(parent);
	QMixIdealConfig dlg(this,0,true);
	if(dlg.exec())
	{
		slotStatusMsg(i18n("Connecting..."));
		nbgroups=dlg.LENbMixedGroups->text().latin1();
		level=dlg.LELevel->text().latin1();
		random=dlg.RBRandom->isChecked();
		mergesame=dlg.RBMergeSame->isChecked();
		mergediff=dlg.RBMergeDiff->isChecked();
		split=dlg.RBSplit->isChecked();
	}
	mix = new GMixIdealGroups(Doc->GetSession(), parent, idealgroups, atoi(nbgroups), atoi(level), mergesame, mergediff, split, random);
	mix->Run();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRunInsts(void)
{
	KApplication::kApplication()->processEvents();
	createClient(Doc,new KViewStat(Doc,pWorkspace,"Run Instructions",0));
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowActivated(QWidget*)
{
	KView* m = dynamic_cast<KView*>(pWorkspace->activeWindow());
	GViewType type;

	if(m)
	{
		// Update caption
		setCaption(m->caption());
		type=m->getType();
	}
	else
	{
		setCaption("");
		type=gNothing;
	}

	// Update menu
	switch(type)
	{
		case gUsers:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;

		case gDocs:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;

		case gDoc:
			createXML->setEnabled(!(((KViewDoc*)m)->IsDocXML()));
			saveXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			analyseXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			profileCalc->setEnabled(false);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;

		case gProfile:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(true);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;

		case gGroups:
			saveXML->setEnabled(false);
			createXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;

		case gGA:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			gaStart->setEnabled(true);
			gaPause->setEnabled(true);
			gaStop->setEnabled(true);
			gaSave->setEnabled(true);
			break;

		case gNothing:
		default:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			gaStart->setEnabled(false);
			gaPause->setEnabled(false);
			gaStop->setEnabled(false);
			gaSave->setEnabled(false);
			break;
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
void KGALILEICenterApp::slotRRunR(void)
{
	KApplication::kApplication()->processEvents();
	createClient(Doc,new KViewR(Doc,pWorkspace,"Run R",0));
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
