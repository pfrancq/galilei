/*

	GALILEI Research Project

	RunGALILEIProgram.cpp

	Command - Implementation.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).
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
// include files for R/GALILEI
#include <gsession.h>
#include <gdoc.h>


//------------------------------------------------------------------------------
// include files for FastCGI
#include <fcgiapp.h>


//------------------------------------------------------------------------------
// include files for current project
#include <rungalileiprogram.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RunGALILEIProgram
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RunGALILEIProgram::RunGALILEIProgram(int argc, char *argv[],const RURI& debug)
	: GGALILEIApp("UpGALILEI",argc,argv,false,"/etc/galilei",""), Debug(debug,"utf-8")
{
	FCGX_Init();
	if(!debug.IsEmpty())
		Debug.Open(RIO::Create);
	Write("Start Program");
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Write(const R::RString& str)
{
	mDebug.Lock();
	if(Debug.IsOpen())
		Debug<<str<<endl;
	mDebug.UnLock();
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Init(void)
{
	GGALILEIApp::Init();
	for(int i = 0; i < NbThreads; i++)
		Threads[i]=new CGIThread(this,i);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Run(void)
{
	Write("Wait for Calls");
	for(int i = 0; i < NbThreads; i++)
		Threads[i]->Start();
	for(;;);
}
