/*

	R Project Library

	Main.cpp

	Main program that updates GALILEI - Implementation.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#include <ggalileiapp.h>
#include <ggalileiprg.h>
#include <gslotlog.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// Class UpGALILEI
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class UpGALILEI : public GGALILEIApp, public GSlot
{
public:
	UpGALILEI(int argc, char *argv[]);
	void Run(void);

	/**
	* The treatment for a specific document will begin.
	* @param doc             Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific document will begin.
	* @param prof            Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);

	/**
	* Start a job.
	* @param job             Description of the job.
	*/
	virtual void StartJob(const R::RString job);

	/**
	* Forward a warning.
	* @param msg             Message.
	*/
	virtual void Warning(R::RString msg);

	/**
	* Forward an error.
	* @param msg             Message.
	*/
	virtual void Error(R::RString msg);

	/**
	* Forward an alert.
	* @param msg             Message.
	*/
	virtual void Alert(R::RString msg);
};


//------------------------------------------------------------------------------
UpGALILEI::UpGALILEI(int argc, char *argv[])
	: GGALILEIApp("UpGALILEI",argc,argv,false), GSlot()
{
}


//------------------------------------------------------------------------------
void UpGALILEI::Run(void)
{
	try
	{
		cout<<"GALILEI Update Version "<<1.98<<endl;
		cout<<"Copyright 1999-2008 by the Université Libre de Bruxelles"<<endl;
		// Init
		Init();
		WriteLog("Running program "+(*Args[1]));
		RunPrg(this,*Args[1]);
		WriteLog("End program "+(*Args[1]));
		cout<<"Program finished"<<endl;
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
void UpGALILEI::NextDoc(const GDoc* doc)
{
	Log->NextDoc(doc);
}


//------------------------------------------------------------------------------
void UpGALILEI::NextProfile(const GProfile* prof)
{
	Log->NextProfile(prof);
}


//------------------------------------------------------------------------------
void UpGALILEI::WriteStr(const RString& str)
{
	cout<<str<<endl;
}


//------------------------------------------------------------------------------
void UpGALILEI::StartJob(const R::RString job)
{
	Log->StartJob(job);
}


//------------------------------------------------------------------------------
void UpGALILEI::Warning(R::RString msg)
{
	cout<<"Warning: "<<msg<<endl;
	Log->Warning(msg);
}


//------------------------------------------------------------------------------
void UpGALILEI::Error(R::RString msg)
{
	cout<<"Error: "<<msg<<endl;
	Log->Error(msg);
}


//------------------------------------------------------------------------------
void UpGALILEI::Alert(R::RString msg)
{
	cout<<"Alert: "<<msg<<endl;
	Log->Alert(msg);
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
