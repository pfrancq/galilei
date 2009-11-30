/*

	GALILEI Research Project

	RunGALILEIProgram.cpp

	Command - Implementation.

	Copyright 2009 by Pascal Francq (pascal@francq.info).
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
#include <rungalileiprogram.h>



//------------------------------------------------------------------------------
//
// Class RunGALILEIProgram
//
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
RunGALILEIProgram::RunGALILEIProgram(int argc, char *argv[])
	: GGALILEIApp("UpGALILEI",argc,argv,false), GSlot(), Params(10), Cmds(20)
{
	Cmds.InsertPtr(new CreateMySQL(this));
	Cmds.InsertPtr(new Show(this));
	Cmds.InsertPtr(new Set(this));
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Run(void)
{
	// Initialize the application
	Init();

	cout<<"UpGALILEI Version "<<1.98<<endl;
	cout<<"Copyright 1999-2009 by the Paul Otlet Institute"<<endl;

	// Look if a program must be executed or a console mode launched
	RString Prg(GetParamValue("run"));
	if(!Prg.IsEmpty())
	{
		// Execute a program
		try
		{
			WriteLog("Running program "+Prg);
			RunPrg(this,Prg);
			WriteLog("End program "+Prg);
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
	else
	{
		bool Cont(true);
		while(Cont)
		{
			cout<<">";
			try
			{
				// Get the command line
				string tmp;
				getline(cin,tmp);
				Params.Clear();
				RString(tmp).Split(Params,' ','"');
				RString Cmd(*Params[0]);
				if(Cmd=="quit")
					Cont=false;
				else
				{
					Params.DeletePtrAt(0);
					Command* Do(Cmds.GetPtr(Cmd));
					if(Do)
						Do->Run(Params);
					else
						cerr<<"'"<<Cmd<<"' is not a valid command. Type 'help' for a list of commands."<<endl;
				}
			}
			catch(RException& e)
			{
				cerr<<"Error: "<<e.GetMsg()<<endl;
			}
			catch(std::exception& e)
			{
				cerr<<"Error: "<<e.what()<<endl;
			}
			catch(...)
			{
				cerr<<"Error while processing"<<endl;
			}
		}
	}
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::NextDoc(const GDoc* doc)
{
	Log->NextDoc(doc);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::NextProfile(const GProfile* prof)
{
	Log->NextProfile(prof);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::WriteStr(const RString& str)
{
	cout<<str<<endl;
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::StartJob(const R::RString job)
{
	Log->StartJob(job);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Warning(R::RString msg)
{
	cout<<"Warning: "<<msg<<endl;
	Log->Warning(msg);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Error(R::RString msg)
{
	cout<<"Error: "<<msg<<endl;
	Log->Error(msg);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Alert(R::RString msg)
{
	cout<<"Alert: "<<msg<<endl;
	Log->Alert(msg);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Help(void)
{
	cout<<"createmysql(db,user[,pwd])\tCreate a MySQL database."<<endl;
	cout<<"help\t\t\t\tPrint this message."<<endl;
	cout<<"quit\t\t\t\tQuit the program."<<endl;
}
