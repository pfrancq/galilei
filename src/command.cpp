/*

	GALILEI Research Project

	Command.h

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
// include files for ANSI C/C++
#include <iostream>
#include <cstdlib>
#include <stdexcept>
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>


//------------------------------------------------------------------------------
// include files for current application
#include <command.h>
#include <rungalileiprogram.h>



//------------------------------------------------------------------------------
//
// class Command
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Command::Command(RunGALILEIProgram* prg,const RString& name,const RString& desc)
	: Name(name), Desc(desc), Prg(prg)
{
}


//------------------------------------------------------------------------------
int Command::Compare(const Command& cmd) const
{
	return(Name.Compare(cmd.Name));
}


//------------------------------------------------------------------------------
int Command::Compare(const RString& cmd) const
{
	return(Name.Compare(cmd));
}


//------------------------------------------------------------------------------
Command::~Command(void)
{
}



//------------------------------------------------------------------------------
//
// class CreateMySQL
//
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
RString CreateMySQL::GetConceptType(const RString& name,const RString& desc,RDb* Db)
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
void CreateMySQL::Run(const RContainer<RString,true,false>& params)
{
	if((params.GetNb()<2)||(params.GetNb()>3))
	{
		cerr<<"'createmysql' takes two or three parameters: name of the database, user name and eventually the password"<<endl;
		return;
	}
	RString Pwd;
	if(params.GetNb()==3)
		Pwd=(*params[2]);
	cout<<"Create database structure"<<endl;
	RDbMySQL::Create(*params[0],"127.0.0.1",*params[1],Pwd);
	RDbMySQL Db(*params[0],"127.0.0.1",*params[1],Pwd,"utf8");
 	cout<<"Dump Database model"<<endl;
 	Db.RunSQLFile("http://www.otlet-institute.org/NewDb.sql");
 	cout<<"Create Languages (terms and stopwords)"<<endl;
 	RCastCursor<GPlugInFactory,GLangFactory> Langs(GALILEIApp->GetFactories("Lang"));
 	RContainer<RString,true,false> Stops(200);
 	for(Langs.Start();!Langs.End();Langs.Next())
 	{
 		// Get the stop list and the code.
 		Stops.Clear();
 		RString Code;
 		RString Name;
 		GLangFactory* Factory(reinterpret_cast<GLangFactory*>(Langs()));
 		Factory->CreateStopWords(Stops,Code,Name);

 		// Create the concept types
 		GetConceptType(Code+"Terms",Name+" Terms",&Db);
 		RString TypeId(GetConceptType(Code+"Stopwords",Name+" Stopwords",&Db));

 		RCursor<RString> Cur(Stops);
 		for(Cur.Start();!Cur.End();Cur.Next())
 			RQuery InsertStopWord(&Db,"INSERT INTO concepts(name,typeid) VALUES("+RQuery::SQLValue(*Cur())+","+TypeId+")");
 	}

 	cout<<"Create Other Concept Types"<<endl;
 	GetConceptType("XMLStruct","XML Structure",&Db);
 	GetConceptType("XMLIndex","XML Index",&Db);
}


void Show::Run(const RContainer<RString,true,false>& params)
{
	switch(params.GetNb())
	{
		case 0:
		{
			if(Prg->CurCat==RString::Null)
			{
				cerr<<"No plug-in category chosen"<<endl;
				return;
			}
			if(Prg->CurPlugin==RString::Null)
			{
				cerr<<"No plug-in chosen for category '"<<Prg->CurCat<<"'"<<endl;
				return;
			}
			else
				cout<<"Current plug-in category '"<<Prg->CurPlugin<<"'"<<endl;
			GPlugInFactory* Config(GALILEIApp->GetPlugIn<GPlugInFactory>(Prg->CurCat,Prg->CurPlugin));
			if(!Config)
				cerr<<"Problem"<<endl;
			break;
		}
		case 1:
		{
			if((*params[0])=="cats")
			{
				RCursor<GPlugInManager> Cur(Prg->GetManagers());
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
			else if((*params[0])=="plugins")
			{
				if(Prg->CurCat==RString::Null)
				{
					cerr<<"No plug-in category chosen"<<endl;
					return;
				}
				if((Prg->GetManager(Prg->CurCat)->GetPlugInType()==GPlugInManager::ptListSelect)&&(Prg->CurList==RString::Null))
				{
					cerr<<"No list selected for plug-in category '"<<Prg->CurCat<<"'"<<endl;
					return;
				}
				RCursor<GPlugInFactory> Cur;
				if(Prg->GetManager(Prg->CurCat)->GetPlugInType()==GPlugInManager::ptListSelect)
					Cur=Prg->GetManager(Prg->CurCat)->GetPlugInList(Prg->CurList)->GetFactories();
				else
					Cur=Prg->GetFactories(Prg->CurCat);
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
			else if((*params[0])=="lists")
			{
				if(Prg->CurCat==RString::Null)
				{
					cerr<<"No plug-in category chosen"<<endl;
					return;
				}
				if(Prg->GetManager(Prg->CurCat)->GetPlugInType()!=GPlugInManager::ptListSelect)
				{
					cerr<<"plug-in category '"<<Prg->CurCat<<"' has no list"<<endl;
					return;
				}
				RCursor<GPlugInList> Cur(Prg->GetManager(Prg->CurCat)->GetPlugInLists());
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



void Set::Run(const RContainer<RString,true,false>& params)
{
	if(params.GetNb()<1)
		cerr<<"Set need at least two parameters"<<endl;
	if((*params[0])=="cat")
	{
		GPlugInManager* Cat(Prg->GetManager(*params[1]));
		if(Cat)
		{
			Prg->CurCat=(*params[1]);
			Prg->CurList=RString::Null;
			Prg->CurPlugin=RString::Null;
			cout<<"Plug-ins category '"<<Prg->CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"Invalid plug-ins category '"<<(*params[1])<<"'"<<endl;
	}
	else if((*params[0])=="list")
	{
		if(Prg->CurCat==RString::Null)
		{
			cerr<<"No plug-in category chosen"<<endl;
			return;
		}
		if(Prg->GetManager(Prg->CurCat)->GetPlugInType()!=GPlugInManager::ptListSelect)
		{
			cerr<<"plug-in category '"<<Prg->CurCat<<"' has no list"<<endl;
			return;
		}
		if(Prg->GetManager(Prg->CurCat)->GetPlugInList(*params[1]))
		{
			Prg->CurList=(*params[1]);
			Prg->CurPlugin=RString::Null;
			cout<<"Plug-in list '"<<Prg->CurList<<"' in '"<<Prg->CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"Invalid plug-in list '"<<*params[2]<<"' for category '"<<Prg->CurCat<<"'"<<endl;
	}
	else if((*params[1])=="plugin")
	{
		if(Prg->CurCat==RString::Null)
			cerr<<"No plug-ins category chosen"<<endl;
		GPlugInManager* Cat(Prg->GetManager(Prg->CurCat));
/*		Cat->GetPluginsName(plugins);
		if(plugins.IsIn(*params[2]))
		{
			CurPlugin=(*params[2]);
			cout<<"Plug-in '"<<CurPlugin<<"' of category '"<<CurCat<<"' chosen"<<endl;
		}
		else
			cerr<<"No plug-in '"<<*params[2]<<"' in category '"<<CurCat<<"'"<<endl;*/
	}
	else if((*params[0])=="config")
	{
		if(Prg->CurPlugin==RString::Null)
			cerr<<"No plug-in chosen"<<endl;
	}
	else
		cerr<<"Set needs either cat, plugin, list or config"<<endl;
}
