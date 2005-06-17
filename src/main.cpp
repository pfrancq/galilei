/*

	R Project Library

	Main.cpp

	Main program that updates GALILEI - Implementation.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for current project
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
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
#include <profiles/gpostprofilemanager.h>
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
	bool DoDocs(true);
	bool DoProfiles(true);
	bool DoGroups(true);
	bool Param=false;

	// Init Part
	cout<<"GALILEI Update Version "<<VERSION<<endl;
	cout<<"Copyright 1999-2004 by the Université Libre de Bruxelles"<<endl;

	// Analyze parameters
	for(int i=0;i<argc;i++)
	{
		if(!strcmp(argv[i],"--only-docs"))
		{
			if(!Param)
			{
				DoProfiles=false;
				DoGroups=false;
				Param=true;
			}
			DoDocs=true;
		}
		if(!strcmp(argv[i],"--only-profiles"))
		{
			if(!Param)
			{
				DoDocs=false;
				DoGroups=false;
				Param=true;
			}
			DoProfiles=true;
		}
		if(!strcmp(argv[i],"--only-groups"))
		{
			if(!Param)
			{
				DoDocs=false;
				DoProfiles=false;
				Param=true;
			}
			DoGroups=true;
		}
	}

	try
	{
		RContainer<RString,true,false> lib(10,5);
		RXMLTag* Tag;

		// Read Config
		cout<<"Read Config ..."<<endl;
		RXMLFile ConfigFile("/etc/galilei/galilei.conf",&Config);
		ConfigFile.Open(RIO::Read);
		Tag=Config.GetTag("Plugins");
		if(!Tag)
			throw GException("Problems with the configure file '/etc/galilei/galilei.conf'");
		lib.InsertPtr(new RString(Tag->GetAttrValue("Dir")));

		//------------------------------------------------------------------------------
		// Managers
		GFilterManagerCURL URLManager(&lib,false);
		GProfileCalcManager ProfilingManager(&lib,false);
		GGroupingManager GroupingManager(&lib,false);
		GGroupCalcManager GroupCalcManager(&lib,false);
		GStatsCalcManager StatsCalcManager(&lib,false);
		GLinkCalcManager LinkCalcManager(&lib,false);
		GLangManager Langs(&lib,true,false);
		GDocAnalyseManager DocAnalyseManager(&lib,false);
		GPostDocManager PostDocManager(&lib,false);
		GPostGroupManager PostGroupManager(&lib,false);
		GPostProfileManager PostProfileManager(&lib,false);

		Tag=Config.GetTag("Config");
		if(!Tag)
			throw GException("Problems with the configure file '/etc/galilei/galilei.conf'");
		GConfig Conf(Tag->GetAttrValue("File"));
		Conf.Load();

		// Options
		GSessionParams SessionParams;
		SessionParams.Set("SameBehaviourMinDocs",15);
		SessionParams.Set("DiffBehaviourMinDocs",15);

		// Create Log files
		Log=new GSlotLog(Config.GetTag("Log")->GetAttrValue("File"));
		Log->WriteLog("GALILEI Update started");
		cout<<"Connect to Session ..."<<endl;

		// Init Session
		GStorageMySQL Str(Config.GetTag("World")->GetAttrValue("Host"),
							Config.GetTag("World")->GetAttrValue("User"),
							Config.GetTag("World")->GetAttrValue("Pwd"),
							Config.GetTag("World")->GetAttrValue("Name"),
							Config.GetTag("World")->GetAttrValue("Encoding"));
		GSession Session(&Str,&SessionParams,false);
		Session.Connect();
		Log->WriteLog("Session created");

		// Load Data from MySQL database
		if(DoDocs||DoProfiles)
			Str.LoadDocs(&Session);
		if(DoGroups)
			Str.LoadGroups(&Session);
		if(DoProfiles||DoGroups)
		{
			Str.LoadUsers(&Session);
			Str.LoadFdbks(&Session);
		}
		Log->WriteLog("Data loaded");

		if(DoDocs)
		{
			cout<<"Analyse Documents ...";
			Session.AnalyseDocs(Log,true);
			cout<<"OK"<<endl;
		}
		if(DoProfiles)
		{
			cout<<"Compute Profiles ...";
			Session.CalcProfiles(Log,true,true,true);
			cout<<"OK"<<endl;
		}
		if(DoGroups)
		{
			cout<<"Groups Profiles ...";
			Session.GroupingProfiles(Log,true,true);
			cout<<"OK"<<endl;
		}
		Log->WriteLog("Session updated");

		// End Session
		Log->WriteLog("GALILEI Update stopped");
		delete Log;
	 }
	catch(GException& e)
	{
		cout<<endl<<"Error: "<<e.GetMsg()<<endl;
		if(Log)
			Log->WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(RException& e)
	{
		cout<<endl<<"Error: "<<e.GetMsg()<<endl;
		if(Log)
			Log->WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(std::exception& e)
	{
		cout<<endl<<"Error: "<<e.what()<<endl;
		if(Log)
			Log->WriteLog(RString("Error: ")+e.what());
	}
	catch(...)
	{
		cout<<endl<<"Error while processing"<<endl;
		if(Log)
			Log->WriteLog("Error while processing");
	}

	return(EXIT_SUCCESS);
}
