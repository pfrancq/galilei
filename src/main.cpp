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
#include <ltmm/loader.hh>
#include <cstdlib>
#include <stdexcept>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlfile.h>
#include <rstd/rxmlstruct.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <docs/gfiltermanagercurl.h>
#include <docs/glinkcalcmanager.h>
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalcmanager.h>
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstoragemysql.h>
#include <sessions/gsession.h>
#include <sessions/gconfig.h>
#include <sessions/gslotlog.h>
#include <groups/gpostgroupmanager.h>
#include <docs/gpostdocmanager.h>
#include <docs/gdocanalysemanager.h>
#include <infos/glangmanager.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// Global Variables
RXMLStruct Config;
GSlotLog* Log=0;


//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Init Part
	map<string,ltmm::symbol_list>& preloaded =ltmm::loader<>::preloaded();
	cout<<"GALILEI Update Version "<<VERSION<<endl;
	cout<<"Copyright 1999-2003 by the Université Libre de Bruxelles"<<endl;
	try
	{
		//------------------------------------------------------------------------------
		// Managers
		GFilterManagerCURL URLManager("/home/pfrancq/prj/galilei_plugins",false);
		GProfileCalcManager ProfilingManager("/home/pfrancq/prj/galilei_plugins",false);
		GGroupingManager GroupingManager("/home/pfrancq/prj/galilei_plugins",false);
		GGroupCalcManager GroupCalcManager("/home/pfrancq/prj/galilei_plugins",false);
		GStatsCalcManager StatsCalcManager ("/home/pfrancq/prj/galilei_plugins",false);
		GLinkCalcManager LinkCalcManager("/home/pfrancq/prj/galilei_plugins",false);
		GLangManager Langs("/home/pfrancq/prj/galilei_plugins",false);
		GDocAnalyseManager DocAnalyseManager("/home/pfrancq/prj/galilei_plugins",false);
		GPostDocManager PostDocManager("/home/pfrancq/prj/galilei_plugins",false);
		GPostGroupManager PostGroupManager("/home/pfrancq/prj/galilei_plugins",false);

		// Read Config
		cout<<"Read Config"<<endl;
		RXMLFile ConfigFile("/etc/galilei/galilei.conf",&Config,R::Read);
		GConfig Conf(Config.GetTag("Config")->GetAttrValue("File"));
		Conf.Load();
		Conf.Read(Langs);
		Conf.Read(DocAnalyseManager);
		Conf.Read(URLManager);
		Conf.Read(ProfilingManager);
		Conf.Read(GroupingManager);
		Conf.Read(GroupCalcManager);
		Conf.Read(StatsCalcManager);
		Conf.Read(LinkCalcManager);
		Conf.Read(PostDocManager);
		Conf.Read(PostGroupManager);

		// Options
		GSessionParams SessionParams;
		SessionParams.Set("SameBehaviourMinDocs",15);
		SessionParams.Set("DiffBehaviourMinDocs",15);

		// Create Log files
		Log=new GSlotLog(Config.GetTag("Log")->GetAttrValue("File"));
		Log->WriteLog("GALILEI Update started");
		cout<<"Connect to Session"<<endl;

		// Init Session
		GStorageMySQL Str(Config.GetTag("World")->GetAttrValue("Host"),
							Config.GetTag("World")->GetAttrValue("User"),
							Config.GetTag("World")->GetAttrValue("Pwd"),
							Config.GetTag("World")->GetAttrValue("Name"));
		GSession Session(&Str,2000,2000,2000,2000,2000,&SessionParams,false);
		Log->WriteLog("Session created");


		// Load Data from MySQL database
		Session.Connect(&Langs,&URLManager,&DocAnalyseManager,&ProfilingManager,&GroupingManager,
			&GroupCalcManager,&StatsCalcManager,&PostDocManager,&PostGroupManager);
		Str.LoadDocs(&Session);
		Str.LoadGroups(&Session);
		Str.LoadUsers(&Session);
		Str.LoadFdbks(&Session);
		Session.PostConnect(&LinkCalcManager);
		Log->WriteLog("Data loaded");

// 		try
		{
			cout<<"Analyse Documents."<<endl;
			Session.AnalyseDocs(0,true);
		}
//		catch(GException& e)
//		{
//			Log->WriteLog(RString("Error: ")+e.GetMsg());
//		}
		cout<<"Compute Profiles."<<endl;
		Session.CalcProfiles(0,true,true);
		cout<<"Groups Profiles."<<endl;
		Session.GroupingProfiles(0,true,true);
		Log->WriteLog("Session updated");

		// End Session
		Log->WriteLog("GALILEI Update stopped");
		delete Log;

		// Destroy Managers
// 		delete URLManager;
// 		delete ProfilingManager;
// 		delete GroupingManager;
// 		delete GroupCalcManager;
// 		delete StatsCalcManager;
// 		delete LinkCalcManager;
	}
	catch(GException& e)
	{
		cout<<"Error: "<<e.GetMsg()<<endl;
		Log->WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(R::RMySQLError& e)
	{
		cout<<"Error: "<<e.GetError()<<endl;
		Log->WriteLog(RString("Error: ")+e.GetError());
	}
	catch(std::exception& e)
	{
		cout<<"Error: "<<e.what()<<endl;
		Log->WriteLog(RString("Error: ")+e.what());
	}
	catch(...)
	{
		cout<<"Error while processing"<<endl;
	}

	return(EXIT_SUCCESS);
}
