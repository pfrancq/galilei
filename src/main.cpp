/*

	R Project Library

	Main.cpp

	Main program that updates GALILEI - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for current project
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlfile.h>
#include <rstd/rxmlstruct.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <galilei/gurlmanagerkde.h>
#include <sessions/gsessionmysql.h>
#include <sessions/gslotlog.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofilecalcfeedback.h>
#include <groups/ggroupinggga.h>
#include <groups/ggroupcalcgravitation.h>
#include <docs/glinkcalchits.h>
#include <docs/gdocoptions.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files fro QT/KDE
#include <kcmdlineargs.h>
#include <kapp.h>
#include <kaboutdata.h>


//-----------------------------------------------------------------------------
// Arguments to add at the command prompt
static KCmdLineOptions options[] =
{
  { 0, 0, 0 }
};


//------------------------------------------------------------------------------
// Global Variables
RXMLStruct Config;
GSlotLog* Log=0;
GIRParams IRParams;
GFeedbackParams FeedbackParams;
GCalcGravitationParams CalcGravitationParams;
GLinkCalcHITSParams LinkCalcHITSParams;


//------------------------------------------------------------------------------
// Init Promethee
void InitProm(RPromCriterionParams& p,double w)
{
	p.P=0.2;
	p.Q=0.05;
	p.Weight=w;
}


//------------------------------------------------------------------------------
// Load Congigs
void LoadConfig(void) throw(GException)
{
}



//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Init Part
	cout<<"GALILEI Update Version "<<VERSION<<endl;
	cout<<"Copyright 1999-2003 by the Université Libre de Bruxelles"<<endl;
	IRParams.PopSize=16;
	IRParams.MaxGen=30;
	IRParams.Step=false;
	IRParams.StepGen=0;
	IRParams.MinCommonOK=0.6;
	IRParams.MinCommonDiff=0.6;
	IRParams.MaxKMeans=30;
	IRParams.SimMeasures=sctCrits;
	InitProm(IRParams.ParamsSim,1.0);
	InitProm(IRParams.ParamsInfo,0.0);
	InitProm(IRParams.ParamsEntropy,0.0);
	InitProm(IRParams.ParamsLikelihood,0.0);
	InitProm(IRParams.ParamsSameFeedbacks,1.0);
	InitProm(IRParams.ParamsDiffFeedbacks,1.0);
	InitProm(IRParams.ParamsSocial,1.0);
	IRParams.GlobalSim=true;
	IRParams.Measures.GetPtr<const char*>("J")->Use=true;
	FeedbackParams.MaxNonZero=100;
	FeedbackParams.RelFactor=1.0;
	FeedbackParams.FuzzyFactor=0.25;
	FeedbackParams.NoRelFactor=0.75;
	FeedbackParams.AddFuzzy=false;
	FeedbackParams.IdfFactor=true;
	CalcGravitationParams.MaxNonZero=100;

	try
	{
		RXMLFile ConfigFile("/etc/galilei/galilei.conf",&Config,R::Read);
	}
	catch(...)
	{
		cout<<"Error while loading config file"<<endl;
		return(EXIT_SUCCESS);
	}

	// Run GALILEI
	try
	{
		// Create Config files
		Log=new GSlotLog(Config.GetTag("Log")->GetAttrValue("File"));
		Log->WriteLog("GALILEI Update started");
		cout<<"Connect to Session"<<endl;

		// Init Session
		GURLManagerKDE* URLMng=new GURLManagerKDE();
		GSessionMySQL Session(Config.GetTag("World")->GetAttrValue("Host"),
							Config.GetTag("World")->GetAttrValue("User"),
							Config.GetTag("World")->GetAttrValue("Pwd"),
							Config.GetTag("World")->GetAttrValue("Name"),
							URLMng);
		Log->WriteLog("Session created");
		Session.GetDocOptions()->MinStopWords=0.01;
		Session.GetDocOptions()->MinWordSize=4;
		Session.GetDocOptions()->MinStemSize=3;
		Session.GetDocOptions()->MinOccur=1;
		Session.GetDocOptions()->MinDocs=5;
		Session.GetDocOptions()->MaxDocs=300;
		Session.GetDocOptions()->MinOccurCluster=2;
		Session.GetDocOptions()->NbIteration=2;
		Session.GetDocOptions()->NonLetterWords=true;
		Session.GetDocOptions()->UseLink=false;
		Session.GetDocOptions()->UseExternalLink=false;
		Session.GetDocOptions()->UseRedirection=false;

		// Load Data from MySQL database
		Session.InitDics();
		Session.InitDocs();
		Session.InitGroups();
		Session.InitUsers();
		Session.InitFdbks();
		Log->WriteLog("Data loaded");

		//
		LoadConfig();
		Session.RegisterProfileDesc(new GSubProfileDesc("Vector space",GSubProfileVector::NewSubProfile));
		Session.RegisterComputingMethod(new GProfileCalcFeedback(&Session, &FeedbackParams));
		Session.RegisterGroupingMethod(new GGroupingGGA(&Session,&IRParams));
		Session.RegisterGroupCalcMethod(new GGroupCalcGravitation(&Session, &CalcGravitationParams));
		Session.RegisterLinkCalcMethod(new GLinkCalcHITS(&Session,&LinkCalcHITSParams));
		Session.SetCurrentProfileDesc("Vector space");
		Session.SetCurrentGroupingMethod("GVCA");
		Session.SetCurrentComputingMethod("User Feedback");
		Session.SetCurrentGroupCalcMethod("Gravitation");
		Session.SetCurrentLinkCalcMethod("HITS Algorithm");

		// Compute all the sessions
			KAboutData aboutData( "upgalilei", "UpGALILEI",
		"0.1", "One Shot Server", KAboutData::License_GPL,
		"(c) 1998-2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");

		KCmdLineArgs::init( argc, argv, &aboutData );
		KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.
		KApplication app;
		if(app.isRestored()) return(EXIT_SUCCESS);
		try
		{
			cout<<"Analyse Documents."<<endl;
			Session.AnalyseDocs(0,true);
		}
		catch(GException& e)
		{
			Log->WriteLog(RString("Error: ")+e.GetMsg());
		}
		cout<<"Compute Profiles."<<endl;
		Session.CalcProfiles(0,true,true);
		cout<<"Groups Profiles."<<endl;
		Session.GroupingProfiles(0,true,true);
		Log->WriteLog("Session updated");

		// End Session
		Log->WriteLog("GALILEI Update stopped");
		delete URLMng;
		delete Log;
	}
	catch(GException& e)
	{
		Log->WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(R::RMySQLError& e)
	{
		Log->WriteLog(RString("Error: ")+e.GetError());
	}
	catch(...)
	{
		cout<<"Error while processing"<<endl;
	}

	return(EXIT_SUCCESS);
}
