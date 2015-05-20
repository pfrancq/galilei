/*

	GALILEI Research Project

	QFillDatabase.cpp

	Fill the database with a list of document - Implementation.

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
#include <rcursor.h>
#include <rqt.h>
#include <rdir.h>
#include <glang.h>
#include <gsubject.h>
#include <gdoc.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <gfilter.h>
#include <gsession.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>
#include <QFileDialog>


//------------------------------------------------------------------------------
// include files for current project
#include <qfilldatabase.h>
#include <qsessionprogress.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// class cLang
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *  Class used to make the correspondence between a given language at an entry
 *  in the combo box.
 */
class cLang
{
public:
	QString Name;
	GLang* Lang;

	cLang(const QString& name,GLang* lang) : Name(name), Lang(lang) {}
	int Compare(const cLang& lang) const {return(Name.compare(lang.Name));}
	int Compare(const QString& lang) const {return(Name.compare(lang));}
};



//-----------------------------------------------------------------------------
//
// class QImportDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * Thread used to import the documents.
 */
class QImportDocs : public QSessionProgress
{
	QFillDatabase* Info;
	GSession* Session;
public:
	QImportDocs(QGALILEIWin* win,QFillDatabase* info);
	virtual void DoIt(void);
	void ParseDir(const RURI& uri,const RString& parent,int depth);
};


//-----------------------------------------------------------------------------
QImportDocs::QImportDocs(QGALILEIWin* win,QFillDatabase* info)
	: QSessionProgress(win,"Import Documents"), Info(info), Session(win->getSession())
{
}


//-----------------------------------------------------------------------------
void QImportDocs::DoIt(void)
{
	// Specify the session that it must save everything
	bool Save(Session->MustSaveResults());
	Session->SetSaveResults(true);
	ParseDir(Info->Dir,Info->Parent,0);
	Session->SetSaveResults(Save);
}


//-----------------------------------------------------------------------------
void QImportDocs::ParseDir(const RURI& uri,const RString& parent,int depth)
{
	bool InDir(true);

	// Go through the directory
	depth++;
	RDir Dir(uri);
	Dir.Open(RIO::Read);
	setLabelText(ToQString(uri.GetPath()));
	RCursor<RFile> Files(Dir.GetEntries());
	for(Files.Start();!Files.End();Files.Next())
	{
		// If directory -> go deep
		if(dynamic_cast<RDir*>(Files()))
		{
			GSubject* Subject(0);
			RString cat(Files()->GetFileName());

			// Find parent subject
			if(Info->Categorized)
			{
				if(!parent.IsEmpty())
				{
					Subject=Session->GetObj(pSubject,parent);
					cat=parent+"/"+cat;
				}
				if((Info->MaxDepth->value()==0)||(depth<=Info->MaxDepth->value()))
				{
					// Create a new subject. Its identifier is one above the greatest.
					GSubject* New(new GSubject(Session,Session->GetMaxObjId(otSubject)+1,cat));
					Session->InsertObj(Subject,New);
					if(Info->Used)
						 Session->SetUsed(New,true);
				}
				else
					cat=parent;
			}

			ParseDir(Files()->GetURI(),cat,depth);
			InDir=false;
		}
		else
		{
			if(!InDir)
			{
				InDir=true;
			//	setLabelText(ToQString(uri.GetPath()));
			}

			// Must be a normal document
			GDoc* doc(new GDoc(Session,Files()->GetURI()(),Files()->GetURI()(),RString::Null,Info->Lang,Info->DefaultMIME));
			Session->InsertObj(doc);
			if(Info->Categorized)
			{
				GSubject* Subject(Session->GetObj(pSubject,parent));
				if(Subject)
				{
					Session->InsertObj(Subject,doc);
					if(Info->Used)
						 Session->SetUsed(doc,Subject,true);
				}
			}
		}
	}
}



//------------------------------------------------------------------------------
//
// class QFillDatabase
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QFillDatabase::QFillDatabase(QGALILEIWin* parent)
	: QDialog(parent), Ui_QFillDatabase(), Win(parent)
{
	setupUi(this);
	connect(EditDir,SIGNAL(pressed()),this,SLOT(editDir()));
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
   connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	setWindowTitle("Fill Database");
}


//------------------------------------------------------------------------------
void QFillDatabase::run(void)
{
	// Init
	Subjects->setChecked(true);
	RContainer<cLang,true,true> theLangs(20);
	Language->setEnabled(false);
	RCastCursor<GPlugIn,GLang> Langs(GALILEIApp->GetPlugIns<GLang>("Lang"));
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		theLangs.InsertPtr(new cLang(ToQString(Langs()->GetLangName()),Langs()));
		Language->addItem(ToQString(Langs()->GetLangName()));
	}

	// Execute
	if(exec())
	{
		Dir=FromQString(DirChosen->text());
		Parent=FromQString(ParentName->text());
		if(Dir.IsEmpty())
		{
			QMessageBox::critical(this,"Error","You must specify a directory containing all the subjects!",QMessageBox::Ok);
			return;
		}
		if((HasParent->isChecked())&&Parent.IsEmpty())
		{
			QMessageBox::critical(this,"Error","You must insert a NAME for the parent or unchecked the \"Has Parent\" option!",QMessageBox::Ok);
			return;
		}
		Lang=0;
		if(FixLanguage->isChecked())
			Lang=theLangs.GetPtr(Language->currentText())->Lang;
		DefaultMIME=FromQString(DefaultMIMEType->text());
		Categorized=Subjects->isChecked();
		QImportDocs(Win,this).run();
	}
}


//------------------------------------------------------------------------------
void QFillDatabase::ChangeParent(int/* s*/)
{
    /*if(s==QButton::On)
    {
	ParentName->setEnabled(true);
	ChooseBtn->setEnabled(true);
    }
    if(s==QButton::Off)
    {
	ParentName->setText("");
	ParentName->setEnabled(false);
	ChooseBtn->setEnabled(false);
    }
    if(s==Qbutton:NoChange)*/
}


//------------------------------------------------------------------------------
void QFillDatabase::InsertSubItem(RDb* /*db*/,int /*parentId*/,QListWidgetItem* /*item*/)
{
/*	QListViewItem* t;
	R::RString sSQL("");

	sSQL="SELECT * FROM topics where parent="+R::RString::Number(parentId);
	auto_ptr<R::RQuery> insert(db->Query(sSQL));

	for(insert->Start();!insert->End();insert->Next())
	{
		item->insertItem(t=new QListViewItem(item,ToQString((*insert)[1])));
		InsertSubItem(db,atoi((*insert)[0]),t);
	}
*/
}


//------------------------------------------------------------------------------
void QFillDatabase::ChooseParentName(void)
{
/*	R::RString sSQL("");
	QListViewItem* t;

	try
	{
		//Init dlg
		QChooseParent dlg(this,0,true);
		TopicsView->setColumnWidth(0,300);

		GSubjects * 	GetSubjects

		sSQL="SELECT * FROM topics where parent=0";
		R::RQuery mainTopics(Db,sSQL);

		for(mainTopics.Start();!mainTopics.End();mainTopics.Next())
		{
			TopicsView->insertItem(t=new QListViewItem(TopicsView,ToQString(mainTopics[1])));
			InsertSubItem(Db,atoi(mainTopics[0].Latin1()),t);
		}

		if(exec())
		{
			R::RString tmp("");
			t=TopicsView->selectedItem();
			if(t)
			{
				tmp=R::FromQString(t->text(0));
				t=t->parent();
				while(t)
				{
					tmp=R::FromQString(t->text(0))+"/"+tmp;
					t=t->parent();
				}
				ParentName->setText(ToQString(tmp));
			}
		}
	}
	//catch(GException& e)
	//{
	//	QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
	//}
	catch(R::RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
	}
	catch(std::bad_alloc)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString("Memory Error"));
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString("Undefined Error"));
	}*/
	std::cout<<"Choose Parent not implemented"<<std::endl;
}


//-----------------------------------------------------------------------------
void QFillDatabase::editDir(void)
{
	QString fileName(QFileDialog::getExistingDirectory(this,"Choose directory",EditDir->text(),QFileDialog::ShowDirsOnly));
   if(!fileName.isEmpty())
		DirChosen->setText(fileName);
}
