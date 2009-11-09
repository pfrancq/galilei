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
#include <rdbmysql.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <ggalileiapp.h>
#include <ggalileiprg.h>
#include <gslotlog.h>
#include <glang.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// Class RunGALILEIProgram
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RunGALILEIProgram : public GGALILEIApp, public GSlot
{
	RContainer<RString,true,false> Cmd;
	RString CurCat;
	RString CurList;
	RString CurPlugin;

public:

	RunGALILEIProgram(int argc, char *argv[]);

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

	void Help(void);
	RString GetConceptType(const RString& name,const RString& desc,RDb* Db);
	void CreateMySQL(void);
	void Show(void);
	void Set(void);
};


//------------------------------------------------------------------------------
RunGALILEIProgram::RunGALILEIProgram(int argc, char *argv[])
	: GGALILEIApp("UpGALILEI",argc,argv,false), GSlot(), Cmd(10)
{
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Run(void)
{
	// Initialize the application
	Init();

	cout<<"UpGALILEI Version "<<1.98<<endl;
	cout<<"Copyright 1999-2009 by the Paul Otlet Institute"<<endl;

	// Look if a program must be executed or a console mode launched
	if(Args.GetNb()==2)
	{
		// Execute a program
		try
		{
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
				Cmd.Clear();
				RString(tmp).Split(Cmd,' ','"');

				// Verify the commands
				if(*Cmd[0]=="quit")
					Cont=false;
				else if(*Cmd[0]=="help")
					Help();
				else if(*Cmd[0]=="createmysql")
					CreateMySQL();
				else if(*Cmd[0]=="show")
					Show();
				else if(*Cmd[0]=="set")
					Set();
				else
					cerr<<"'"<<(*Cmd[0])<<"' is not a valid command. Type 'help' for a list of commands."<<endl;
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


//-----------------------------------------------------------------------------
RString RunGALILEIProgram::GetConceptType(const RString& name,const RString& desc,RDb* Db)
{
	RQuery Select(Db,"SELECT typeid FROM concepttypes WHERE name='"+name+"'");
	Select.Start();
	if(!Select.End())
	{
		return(Select[0]);
	}

	RQuery Insert(Db,"INSERT INTO concepttypes(name,description) VALUES('"+name+"','"+desc+"')");
	RQuery GetId(Db,"SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
	GetId.Start();
	return(GetId[0]);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::CreateMySQL(void)
{
	if((Cmd.GetNb()<3)||(Cmd.GetNb()>4))
	{
		cerr<<"'createmysql' takes two or three parameters: name of the database, user name and eventually the password"<<endl;
		return;
	}
	RString Pwd;
	if(Cmd.GetNb()==4)
		Pwd=(*Cmd[3]);
	cout<<"Create database structure"<<endl;
	RDbMySQL::Create(*Cmd[1],"127.0.0.1",*Cmd[2],Pwd);
	RDbMySQL Db(*Cmd[1],"127.0.0.1",*Cmd[2],Pwd,"utf8");
 	cout<<"Dump Database model"<<endl;
 	Db.RunSQLFile("http://www.otlet-institute.org/NewDb.sql");
 	cout<<"Create Languages (terms and stopwords)"<<endl;
 	RCastCursor<GPlugIn,GLang> Langs(GALILEIApp->GetPlugIns<GLang>("Lang"));
 	RContainer<RString,true,false> Stops(200);
 	for(Langs.Start();!Langs.End();Langs.Next())
 	{
 		// Create the concept types
 		RString Code(Langs()->GetCode());
 		GetConceptType(Code+"Terms",Langs()->GetLangName()+" Terms",&Db);
 		RString TypeId(GetConceptType(Code+"Stopwords",Langs()->GetLangName()+" Stopwords",&Db));

 		Stops.Clear();
 		Langs()->GetStopWords(Stops);
 		RCursor<RString> Cur(Stops);
 		for(Cur.Start();!Cur.End();Cur.Next())
 			RQuery InsertStopWord(&Db,"INSERT INTO concepts(name,typeid) VALUES("+RQuery::SQLValue(*Cur())+","+TypeId+")");
 	}

 	cout<<"Create Other Concept Types"<<endl;
 	GetConceptType("XMLStruct","XML Structure",&Db);
 	GetConceptType("XMLIndex","XML Index",&Db);
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Show(void)
{
	switch(Cmd.GetNb())
	{
		case 1:
		{
			if(CurCat==RString::Null)
			{
				cerr<<"No plug-in category chosen"<<endl;
				return;
			}
			if(CurPlugin==RString::Null)
			{
				cerr<<"No plug-in chosen for category '"<<CurCat<<"'"<<endl;
				return;
			}
			else
				cout<<"Current plug-in category '"<<CurCat<<"', current plug-in '"<<CurPlugin<<"'"<<endl;
			GPlugInFactory* Config(GALILEIApp->GetPlugIn<GPlugInFactory>(CurCat,CurPlugin));
			if(!Config)
				cerr<<"Problem"<<endl;
			break;
		}
		case 2:
		{
			if((*Cmd[1])=="cats")
			{
				R::RCursor<GPlugInManager> Cur(GetManagers());
				int nb(0);
				for(Cur.Start();!Cur.End();Cur.Next(),nb++)
				{
					if(nb==3)
					{
						cout<<endl;
						nb=0;
					}
					RString Name(Cur()->GetName());
					Name.SetLen(20," ");
					cout<<Name<<"  ";
				}
				cout<<endl;
			}
			else if((*Cmd[1])=="plugins")
			{
				if(CurCat==RString::Null)
				{
					cerr<<"No plug-in category chosen"<<endl;
					return;
				}
				if((GetManager(CurCat)->GetPlugInType()==GPlugInManager::ptListSelect)&&(CurList==RString::Null))
				{
					cerr<<"No list selected for plug-in category '"<<CurCat<<"'"<<endl;
					return;
				}
				RCursor<GPlugInFactory> Cur;
				if(GetManager(CurCat)->GetPlugInType()==GPlugInManager::ptListSelect)
					Cur=GetManager(CurCat)->GetPlugInList(CurList)->GetFactories();
				else
					Cur=GetFactories(CurCat);
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					cout<<"[";
					if(Cur()->GetPlugIn<GPlugIn>())
						cout<<"*";
					else
						cout<<" ";
					cout<<"]  "<<Cur()->GetName();
					if(Cur()->GetName()!=Cur()->GetDesc())
						cout<<" ("<<Cur()->GetDesc()<<")";
					cout<<endl;
				}
			}
			else if((*Cmd[1])=="lists")
			{
				if(CurCat==RString::Null)
				{
					cerr<<"No plug-in category chosen"<<endl;
					return;
				}
				if(GetManager(CurCat)->GetPlugInType()!=GPlugInManager::ptListSelect)
				{
					cerr<<"plug-in category '"<<CurCat<<"' has no list"<<endl;
					return;
				}
				RCursor<GPlugInList> Cur(GetManager(CurCat)->GetPlugInLists());
				int nb(0);
				for(Cur.Start();!Cur.End();Cur.Next(),nb++)
				{
					if(nb==3)
					{
						cout<<endl;
						nb=0;
					}
					RString Name(Cur()->GetName());
					Name.SetLen(30," ");
					cout<<Name<<"  ";
				}
				cout<<endl;
			}
			else
				cerr<<"The possible parameters of show are: cats or plugins"<<endl;
			break;
		}
		default:
			cerr<<"show has one or no argument"<<endl;
			break;
	}
}


//------------------------------------------------------------------------------
void RunGALILEIProgram::Set(void)
{
	if(Cmd.GetNb()<3)
		cerr<<"Set need at least two parameters"<<endl;
	if((*Cmd[1])=="cat")
	{
		GPlugInManager* Cat(GetManager(*Cmd[2]));
		if(Cat)
		{
			CurCat=(*Cmd[2]);
			CurList=RString::Null;
			CurPlugin=RString::Null;
			cout<<"Plug-ins category '"<<CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"Invalid plug-ins category '"<<(*Cmd[2])<<"'"<<endl;
	}
	else if((*Cmd[1])=="list")
	{
		if(CurCat==RString::Null)
		{
			cerr<<"No plug-in category chosen"<<endl;
			return;
		}
		if(GetManager(CurCat)->GetPlugInType()!=GPlugInManager::ptListSelect)
		{
			cerr<<"plug-in category '"<<CurCat<<"' has no list"<<endl;
			return;
		}
		if(GetManager(CurCat)->GetPlugInList(*Cmd[2]))
		{
			CurList=(*Cmd[2]);
			CurPlugin=RString::Null;
			cout<<"Plug-in list '"<<CurList<<"' in '"<<CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"Invalid plug-in list '"<<*Cmd[2]<<"' for category '"<<CurCat<<"'"<<endl;
	}
	else if((*Cmd[1])=="plugin")
	{
		if(CurCat==RString::Null)
			cerr<<"No plug-ins category chosen"<<endl;
		GPlugInManager* Cat(GetManager(CurCat));
/*		Cat->GetPluginsName(plugins);
		if(plugins.IsIn(*Cmd[2]))
		{
			CurPlugin=(*Cmd[2]);
			cout<<"Plug-in '"<<CurPlugin<<"' of category '"<<CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"No plug-in '"<<*Cmd[2]<<"' in category '"<<CurCat<<"'"<<endl;*/
	}
	else if((*Cmd[1])=="config")
	{
		if(CurPlugin==RString::Null)
			cerr<<"No plug-in chosen"<<endl;
	}
	else
		cerr<<"Set needs either cat, plugin, list or config"<<endl;
}



//------------------------------------------------------------------------------
//
// Main
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");
	RunGALILEIProgram App(argc,argv);
	App.Run();
	return(EXIT_SUCCESS);
}
