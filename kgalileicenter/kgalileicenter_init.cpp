/*

	GALILEI Research Project

	KGALILEICenter_init.cpp

	Main Window - Implementation of the Init part.

	(C) 2002 by Pascal Francq

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
// include files for GALILEI
#include <profiles/gprofilecalcvector.h>
#include <sessions/gsessionmysql.h>
#include <groups/ggroupingsim.h>
#include <docs/gdocoptions.h>
using namespace GALILEI;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qdir.h>
#include <qprinter.h>
#include <qvbox.h>
#include <qtooltip.h>
#include <qprinter.h>
#include <qworkspace.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <kshortcut.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KGALILEICenterApp::KGALILEICenterApp(void)
	: KMainWindow(0,"KGALILEICenterApp"), GURLManagerKDE(),
	  dbHost(""),dbName(""),dbUser(""),dbPwd(""),Doc(0),DocOptions(0)
{
	Config=kapp->config();
	Printer = new QPrinter;
	DocOptions=new GDocOptions();
	initStatusBar();
	initView();
	initActions();

	// read options
	readOptions();

	// disable actions at startup
	DisableAllActions();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initActions(void)
{
	// Menu "Connect"
	sessionAlwaysCalc=new KToggleAction(i18n("Enables/disables session Recomputing"),0,0,0,actionCollection(),"sessionAlwaysCalc");
	sessionConnect=new KAction(i18n("&Connect Database"),"connect_established",0,this,SLOT(slotSessionConnect()),actionCollection(),"sessionConnect");
	sessionCompute=new KAction(i18n("Compute &Session"),"make_kdevelop",0,this,SLOT(slotSessionCompute()),actionCollection(),"sessionCompute");
	runProgram=new KAction(i18n("&Run Program"),"rebuild",0,this,SLOT(slotRunProgram()),actionCollection(),"runProgram");
	runInsts=new KAction(i18n("Run &Instructions"),"help",0,this,SLOT(slotRunInsts()),actionCollection(),"runInsts");;
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionTest=new KAction(i18n("&Test"),"gohome",0,this,SLOT(slotSessionTest()),actionCollection(),"sessionTest");
	sessionQuit=new KAction(i18n("E&xit"),"exit",0,this,SLOT(slotSessionQuit()),actionCollection(),"sessionQuit");

	// Menu "Users"
	profileAlwaysCalc=new KToggleAction(i18n("Enables/disables users Recomputing"),0,0,0,actionCollection(),"profileAlwaysCalc");
	profileAlwaysSave=new KToggleAction(i18n("Enables/disables users Saving"),0,0,0,actionCollection(),"profileAlwaysSave");
	showUsers=new KAction(i18n("&Show Users"),"kdmconfig",0,this,SLOT(slotShowUsers()),actionCollection(),"showUsers");
	profilesCalc=new KAction(i18n("&Calc Profiles"),"run",0,this,SLOT(slotProfilesCalc()),actionCollection(),"profilesCalc");
	profileCalc=new KAction(i18n("&Calc Profile"),"run",0,this,SLOT(slotProfileCalc()),actionCollection(),"profilecalc");

	// Menu "Groups"
	groupAlwaysCalc=new KToggleAction(i18n("Enables/disables groups Recomputing"),0,0,0,actionCollection(),"groupAlwaysCalc");
	groupAlwaysSave=new KToggleAction(i18n("Enables/disables groups Saving"),0,0,0,actionCollection(),"groupAlwaysSave");
	showGroups=new KAction(i18n("&Show Groups"),"window_list",0,this,SLOT(slotShowGroups()),actionCollection(),"showGroups");
	groupsCalc=new KAction(i18n("Compute &Groups"),"exec",0,this,SLOT(slotGroupsCalc()),actionCollection(),"groupsCalc");
	groupingCompare=new KAction(i18n("&Compare Grouping"),"fileopen",0,this,SLOT(slotGroupingCompare()),actionCollection(),"groupingCompare");
	groupsEvaluation=new KAction(i18n("&Evaluate Grouping"),"fileopen",0,this,SLOT(slotGroupsEvaluation()),actionCollection(),"groupsEvaluation");


	// Menu "Document"
	docAlwaysCalc=new KToggleAction(i18n("Enables/disables documents Recomputing"),0,0,0,actionCollection(),"docAlwaysCalc");
	showDocs=new KAction(i18n("&Show Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docAnalyse=new KAction(i18n("&Load and Analyse a Document"),0,this,SLOT(slotDocAnalyse()),actionCollection(),"docAnalyse");;
	docsAnalyse=new KAction(i18n("&Analyse Documents"),0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");;
	docsStats=new KAction(i18n("S&tatistics about Documents"),0,this,SLOT(slotDocsStats()),actionCollection(),"docsStats");;
	createXML=new KAction(i18n("&Create XML Structure"),"readme",0,this,SLOT(slotCreateXML()),actionCollection(),"createXML");
	saveXML=new KAction(i18n("&Save XML Structure"),"readme",0,this,SLOT(slotSaveXML()),actionCollection(),"saveXML");
	analyseXML=new KAction(i18n("&Analyse XML Structure"),"filefind",0,this,SLOT(slotAnalyseXML()),actionCollection(),"analyseXML");

	// Menu "Texts"
	textFrench=new KAction(i18n("Analyze &French Stems"),0,this,SLOT(slotTextFrench()),actionCollection(),"textFrench");
	textEnglish=new KAction(i18n("Analyze &English Stems"),0,this,SLOT(slotTextEnglish()),actionCollection(),"textEnglish");

	// Menu "R Stat
	rRunR=new KAction(i18n("Run &R"),0,this,SLOT(slotRRunR()),actionCollection(),"rRunR");

	// Menu "GA"
	gaInit=new KAction(i18n("&Initialize"),"reload",KKey("Alt+I").keyCodeQt(),this,SLOT(slotGAInit(void)),actionCollection(),"gaInit");
	gaStart=new KAction(i18n("&Start"),"exec",KKey("Alt+S").keyCodeQt(),this,SLOT(slotGAStart(void)),actionCollection(),"gaStart");
	gaPause=new KAction(i18n("&Pause"),"player_pause",KKey("Alt+P").keyCodeQt(),this,SLOT(slotGAPause(void)),actionCollection(),"gaPause");
	gaStop=new KAction(i18n("&Stop"),"stop",KKey("Alt+T").keyCodeQt(),this,SLOT(slotGAStop(void)),actionCollection(),"gaStop");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	docsOptions=new KAction(i18n("&Documents Options"),"configure",0,this,SLOT(slotDocsOptions()),actionCollection(),"docsOptions");
	plugins=new KAction(i18n("&Plugins"),"wizard",0,this,SLOT(slotPlugins()),actionCollection(),"plugins");

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
	(*doc->GetSession()->GetDocOptions())=(*DocOptions);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::createClient(KDoc* doc,KView* view)
{
	view->installEventFilter(this);
	doc->addView(view);
	view->resize(600,400);
	view->show();
	view->setFocus();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveOptions(void)
{
	Config->setGroup("General Options");
	Config->writeEntry("Geometry", size());
	Config->writeEntry("Show Toolbar", toolBar()->isVisible());
	Config->writeEntry("Show Statusbar",statusBar()->isVisible());
	Config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	Config->writeEntry("Always Calc Session",sessionAlwaysCalc->isChecked());
	Config->writeEntry("Always Calc Profiles",profileAlwaysCalc->isChecked());
	Config->writeEntry("Always Save Profiles",profileAlwaysSave->isChecked());
	Config->writeEntry("Always Calc Groups",groupAlwaysCalc->isChecked());
	Config->writeEntry("Always Save Groups",groupAlwaysSave->isChecked());
	Config->writeEntry("Always Calc Docs",docAlwaysCalc->isChecked());

	Config->setGroup("Database Options");
	Config->writeEntry("Host", dbHost());
	Config->writeEntry("Name", dbName());
	Config->writeEntry("User", dbUser());
	Config->writeEntry("Password", dbPwd());

	Config->setGroup("GALILEI Options");
	Config->writeEntry("Static Language",DocOptions->StaticLang);
	Config->writeEntry("Minimum Stop Percentage",DocOptions->MinStopWords);
	Config->writeEntry("Minimum Word's Size",DocOptions->MinWordSize);
	Config->writeEntry("Minimum Stem's Size",DocOptions->MinStemSize);
	Config->writeEntry("Minimum Occurence",DocOptions->MinOccur);
	Config->writeEntry("Accept Non-Letter words",DocOptions->NonLetterWords);
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

	// Always Calc Enable/Disable
	sessionAlwaysCalc->setChecked(Config->readBoolEntry("Always Calc Session",false));
	profileAlwaysCalc->setChecked(Config->readBoolEntry("Always Calc Profiles",false));
	profileAlwaysSave->setChecked(Config->readBoolEntry("Always Save Profiles",true));
	groupAlwaysCalc->setChecked(Config->readBoolEntry("Always Calc Groups",false));
	groupAlwaysSave->setChecked(Config->readBoolEntry("Always save Groups",true));
	docAlwaysCalc->setChecked(Config->readBoolEntry("Always Calc Docs",false));

	// Size
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
	DocOptions->StaticLang=Config->readBoolEntry("Static Language",true);
	DocOptions->MinStopWords=Config->readDoubleNumEntry("Minimum Stop Percentage",0.1);
	DocOptions->MinWordSize=Config->readUnsignedNumEntry("Minimum Word's Size",4);
	DocOptions->MinStemSize=Config->readUnsignedNumEntry("Minimum Stem's Size",3);
	DocOptions->MinOccur=Config->readUnsignedNumEntry("Minimum Occurence",1);
	DocOptions->NonLetterWords=Config->readBoolEntry("Accept Non-Letter words",true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveProperties(KConfig* /*_cfg*/)
{

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readProperties(KConfig* /*_cfg*/)
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

	// Menu "Groups"
	showGroups->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad());
	groupsCalc->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad()&&Doc->GetSession()->IsUsersLoad());
	groupingCompare->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad());
	groupsEvaluation->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad());


	// Menu "Document"
	showDocs->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad());
	docAnalyse->setEnabled(Doc&&Doc->GetSession()->IsDicsLoad());
	docsAnalyse->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());
	docsStats->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());
	
	// Menu "GA"
	gaInit->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad()&&Doc->GetSession()->IsUsersLoad());
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
	groupsEvaluation->setEnabled(false);
	textFrench->setEnabled(false);
	textEnglish->setEnabled(false);
	createXML->setEnabled(false);
	saveXML->setEnabled(false);
	analyseXML->setEnabled(false);
	groupsCalc->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	docsStats->setEnabled(false);
	gaInit->setEnabled(false);
	gaPause->setEnabled(false);
	gaStart->setEnabled(false);
	gaStop->setEnabled(false);
	plugins->setEnabled(false);
	runProgram->setEnabled(false);
	runInsts->setEnabled(false);
	rRunR->setEnabled(false);
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
		return(true);
	}
	else
		return(QWidget::eventFilter(object,event));
}
