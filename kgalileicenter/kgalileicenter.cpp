/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation.

	(C) 2001 by Pascal Francq

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
#include <rstd/rcontainercursor.h>
using namespace RStd;
#include <rxml/rxmlfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <docs/gdocxml.h>
#include <urlmanagers/gurlmanagerkde.h>
#include <galilei/qgdocxml.h>
#include <galilei/qlistviewitemtype.h>
#include <sessions/gsessionmysql.h>
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofilecalcvector.h>
#include <profiles/gprofile.h>
#include <groups/ggroupingsim.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for GALILEIInterface
#include <python/galileiprogram.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qdir.h>
#include <qprinter.h>
#include <qvbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qfiledialog.h>
#include <qworkspace.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kapp.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>


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
#include "kviewprofile.h"
#include "qconnectmysql.h"
#include "qloadonedoc.h"
#include "qaddnewuser.h"
#include "qaddjudgementselectdoc.h"
#include "qaddjudgementselectprofile.h"
#include "qeditjudgementselectdoc.h"
#include "qeditjudgementselectprofile.h"
#include "qaddnewdocument.h"
#include "qaddprofile.h"
#include "qsessionprogress.h"
#include "qgroupsoptionsdlg.h"
#include "qdocsoptionsdlg.h"
#include "qprofilesoptionsdlg.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KGALILEICenterApp::KGALILEICenterApp(void)
	: KMainWindow(0,"KGALILEICenterApp"), GURLManagerKDE(), Doc(0)
{
	Config=kapp->config();
	Printer = new QPrinter;
	initStatusBar();
	initView();
	initActions();
	readOptions();

	// disable actions at startup
	DisableAllActions();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initActions(void)
{
	// Menu "Connect"
	sessionAlwaysCalc=new KToggleAction(i18n("Enables/disables session' Recomputing"),0,0,0,actionCollection(),"sessionAlwaysCalc");
	sessionConnect=new KAction(i18n("&Connect Database"),"connect_established",0,this,SLOT(slotSessionConnect()),actionCollection(),"sessionConnect");
	sessionCompute=new KAction(i18n("Compute &Session"),0,this,SLOT(slotSessionCompute()),actionCollection(),"sessionCompute");
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionTest=new KAction(i18n("&Test"),"gohome",0,this,SLOT(slotSessionTest()),actionCollection(),"sessionTest");
	sessionQuit=new KAction(i18n("E&xit"),"exit",0,this,SLOT(slotSessionQuit()),actionCollection(),"sessionQuit");

	// Menu "Users"
	profileAlwaysCalc=new KToggleAction(i18n("Enables/disables users' Recomputing"),0,0,0,actionCollection(),"profileAlwaysCalc");
	showUsers=new KAction(i18n("&Show Users"),"kdmconfig",0,this,SLOT(slotShowUsers()),actionCollection(),"showUsers");
	addUser=new KAction(i18n("Add &User"),0,this,SLOT(slotAddUser()),actionCollection(),"addUser");
	addProfile=new KAction(i18n("Add &Profile"),0,this,SLOT(slotAddProfile()),actionCollection(),"addProfile");
	profilesCalc=new KAction(i18n("&Calc Profiles"),"run",0,this,SLOT(slotProfilesCalc()),actionCollection(),"profilesCalc");
	profileCalc=new KAction(i18n("&Calc Profile"),"run",0,this,SLOT(slotProfileCalc()),actionCollection(),"profilecalc");


	// Menu "Groups"
	groupAlwaysCalc=new KToggleAction(i18n("Enables/disables groups' Recomputing"),0,0,0,actionCollection(),"groupAlwaysCalc");
	showGroups=new KAction(i18n("&Show Groups"),"window_list",0,this,SLOT(slotShowGroups()),actionCollection(),"showGroups");
	groupsCalc=new KAction(i18n("Compute &Groups"),"exec",0,this,SLOT(slotGroupsCalc()),actionCollection(),"groupsCalc");
	groupingCompare=new KAction(i18n("&Compare Grouping"),0,this,SLOT(slotGroupingCompare()),actionCollection(),"groupingCompare");

	// Menu "Document"
	docAlwaysCalc=new KToggleAction(i18n("Enables/disables documents' Recomputing"),0,0,0,actionCollection(),"docAlwaysCalc");
	showDocs=new KAction(i18n("&Show Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docsAnalyse=new KAction(i18n("&Analyse Documents"),0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");;
	docsStats=new KAction(i18n("S&tatistics about Documents"),0,this,SLOT(slotDocsStats()),actionCollection(),"docsStats");;
	addDoc=new KAction(i18n("Add &Document"),0,this,SLOT(slotAddDoc()),actionCollection(),"addDoc");
	addJudgement=new KAction(i18n("Add &Judgement"),0,this,SLOT(slotAddJudgement()),actionCollection(),"addJudgement");
	editJudgement=new KAction(i18n("&Edit Judgement"),0,this,SLOT(slotEditJudgement()),actionCollection(),"editJudgement");
	createXML=new KAction(i18n("&Create XML Structure"),"readme",0,this,SLOT(slotCreateXML()),actionCollection(),"createXML");
	analyseXML=new KAction(i18n("&Analyse XML Structure"),"filefind",0,this,SLOT(slotAnalyseXML()),actionCollection(),"analyseXML");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	docsOptions=new KAction(i18n("&Documents Options"),"configure",0,this,SLOT(slotDocsOptions()),actionCollection(),"docsOptions");
	groupsOptions=new KAction(i18n("&Groups Options"),"configure",0,this,SLOT(slotGroupsOptions()),actionCollection(),"groupsOptions");
	profilesOptions=new KAction(i18n("&Profiles' Options"),"configure",0,this,SLOT(slotProfilesOptions()),actionCollection(),"profilesOptions");

	// Menu "Program"
	loadProgram=new KAction(i18n("&Load Program"),"configure",0,this,SLOT(slotLoadProgram()),actionCollection(),"loadProgram");


	// Menu "Window"
	windowTile = new KAction(i18n("&Tile"), 0, this, SLOT(slotWindowTile()), actionCollection(),"window_tile");
	windowCascade = new KAction(i18n("&Cascade"), 0, this, SLOT(slotWindowCascade()), actionCollection(),"window_cascade");
	windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
	connect(windowMenu->popupMenu(),SIGNAL(aboutToShow()),this,SLOT(windowMenuAboutToShow()));

	createGUI();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initStatusBar(void)
{
	dbStatus= new QLabel(statusBar());
	dbStatus->setPixmap(QPixmap("/usr/share/icons/hicolor/16x16/actions/connect_no.png"));
	statusBar()->addWidget(dbStatus, 0, true );
	statusBar()->insertItem(i18n("Ready."),1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initView(void)
{
	QVBox* view_back = new QVBox(this);
	view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	pWorkspace = new QWorkspace(view_back);
	connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this, SLOT(slotWindowActivated(QWidget*)));
	setCentralWidget(view_back);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::setDocParams(KDoc* doc)
{
	if(!doc) return;
	doc->GetStat()->SetSize(ParamOKKOSize);
	doc->GetGroupingSim()->SetLevel(ParamLevelSim);
	doc->GetGroupingSim()->SetFull(ParamFullSim);
	doc->GetSession()->SetStaticLang(ParamStaticLang);
	doc->GetSession()->SetMinStopWords(ParamMinStop);
	doc->GetSession()->SetMinWordSize(ParamMinWordSize);
	doc->GetSession()->SetMinStemSize(ParamMinStemSize);
	doc->GetSession()->SetMinOccur(ParamMinOccur);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::createClient(KDoc* doc,KView* view)
{
	view->installEventFilter(this);
	doc->addView(view);
	view->show();
	view->resize(pWorkspace->sizeHint());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveOptions(void)
{
	Config->setGroup("General Options");
	Config->writeEntry("Geometry", size());
	Config->writeEntry("Show Toolbar", toolBar()->isVisible());
	Config->writeEntry("Show Statusbar",statusBar()->isVisible());
	Config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());

	Config->setGroup("Database Options");
	Config->writeEntry("Host", dbHost);
	Config->writeEntry("Name", dbName);
	Config->writeEntry("User", dbUser);
	Config->writeEntry("Password", dbPwd);

	Config->setGroup("GALILEI Options");
	Config->writeEntry("Size of OK/KO Lists",ParamOKKOSize);
	Config->writeEntry("Full Similarity",ParamFullSim);
	Config->writeEntry("Level of Similarity",ParamLevelSim);
	Config->writeEntry("Static Language",ParamStaticLang);
	Config->writeEntry("Minimum Stop Percentage",ParamMinStop);
	Config->writeEntry("Minimum Word's Size",ParamMinWordSize);
	Config->writeEntry("Minimum Stem's Size",ParamMinStemSize);
	Config->writeEntry("Minimum Occurence",ParamMinOccur);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readOptions(void)
{
	Config->setGroup("General Options");

	// bar status settings
	bool bViewToolbar = Config->readBoolEntry("Show Toolbar", true);
	viewToolBar->setChecked(bViewToolbar);
	slotViewToolBar();
	bool bViewStatusbar = Config->readBoolEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();

	// bar position settings
	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) Config->readNumEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);

	QSize size=Config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	Config->setGroup("Database Options");
	dbHost=Config->readEntry("Host","cfao20.ulb.ac.be");
	dbName=Config->readEntry("Name","hp");
	dbUser=Config->readEntry("User","admin");
	dbPwd=Config->readEntry("Password","gillian");

	Config->setGroup("GALILEI Options");
	ParamOKKOSize=Config->readUnsignedNumEntry("Size of OK/KO Lists",25);
	ParamFullSim=Config->readBoolEntry("Full Similarity",true);
	ParamLevelSim=Config->readDoubleNumEntry("Level of Similarity",0.5);
	ParamStaticLang=Config->readBoolEntry("Static Language",true);
	ParamMinStop=Config->readDoubleNumEntry("Minimum Stop Percentage",0.1);
	ParamMinWordSize=Config->readUnsignedNumEntry("Minimum Word's Size",4);
	ParamMinStemSize=Config->readUnsignedNumEntry("Minimum Stem's Size",3);
	ParamMinOccur=Config->readUnsignedNumEntry("Minimum Occurence",1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveProperties(KConfig* _cfg)
{

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readProperties(KConfig* _cfg)
{
}


//-----------------------------------------------------------------------------
bool KGALILEICenterApp::queryClose(void)
{
	return(true);
}


//-----------------------------------------------------------------------------
bool KGALILEICenterApp::queryExit(void)
{
	saveOptions();
	return(true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::UpdateMenusEntries(void)
{
	// Menu "Users"
	showUsers->setEnabled(Doc&&Doc->GetSession()->IsUsersLoad());
	profilesCalc->setEnabled(Doc&&Doc->GetSession()->IsUsersLoad()&&Doc->GetSession()->IsDocsLoad());
	addUser->setEnabled(Doc&&Doc->GetSession()->IsUsersLoad());

	// Menu "Groups"
	showGroups->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad());
	groupsCalc->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad()&&Doc->GetSession()->IsUsersLoad());
	groupingCompare->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad());

	// Menu "Document"
	showDocs->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad());
	docsAnalyse->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());
	docsStats->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());
	addDoc->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::DisableAllActions(void)
{
	sessionDisconnect->setEnabled(false);
	showUsers->setEnabled(false);
	showGroups->setEnabled(false);
	profileCalc->setEnabled(false);
	profilesCalc->setEnabled(false);
	sessionConnect->setEnabled(true);
	sessionCompute->setEnabled(false);
	groupingCompare->setEnabled(false);
	createXML->setEnabled(false);
	analyseXML->setEnabled(false);
	addProfile->setEnabled(false);
	addUser->setEnabled(false);
	groupsCalc->setEnabled(false);
	showDocs->setEnabled(false);
	addDoc->setEnabled(false);
	docsAnalyse->setEnabled(false);
	docsStats->setEnabled(false);
	addJudgement->setEnabled(false);
	editJudgement->setEnabled(false);
}


//-----------------------------------------------------------------------------
bool KGALILEICenterApp::eventFilter(QObject* object, QEvent* event)
{
	if((event->type() == QEvent::Close)&&((KGALILEICenterApp*)object!=this))
	{
		QCloseEvent* e=(QCloseEvent*)event;
		KView* pView=(KView*)object;
		KDoc* pDoc=pView->getDocument();
		if(pDoc->canCloseFrame(pView))
		{
			pDoc->removeView(pView);
			e->accept();
		}
		else
			e->ignore();
	}

	// standard event processing
	return (QWidget::eventFilter(object,event));
}


//-----------------------------------------------------------------------------
// Slots
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionConnect(void)
{
	QConnectMySQL dlg(this,0,true);
	dlg.txtDb->setText(dbName);
	dlg.txtLogin->setText(dbUser);
	dlg.txtPwd->setText(dbPwd);
	dlg.txtHost->setText(dbHost);
	if(dlg.exec())
	{
		slotStatusMsg(i18n("Connecting..."));
		dbName=dlg.txtDb->text().latin1();
		dbHost=dlg.txtHost->text().latin1();
		dbUser=dlg.txtLogin->text().latin1();
		dbPwd=dlg.txtPwd->text().latin1();
		GSessionMySQL* Sess = new GSessionMySQL(dbHost,dbUser,dbPwd,dbName,this);
		unsigned int cmd=dlg.cbLoad->currentItem();
		QSessionProgressDlg* d=new QSessionProgressDlg(this,Sess,"Loading from Database");
		d->LoadSession(cmd);
		Doc=new KDoc(this,Sess);
		sessionDisconnect->setEnabled(true);
		sessionCompute->setEnabled(true);
		sessionConnect->setEnabled(false);
		UpdateMenusEntries();
		dbStatus->setPixmap(QPixmap("/usr/share/icons/hicolor/16x16/actions/connect_established.png"));
		slotStatusMsg(i18n("Ready"));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	setDocParams(Doc);
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Complete Session");
	d->ComputeAll(sdVector,Doc->GetCurComputeProfile(),Doc->GetCurGrouping(),!sessionAlwaysCalc->isChecked());
	Doc->updateAllViews(0);
	Doc->updateAllViews(1);
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDisconnect(void)
{
	Doc->closeDocument();
	delete Doc;
	Doc=0;
	DisableAllActions();
	sessionConnect->setEnabled(true);
	sessionDisconnect->setEnabled(false);
	statusBar()->changeItem("Not Connected !",1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionTest(void)
{
//	GDocXML *s=0;
//	GSessionMySQL Sess(dbHost,dbUser,dbPwd,dbName,new GURLManagerKDE());
//	QSessionProgressDlg* d=new QSessionProgressDlg(this,&Sess,"Test");
//	GDoc doc("/home/pfrancq/docE24.html","Test",cNoRef,0,Sess.GetMIMEType("text/html"),0,0,0,0,0,0);
//	d->CreateDocXML(s,&doc);
//	RXMLFile f("/home/pfrancq/test.xml",s,RIO::RTextFile::Create);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionQuit(void)
{
	KMainWindow* w;

	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	if(memberList)
	{
		for(w=memberList->first();w!=0;w=memberList->first())
		{
			if(!w->close())
			break;
		}
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowUsers(void)
{
	createClient(Doc,new KViewUsers(Doc,pWorkspace,"View users",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddUser(void)
{

	QAddNewUser dlg(this,0,true);
	if(dlg.exec())
	{
		Doc->GetSession()->NewUser(dlg.LEUserName->text().latin1(),dlg.LEPassword->text().latin1(),dlg.LEFullname->text().latin1(),dlg.LEEmail->text().latin1(),
		                  dlg.LETitle->text().latin1(),dlg.LEOrganisation->text().latin1(),dlg.LEAddress1->text().latin1(),
		                  dlg.LEAddress2->text().latin1(),dlg.LECity->text().latin1(),dlg.LECountry->text().latin1());
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddProfile(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gUsers) return;
	QAddProfile dlg(this,0,true);
	if(dlg.exec())
	{
		Doc->GetSession()->NewProfile(((KViewUsers*)m)->GetCurrentUser(),dlg.Description->text().latin1());
	}
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
	d->ComputeProfiles(Doc->GetCurComputeProfile(),!profileAlwaysCalc->isChecked());
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
	d->GroupProfiles(sdVector,Doc->GetCurGrouping(),!groupAlwaysCalc->isChecked());
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
void KGALILEICenterApp::slotAddDoc(void)
{
	QAddNewDoc dlg(this,0,true);
	dlg.UrlTxt->setText(QString("http://cfao47.ulb.ac.be/"));
	if(dlg.exec())
	{
		switch (dlg.ComboBox1->currentItem())
		{
			case 0:
				Doc->GetSession()->NewDoc(dlg.UrlTxt->text().latin1(),dlg.NameTxt->text().latin1(),"text/html");
				break;
			case 1:
				Doc->GetSession()->NewDoc(dlg.UrlTxt->text().latin1(),dlg.NameTxt->text().latin1(),"text/email");
				break;
		}
	}
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
void KGALILEICenterApp::slotAddJudgement(void)
{
	// rajouter les 2 cas avec fenetre selectionne = gdocs et fenetre selectionne =  gusers
	KView* m = (KView*)pWorkspace->activeWindow();
	// if the current window is a profile
	if (m->getType()==gProfile)
	{
		QAddJudgementSelectDoc dlg(this,0,true);
		GProfile* profile =((KViewProfile*)m)->GetProfile();
		
		RContainerCursor<GDoc,unsigned int,true,true> CurDoc (Doc->GetSession()->GetDocs());
		dlg.TLTitle->setText(QString(profile->GetName()));
		dlg.TLURL->setText(QString(profile->GetUser()->GetName()));
		QListViewItem* docitem = new QListViewItem (dlg.LVUsers, "Doc");
		for (CurDoc.Start();!CurDoc.End();CurDoc.Next())
		{
			QListViewItemType* useritem = new QListViewItemType (CurDoc(),docitem, QString (CurDoc()->GetName()));
			docitem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}

		if(dlg.exec())
		{
			char fdbk;
			switch (dlg.CBJudgement->currentItem())
			{
				case 0:
					fdbk='O'; break;
				case 1:
					fdbk='K'; break;
				case 2:
					fdbk='N'; break;
				case 3:
					fdbk='H'; break;
			}
			QListViewItem* selecteditem= dlg.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
//			Doc->GetSession()->NewJudgement(profile,type->Obj.Doc,fdbk);
			cout<< profile->GetName()<<type->Obj.Doc->GetName()<<fdbk<<endl;
		}
	}

	// if the current window is a Document	
	else if (m->getType()==gDoc)
	{
		QAddJudgementSelectProfile dlg2(this,0,true);
		GDoc* doc=((KViewDoc*)m)->GetDoc();
		RContainerCursor<GUser,unsigned int,true,true> CurUsr(Doc->GetSession()->GetUsers());
		dlg2.TLTitle->setText(QString(doc->GetName()));
		dlg2.TLURL->setText(QString(doc->GetURL()));
		if(doc->GetLang()) dlg2.TLLang->setText(doc->GetLang()->GetName());
		else dlg2.TLLang->setText(" Le Documnt n a pas encore ete analyse");
		dlg2.MimeFilter->setText(doc->GetMIMEType()->GetName());
		for (CurUsr.Start();!CurUsr.End();CurUsr.Next())
		{
			QListViewItem* useritem = new QListViewItem (dlg2.LVUsers, QString (CurUsr()->GetName()));
			useritem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
			for (CurUsr()->Start(); !CurUsr()->End(); CurUsr()->Next())
				new QListViewItemType((*CurUsr())(),useritem, QString((*CurUsr())()->GetName()));
		}
		if(dlg2.exec())
		{
			// if a item was selected and is a profile
			QListViewItem* selecteditem= dlg2.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
			if ((dlg2.LVUsers->selectedItem())&&(type->Obj.Profile))
			{
				char fdbk;
				switch (dlg2.CBJudgement->currentItem())
				{
					case 0:
						fdbk='O'; break;
					case 1:
						fdbk='K'; break;
					case 2:
						fdbk='N'; break;
					case 3:
						fdbk='H'; break;
				}
				//Doc->GetSession()->NewJudgement(type->Obj.Profile,doc,fdbk);
				cout<< type->Obj.Profile->GetName()<<doc->GetName()<<fdbk<<endl;

			}
			else // if no profile was selected
			{
				QMessageBox mb( QString("Error !!"),QString( "Judgement not registered : No profile seleted  !"),QMessageBox::NoIcon,0,0,1);
				mb.show();
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotEditJudgement(void)
{
	GProfDocCursor Cur;

	// rajouter les 2 cas avec fenetre selectionne = gdocs et fenetre selectionne =  gusers
	KView* m = (KView*)pWorkspace->activeWindow();
	// if the current window is a profile
	if (m->getType()==gProfile)
	{
		QEditJudgementSelectDoc dlg(this,0,true);
		GProfile* profile =((KViewProfile*)m)->GetProfile();

		dlg.TLTitle->setText(QString(profile->GetName()));
		dlg.TLURL->setText(QString(profile->GetUser()->GetName()));
		QListViewItem* docitem = new QListViewItem (dlg.LVUsers, "Doc");
		Cur=profile->GetProfDocCursor();
		for (Cur.Start();!Cur.End();Cur.Next())
		{
			QListViewItemType* useritem = new QListViewItemType(Cur()->GetDoc(),docitem,Cur()->GetDoc()->GetName(), QString(QChar(Cur()->GetFdbk())));
			docitem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}

		if(dlg.exec())
		{
			char fdbk;
			switch (dlg.CBJudgement->currentItem())
			{
				case 0:
					fdbk='O'; break;
				case 1:
					fdbk='K'; break;
				case 2:
					fdbk='N'; break;
				case 3:
					fdbk='H'; break;
			}
			QListViewItem* selecteditem= dlg.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
//			Doc->GetSession()->NewJudgement(profile,type->Obj.Doc,fdbk);
			cout<< profile->GetName()<<type->Obj.Doc->GetName()<<fdbk<<endl;
		}
	}	
	else if (m->getType()==gDoc)
	{
		QEditJudgementSelectProfile dlg2(this,0,true);
		GDoc* doc=((KViewDoc*)m)->GetDoc();
		dlg2.TLTitle->setText(QString(doc->GetName()));
		dlg2.TLURL->setText(QString(doc->GetURL()));
		if(doc->GetLang()) dlg2.TLLang->setText(doc->GetLang()->GetName());
		else dlg2.TLLang->setText(" Le Documnt n a pas encore ete analyse");
		dlg2.MimeFilter->setText(doc->GetMIMEType()->GetName());
		Cur=doc->GetProfDocCursor();
		for (Cur.Start();!Cur.End();Cur.Next())
		{
			QListViewItem* useritem = new QListViewItem (dlg2.LVUsers,Cur()->GetProfile()->GetName(), QString(QChar(Cur()->GetFdbk())));
			useritem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}
		if(dlg2.exec())
		{
			// if a item was selected and is a profile
			QListViewItem* selecteditem= dlg2.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
			if ((dlg2.LVUsers->selectedItem())&&(type->Obj.Profile))
			{
				char fdbk;
				switch (dlg2.CBJudgement->currentItem())
				{
					case 0:
						fdbk='O'; break;
					case 1:
						fdbk='K'; break;
					case 2:
						fdbk='N'; break;
					case 3:
						fdbk='H'; break;
				}
				
			//Doc->GetSession()->NewJudgement(type->Obj.Profile,doc,fdbk);
			cout<< type->Obj.Profile->GetName()<<doc->GetName()<<fdbk<<endl;
			}
			else // if no profile was selected
			{
				QMessageBox mb( QString("Error !!"),QString( "Judgement not registered : No profile seleted  !"),QMessageBox::NoIcon,0,0,1);
				mb.show();
			}
		}
	}
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
void KGALILEICenterApp::slotProfilesOptions(void)
{
	QProfilesOptionsDlg dlg(this,0,true);
	dlg.spStatisticalOKKOSize->setValue(ParamOKKOSize);
	if(dlg.exec())
	{
		ParamOKKOSize=dlg.spStatisticalOKKOSize->value();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsOptions(void)
{
	QString tmp;

	QGroupsOptionsDlg dlg(this,0,true);
	dlg.txtLevelSim->setValidator(new QDoubleValidator(0.0,1.0,3,dlg.txtLevelSim));
	tmp.setNum(ParamLevelSim);
	dlg.txtLevelSim->setText(tmp);
	dlg.cbFullSim->setChecked(ParamFullSim);
	if(dlg.exec())
	{
		double d=dlg.txtLevelSim->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ParamLevelSim=d;
		ParamFullSim=dlg.cbFullSim->isChecked();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsOptions(void)
{
	QString tmp;

	QDocsOptionsDlg dlg(this,0,true);
	dlg.cbStaticLang->setChecked(ParamStaticLang);
	tmp.setNum(ParamMinStop);
	dlg.txtMinStop->setText(tmp);
	dlg.txtMinWordSize->setValue(ParamMinWordSize);
	dlg.txtMinStemSize->setValue(ParamMinStemSize);
	dlg.txtMinOcc->setValue(ParamMinOccur);
	for(Filters.Start();!Filters.End();Filters.Next())
		new QListViewItem(dlg.lvFilters,Filters()->GetName(),Filters()->GetMIMES(),Filters()->GetVersion());
	if(dlg.exec())
	{
		double d=dlg.txtMinStop->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ParamMinStop=d;
		ParamStaticLang=dlg.cbStaticLang->isChecked();
		ParamMinWordSize=dlg.txtMinWordSize->value();
		ParamMinStemSize=dlg.txtMinStemSize->value();
		ParamMinOccur=dlg.txtMinOcc->value();
	}
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
void KGALILEICenterApp::slotLoadProgram()
{
	QFileDialog* dlg=new QFileDialog ( QString::null, "Program file (*.txt)", this );
	QString s=dlg->getOpenFileName( QString::null, "Program file (*.txt)", this );
	if (s)
	{
		GALILEIProgram* prog = new GALILEIProgram (RString(s));
		prog->Load();
		prog->Run();
	}
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
				break;

			case gDocs:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				break;

			case gDoc:
				createXML->setEnabled(true);
				analyseXML->setEnabled(true);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(true);
				editJudgement->setEnabled(true);
				break;

			case gProfile:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(true);
				addJudgement->setEnabled(true);
				editJudgement->setEnabled(true);
				break;

			case gGroups:
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				addProfile->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
				break;

			case gNothing:
			default:
				addProfile->setEnabled(false);
				createXML->setEnabled(false);
				analyseXML->setEnabled(false);
				profileCalc->setEnabled(false);
				addJudgement->setEnabled(false);
				editJudgement->setEnabled(false);
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
KGALILEICenterApp::~KGALILEICenterApp(void)
{
	delete Printer;
	if(Doc) delete(Doc);
}
