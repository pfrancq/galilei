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
	gaStop=new KAction(i18n("S&top"),"stop",KKey("Alt+T").keyCodeQt(),this,SLOT(slotGAStop(void)),actionCollection(),"gaStop");
	gaAnalyse=new KAction(i18n("&Analyse Stored Chromosomes"),"viewmag",0,this,SLOT(slotGAAnalyse(void)),actionCollection(),"gaAnalyse");
	gaSave=new KAction(i18n("S&tore Chromosomes"),"save",0,this,SLOT(slotGASave(void)),actionCollection(),"gaSave");

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
void KGALILEICenterApp::saveOptions(GSimMeasure* s)
{
	Config->writeEntry(QString(s->Name)+" Measure Weight",s->Weight);
	Config->writeEntry(QString(s->Name)+" Measure Use",s->Use);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::saveOptions(const char* n,RPromethee::RPromCriterionParams& c)
{
	Config->writeEntry(QString(n)+" Weight",c.Weight);
	Config->writeEntry(QString(n)+" P",c.P);
	Config->writeEntry(QString(n)+" Q",c.Q);
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

	// Write Config of Session Options
	Config->setGroup("Session Options");
	Config->writeEntry("Description Method",GetCurrentProfileDesc());
	Config->writeEntry("Computing Method",GetCurrentComputingMethod());
	Config->writeEntry("Grouping Method",GetCurrentGroupingMethod());
	Config->writeEntry("Group Description Method",GetCurrentGroupCalcMethod());

	// Write Config of GA
	Config->setGroup(IRParams.GetGroupingName());
	Config->writeEntry("Population Size",IRParams.PopSize);
	Config->writeEntry("Maximal Generations",IRParams.MaxGen);
	Config->writeEntry("Step Mode",IRParams.Step);
	Config->writeEntry("Step Generations",IRParams.StepGen);
	
	Config->writeEntry("Minimum similarity",IRParams.MinSimLevel);
	Config->writeEntry("Minimum Common OK",IRParams.MinCommonOK);
	Config->writeEntry("Minimum Common Diff",IRParams.MinCommonDiff);
	Config->writeEntry("Maximum K-Means Interation",IRParams.MaxKMeans);
	Config->writeEntry("Type of Similarity Criterion",IRParams.SimMeasures);
	for(IRParams.Measures.Start();!IRParams.Measures.End();IRParams.Measures.Next())
		saveOptions(IRParams.Measures());
	saveOptions("Similarity Criterion",IRParams.ParamsSim);
	saveOptions("Information Criterion",IRParams.ParamsInfo);
	saveOptions("Entropy Criterion",IRParams.ParamsEntropy);
	saveOptions("Likelihood Criterion",IRParams.ParamsLikelihood);
	saveOptions("Same Feedbacks Criterion",IRParams.ParamsSameFeedbacks);
	saveOptions("Different Feedbacks Criterion",IRParams.ParamsDiffFeedbacks);
	saveOptions("Social Criterion",IRParams.ParamsSocial);
	Config->writeEntry("Global Similarity",IRParams.GlobalSim);

	// Write Config of First-Fit
	Config->setGroup(SimParams.GetGroupingName());
	Config->writeEntry("Full Similariry",SimParams.FullSim);
	Config->writeEntry("Minium Similarity",SimParams.LevelSim);
	Config->writeEntry("Global Similarity",SimParams.GlobalSim);

	// Write Config of KMeans
	Config->setGroup(KMeansParams.GetGroupingName());
	Config->writeEntry("Number of Groups",KMeansParams.NbGroups);
	Config->writeEntry("Maximum number of iterations",KMeansParams.NbIters);
	Config->writeEntry("Number of subsamples",KMeansParams.NbSubSamples);
	Config->writeEntry("Rate of element by subsample",KMeansParams.SubSamplesRate);
	Config->writeEntry("Epsilon",KMeansParams.Epsilon);

	// Write Config of KProto
	Config->setGroup(KProtosParams.GetGroupingName());
	Config->writeEntry("Number of Groups",KProtosParams.NbGroups);
	Config->writeEntry("Number of Protos",KProtosParams.NbProtos);
	Config->writeEntry("Maximum number of iterations",KProtosParams.MaxIters);
	Config->writeEntry("Number of subsamples",KProtosParams.NbSubSamples);
	Config->writeEntry("Rate of element by subsample",KProtosParams.SubSamplesRate);
	Config->writeEntry("Distance Parameter",KProtosParams.Lambda);
	Config->writeEntry("VerifyKMeans Limit",KProtosParams.VerifyKMeansLimit);

	// Write Config of Cure
	Config->setGroup(CureParams.GetGroupingName());
	Config->writeEntry("Number of Groups",CureParams.NbGroups);
	Config->writeEntry("Number of Prototyes per Group",CureParams.NbProtos);

	// Write Config of SupKMeans
	Config->setGroup(SupKMeansParams.GetGroupingName());
	Config->writeEntry("UsedAsInitialization",SupKMeansParams.UsedAsInitialization);
	Config->writeEntry("NbPcSame",SupKMeansParams.NbPcSame);
	Config->writeEntry("NbPcDiff",SupKMeansParams.NbPcDiff);
	Config->writeEntry("MinSim",SupKMeansParams.MinSim);
	Config->writeEntry("NbIters",SupKMeansParams.NbIters);
	Config->writeEntry("DoubleKMeans",SupKMeansParams.DoubleKMeans);
	Config->writeEntry("SameGroupRate",SupKMeansParams.SameGroupRate);
	Config->writeEntry("NbSamples",SupKMeansParams.NbSamples);
	Config->writeEntry("InitMode",SupKMeansParams.InitMode);
	Config->writeEntry("Debug",SupKMeansParams.Debug);

	// Write Config of GroupCalcRelevant
	Config->setGroup(CalcRelevantParams.GetComputingName());
	Config->writeEntry("Global Similarity",CalcRelevantParams.GlobalSim);
	Config->writeEntry("MaxNonZero",CalcRelevantParams.MaxNonZero);

	// Write Config of GroupCalcGravitation
	Config->setGroup(CalcGravitationParams.GetComputingName());
	Config->writeEntry("MaxNonZero",CalcGravitationParams.MaxNonZero);

	// Write Config of ProfileCalcFeedback
	Config->setGroup(FeedbackParams.GetComputingName());
	Config->writeEntry("MaxNonZero",FeedbackParams.MaxNonZero);
	Config->writeEntry("RelFactor",FeedbackParams.RelFactor);
	Config->writeEntry("FuzzyFactor",FeedbackParams.FuzzyFactor);
	Config->writeEntry("NoRelFactor",FeedbackParams.NoRelFactor);
	Config->writeEntry("AddFuzzy",FeedbackParams.AddFuzzy);
	Config->writeEntry("IdfFactor",FeedbackParams.IdfFactor);

	// Write Config of ProfileCalcVector
	Config->setGroup(StatisticalParams.GetComputingName());
	Config->writeEntry("MaxNonZero",StatisticalParams.MaxNonZero);
 	Config->writeEntry("IdfFactor",StatisticalParams.IdfFactor);

	// Write Config of ProfileCalcReWeighting
	Config->setGroup(ReWeightingParams.GetComputingName());
	Config->writeEntry("MaxNonZero",ReWeightingParams.MaxNonZero);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readOptions(GSimMeasure* s)
{
	s->Weight=Config->readDoubleNumEntry(QString(s->Name)+" Measure Weight",0.0);
	s->Use=Config->readBoolEntry(QString(s->Name)+" Measure Use",false);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::readOptions(const char* n,RPromethee::RPromCriterionParams& c)
{
	c.Weight=Config->readDoubleNumEntry(QString(n)+" Weight",1.0);
	c.P=Config->readDoubleNumEntry(QString(n)+" P",0.2);
	c.Q=Config->readDoubleNumEntry(QString(n)+" Q",0.05);
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

	// Read Session Options
	GroupingMethod = new RStd::RContainer<RStd::RString,unsigned int,true,true>(3,3);
	GroupingMethod->InsertPtr(new RStd::RString("CURE"));
	GroupingMethod->InsertPtr(new RStd::RString("First-Fit Heuristic"));
	GroupingMethod->InsertPtr(new RStd::RString("Grouping Genetic Algorithms"));
	GroupingMethod->InsertPtr(new RStd::RString("KMeansCosinus"));
	GroupingMethod->InsertPtr(new RStd::RString("SUPKMeans"));
	GroupingMethod->InsertPtr(new RStd::RString("KMeans Prototypes"));
	GroupingMethod->InsertPtr(new RStd::RString("Random Heuristic"));
	ComputingMethod = new RStd::RContainer<RStd::RString,unsigned int,true,true>(3,3);
	ComputingMethod->InsertPtr(new RStd::RString("Statistical"));
	ComputingMethod->InsertPtr(new RStd::RString("Direct Reweighting"));
	ComputingMethod->InsertPtr(new RStd::RString("User Feedback"));
	ProfileDesc = new RStd::RContainer<RStd::RString,unsigned int,true,true>(3,3);
	ProfileDesc->InsertPtr(new RStd::RString("Vector space"));
	GroupCalcMethod = new RStd::RContainer<RStd::RString,unsigned int,true,true>(3,3);
	GroupCalcMethod->InsertPtr(new RStd::RString("Gravitation"));
	GroupCalcMethod->InsertPtr(new RStd::RString("Prototype"));
	Config->setGroup("Session Options");
	CurrentProfileDesc=Config->readEntry("Description Method","Vector space");
	CurrentComputingMethod=Config->readEntry("Computing Method","Statistical");
	CurrentGroupingMethod=Config->readEntry("Grouping Method","First-Fit Heuristic");
	CurrentGroupCalcMethod=Config->readEntry("Group Description Method","Prototype");

	// Read Config of GA
	Config->setGroup(IRParams.GetGroupingName());
	IRParams.PopSize=Config->readNumEntry("Population Size",16);
	IRParams.MaxGen=Config->readNumEntry("Maximal Generations",30);
	IRParams.Step=Config->readBoolEntry("Step Mode",false);
	IRParams.StepGen=Config->readNumEntry("Step Generations",0);
	IRParams.MinSimLevel=Config->readDoubleNumEntry("Minimum similarity",0.1);
	IRParams.MinCommonOK=Config->readDoubleNumEntry("Minimum Common OK",0.6);
	IRParams.MinCommonDiff=Config->readDoubleNumEntry("Minimum Common Diff",0.6);
	IRParams.MaxKMeans=Config->readNumEntry("Maximum K-Means Interation",60);
	IRParams.SimMeasures=static_cast<SimCritType>(Config->readNumEntry("Type of Similarity Criterion",sctCrits));
	for(IRParams.Measures.Start();!IRParams.Measures.End();IRParams.Measures.Next())
		readOptions(IRParams.Measures());
	readOptions("Similarity Criterion",IRParams.ParamsSim);
	readOptions("Information Criterion",IRParams.ParamsInfo);
	readOptions("Entropy Criterion",IRParams.ParamsEntropy);
	readOptions("Likelihood Criterion",IRParams.ParamsLikelihood);
	readOptions("Same Feedbacks Criterion",IRParams.ParamsSameFeedbacks);
	readOptions("Different Feedbacks Criterion",IRParams.ParamsDiffFeedbacks);
	readOptions("Social Criterion",IRParams.ParamsSocial);
	IRParams.GlobalSim=Config->readBoolEntry("Global Similarity",true);

	// Red Config of First-Fit
	Config->setGroup(SimParams.GetGroupingName());
	SimParams.FullSim=Config->readBoolEntry("Full Similariry",true);
	SimParams.LevelSim=Config->readDoubleNumEntry("Minium Similarity",0.1);
	SimParams.GlobalSim=Config->readBoolEntry("Global Similarity",true);

	// Read Config of KMeans
	Config->setGroup(KMeansParams.GetGroupingName());
	KMeansParams.NbGroups=Config->readNumEntry("Number of Groups",13);
	KMeansParams.NbIters=Config->readNumEntry("Maximum number of iterations",20);
	KMeansParams.NbSubSamples=Config->readNumEntry("Number of subsamples",10);
	KMeansParams.SubSamplesRate=Config->readNumEntry("Rate of element by subsample",80);
	KMeansParams.Epsilon=Config->readDoubleNumEntry("Epsilon",0.005);

	// Read Config of KProtos
	Config->setGroup(KProtosParams.GetGroupingName());
	KProtosParams.NbGroups=Config->readNumEntry("Number of Groups",13);
	KProtosParams.NbProtos=Config->readNumEntry("Number of Protos",13);
	KProtosParams.MaxIters=Config->readNumEntry("Maximum number of iterations",20);
	KProtosParams.NbSubSamples=Config->readNumEntry("Number of subsamples",10);
	KProtosParams.SubSamplesRate=Config->readNumEntry("Rate of element by subsample",80);
	KProtosParams.Lambda=Config->readDoubleNumEntry("Distance Parameter",0.5);
	KProtosParams.VerifyKMeansLimit=Config->readNumEntry("VerifyKMeans Limit",KProtosParams.VerifyKMeansLimit);

	// Read Config of Cure
	Config->setGroup(CureParams.GetGroupingName());
	CureParams.NbGroups=Config->readNumEntry("Number of Groups",13);
	CureParams.NbProtos=Config->readNumEntry("Number of Prototyes per Group",2);

	// Read Config of SupKMeans
	Config->setGroup(SupKMeansParams.GetGroupingName());
	SupKMeansParams.UsedAsInitialization=Config->readBoolEntry("UsedAsInitialization",false);
	SupKMeansParams.NbPcSame=Config->readNumEntry("NbPcSame",60);
	SupKMeansParams.NbPcDiff=Config->readNumEntry("NbPcDiff",60);
	SupKMeansParams.MinSim=Config->readDoubleNumEntry("MinSim",0.1);
	SupKMeansParams.NbIters=Config->readNumEntry("NbIters",20);
	SupKMeansParams.DoubleKMeans=Config->readBoolEntry("DoubleKMeans",false);
	SupKMeansParams.SameGroupRate=Config->readNumEntry("SameGroupRate",100);
	SupKMeansParams.NbSamples=Config->readNumEntry("NbSamples",10);
	SupKMeansParams.InitMode=static_cast<GSupKMeansParams::Initial>(Config->readNumEntry("InitMode",GSupKMeansParams::Relevant));
	SupKMeansParams.Debug=Config->readBoolEntry("Debug",false);

	// Read Config of GroupCalcRelevant
	Config->setGroup(CalcRelevantParams.GetComputingName());
	CalcRelevantParams.GlobalSim=Config->readBoolEntry("Global Similarity",1);
	CalcRelevantParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);

	// Read Config of GroupCalcGravitation
	Config->setGroup(CalcGravitationParams.GetComputingName());
	CalcGravitationParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);

	// Read Config of ProfileCalcFeedback
	Config->setGroup(FeedbackParams.GetComputingName());
	FeedbackParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);
	FeedbackParams.RelFactor=Config->readDoubleNumEntry("RelFactor",1);
	FeedbackParams.FuzzyFactor=Config->readDoubleNumEntry("FuzzyFactor",1);
	FeedbackParams.NoRelFactor=Config->readDoubleNumEntry("NoRelFactor",0);
	FeedbackParams.AddFuzzy=Config->readBoolEntry("AddFuzzy",1);
	FeedbackParams.IdfFactor=Config->readBoolEntry("IdfFactor",0);

	// Read Config of ProfileCalcVector
	Config->setGroup(StatisticalParams.GetComputingName());
	StatisticalParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);
 	StatisticalParams.IdfFactor=Config->readBoolEntry("IdfFactor",1);

	// Read Config of ProfileCalcReWeighting
	Config->setGroup(ReWeightingParams.GetComputingName());
	ReWeightingParams.MaxNonZero=Config->readNumEntry("MaxNonZero",500);
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
	mixIdealGroups->setEnabled(false);
	showDocs->setEnabled(false);
	docAnalyse->setEnabled(false);
	docsAnalyse->setEnabled(false);
	docsStats->setEnabled(false);
	gaInit->setEnabled(false);
	gaPause->setEnabled(false);
	gaStart->setEnabled(false);
	gaStop->setEnabled(false);
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
