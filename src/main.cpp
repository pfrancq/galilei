/*

	R Project Library

	Main.cpp

	Main program that updates GALILEI - Implementation.

	Copyright 1999-2007 by the Université Libre de Bruxelles.

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
	bool DoDocs;        // Only documents should be analyze
	bool DoProfiles;    // Only profiles should be computed
	bool DoGroups;      // Only profiles should be clustered.

public:
	UpGALILEI(int argc, char *argv[]);
	void Run(void);
};


//------------------------------------------------------------------------------
UpGALILEI::UpGALILEI(int argc, char *argv[])
	: GGALILEIApp("UpGALILEI",argc,argv,false), DoDocs(true), DoProfiles(true),
	  DoGroups(true)
{
	// Show Part
	cout<<"GALILEI Update Version "<<VERSION<<endl;
	cout<<"Copyright 1999-2007 by the Université Libre de Bruxelles"<<endl;

	// Analyze parameters
	bool OnlyDocs=Args.IsIn("--only-docs");
	bool OnlyProfiles=Args.IsIn("--only-profiles");
	bool OnlyGroups=Args.IsIn("--only-groups");
	if((!OnlyDocs)&&(OnlyProfiles||OnlyGroups))
		DoDocs=false;
	if((!OnlyProfiles)&&(OnlyDocs||OnlyGroups))
		DoProfiles=false;
	if((!OnlyGroups)&&(OnlyDocs||OnlyProfiles))
		DoGroups=false;
}


//------------------------------------------------------------------------------
void UpGALILEI::Run(void)
{
	try
	{
		// Init
		Init();
		WriteLog("GALILEI Update started");
		CreateSession();

		// Load database
		cout<<"Load Data ...";
		cout.flush();
		if(DoDocs||DoProfiles)
			Session->GetStorage()->LoadDocs();
		if(DoGroups)
			Session->GetStorage()->LoadCommunities();
		if(DoProfiles||DoGroups)
			Session->GetStorage()->LoadUsers();
		WriteLog("Data loaded");
		cout<<"OK"<<endl;

		// Analyse Docs
		if(DoDocs)
		{
			cout<<"Analyse Documents ...";
			cout.flush();
			Session->AnalyseDocs(Log);
			WriteLog("Documents analysed");
			cout<<"OK"<<endl;
		}

		// Compute profiles
		if(DoProfiles)
		{
			cout<<"Compute Profiles ...";
			cout.flush();
			Session->CalcProfiles(Log);
			cout<<"OK"<<endl;
			WriteLog("Profiles computed");
		}

		// Group profiles
		if(DoGroups)
		{
			cout<<"Groups Profiles ...";
			cout.flush();
			Session->GroupProfiles(Log);
			cout<<"OK"<<endl;
			WriteLog("Groups computed");
		}

		// End Session
		WriteLog("Session updated");
		WriteLog("GALILEI Update stopped");
	 }
	catch(GException& e)
	{
		cout<<endl<<"Error: "<<e.GetMsg()<<endl;
		WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(RException& e)
	{
		cout<<endl<<"Error: "<<e.GetMsg()<<endl;
		WriteLog(RString("Error: ")+e.GetMsg());
	}
	catch(std::exception& e)
	{
		cout<<endl<<"Error: "<<e.what()<<endl;
		WriteLog(RString("Error: ")+e.what());
	}
	catch(...)
	{
		cout<<endl<<"Error while processing"<<endl;
		WriteLog("Error while processing");
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
