/*

	GALILEI Research Project

	QCreateDatabase.cpp

	Create a database- Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
	QCreateDB(QCreateDatabase* info) : Info(info) {}
	RString GetConceptType(const RString& name,const RString& desc,RDb* Db);
	virtual void DoIt(void);
	void RunSQL(const RURI& path,RDb* Db);
};


//-----------------------------------------------------------------------------
RString QCreateDB::GetConceptType(const RString& name,const RString& desc,RDb* Db)
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


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	Parent->setLabelText("Database structure created");
	RDbMySQL::Create(Info->Name,Info->Host,Info->User,Info->Pwd);
	RDbMySQL Db(Info->Name,Info->Host,Info->User,Info->Pwd,"utf8");
 	Parent->setLabelText("Dump Database model");
 	RunSQL(Info->DbSchema,&Db);
 	Parent->setLabelText("Create Languages (terms and stopwords)");
 	RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
 	RContainer<RString,true,false> Stops(200);
 	for(Langs.Start();!Langs.End();Langs.Next())
 	{
 		// Create the concept types
 		RString Code(Langs()->GetCode());
 		GetConceptType(Code+"Terms",Langs()->GetName()+" Terms",&Db);
 		RString TypeId(GetConceptType(Code+"Stopwords",Langs()->GetName()+" Stopwords",&Db));

 		Stops.Clear();
 		Langs()->GetStopWords(Stops);
 		RCursor<RString> Cur(Stops);
 		for(Cur.Start();!Cur.End();Cur.Next())
 			RQuery InsertStopWord(&Db,"INSERT INTO concepts(name,typeid) VALUES("+RQuery::SQLValue(*Cur())+","+TypeId+")");
 	}

 	Parent->setLabelText("Create Other Concept Types");
 	GetConceptType("XMLStruct","XML Structure",&Db);
 	GetConceptType("XMLIndex","XML Index",&Db);
}


//-----------------------------------------------------------------------------
void QCreateDB::RunSQL(const RURI& path,RDb* Db)
{
	RString sql("");
	RString line("");
	bool endFound=false;

 	RTextFile file(path,"utf-8");
	file.Open(RIO::Read);

	while(!file.End())
	{
		line=file.GetLine();
		if(line.IsEmpty() || line.FindStr("/*!")>=0 || line.FindStr("--")>=0 || line.Find('#')>=0)
			continue;

		endFound=false;
		while(!file.End() && !endFound)
		{
			if(line.IsEmpty() || line.FindStr("--")>=0 || line.FindStr("--")>=0 || line.Find('#')>=0)
			{
				sql="";
				endFound=true;
				continue;
			}
			sql+=line;
			if(line.Find(';')>=0)
				endFound=true;
			else
				line=file.GetLine();
		}
		if(!sql.IsEmpty())
		{
			RQuery Sendquery(Db,sql);
		}

		sql="";
	}
}



//------------------------------------------------------------------------------
//
// class QCreateDatabase
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QCreateDatabase::QCreateDatabase(QWidget* parent)
	: KDialog(parent), Ui_QCreateDatabase()
{
	setCaption("Fill Database");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
}


//------------------------------------------------------------------------------
void QCreateDatabase::run(void)
{
	URL->setUrl(KUrl("http://www.imrdp.org/NewDb.sql"));
	if(!exec())
		return;
	try
	{
		Name=FromQString(DbName->text());
		Host=FromQString(hostName->text());
		User=FromQString(userName->text());
		Pwd=FromQString(password->text());
		DbSchema=FromQString(URL->url().url());
		QSessionProgressDlg Dlg(this,"Create Database");
		Dlg.Run(new QCreateDB(this));
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
