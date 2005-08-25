/*

	GALILEI Research Project

	KGALILEICenter_init.cpp

	Main Window - Implementation of the Init part.

	Copyright 2002 by the Universit�Libre de Bruxelles.

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
#include <gstoragemysql.h>
#include <gconfig.h>
#include <gpluginmanagers.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
using namespace GALILEI;
using namespace R;


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
KGALILEICenterApp::KGALILEICenterApp(void) throw(GException)
	: KMainWindow(0,"KGALILEICenterApp"), dbHost(),dbName(),dbUser(),dbPwd(),pluginsPath(10,5),
	  Doc(0)
{
	Config=kapp->config();
	initStatusBar();
	initView();
	initActions();

	//read the kgalileicenter options
	readOptions();
	try
	{
		GPluginManagers::PluginManagers.Load(pluginsPath);
	}
	catch(GException e)
	{
		std::cout<<e.GetMsg()<<std::endl;
	}
	//read the galilei & galilei_plugins options
	readGALILEIOptions();

	DisableAllActions();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::initActions(void)
{
	// Menu "Connect"
	sessionAlwaysCalc=new KToggleAction(i18n("Enables/disables session Recomputing"),0,0,0,actionCollection(),"sessionAlwaysCalc");
	connect(sessionAlwaysCalc,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	sessionConnect=new KAction(i18n("&Connect Database"),"connect_established",0,this,SLOT(slotSessionConnect()),actionCollection(),"sessionConnect");
	sessionCompute=new KAction(i18n("Compute &Session"),"make_kdevelop",0,this,SLOT(slotSessionCompute()),actionCollection(),"sessionCompute");
	createDatabase=new KAction(i18n("Create &Database"),"exec",0,this,SLOT(slotCreateDatabase()),actionCollection(),"createDatabase");
	fillEmptyDb=new KAction(i18n("&Fill Empty Database"),"exec",0,this,SLOT(slotFillEmptyDb()),actionCollection(),"fillEmptyDb");
	runProgram=new KAction(i18n("&Run Program"),"rebuild",0,this,SLOT(slotRunProgram()),actionCollection(),"runProgram");
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionStats=new KAction(i18n("&Statistics"),"gohome",0,this,SLOT(slotSessionStats()),actionCollection(),"sessionStats");
	sessionQuit=new KAction(i18n("E&xit"),"exit",0,this,SLOT(slotSessionQuit()),actionCollection(),"sessionQuit");

	// Menu "Users"
	profileAlwaysCalc=new KToggleAction(i18n("Enables/disables users Recomputing"),0,0,0,actionCollection(),"profileAlwaysCalc");
	connect(profileAlwaysCalc,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	profileAlwaysSave=new KToggleAction(i18n("Enables/disables users Saving"),0,0,0,actionCollection(),"profileAlwaysSave");
	connect(profileAlwaysSave,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	showUsers=new KAction(i18n("&Show Users"),"kdmconfig",0,this,SLOT(slotShowUsers()),actionCollection(),"showUsers");
	profilesCalc=new KAction(i18n("&Calc Profiles"),"run",0,this,SLOT(slotProfilesCalc()),actionCollection(),"profilesCalc");
	profileCalc=new KAction(i18n("Calc &Profile"),"run",0,this,SLOT(slotProfileCalc()),actionCollection(),"profileCalc");

	// Menu "Groups"
	useExistingGroups=new KToggleAction(i18n("Use existing groups for incremental clustering"),0,0,0,actionCollection(),"groupAlwaysCalc");
	connect(useExistingGroups,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	groupAlwaysSave=new KToggleAction(i18n("Enables/disables groups Saving"),0,0,0,actionCollection(),"groupAlwaysSave");
	connect(groupAlwaysSave,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	showGroups=new KAction(i18n("&Show Groups"),"window_list",0,this,SLOT(slotShowGroups()),actionCollection(),"showGroups");
	groupsCalc=new KAction(i18n("Compute &Groups"),"exec",0,this,SLOT(slotGroupsCalc()),actionCollection(),"groupsCalc");
	somView=new KAction(i18n("View Self-Organizing Map"),"exec",0,this,SLOT(slotChooseSOM()),actionCollection(),"somView");
	showGroupsHistory=new KAction(i18n("Show Groups &History"),"exec",0,this,SLOT(slotShowHistory	()),actionCollection(),"showGroupsHistorys");

	// Menu "Document"
	docAlwaysCalc=new KToggleAction(i18n("Enables/disables documents Recomputing"),0,0,0,actionCollection(),"docAlwaysCalc");
	connect(docAlwaysCalc,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	docAlwaysSave=new KToggleAction(i18n("Enables/disables documents Saving"),0,0,0,actionCollection(),"docAlwaysSave");
	connect(docAlwaysSave,SIGNAL(toggled(bool)),this,SLOT(slotChangeModifiers()));
	showDocs=new KAction(i18n("Show &Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docAnalyse=new KAction(i18n("&Load and Analyse a Document"),0,this,SLOT(slotDocAnalyse()),actionCollection(),"docAnalyse");
	docsAnalyse=new KAction(i18n("&Analyse Documents"),"kfind",0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");
	docsIndexer=new KAction(i18n("&Export Documents"),"save",0,this,SLOT(slotDocsIndexer()),actionCollection(),"docsIndexer");
	createXML=new KAction(i18n("&Create XML Structure"),"readme",0,this,SLOT(slotCreateXML()),actionCollection(),"createXML");
	saveXML=new KAction(i18n("&Save XML Structure"),"readme",0,this,SLOT(slotSaveXML()),actionCollection(),"saveXML");
	analyseXML=new KAction(i18n("Analyse &XML Structure"),"filefind",0,this,SLOT(slotAnalyseXML()),actionCollection(),"analyseXML");
	queryMetaEngine=new KAction(i18n("&Query Meta Engine"),"find",0,this,SLOT(slotQueryMetaEngine()),actionCollection(),"queryMetaEngine");
	fillMIMETypes=new KAction(i18n("Construct &MIME types from KDE"),"desktop",0,this,SLOT(slotFillMIMETypes()),actionCollection(),"fillMIMETypes");

	// Menu "Debug"
	simulationDlg=new KAction(i18n("Simulation &Parameters"),0,this,SLOT(slotSimulationDlg()),actionCollection(),"simulationDlg");
	groupingCreate=new KAction(i18n("&Create Ideal Groups"),0,0,this,SLOT(slotGroupsCreate()),actionCollection(),"groupingCreate");
	doFdbks=new KAction(i18n("&Feedback Cycle"),0,0,this,SLOT(slotDoFdbks()),actionCollection(),"doFdbks");
	groupingCompare=new KAction(i18n("Ideal clustering"),"fileopen",0,this,SLOT(slotGroupingCompare()),actionCollection(),"groupingCompare");
	textFrench=new KAction(i18n("Analyze &French Stems"),0,this,SLOT(slotTextFrench()),actionCollection(),"textFrench");
	textEnglish=new KAction(i18n("Analyze &English Stems"),0,this,SLOT(slotTextEnglish()),actionCollection(),"textEnglish");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	plugins=new KAction(i18n("&Plug-Ins"),"wizard",0,this,SLOT(slotPlugins()),actionCollection(),"plugins");

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
	dbStatus->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("connect_no.png",KIcon::Small)));
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
	Config->setGroup("General Options");
	Config->writeEntry("Geometry", size());
	Config->writeEntry("Show Toolbar", toolBar()->isVisible());
	Config->writeEntry("Show Statusbar",statusBar()->isVisible());
	Config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	Config->writeEntry("Always Calc Session",sessionAlwaysCalc->isChecked());
	Config->writeEntry("Always Calc Profiles",profileAlwaysCalc->isChecked());
	Config->writeEntry("Always Save Profiles",profileAlwaysSave->isChecked());
	Config->writeEntry("Always Save Groups",groupAlwaysSave->isChecked());
	Config->writeEntry("Use Existing Groups",useExistingGroups->isChecked());
	Config->writeEntry("Always Calc Docs",docAlwaysCalc->isChecked());
	Config->writeEntry("Always Save Docs",docAlwaysSave->isChecked());
	RCursor<RString> cPath(pluginsPath);
	for(cPath.Start();!cPath.End();cPath.Next())
		paths+=(*cPath())+RString(";");
	Config->writeEntry("PluginsPath", paths);

	Config->setGroup("Database Options");
	Config->writeEntry("Host", dbHost);
	Config->writeEntry("Name", dbName);
	Config->writeEntry("User", dbUser);
	Config->writeEntry("Password", dbPwd);
	Config->writeEntry("Encoding", dbEncoding);

	// Save options for database creation
	Config->writeEntry("CreateDbSQLpath",CreateDbSQLpath);
	Config->writeEntry("CreateDbUseStopList",CreateDbUseStopList);
	Config->writeEntry("CreateDbUseUsers",CreateDbUseUsers);

	// Save options for the plug-ins dialog box
	Config->writeEntry("DlgMainTabIdx",DlgMainTabIdx);
	Config->writeEntry("DlgDocsTabIdx",DlgDocsTabIdx);
	Config->writeEntry("DlgProfilesTabIdx",DlgProfilesTabIdx);
	Config->writeEntry("DlgCommunitiesTabIdx",DlgCommunitiesTabIdx);
	Config->writeEntry("DlgSearchTabIdx",DlgSearchTabIdx);

	// Save Config
	try
	{
		GConfig Conf("/etc/galilei/galilei.galileiconfig");
		Conf.Save();
	}
	catch(...)
	{
	}
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
	profileAlwaysSave->setChecked(Config->readBoolEntry("Always Save Profiles",false));
	useExistingGroups->setChecked(Config->readBoolEntry("Use Existing Groups",false));
	groupAlwaysSave->setChecked(Config->readBoolEntry("Always Save Groups",false));
	docAlwaysCalc->setChecked(Config->readBoolEntry("Always Calc Docs",false));
	docAlwaysSave->setChecked(Config->readBoolEntry("Always Save Docs",false));

	//plugins path
	RString pluginsPaths=Config->readPathEntry("PluginsPath","/home").ascii();
	int findindex=pluginsPaths.FindStr(";",0);
	while(findindex!=-1)
	{
		if (!pluginsPaths.Mid(0,findindex).IsEmpty())
			pluginsPath.InsertPtr(new RString(pluginsPaths.Mid(0,findindex)));
		pluginsPaths=pluginsPaths.Mid(findindex+1);
		findindex=pluginsPaths.FindStr(";",0);
	}
	if (!pluginsPaths.IsEmpty())
		pluginsPath.InsertPtr(new RString(pluginsPaths));

	// Size
	QSize size=Config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	Config->setGroup("Database Options");
	dbHost=Config->readEntry("Host","127.0.0.1").ascii();
	dbName=Config->readEntry("Name","galilei").ascii();
	dbUser=Config->readEntry("User","root").ascii();
	dbPwd=Config->readEntry("Password","").ascii();
	dbEncoding=Config->readEntry("Encoding","latin1").ascii();

	// Read create database options
	CreateDbSQLpath=Config->readEntry("CreateDbPath","/usr/share/galilei/sql/").ascii();
	CreateDbUseStopList=Config->readBoolEntry("CreateDbUseStopList",true);
	CreateDbUseUsers=Config->readBoolEntry("CreateDbUseUsers",true);

	// Save options for the plug-ins dialog box
	DlgMainTabIdx=Config->readNumEntry("DlgMainTabIdx",0);
	DlgDocsTabIdx=Config->readNumEntry("DlgDocsTabIdx",0);
	DlgProfilesTabIdx=Config->readNumEntry("DlgProfilesTabIdx",0);
	DlgCommunitiesTabIdx=Config->readNumEntry("DlgCommunitiesTabIdx",0);
	DlgSearchTabIdx=Config->readNumEntry("DlgSearchTabIdx",0);

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readGALILEIOptions(void)
{
	// Read GALILEI Config
	try
	{
		GConfig Conf("/etc/galilei/galilei.galileiconfig");
		Conf.Load();
	}
	catch(...)
	{
	}
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
	sessionDisconnect->setEnabled(true);
	showUsers->setEnabled(true);
	showGroups->setEnabled(true);
	profilesCalc->setEnabled(true);
	profileCalc->setEnabled(true);
	simulationDlg->setEnabled(true);
	sessionCompute->setEnabled(true);
	doFdbks->setEnabled(true);
	groupingCompare->setEnabled(true);
	somView->setEnabled(true);
	textFrench->setEnabled(true);
	textEnglish->setEnabled(true);
	groupsCalc->setEnabled(true);
	groupingCreate->setEnabled(true);
	showGroupsHistory->setEnabled(true);
	showDocs->setEnabled(true);
	docAnalyse->setEnabled(true);
	docsAnalyse->setEnabled(true);
	docsIndexer->setEnabled(true);
	queryMetaEngine->setEnabled(true);
	runProgram->setEnabled(true);
	sessionStats->setEnabled(true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::DisableAllActions(void)
{
	sessionDisconnect->setEnabled(false);
	showUsers->setEnabled(false);
	showGroups->setEnabled(false);
	profilesCalc->setEnabled(false);
	profileCalc->setEnabled(false);
	simulationDlg->setEnabled(false);
	sessionCompute->setEnabled(false);
	doFdbks->setEnabled(false);
	groupingCompare->setEnabled(false);
	somView->setEnabled(false);
	textFrench->setEnabled(false);
	textEnglish->setEnabled(false);
	createXML->setEnabled(false);
	saveXML->setEnabled(false);
	analyseXML->setEnabled(false);
	groupsCalc->setEnabled(false);
	groupingCreate->setEnabled(false);
	showGroupsHistory->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	docsIndexer->setEnabled(false);
	queryMetaEngine->setEnabled(false);
	runProgram->setEnabled(false);
	sessionStats->setEnabled(false);
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
			e->accept();
			return(true);
		}
	}
	else
		return(QWidget::eventFilter(object,event));
}
