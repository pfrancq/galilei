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
#include <rxmlfile.h>
#include <rxmlstruct.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gprofilecalc.h>
#include <gsession.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gslotlog.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// Class UpGALILEI
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class UpGALILEI : public GGALILEIApp
{
	bool DoDocs;
	bool DoProfiles;
	bool DoGroups;
	bool Param;
	
public:
	UpGALILEI(int argc, char *argv[]);
	void Run(void);
};


//------------------------------------------------------------------------------
UpGALILEI::UpGALILEI(int argc, char *argv[])
	: GGALILEIApp("UpGALILEI",argc,argv,false), DoDocs(true), DoProfiles(true),
	  DoGroups(true),Param(false)
	
{
	// Show Part
	cout<<"GALILEI Update Version "<<VERSION<<endl;
	cout<<"Copyright 1999-2007 by the Université Libre de Bruxelles"<<endl;
	
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
}


//------------------------------------------------------------------------------
void UpGALILEI::Run(void)
{
	try
	{
		// Init
		Init();
		if(Log)
			Log->WriteLog("GALILEI Update started");
		CreateSession();
		Session->Init();

		// Load database
		cout<<"Load Data ...";
		cout.flush();
		if(DoDocs||DoProfiles)
			Session->GetStorage()->LoadDocs();
		if(DoGroups)
			Session->GetStorage()->LoadGroups();
		if(DoProfiles||DoGroups)
			Session->GetStorage()->LoadUsers();
		if(Log)
			Log->WriteLog("Data loaded");
		cout<<"OK"<<endl;
		
		// Analyse Docs
		if(DoDocs)
		{
			cout<<"Analyse Documents ...";
			cout.flush();
			Session->AnalyseDocs(Log);
			if(Log)
				Log->WriteLog("Documents analysed");
			cout<<"OK"<<endl;
		}
		
		// Compute profiles
		if(DoProfiles)
		{
			cout<<"Compute Profiles ...";
			cout.flush();
			Session->CalcProfiles(Log);
			cout<<"OK"<<endl;
			if(Log)
				Log->WriteLog("Profiles computed");
		}
		
		// Group profiles
		if(DoGroups)
		{
			cout<<"Groups Profiles ...";
			cout.flush();
			Session->GroupingProfiles(Log);
			cout<<"OK"<<endl;
			if(Log)
				Log->WriteLog("Groups computed");
		}

		// End Session
		if(Log)
		{
			Log->WriteLog("Session updated");
			Log->WriteLog("GALILEI Update stopped");
		}
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
}



//------------------------------------------------------------------------------
//
// Main
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	UpGALILEI App(argc,argv);
	App.Run();
	return(EXIT_SUCCESS);
}
