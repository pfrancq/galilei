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
#include <rqt.h>
#include <rxmlfile.h>
#include <rxmltag.h>
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
KGALILEICenterApp::KGALILEICenterApp(void)
	: KMainWindow(0,"KGALILEICenterApp"), pluginsPath(10,5), Doc(0), Debug(0)
{
	Config=kapp->config();
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
	sessionSave=new KToggleAction(i18n("Save Results"),0,0,0,actionCollection(),"sessionSave");
	connect(sessionSave,SIGNAL(toggled(bool)),this,SLOT(slotSaveModifier()));
	sessionConnect=new KAction(i18n("&Connect Database"),"connect_established",0,this,SLOT(slotSessionConnect()),actionCollection(),"sessionConnect");
	sessionCompute=new KAction(i18n("Compute &Session"),"make_kdevelop",0,this,SLOT(slotSessionCompute()),actionCollection(),"sessionCompute");
	createDatabase=new KAction(i18n("Create &Database"),"exec",0,this,SLOT(slotCreateDatabase()),actionCollection(),"createDatabase");
	fillEmptyDb=new KAction(i18n("&Fill Empty Database"),"exec",0,this,SLOT(slotFillEmptyDb()),actionCollection(),"fillEmptyDb");
	runProgram=new KAction(i18n("&Run Program"),"rebuild",0,this,SLOT(slotRunProgram()),actionCollection(),"runProgram");
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionStats=new KAction(i18n("&Statistics"),"gohome",0,this,SLOT(slotSessionStats()),actionCollection(),"sessionStats");
	sessionQuit=new KAction(i18n("E&xit"),"exit",0,this,SLOT(slotSessionQuit()),actionCollection(),"sessionQuit");

	// Menu "Users"
	usersClear=new KAction(i18n("&Force Re-computing Users"),0,0,this,SLOT(slotUsersClear()),actionCollection(),"usersClear");
	showUsers=new KAction(i18n("&Show Users"),"kdmconfig",0,this,SLOT(slotShowUsers()),actionCollection(),"showUsers");
	profilesCalc=new KAction(i18n("&Calc Profiles"),"run",0,this,SLOT(slotProfilesCalc()),actionCollection(),"profilesCalc");
	postProfilesCalc=new KAction(i18n("Execute &Post-Profiles Methods"),0,0,this,SLOT(slotPostProfilesCalc()),actionCollection(),"postProfilesCalc");
	profileCalc=new KAction(i18n("Calc &Profile"),"run",0,this,SLOT(slotProfileCalc()),actionCollection(),"profileCalc");

	// Menu "Groups"
	groupsClear=new KAction(i18n("&Force Re-computing Groups"),0,0,this,SLOT(slotGroupsClear()),actionCollection(),"groupsClear");
	showGroups=new KAction(i18n("&Show Groups"),"window_list",0,this,SLOT(slotShowGroups()),actionCollection(),"showGroups");
	groupsCalc=new KAction(i18n("Compute &Groups"),"exec",0,this,SLOT(slotGroupsCalc()),actionCollection(),"groupsCalc");
	postGroupsCalc=new KAction(i18n("Execute &Post-Groups Methods"),0,0,this,SLOT(slotPostGroups()),actionCollection(),"postGroupsCalc");
	somView=new KAction(i18n("View Self-Organizing Map"),"exec",0,this,SLOT(slotChooseSOM()),actionCollection(),"somView");
	showGroupsHistory=new KAction(i18n("Show Groups &History"),"exec",0,this,SLOT(slotShowHistory	()),actionCollection(),"showGroupsHistorys");

	// Menu "Document"
	docsClear=new KAction(i18n("&Force Re-computing Documents"),0,0,this,SLOT(slotDocsClear()),actionCollection(),"docsClear");
	showDocs=new KAction(i18n("Show &Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docAnalyse=new KAction(i18n("&Load and Analyse a Document"),0,this,SLOT(slotDocAnalyse()),actionCollection(),"docAnalyse");
	docsAnalyse=new KAction(i18n("&Analyse Documents"),"kfind",0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");
	postDocsAnalyse=new KAction(i18n("Execute &Post-Documents Methods"),0,0,this,SLOT(slotPostDocsAnalyse()),actionCollection(),"postDocsAnalyse");
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
	doAssessments=new KAction(i18n("&Assessments Cycle"),0,0,this,SLOT(slotDoAssessments()),actionCollection(),"doAssessments");
	groupingCompare=new KAction(i18n("Ideal clustering"),"fileopen",0,this,SLOT(slotGroupingCompare()),actionCollection(),"groupingCompare");
	textFrench=new KAction(i18n("Analyze &French Stems"),0,this,SLOT(slotTextFrench()),actionCollection(),"textFrench");
	textEnglish=new KAction(i18n("Analyze &English Stems"),0,this,SLOT(slotTextEnglish()),actionCollection(),"textEnglish");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection(),"settings_show_toolbar");
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection(), "settings_statusbar");
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	plugins=new KAction(i18n("Configure &Plug-Ins..."),"wizard",0,this,SLOT(slotPlugins()),actionCollection(),"plugins");
	changeDebug=new KAction(i18n("&Configure GALILEI..."),"configure",0,this,SLOT(slotChangeDebug()),actionCollection(),"changeDebug");

	// Menu "Window"
	windowTile = new KAction(i18n("&Tile"), 0, this, SLOT(slotWindowTile()), actionCollection(),"window_tile");
	windowCascade = new KAction(i18n("&Cascade"), 0, this, SLOT(slotWindowCascade()), actionCollection(),"window_cascade");
	windowMenu = new KActionMenu(i18n("&Window"), actionCollection(), "window_menu");
	connect(windowMenu->popupMenu(),SIGNAL(aboutToShow()),this,SLOT(windowMenuAboutToShow()));

	// Help Menu
	helpProgram = new KAction(i18n("List of all classes"), 0, this, SLOT(slotHelpProgram()), actionCollection(),"helpProgram");
/*	helpProgram=new KAction("List of all classes",0,0,this,SLOT(slotHelpProgram()),this);
	menuBar()->insertItem ("&Help",helpProgram);*/

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
	Config->writeEntry("Save Results",sessionSave->isChecked());

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
	Config->writeEntry("Config File",ToQString(ConfigFile));
	try
	{
		// Write PlugIns config
		GConfig Conf(PlugInsConfig);
		Conf.Save();

		// Write config
		RXMLStruct Config;
		RXMLTag* Top=new RXMLTag("GALILEI");
		Config.AddTag(0,Top);
		RXMLTag* Tag=new RXMLTag("log");
		Tag->InsertAttr("file",LogFile);
		Config.AddTag(Top,Tag);
		Tag=new RXMLTag("debug");
		if(Debug)
			Tag->InsertAttr("file",Debug->GetName());
		else
			Tag->InsertAttr("file",RString::Null);
		Config.AddTag(Top,Tag);
		Tag=new RXMLTag("plugins");
		Config.AddTag(Top,Tag);
		RXMLTag* Tag2=new RXMLTag("config");
		Tag2->InsertAttr("file",PlugInsConfig);
		Config.AddTag(Tag,Tag2);
		RCursor<RString> cPath(pluginsPath);
		for(cPath.Start();!cPath.End();cPath.Next())
		{
			Tag2=new RXMLTag("search");
			Tag2->InsertAttr("dir",*cPath());
			Config.AddTag(Tag,Tag2);
		}
		RXMLFile File(ConfigFile,&Config);
		File.Open(RIO::Create);
	}
	catch(...)
	{
		KMessageBox::error(this,"Problem while saving the configuration file '"+ToQString(ConfigFile)+"'","Configuration Error");
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
	sessionSave->setChecked(Config->readBoolEntry("Save Results",false));

	// Size
	QSize size=Config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	QString configName=Config->readEntry("Config File","/etc/galilei/galilei.conf");
	ConfigFile=FromQString(configName);
	try
	{
		// Load configuration for plug-ins and plug-ins
		RXMLStruct Config;
		RXMLFile File(ConfigFile,&Config);
		File.Open(RIO::Read);
		RXMLTag* Tag=Config.GetTag("plugins");
		if(!Tag)
			throw GException("Problems with the configure file '"+ConfigFile+"'");
		RCursor<RXMLTag> Plugins(Tag->GetNodes());
		for(Plugins.Start();!Plugins.End();Plugins.Next())
		{
			if((Plugins()->GetName()=="search")&&(Plugins()->IsAttrDefined("dir")))
				pluginsPath.InsertPtr(new RString(Plugins()->GetAttrValue("dir")));
			if(Plugins()->GetName()=="config")
				PlugInsConfig=Plugins()->GetAttrValue("file");
		}
		LogFile=Config.GetTagAttrValue("log","file");
		RString DebugFile=Config.GetTagAttrValue("debug","file");

		// Load and configure all plug-ins
		GPluginManagers::PluginManagers.Load(pluginsPath);
		GConfig Conf(PlugInsConfig);
		Conf.Load();

		// Create (if necessary) the debug file
		if(DebugFile!=RString::Null)
			Debug=new RDebugXML(DebugFile);
	}
	catch(...)
	{
		KMessageBox::error(this,"Problem while analyzing the configuration file '"+configName+"'","Configuration Error");
	}

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
	postProfilesCalc->setEnabled(true);
	profileCalc->setEnabled(true);
	simulationDlg->setEnabled(true);
	sessionCompute->setEnabled(true);
	doFdbks->setEnabled(true);
	doAssessments->setEnabled(true);
	groupingCompare->setEnabled(true);
	somView->setEnabled(true);
	textFrench->setEnabled(true);
	textEnglish->setEnabled(true);
	groupsCalc->setEnabled(true);
	postGroupsCalc->setEnabled(true);
	groupingCreate->setEnabled(true);
	showGroupsHistory->setEnabled(true);
	showDocs->setEnabled(true);
	docAnalyse->setEnabled(true);
	docsAnalyse->setEnabled(true);
	postDocsAnalyse->setEnabled(true);
	docsIndexer->setEnabled(true);
	queryMetaEngine->setEnabled(true);
	runProgram->setEnabled(true);
	sessionStats->setEnabled(true);
	sessionSave->setEnabled(true);
	docsClear->setEnabled(true);
	usersClear->setEnabled(true);
	groupsClear->setEnabled(true);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::DisableAllActions(void)
{
	sessionDisconnect->setEnabled(false);
	showUsers->setEnabled(false);
	showGroups->setEnabled(false);
	profilesCalc->setEnabled(false);
	postProfilesCalc->setEnabled(false);
	profileCalc->setEnabled(false);
	simulationDlg->setEnabled(false);
	sessionCompute->setEnabled(false);
	doFdbks->setEnabled(false);
	doAssessments->setEnabled(false);
	groupingCompare->setEnabled(false);
	somView->setEnabled(false);
	textFrench->setEnabled(false);
	textEnglish->setEnabled(false);
	createXML->setEnabled(false);
	saveXML->setEnabled(false);
	analyseXML->setEnabled(false);
	groupsCalc->setEnabled(false);
	postGroupsCalc->setEnabled(false);
	groupingCreate->setEnabled(false);
	showGroupsHistory->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	postDocsAnalyse->setEnabled(false);
	docsIndexer->setEnabled(false);
	queryMetaEngine->setEnabled(false);
	runProgram->setEnabled(false);
	sessionStats->setEnabled(false);
	sessionSave->setEnabled(false);
	docsClear->setEnabled(false);
	usersClear->setEnabled(false);
	groupsClear->setEnabled(false);
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
