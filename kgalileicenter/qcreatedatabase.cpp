/*

	GALILEI Research Project

	QCreateDatabase.cpp

	Create a database- Implementation.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
#include <qsessionprogress.h>
#include <qcreatedatabase.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <rdbmysql.h>
#include <ggalileiapp.h>
#include <glang.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <kmessagebox.h>



//-----------------------------------------------------------------------------
//
// class QCreateDB
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * Thread used to create the documents.
 */
class QCreateDB : public QSessionThread
{
	QCreateDatabase* Info;
public:
	QCreateDB(KGALILEICenter* app,QCreateDatabase* info) : QSessionThread(app), Info(info) {}
	RString GetConceptType(const RString& cat,const RString& name,const RString& desc,RDb* Db);
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
RString QCreateDB::GetConceptType(const RString& cat,const RString& name,const RString& desc,RDb* Db)
{
	// Look if the concept type exists
	RQuery Select(Db,"SELECT typeid FROM concepttypes WHERE name='"+name+"'");
	Select.Start();
	if(!Select.End())
	{
		return(Select[0]);
	}

	// Look for the concept category
	RString CatId;
	RQuery Cat(Db,"SELECT catid FROM conceptcats WHERE name='"+cat+"'");
	Cat.Start();
	if(Cat.End())
	{
		// Create the category
		RQuery Insert(Db,"INSERT INTO conceptcats(name) VALUES('"+cat+"')");
		RQuery GetId(Db,"SELECT catid FROM conceptcats WHERE catid=LAST_INSERT_ID()");
		GetId.Start();
		CatId=GetId[0];
	}
	else
		CatId=Cat[0];

	// Create the type
	RQuery Insert(Db,"INSERT INTO concepttypes(name,description,catid) VALUES('"+name+"','"+desc+"','"+CatId+"')");
	RQuery GetId(Db,"SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
	GetId.Start();
	return(GetId[0]);
}


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	Parent->setLabelText("Database structure created");
	RDbMySQL::Create(Info->Name,Info->Host,Info->User,Info->Pwd);
	RDbMySQL Db(Info->Name,Info->Host,Info->User,Info->Pwd,"utf8");
 	Parent->setLabelText("Dump Database model");
 	Db.RunSQLFile(Info->DbSchema);
 	Parent->setLabelText("Create Languages (terms and stopwords)");
 	RCursor<GPlugInFactory> Langs(GALILEIApp->GetFactories("Lang"));
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
 		RString TypeId(GetConceptType("Text",Code+"Stopwords",Name+" Stopwords",&Db));

 		RCursor<RString> Cur(Stops);
 		for(Cur.Start();!Cur.End();Cur.Next())
 			RQuery InsertStopWord(&Db,"INSERT INTO concepts(name,typeid) VALUES("+RQuery::SQLValue(*Cur())+","+TypeId+")");
 	}

 	Parent->setLabelText("Create Other Concept Types");
 	GetConceptType("Structure","XMLStruct","XML Structure",&Db);
 	GetConceptType("MetaData","XMLIndex","XML Index",&Db);

 	// Create the configuration file with MySQL as default storage
 	RTextFile Config(RString(getenv("HOME"))+"/.r/config/lib/galilei/sessions/"+Info->Name+".config","utf-8");
 	Config.Open(RIO::Create);
 	Config<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
 	Config<<"<!DOCTYPE config>"<<endl;
 	Config<<"<config>"<<endl;
 	Config<<"	<category name=\"Storage\">"<<endl;
 	Config<<"		<param name=\"Storage\" value=\"MySQL\"/>"<<endl;
 	Config<<"		<category name=\"MySQL\">"<<endl;
 	Config<<"			<param name=\"All\" value=\"True\"/>"<<endl;
 	Config<<"			<param name=\"Database\" value=\""+Info->Name+"\"/>"<<endl;
 	Config<<"			<param name=\"Encoding\" value=\"utf8\"/>"<<endl;
 	Config<<"			<param name=\"Filter\"/>"<<endl;
 	Config<<"			<param name=\"Filtering\" value=\"False\"/>"<<endl;
 	Config<<"			<param name=\"Host\" value=\""+Info->Host+"\"/>"<<endl;
 	Config<<"			<param name=\"Password\" value=\""+Info->Pwd+"\"/>"<<endl;
 	Config<<"			<param name=\"User\" value=\""+Info->User+"\"/>"<<endl;
 	Config<<"		</category>"<<endl;
 	Config<<"	</category>"<<endl;
 	Config<<"</config>"<<endl;
}



//------------------------------------------------------------------------------
//
// class QCreateDatabase
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QCreateDatabase::QCreateDatabase(KGALILEICenter* parent)
	: KDialog(parent), Ui_QCreateDatabase(), App(parent)
{
	setCaption("Fill Database");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
}


//------------------------------------------------------------------------------
void QCreateDatabase::run(void)
{
	URL->setUrl(KUrl("http://www.otlet-institute.org/NewDb.sql"));
	ConfigPath->setUrl(QString(getenv("HOME"))+"/.r/config/lib/galilei/sessions");
	if(!exec())
		return;
	try
	{
		Name=FromQString(DbName->text());
		Host=FromQString(hostName->text());
		User=FromQString(userName->text());
		Pwd=FromQString(password->text());
		DbSchema=FromQString(URL->url().url());
		ConfigDir=R::FromQString(ConfigPath->url().url());
		QSessionProgressDlg Dlg(App,"Create Database");
		Dlg.Run(new QCreateDB(App,this));
	}
	catch(GException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}
