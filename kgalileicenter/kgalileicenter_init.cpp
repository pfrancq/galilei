/*

	GALILEI Research Project

	KGALILEICenter_init.cpp

	Main Window - Implementation of the Init part.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <rqt.h>
#include <rxmlfile.h>
#include <rxmltag.h>
#include <ggalileiapp.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QtCore/QDir>
#include <QtGui/QToolTip>
#include <QtGui/QWorkspace>
#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>
#include <QtGui/QCloseEvent>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kactioncollection.h>
#include <ktoggleaction.h>
#include <kapplication.h>
#include <kiconloader.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <kshortcut.h>
#include <kstatusbar.h>
#include <ktoolbar.h>
#include <kmenu.h>
#include <kmessagebox.h>


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
KGALILEICenterApp::KGALILEICenterApp(int argc, char *argv[])
	: KXmlGuiWindow(0), GGALILEIApp("KGALILEICenter",argc,argv,true), Doc(0), Debug(0)
{
	Init();
//	Config=kapp->config();
	initStatusBar();
	initView();
	initActions();

	//read the kgalileicenter options
	readOptions();

	DisableAllActions();
}

//-----------------------------------------------------------------------------
void KGALILEICenterApp::initActions(void)
{
	// Menu "Connect"
//	sessionSave=new KToggleAction(i18n("Save Results"),0,0,0,actionCollection(),"sessionSave");
//	connect(sessionSave,SIGNAL(toggled(bool)),this,SLOT(slotSaveModifier()));
	//sessionConnect=new KAction(i18n("&Connect Database"),"connect_established",0,this,SLOT(slotSessionConnect()),actionCollection(),"sessionConnect");
	sessionConnect=actionCollection()->addAction("sessionConnect");
	sessionConnect->setText(i18n("&Connect Database"));
	sessionConnect->setIcon(KIcon("connect_established"));
//	sessionConnect->setShortcut(Qt::Key_F6);
	connect(sessionConnect,SIGNAL(triggered()),this,SLOT(sessionConnect()));
/*	sessionCompute=new KAction(i18n("Compute &Session"),"make_kdevelop",0,this,SLOT(slotSessionCompute()),actionCollection(),"sessionCompute");
	sessionDebugInfo=new KAction(i18n("&Konsole"),"help","Ctrl+K",this,SLOT(slotSessionDebugInfo()),actionCollection(),"sessionDebugInfo");
	createDatabase=new KAction(i18n("Create &MySQL Database"),"exec",0,this,SLOT(slotCreateDatabase()),actionCollection(),"createDatabase");
	importUsersData=new KAction(i18n("Import &Users' Data"),0,0,this,SLOT(slotImportUsersData()),actionCollection(),"importUsersData");
	importDocs=new KAction(i18n("Import Documents"),"exec",0,this,SLOT(slotImportDocs()),actionCollection(),"importDocs");
	runProgram=new KAction(i18n("&Run Program"),"rebuild","Ctrl+R",this,SLOT(slotRunProgram()),actionCollection(),"runProgram");
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionStats=new KAction(i18n("&Statistics"),"gohome",0,this,SLOT(slotSessionStats()),actionCollection(),"sessionStats");
	sessionQuit=new KAction(i18n("E&xit"),"exit",0,this,SLOT(slotSessionQuit()),actionCollection(),"sessionQuit");

	// Menu "Knowledge"
	seeDicts=new KAction(i18n("See &Dictionnaries"),0,0,this,SLOT(slotSeeDicts()),actionCollection(),"seeDicts");

	// Menu "Document"
	docsClear=new KAction(i18n("&Force Re-computing Documents"),0,0,this,SLOT(slotDocsClear()),actionCollection(),"docsClear");
	showDocs=new KAction(i18n("Show &Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docAnalyse=new KAction(i18n("&Load and Analyze a Document"),0,this,SLOT(slotDocAnalyse()),actionCollection(),"docAnalyse");
	docsAnalyse=new KAction(i18n("&Analyze Documents"),"kfind",0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");
	postDocsAnalyse=new KAction(i18n("Execute &Post-Documents Methods"),0,0,this,SLOT(slotPostDocsAnalyse()),actionCollection(),"postDocsAnalyse");
	docsIndexer=new KAction(i18n("&Export Documents"),0,0,this,SLOT(slotDocsIndexer()),actionCollection(),"docsIndexer");
	createXML=new KAction(i18n("&Create XML Structure"),0,0,this,SLOT(slotCreateXML()),actionCollection(),"createXML");
	saveXML=new KAction(i18n("&Save XML Structure"),0,0,this,SLOT(slotSaveXML()),actionCollection(),"saveXML");
	analyseXML=new KAction(i18n("Analyze &XML Structure"),"filefind",0,this,SLOT(slotAnalyseXML()),actionCollection(),"analyseXML");
	queryMetaEngine=new KAction(i18n("&Query Meta Engine"),"find",0,this,SLOT(slotQueryMetaEngine()),actionCollection(),"queryMetaEngine");

	// Menu "Topics"
	topicsClear=new KAction(i18n("Force Re-computing Topics"),0,0,this,SLOT(slotTopicsClear()),actionCollection(),"topicsClear");
	showTopics=new KAction(i18n("Show &Topics"),0,0,this,SLOT(slotShowTopics()),actionCollection(),"showTopics");
	topicsCalc=new KAction(i18n("Group Documents"),0,0,this,SLOT(slotTopicsCalc()),actionCollection(),"topicsCalc");
	postTopicsCalc=new KAction(i18n("Execute Post-Topic Methods"),0,0,this,SLOT(slotTopicsCalc()),actionCollection(),"postTopicsCalc");

	// Menu "Users"
	usersClear=new KAction(i18n("&Force Re-computing profiles"),0,0,this,SLOT(slotProfilesClear()),actionCollection(),"profilesClear");
	showUsers=new KAction(i18n("&Show Users"),"kdmconfig",0,this,SLOT(slotShowUsers()),actionCollection(),"showUsers");
	profilesCalc=new KAction(i18n("&Calc Profiles"),"run",0,this,SLOT(slotProfilesCalc()),actionCollection(),"profilesCalc");
	postProfilesCalc=new KAction(i18n("Execute &Post-Profiles Methods"),0,0,this,SLOT(slotPostProfilesCalc()),actionCollection(),"postProfilesCalc");
	profileCalc=new KAction(i18n("Calc &Profile"),0,0,this,SLOT(slotProfileCalc()),actionCollection(),"profileCalc");

	// Menu "Communities"
	communitiesClear=new KAction(i18n("&Force Re-computing Communities"),0,0,this,SLOT(slotCommunitiesClear()),actionCollection(),"communitiesClear");
	showCommunities=new KAction(i18n("&Show Communities"),"window_list",0,this,SLOT(slotShowCommunities()),actionCollection(),"showCommunities");
	communitiesCalc=new KAction(i18n("&Group Profiles"),"exec",0,this,SLOT(slotCommunitiesCalc()),actionCollection(),"communitiesCalc");
	postCommunitiesCalc=new KAction(i18n("Execute &Post-Communities Methods"),0,0,this,SLOT(slotPostCommunities()),actionCollection(),"postCommunitiesCalc");

	// Menu "Debug"
	simulationDlg=new KAction(i18n("Simulation &Parameters"),0,0,this,SLOT(slotSimulationDlg()),actionCollection(),"simulationDlg");
	subjectsCreate=new KAction(i18n("Initialize the simulation"),0,0,this,SLOT(slotSubjectsCreate()),actionCollection(),"subjectsCreate");
	communitiesCreate=new KAction(i18n("Create Ideal &Communities"),0,0,this,SLOT(slotCommunitiesCreate()),actionCollection(),"communitiesCreate");
	topicsCreate=new KAction(i18n("Create Ideal &Topics"),0,0,this,SLOT(slotTopicsCreate()),actionCollection(),"topicsCreate");
	doFdbks=new KAction(i18n("&Feedback Cycle"),0,0,this,SLOT(slotDoFdbks()),actionCollection(),"doFdbks");
	doAssessments=new KAction(i18n("&Assessments Cycle"),0,0,this,SLOT(slotDoAssessments()),actionCollection(),"doAssessments");
	communitiesCompare=new KAction(i18n("Compare Ideal Communities"),"fileopen",0,this,SLOT(slotCommunitiesCompare()),actionCollection(),"communitiesCompare");
	topicsCompare=new KAction(i18n("Compare Ideal Topics"),"fileopen",0,this,SLOT(slotTopicsCompare()),actionCollection(),"topicsCompare");

	// Menu "Settings"
	setStandardToolBarMenuEnabled(true);
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection(), "settings_statusbar");
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	plugins=new KAction(i18n("Configure &Plug-Ins..."),"wizard",0,this,SLOT(slotPlugins()),actionCollection(),"plugins");
	configure=new KAction(i18n("&Configure GALILEI..."),"configure",0,this,SLOT(slotConfigure()),actionCollection(),"configure");

	// Menu "Window"
	windowTile = new KAction(i18n("&Tile"), 0, 0, this, SLOT(slotWindowTile()), actionCollection(),"window_tile");
	windowCascade = new KAction(i18n("&Cascade"), 0, 0, this, SLOT(slotWindowCascade()), actionCollection(),"window_cascade");
	windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
	connect(windowMenu->popupMenu(),SIGNAL(aboutToShow()),this,SLOT(windowMenuAboutToShow()));*/

	// Help Menu
/*	helpProgram = new KAction(i18n("List of all classes"), 0, 0, this, SLOT(slotHelpProgram()), actionCollection(),"helpProgram");
	helpProgram=new KAction("List of all classes",0,0,this,SLOT(slotHelpProgram()),this);
	menuBar()->insertItem ("&Help",helpProgram);*/

	createGUI();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initStatusBar(void)
{
	dbStatus= new QLabel(statusBar());
	dbStatus->setPixmap(KIconLoader::global()->loadIcon("connect_no.png",KIconLoader::Small));
	statusBar()->insertWidget(0,dbStatus);
	statusBar()->insertItem(i18n("Ready."),1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initView(void)
{
	QWidget* view_back(new QWidget(this));
	QVBoxLayout *layout = new QVBoxLayout;
//	view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	pWorkspace = new QWorkspace(view_back);
	layout->addWidget(pWorkspace);
	view_back->setLayout(layout);
	connect(pWorkspace, SIGNAL(windowActivated(QWidget*)), this, SLOT(slotWindowActivated(QWidget*)));
	setCentralWidget(view_back);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::createClient(KDoc* doc,KView* view)
{
	view->installEventFilter(this);
	if(doc)
		doc->addView(view);
	view->resize(600,400);
	view->show();
	view->setFocus();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveOptions(void)
{
	RString paths("");
	KConfig Config;
	KConfigGroup General(&Config,"General");

	General.writeEntry("Geometry", size());
	General.writeEntry("Show Toolbar", toolBar()->isVisible());
	General.writeEntry("Show Statusbar",statusBar()->isVisible());
//	General.writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	General.writeEntry("Save Results",sessionSave->isChecked());

	// Save options for database creation
	General.writeEntry("StructuresPath",ToQString(StructuresPath));
	General.writeEntry("PrgPath",ToQString(PrgPath));

	// Save options for the plug-ins dialog box
	General.writeEntry("DlgMainTabIdx",DlgMainTabIdx);
	General.writeEntry("DlgDocsTabIdx",DlgDocsTabIdx);
	General.writeEntry("DlgProfilesTabIdx",DlgProfilesTabIdx);
	General.writeEntry("DlgCommunitiesTabIdx",DlgCommunitiesTabIdx);
	General.writeEntry("DlgSearchTabIdx",DlgSearchTabIdx);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"General");

	// bar status settings
	bool bViewToolbar = General.readEntry("Show Toolbar", true);
	if(bViewToolbar)
		toolBar()->show();
	else
		toolBar()->hide();
	bool bViewStatusbar = General.readEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();

	// bar position settings
/*	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) General.readEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);*/

	// Always Calc Enable/Disable
	sessionSave->setChecked(General.readEntry("Save Results",false));

	// Size
	QSize size=General.readEntry("Geometry",QSize());
	if(!size.isEmpty())
	{
		resize(size);
	}

	// Read create database options
	StructuresPath=FromQString(General.readEntry("StructuresPath","http://stic.ulb.ac.be"));
	PrgPath=FromQString(General.readEntry("PrgPath",""));

	// Save options for the plug-ins dialog box
	DlgMainTabIdx=General.readEntry("DlgMainTabIdx",0);
	DlgDocsTabIdx=General.readEntry("DlgDocsTabIdx",0);
	DlgProfilesTabIdx=General.readEntry("DlgProfilesTabIdx",0);
	DlgCommunitiesTabIdx=General.readEntry("DlgCommunitiesTabIdx",0);
	DlgSearchTabIdx=General.readEntry("DlgSearchTabIdx",0);
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
	sessionDisconnect->setEnabled(true);
	showUsers->setEnabled(true);
	showCommunities->setEnabled(true);
	communitiesCreate->setEnabled(true);
	topicsCreate->setEnabled(true);
	profilesCalc->setEnabled(true);
	postProfilesCalc->setEnabled(true);
	profileCalc->setEnabled(true);
	sessionCompute->setEnabled(true);
	doFdbks->setEnabled(true);
	doAssessments->setEnabled(true);
	communitiesCompare->setEnabled(true);
	topicsCompare->setEnabled(true);
	communitiesCalc->setEnabled(true);
	postCommunitiesCalc->setEnabled(true);
	subjectsCreate->setEnabled(true);
	communitiesCreate->setEnabled(true);
	topicsCreate->setEnabled(true);
	showDocs->setEnabled(true);
	docAnalyse->setEnabled(true);
	docsAnalyse->setEnabled(true);
	postDocsAnalyse->setEnabled(true);
	docsIndexer->setEnabled(true);
	queryMetaEngine->setEnabled(true);
	sessionStats->setEnabled(true);
	sessionSave->setEnabled(true);
	docsClear->setEnabled(true);
	usersClear->setEnabled(true);
	communitiesClear->setEnabled(true);
	seeDicts->setEnabled(true);
	importUsersData->setEnabled(true);
	importDocs->setEnabled(true);
	topicsClear->setEnabled(true);
	showTopics->setEnabled(true);
	topicsCalc->setEnabled(true);
	postTopicsCalc->setEnabled(true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::DisableAllActions(void)
{
	sessionDisconnect->setEnabled(false);
	showUsers->setEnabled(false);
	showCommunities->setEnabled(false);
	communitiesCreate->setEnabled(false);
	topicsCreate->setEnabled(false);
	profilesCalc->setEnabled(false);
	postProfilesCalc->setEnabled(false);
	profileCalc->setEnabled(false);
	sessionCompute->setEnabled(false);
	doFdbks->setEnabled(false);
	doAssessments->setEnabled(false);
	communitiesCompare->setEnabled(false);
	topicsCompare->setEnabled(false);
	createXML->setEnabled(false);
	saveXML->setEnabled(false);
	analyseXML->setEnabled(false);
	communitiesCalc->setEnabled(false);
	postCommunitiesCalc->setEnabled(false);
	subjectsCreate->setEnabled(false);
	communitiesCompare->setEnabled(false);
	topicsCompare->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	postDocsAnalyse->setEnabled(false);
	docsIndexer->setEnabled(false);
	queryMetaEngine->setEnabled(false);
	sessionStats->setEnabled(false);
	sessionSave->setEnabled(false);
	docsClear->setEnabled(false);
	usersClear->setEnabled(false);
	communitiesClear->setEnabled(false);
	seeDicts->setEnabled(false);
	importUsersData->setEnabled(false);
	importDocs->setEnabled(false);
	topicsClear->setEnabled(false);
	showTopics->setEnabled(false);
	topicsCalc->setEnabled(false);
	postTopicsCalc->setEnabled(false);
}


//-----------------------------------------------------------------------------
bool KGALILEICenterApp::eventFilter(QObject* object, QEvent* event)
{
	if((event->type() == QEvent::Close)&&((KGALILEICenterApp*)object!=this))
	{
		QCloseEvent* e=(QCloseEvent*)event;
		KView* pView=(KView*)object;
		KDoc* pDoc=pView->getDocument();
		if(pDoc)
		{
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
		{
			delete pView;
			e->accept();
			return(true);
		}
	}
	else
		return(QWidget::eventFilter(object,event));
}
