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



//-----------------------------------------------------------------------------
// include files for R/GALILEI
/*#include <rcursor.h>

#include <rdir.h>**/
#include <rqt.h>
#include <glang.h>
#include <ggalileiapp.h>
#include <gsession.h>
/*#include <gsubjects.h>
#include <gsubject.h>
#include <gdoc.h>

#include <gfilter.h>*/
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <kmessagebox.h>


//------------------------------------------------------------------------------
// include files for current project
#include <qcreatedatabase.h>
#include <qsessionprogress.h>



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
	virtual void DoIt(void);
	void RunSQL(const RURI& path,RDb* Db);
};


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	Parent->setLabelText("Database structure created");
	RDbMySQL::Create(Info->Name,Info->Host,Info->User,Info->Pwd);
	RDbMySQL Db(Info->Name,Info->Host,Info->User,Info->Pwd,"utf-8");
 	Parent->setLabelText("Dump Database model");
 	RunSQL(Info->DbSchema,&Db);
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
	URL->setUrl(KUrl("http://homepages.ulb.ac.be/~pfrancq/data/NewDb.sql"));
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
