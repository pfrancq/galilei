/*

	GALILEI Research Project

	KGALILEICenter_init.cpp

	Main Window - Implementation of the Init part.

	Copyright 2002 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <sessions/gsessionmysql.h>
#include <sessions/gconfig.h>
#include <docs/gdocoptions.h>
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
KGALILEICenterApp::KGALILEICenterApp(void)
	: KMainWindow(0,"KGALILEICenterApp"), URLManager(getenv("GALILEI_PLUGINS_LIB")),
	  ProfilingManager(getenv("GALILEI_PLUGINS_LIB")), GroupingManager(getenv("GALILEI_PLUGINS_LIB")),
	  dbHost(""),dbName(""),dbUser(""),dbPwd(""),Doc(0), DocOptions(0)
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
	createDatabase=new KAction(i18n("Create &Database"),"exec",0,this,SLOT(slotCreateDatabase()),actionCollection(),"createDatabase");
	fillEmptyDb=new KAction(i18n("&Fill Empty Database"),"exec",0,this,SLOT(slotFillEmptyDb()),actionCollection(),"fillEmptyDb");
	runProgram=new KAction(i18n("&Run Program"),"rebuild",0,this,SLOT(slotRunProgram()),actionCollection(),"runProgram");
	runInsts=new KAction(i18n("Run &Instructions"),"help",0,this,SLOT(slotRunInsts()),actionCollection(),"runInsts");;
	sessionDisconnect=new KAction(i18n("&Disconnect Database"),"connect_no",0,this,SLOT(slotSessionDisconnect()),actionCollection(),"sessionDisconnect");
	sessionTest=new KAction(i18n("&Test"),"gohome",0,this,SLOT(slotSessionTest()),actionCollection(),"sessionTest");
	sessionStat=new KAction(i18n("&Stat"),"gohome",0,this,SLOT(slotSessionStat()),actionCollection(),"sessionStat");
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
	groupsEvaluation=new KAction(i18n("&Evaluate Grouping"),"fileopen",0,this,SLOT(slotGroupsEvaluation()),actionCollection(),"groupsEvaluation");
	runQuery=new KAction(i18n("Run &Query"),"help",0,this,SLOT(slotRunQuery()),actionCollection(),"runQuery");
	groupingCompareFromFile=new KAction(i18n("From &File"),"fileopen",0,this,SLOT(slotGroupingCompareFromFile()),actionCollection(),"groupingCompareFromFile");
	groupingCompare=new KAction(i18n("From &Memory"),"fileopen",0,this,SLOT(slotGroupingCompare()),actionCollection(),"groupingCompare");
	mixIdealGroups=new KAction(i18n("Load&Mix Ideal Groups"),"exec",0,this,SLOT(slotMixIdealGroups()),actionCollection(),"mixIdealGroups");
	showGroupsHistory=new KAction(i18n("Show Groups History"),"exec",0,this,SLOT(slotShowHistory()),actionCollection(),"showGroupsHistory");

	// Menu "Document"
	docAlwaysCalc=new KToggleAction(i18n("Enables/disables documents Recomputing"),0,0,0,actionCollection(),"docAlwaysCalc");
	wordsClusteringSave=new KToggleAction(i18n("Enables/disables Clusters of Words Saving"),0,0,0,actionCollection(),"wordsClusteringSave");
	showDocs=new KAction(i18n("&Show Documents"),"kmultiple",0,this,SLOT(slotShowDocs()),actionCollection(),"showDocs");
	docAnalyse=new KAction(i18n("&Load and Analyse a Document"),0,this,SLOT(slotDocAnalyse()),actionCollection(),"docAnalyse");;
	docsAnalyse=new KAction(i18n("&Analyse Documents"),0,this,SLOT(slotDocsAnalyse()),actionCollection(),"docsAnalyse");;
	docsStats=new KAction(i18n("S&tatistics about Documents"),0,this,SLOT(slotDocsStats()),actionCollection(),"docsStats");;
	createXML=new KAction(i18n("&Create XML Structure"),"readme",0,this,SLOT(slotCreateXML()),actionCollection(),"createXML");
	saveXML=new KAction(i18n("&Save XML Structure"),"readme",0,this,SLOT(slotSaveXML()),actionCollection(),"saveXML");
	analyseXML=new KAction(i18n("&Analyse XML Structure"),"filefind",0,this,SLOT(slotAnalyseXML()),actionCollection(),"analyseXML");
	wordsClustering=new KAction(i18n("&Words Clustering"),0,this,SLOT(slotWordsClustering()),actionCollection(),"wordsClustering");
	removeCluster=new KAction(i18n("&Remove Cluster"),0,this,SLOT(slotRemoveCluster()),actionCollection(),"removeCluster");

	// Menu "Texts"
	textFrench=new KAction(i18n("Analyze &French Stems"),0,this,SLOT(slotTextFrench()),actionCollection(),"textFrench");
	textEnglish=new KAction(i18n("Analyze &English Stems"),0,this,SLOT(slotTextEnglish()),actionCollection(),"textEnglish");

	// Menu "R Stat
	rRunR=new KAction(i18n("Run &R"),0,this,SLOT(slotRRunR()),actionCollection(),"rRunR");

	// Menu "GA"
	gaInit=new KAction(i18n("&Initialize"),"reload",KKey("Alt+I").keyCodeQt(),this,SLOT(slotGAInit(void)),actionCollection(),"gaInit");
	gaStart=new KAction(i18n("&Start"),"exec",KKey("Alt+S").keyCodeQt(),this,SLOT(slotGAStart(void)),actionCollection(),"gaStart");
	gaPause=new KAction(i18n("&Pause"),"player_pause",KKey("Alt+P").keyCodeQt(),this,SLOT(slotGAPause(void)),actionCollection(),"gaPause");
	gaStop=new KAction(i18n("S&top"),"stop",KKey("Alt+T").keyCodeQt(),this,SLOT(slotGAStop(void)),actionCollection(),"gaStop");
	gaAnalyse=new KAction(i18n("&Analyse Stored Chromosomes"),"viewmag",0,this,SLOT(slotGAAnalyse(void)),actionCollection(),"gaAnalyse");
	gaSave=new KAction(i18n("S&tore Chromosomes"),"save",0,this,SLOT(slotGASave(void)),actionCollection(),"gaSave");
	gaShow=new KAction(i18n("Sho&w Chromosomes"),"save",0,this,SLOT(slotGAShow(void)),actionCollection(),"gaShow");

	// Menu "Settings"
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));
	docsOptions=new KAction(i18n("&Documents Options"),"configure",0,this,SLOT(slotDocsOptions()),actionCollection(),"docsOptions");
	plugins=new KAction(i18n("&Plugins"),"wizard",0,this,SLOT(slotPlugins()),actionCollection(),"plugins");
	sessionOptions=new KAction(i18n("&Session Options"),"configure",0,this,SLOT(slotSessionOptions()),actionCollection(),"sessionOptions");
	oldPlugins=new KAction(i18n("&Old Plugins"),"wizard",0,this,SLOT(slotOldPlugins()),actionCollection(),"oldplugins");

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
	Config->writeEntry("Always Save Clusters",wordsClusteringSave->isChecked());

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
	Config->writeEntry("Minimum Documents",DocOptions->MinDocs);
	Config->writeEntry("Maximum Documents",DocOptions->MaxDocs);
	Config->writeEntry("Minimum Occurence Clustering",DocOptions->MinOccurCluster);
	Config->writeEntry("Iteration Number",DocOptions->NbIteration);
	Config->writeEntry("Window Size",DocOptions->WindowSize);
	Config->writeEntry("Minimum Confidence",DocOptions->MinConfidence);
	Config->writeEntry("Using Document Structure",DocOptions->Distance);
	Config->writeEntry("Minimum of Appearance",DocOptions->NbDocsMin);
	Config->writeEntry("Use Link",DocOptions->UseLink);
	Config->writeEntry("Use External Link",DocOptions->UseExternalLink);
	Config->writeEntry("Use Redirection",DocOptions->UseRedirection);

	// Write Config of Session Options
	Config->setGroup("Session Options");
	Config->writeEntry("Description Method",GetCurrentProfileDesc());
	Config->writeEntry("Group Description Method",GetCurrentGroupCalcMethod());
	Config->writeEntry("Link Description Method",GetCurrentLinkCalcMethod());

	// Write Config of GroupCalcRelevant
	Config->setGroup(CalcRelevantParams.GetComputingName());
	Config->writeEntry("Global Similarity",CalcRelevantParams.GlobalSim);
	Config->writeEntry("MaxNonZero",CalcRelevantParams.MaxNonZero);

	// Write Config of GroupCalcGravitation
	Config->setGroup(CalcGravitationParams.GetComputingName());
	Config->writeEntry("MaxNonZero",CalcGravitationParams.MaxNonZero);

	// Write Config of LinkCalcHITS
	Config->setGroup(LinkCalcHITSParams.GetComputingName());
	Config->writeEntry("NbIteration",LinkCalcHITSParams.NbIteration);
	Config->writeEntry("NbResultsHub",LinkCalcHITSParams.NbResultsHub);
	Config->writeEntry("NbResultsAuto",LinkCalcHITSParams.NbResultsAuto);
	Config->writeEntry("LimitLink",LinkCalcHITSParams.LimitLink);
	Config->writeEntry("NbLinks",LinkCalcHITSParams.NbLinks);
	Config->writeEntry("UseMultipleLink",LinkCalcHITSParams.UseMultipleLink);

	// Write Config of LinkCalcCorrespondence
	Config->setGroup(LinkCalcCorrespondenceParams.GetComputingName());
	Config->writeEntry("NbIteration",LinkCalcCorrespondenceParams.NbIteration);
	Config->writeEntry("NbResultsHub",LinkCalcCorrespondenceParams.NbResultsHub);
	Config->writeEntry("NbResultsAuto",LinkCalcCorrespondenceParams.NbResultsAuto);
	Config->writeEntry("LimitLink",LinkCalcCorrespondenceParams.LimitLink);
	Config->writeEntry("NbLinks",LinkCalcCorrespondenceParams.NbLinks);
	Config->writeEntry("UseMultipleLink",LinkCalcCorrespondenceParams.UseMultipleLink);

	// Write Config of LinkCalcSALSA
	Config->setGroup(LinkCalcSALSAParams.GetComputingName());
	Config->writeEntry("NbResultsHub",LinkCalcSALSAParams.NbResultsHub);
	Config->writeEntry("NbResultsAuto",LinkCalcSALSAParams.NbResultsAuto);
	Config->writeEntry("LimitLink",LinkCalcSALSAParams.LimitLink);
	Config->writeEntry("NbLinks",LinkCalcSALSAParams.NbLinks);
	Config->writeEntry("UseMultipleLink",LinkCalcSALSAParams.UseMultipleLink);

	// Write Config of LinkCalcTresh
	Config->setGroup(LinkCalcTreshParams.GetComputingName());
	Config->writeEntry("NbIteration",LinkCalcTreshParams.NbIteration);
	Config->writeEntry("NbResultsHub",LinkCalcTreshParams.NbResultsHub);
	Config->writeEntry("NbResultsAuto",LinkCalcTreshParams.NbResultsAuto);
	Config->writeEntry("HTresh",LinkCalcTreshParams.HTresh);
	Config->writeEntry("ATresh",LinkCalcTreshParams.ATresh);
	Config->writeEntry("FTresh",LinkCalcTreshParams.FTresh);
	Config->writeEntry("AKTresh",LinkCalcTreshParams.AKTresh);
	Config->writeEntry("FKTresh",LinkCalcTreshParams.FKTresh);
	Config->writeEntry("Kvalue",LinkCalcTreshParams.Kvalue);

	// Write the session parameters
	Config->setGroup(SessionParams.GetName());
	Config->writeEntry("SameBehaviourMinDocs",SessionParams.GetUInt("SameBehaviourMinDocs"));
	Config->writeEntry("DiffBehaviourMinDocs",SessionParams.GetUInt("DiffBehaviourMinDocs"));

	// Save Config
	GConfig Conf("/etc/galilei/galilei.galileiconfig");
	Conf.Store(URLManager);
	Conf.Store(ProfilingManager);
	Conf.Store(GroupingManager);
	Conf.Save();
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
	wordsClusteringSave->setChecked(Config->readBoolEntry("Always Save Clusters",false));

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
	DocOptions->MinDocs=Config->readUnsignedNumEntry("Minimum Documents",5);
	DocOptions->MaxDocs=Config->readUnsignedNumEntry("Maximum Documents",300);
	DocOptions->MinOccurCluster=Config->readUnsignedNumEntry("Minimum Occurence Clustering",1);
	DocOptions->NbIteration=Config->readUnsignedNumEntry("Iteration Number",2);
	DocOptions->WindowSize=Config->readUnsignedNumEntry("Window Size",10);
	DocOptions->MinConfidence=Config->readDoubleNumEntry("Minimum Confidence",0.15);
	DocOptions->Distance=Config->readBoolEntry("Using Document Structure",false);
	DocOptions->NbDocsMin=Config->readUnsignedNumEntry("Minimum of Appearance",5);
	DocOptions->UseLink=Config->readBoolEntry("Use Link",true);
	DocOptions->UseExternalLink=Config->readBoolEntry("Use External Link",false);
	DocOptions->UseRedirection=Config->readBoolEntry("Use Redirection",false);

	// Read Session Options
	ProfileDesc = new R::RContainer<R::RString,unsigned int,true,true>(3,3);
	ProfileDesc->InsertPtr(new R::RString("Vector space"));
	GroupCalcMethod = new R::RContainer<R::RString,unsigned int,true,true>(3,3);
	GroupCalcMethod->InsertPtr(new R::RString("Gravitation"));
	GroupCalcMethod->InsertPtr(new R::RString("Prototype"));
	LinkCalcMethod=new R::RContainer<R::RString,unsigned int,true,true>(3,3);
	LinkCalcMethod->InsertPtr(new R::RString("HITS Algorithm"));
	LinkCalcMethod->InsertPtr(new R::RString("Correspondence Algorithm"));
	LinkCalcMethod->InsertPtr(new R::RString("SALSA Algorithm"));
	LinkCalcMethod->InsertPtr(new R::RString("Treshold Kleinberg Algorithm"));
	Config->setGroup("Session Options");
	CurrentProfileDesc=Config->readEntry("Description Method","Vector space");
	CurrentGroupCalcMethod=Config->readEntry("Group Description Method","Prototype");
	CurrentLinkCalcMethod=Config->readEntry("Link Description Method","HITS Algorithm");

	// Read Config of GroupCalcRelevant
	Config->setGroup(CalcRelevantParams.GetComputingName());
	CalcRelevantParams.GlobalSim=Config->readBoolEntry("Global Similarity",1);
	CalcRelevantParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);

	// Read Config of GroupCalcGravitation
	Config->setGroup(CalcGravitationParams.GetComputingName());
	CalcGravitationParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);

	// Read Config of LinkCalcHITS
	Config->setGroup(LinkCalcHITSParams.GetComputingName());
	LinkCalcHITSParams.NbIteration=Config->readNumEntry("NbIteration",5);
	LinkCalcHITSParams.NbResultsHub=Config->readNumEntry("NbResultsHub",5);
	LinkCalcHITSParams.NbResultsAuto=Config->readNumEntry("NbResultsAuto",5);
	LinkCalcHITSParams.LimitLink=Config->readBoolEntry("LimitLink",false);
	LinkCalcHITSParams.NbLinks=Config->readNumEntry("NbLinks",10);
	LinkCalcHITSParams.UseMultipleLink=Config->readBoolEntry("UseMultipleLink",true);

	// Read Config of LinkCalcCorrespondence
	Config->setGroup(LinkCalcCorrespondenceParams.GetComputingName());
	LinkCalcCorrespondenceParams.NbIteration=Config->readNumEntry("NbIteration",5);
	LinkCalcCorrespondenceParams.NbResultsHub=Config->readNumEntry("NbResultsHub",5);
	LinkCalcCorrespondenceParams.NbResultsAuto=Config->readNumEntry("NbResultsAuto",5);
	LinkCalcCorrespondenceParams.LimitLink=Config->readBoolEntry("LimitLink",false);
	LinkCalcCorrespondenceParams.NbLinks=Config->readNumEntry("NbLinks",10);
	LinkCalcCorrespondenceParams.UseMultipleLink=Config->readBoolEntry("UseMultipleLink",true);

	// Read Config of LinkCalcSALSA
	Config->setGroup(LinkCalcSALSAParams.GetComputingName());
	LinkCalcSALSAParams.NbResultsHub=Config->readNumEntry("NbResultsHub",5);
	LinkCalcSALSAParams.NbResultsAuto=Config->readNumEntry("NbResultsAuto",5);
	LinkCalcSALSAParams.LimitLink=Config->readBoolEntry("LimitLink",false);
	LinkCalcSALSAParams.NbLinks=Config->readNumEntry("NbLinks",10);
	LinkCalcSALSAParams.UseMultipleLink=Config->readBoolEntry("UseMultipleLink",true);

	// Read Config of LinkCalcTresh
	Config->setGroup(LinkCalcTreshParams.GetComputingName());
	LinkCalcTreshParams.NbIteration=Config->readNumEntry("NbIteration",5);
	LinkCalcTreshParams.NbResultsHub=Config->readNumEntry("NbResultsHub",5);
	LinkCalcTreshParams.NbResultsAuto=Config->readNumEntry("NbResultsAuto",5);
	LinkCalcTreshParams.HTresh=Config->readBoolEntry("HTresh",true);
	LinkCalcTreshParams.ATresh=Config->readBoolEntry("ATresh",false);
	LinkCalcTreshParams.FTresh=Config->readBoolEntry("FTresh",false);
	LinkCalcTreshParams.AKTresh=Config->readBoolEntry("AKTresh",false);
	LinkCalcTreshParams.FKTresh=Config->readBoolEntry("FKTresh",false);
	LinkCalcTreshParams.Kvalue=Config->readNumEntry("Kvalue",60);

	// Read the Session Parameters
	Config->setGroup(SessionParams.GetName());
	SessionParams.Set("SameBehaviourMinDocs",Config->readNumEntry("SameBehaviourMinDocs",0));
	SessionParams.Set("DiffBehaviourMinDocs",Config->readNumEntry("DiffBehaviourMinDocs",0));

	// Read Config
	GConfig Conf("/etc/galilei/galilei.galileiconfig");
	Conf.Load();
	Conf.Read(URLManager);
	Conf.Read(ProfilingManager);
	Conf.Read(GroupingManager);
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
	mixIdealGroups->setEnabled(true);

	// Menu "Document"
	showDocs->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad());
	docAnalyse->setEnabled(Doc&&Doc->GetSession()->IsDicsLoad());
	docsAnalyse->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());
	//linksCalc->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad());
	docsStats->setEnabled(Doc&&Doc->GetSession()->IsDocsLoad()&&Doc->GetSession()->IsDicsLoad());

	// Menu "GA"
	gaInit->setEnabled(Doc&&Doc->GetSession()->IsGroupsLoad()&&Doc->GetSession()->IsUsersLoad());
	gaShow->setEnabled(true);
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
	wordsClustering->setEnabled(false);
	removeCluster->setEnabled(false);
	groupsCalc->setEnabled(false);
	mixIdealGroups->setEnabled(false);
	showGroupsHistory->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	docsStats->setEnabled(false);
	gaInit->setEnabled(false);
	gaPause->setEnabled(false);
	gaStart->setEnabled(false);
	gaStop->setEnabled(false);
	gaShow->setEnabled(false);
	runProgram->setEnabled(false);
	runInsts->setEnabled(false);
	runQuery->setEnabled(false);
	rRunR->setEnabled(false);
	gaAnalyse->setEnabled(false);
	gaSave->setEnabled(false);
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
