/*

	GALILEI Research Project

	QCreateDatabase.cpp

	Create a database- Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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



//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <rdbmysql.h>
#include <ggalileiapp.h>
#include <glang.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>
#include <QDialog>


//-----------------------------------------------------------------------------
// include files for current application
#include <qgalileiwin.h>
#include <qsessionprogress.h>
#include <qcreatedatabase.h>



//-----------------------------------------------------------------------------
//
// class QCreateDB
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * Thread used to create the documents.
 */
class QCreateDB : public QSessionProgress
{
	QCreateDatabase* Info;
public:
	QCreateDB(QGALILEIWin* win,QCreateDatabase* info) : QSessionProgress(win,"Create Database"), Info(info) {}
	RString GetConceptType(tConceptCat cat,const RString& name,const RString& desc,RDb* Db);
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
RString QCreateDB::GetConceptType(tConceptCat cat,const RString& name,const RString& desc,RDb* Db)
{
	// Look if the concept type exists
	RQuery Select(Db,"SELECT typeid FROM concepttypes WHERE name='"+name+"'");
	Select.Start();
	if(!Select.End())
	{
		return(Select[0]);
	}

	// Create the type
	RQuery Insert(Db,"INSERT INTO concepttypes(name,description,catid) VALUES('"+name+"','"+desc+"','"+RString::Number(cat)+"')");
	RQuery GetId(Db,"SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
	GetId.Start();
	return(GetId[0]);
}


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	WriteStr("Database structure created");
	RDbMySQL::Create(Info->Name,Info->Host,Info->User,Info->Pwd);
	RDbMySQL Db(Info->Name,Info->Host,Info->User,Info->Pwd,"utf8");
 	WriteStr("Dump database model");
 	Db.RunSQLFile(Info->DbSchema);

	// Create Languages
 	WriteStr("Insert the language stopwords");
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
 		RString TypeId(GetConceptType(ccText,Code+"Stopwords",Name+" Stopwords",&Db));

 		RCursor<RString> Cur(Stops);
 		for(Cur.Start();!Cur.End();Cur.Next())
 			RQuery InsertStopWord(&Db,"INSERT INTO concepts(name,typeid) VALUES("+RQuery::SQLValue(*Cur())+","+TypeId+")");
 	}

 	// Create the configuration file with MySQL as default storage if it doesn't exist
	RURI URI(RString(getenv("HOME"))+"/.r/config/lib/galilei/sessions/"+Info->Name+".config");
	if(RFile::Exists(URI))
		return;
 	RTextFile Config(URI,"utf-8");
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
QCreateDatabase::QCreateDatabase(QGALILEIWin* parent)
	: QDialog(parent), Ui_QCreateDatabase(), Win(parent)
{
	setupUi(this);
	setWindowTitle("Fill Database");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
   connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	adjustSize();
}


//------------------------------------------------------------------------------
void QCreateDatabase::run(void)
{
	URL->setText("http://www.otlet-institute.org/docs/NewDb.sql");
	ConfigPath->setText(QString(getenv("HOME"))+"/.r/config/lib/galilei/sessions");
	if(!exec())
		return;
	try
	{
		Name=FromQString(DbName->text());
		Host=FromQString(hostName->text());
		User=FromQString(userName->text());
		Pwd=FromQString(password->text());
		DbSchema=FromQString(URL->text());
		ConfigDir=R::FromQString(ConfigPath->text());
		QCreateDB Task(Win,this);
		QSessionProgress::execute(Task);
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,QWidget::tr("R Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,QWidget::tr("Standard exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QMessageBox::critical(this,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
}
