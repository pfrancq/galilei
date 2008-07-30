/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>
#include <fnmatch.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rqt.h>
#include <rdir.h>
#include <rxmlfile.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlistviewitemtype.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kapplication.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gstorage.h>
#include <gdoc.h>
#include <glang.h>
#include <gdocanalyse.h>
#include <gdocxml.h>
#include <gpostdoc.h>
#include <gdoc.h>
#include <guser.h>
#include <gsession.h>
#include <gprofile.h>
#include <gprofilecalc.h>
#include <ggroupprofiles.h>
#include <ggroup.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for current project
#include "qsessionprogress.h"
#include <kdoc.h>


//-----------------------------------------------------------------------------
//
// class All session threads
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionThread::QSessionThread(void)
	: QThread(), Parent(0), Session(0)
{
}

//-----------------------------------------------------------------------------
void QSessionThread::Set(GSession* session,QSessionProgressDlg* parent)
{
	Parent=parent;
	Session=session;
}


//-----------------------------------------------------------------------------
void QSessionThread::run(void)
{
	try
	{
		DoIt();
	}
	catch(GException& e)
	{
		Parent->PutError(e.GetMsg());
	}
	catch(RException& e)
	{
		Parent->PutError(e.GetMsg());
	}
	catch(std::exception& e)
	{
		Parent->PutError(e.what());
	}
	catch(...)
	{
		Parent->PutError("Undefined Error");
	}
	Parent->Finish();
}


//-----------------------------------------------------------------------------
QLoadSession::QLoadSession(void)
{
}


//-----------------------------------------------------------------------------
void QLoadSession::DoIt(void)
{
	Parent->PutText("Loading Documents ...");
	Session->GetStorage()->LoadDocs();
	if(GSession::Break())
		return;
	Parent->PutText("Load Groups ...");
	Session->GetStorage()->LoadCommunities();
	if(GSession::Break())
		return;
	Parent->PutText("Load Users/Profiles/Feedbacks ...");
	Session->GetStorage()->LoadUsers();
}


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	Parent->PutText("Database structure created");
	RDb::CreateMySQLDatabase(Host,User,Pass,Name);
	auto_ptr<RDb> Db(RDb::Get(RDb::MySQL,Name,Host,User,Pass,"utf-8"));
	if(GSession::Break())
		return;

 	// Construct tables
 	Parent->PutText("Dump Database model");
 	RunSQL(SchemaURL+"DbModel.sql",Db);
	if(GSession::Break())
		return;

 	// Import stoplists
 	RCursor<GLang> Langs=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
 	for(Langs.Start();!Langs.End();Langs.Next())
 	{
 		Parent->PutText("Import stoplist for "+Langs()->GetName());
 		RString Stop=SchemaURL+"DbStopList_"+Langs()->GetCode()+".sql";
 		try
 		{
 		//if(RFile::Exists(Stop))
 				RunSQL(Stop,Db);
 		}
 		catch(...)
 		{
 		}
 		if(GSession::Break())
 			return;
 	}
}


//-----------------------------------------------------------------------------
void QCreateDB::RunSQL(const RURI& path,std::auto_ptr<RDb>& Db)
{
	RString sql("");
	RString line("");
	bool endFound=false;

 	RTextFile file(path,"utf-8");
	file.Open(RIO::Read);

	while((!file.End())&&(!GSession::Break()))
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
			auto_ptr<RQuery> Sendquery(Db->Query(sql));
		}

		sql="";
	}
}


//-----------------------------------------------------------------------------
class ImportFile : public RXMLFile
{
	GDoc* doc;
	GUser* user;
	GProfile* profile;
	tDocAssessment docass;
	int what;
	GSession* Session;

public:

	ImportFile(const RString& name,const RString& encoding,GSession* session)
		: RXMLFile(name,0,encoding), doc(0), user(0), profile(0), docass(djUnknow), what(0), Session(session) {}

protected:

	//-------------------------------------------------------------------------
	void BeginTag(const RString&, const RString&, const RString& name,RContainer<RXMLAttr,true,true>&)
	{
		if(name=="url")
			what=1;
		else if(name=="user")
			what=2;
		else if(name=="cat")
			what=3;
		else if(name=="valeur")
			what=4;
	}


	//-------------------------------------------------------------------------
	void EndTag(const RString&, const RString&, const RString& name)
	{
		if(name!="vote")
			return;
		Session->InsertFdbk(profile->GetId(),doc->GetId(),docass,RDate::GetToday(),RDate::Null,true);
		doc=0;
		user=0;
		profile=0;
		docass=djUnknow;
		what=0;
	}


	//-------------------------------------------------------------------------
	void Text(const RString& text)
	{
		RCharCursor Cur(text);

		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();
		if(Cur.End())
			return;
		switch(what)
		{
			case 1:
				doc=Session->GetDoc(text,true,true);
				if(!doc)
					Session->InsertDoc(doc=new GDoc(text,text,cNoRef,0,"text/html",cNoRef,RDate::GetToday(),RDate::Null,RDate::Null));
				break;
			case 2:
				user=Session->GetUser(text,true,true);
				if(!user)
					Session->InsertUser(user=new GUser(cNoRef,text,text));
				break;
			case 3:
				profile=user->GetPtr(text,false);
				if(!profile)
					Session->InsertProfile(profile=new GProfile(user,cNoRef,text,0,RDate::GetToday(),RDate::GetToday(),RDate::GetToday(),true));
				break;
			case 4:
				if(text=="+1")
					docass=djOK;
				else if(text=="-1")
					docass=djOutScope;
				break;
		}
		what=0;
	}
};


//-----------------------------------------------------------------------------
void QImportUsersData::DoIt(void)
{
	Parent->PutText("Analyze XML File");
	ImportFile File(XML,"utf-8",Session);
	File.Open(RIO::Read);
}


//-----------------------------------------------------------------------------
void QImportDocs::DoIt(void)
{
	FilterManager=GALILEIApp->GetManager<GFilterManager>("Filter");
	Subjects=Session->GetSubjects(true);
	CurDepth=0;
	ParseDir(Dir,Parent);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveSubjects();
}


//-----------------------------------------------------------------------------
void QImportDocs::ParseDir(const RURI& uri,const RString& parent)
{
	// Go through the directory
	CurDepth++;
	RDir Dir(uri);
	Dir.Open(RIO::Read);
	RCursor<RFile> Files(Dir.GetEntries());
	for(Files.Start();!Files.End();Files.Next())
	{
		// If directory -> go deep
		if(dynamic_cast<RDir*>(Files()))
		{

			RString cat(parent);
			GSubject* Topic;

			// Find parent topic
			if(parent.IsEmpty())
				Topic=Subjects->GetTop();
			else
				Topic=Subjects->GetNode(parent);

			if(CurDepth<=Depth)
			{
				if(!cat.IsEmpty())
					cat+="/";
				cat+=Files()->GetFileName();
				Subjects->InsertNode(Topic,new GSubject(Subjects->GetNbNodes(),cat,false));
			}
			ParseDir(Files()->GetURI(),cat);
		}
		else
		{
			// Must be a normal document
			GSubject* Topic=Subjects->GetNode(parent);
			GDoc* doc=new GDoc(Files()->GetURI(),Files()->GetURI(),cNoRef,0,DefaultMIME,cNoRef,RDate::GetToday(),RDate::Null,RDate::Null);
			Session->InsertDoc(doc);
			if(Topic)
				Topic->Insert(doc);
		}
	}
	CurDepth--;
}


//-----------------------------------------------------------------------------
void QCreateDocXML::DoIt(void)
{
	Parent->PutText("Creating XML Structure ...");
	if(XML)
	{
		delete XML;
		XML=0;
	}

	RIO::RSmartTempFile docxml;
	bool Native;
	RURI uri=GALILEIApp->GetManager<GFilterManager>("Filter")->WhatAnalyze(Doc,docxml,Native);
	if(!uri.IsEmpty())
	{
		XML=new RXMLStruct();
		RXMLFile In(uri,XML);
		In.Open(RIO::Read);
	}
}


//-----------------------------------------------------------------------------
void QAnalyzeXML::DoIt(void)
{
	Session->AnalyseDoc(Doc,0,Parent);
}


//-----------------------------------------------------------------------------
void QAnalyzeDocs::DoIt(void)
{
	Parent->PutText("Analyse Documents ...");
	Session->AnalyseDocs(Parent);
}


//-----------------------------------------------------------------------------
void QPostAnalyzeDocs::DoIt(void)
{
	Parent->PutText("Do Post-Documents Methods ...");
	Session->DoPostDocs(Parent);
}


//-----------------------------------------------------------------------------
void QComputeProfiles::DoIt(void)
{
	Parent->PutText("Compute Profiles ...");
	Session->CalcProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QPostComputeProfiles::DoIt(void)
{
	Parent->PutText("Do Post-Profiles Methods ...");
	Session->DoPostProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QComputeProfile::DoIt(void)
{
	Parent->PutText("Compute Profile ...");
	Session->CalcProfile(Profile,Parent);
}


//-----------------------------------------------------------------------------
void QGroupProfiles::DoIt(void)
{
	Parent->PutText("Groups Profiles ...");
	Session->GroupProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QPostGroupProfiles::DoIt(void)
{
	Parent->PutText("Do Post-Community Methods ...");
	Session->DoPostCommunity(Parent);
}


//-----------------------------------------------------------------------------
void QGroupDocs::DoIt(void)
{
	Parent->PutText("Groups Documents ...");
	Session->GroupDocs(Parent);
}


//-----------------------------------------------------------------------------
void QPostGroupDocs::DoIt(void)
{
	Parent->PutText("Do Post-Topic Methods ...");
	Session->DoPostTopic(Parent);
}


//-----------------------------------------------------------------------------
void QCreateIdealSubjects::DoIt(void)
{
	Parent->PutText("Create Ideal Communities ...");
	Session->GetSubjects()->Apply();
	Session->GetSubjects()->CreateIdeal();
}


//-----------------------------------------------------------------------------
void QCreateIdealCommunities::DoIt(void)
{
	Parent->PutText("Create Ideal Communities ...");
	Session->BuildGroupsFromIdeal(otCommunity);
}


//-----------------------------------------------------------------------------
void QCreateIdealTopics::DoIt(void)
{
	Parent->PutText("Create Ideal Topics ...");
	Session->BuildGroupsFromIdeal(otTopic);
}


//-----------------------------------------------------------------------------
void QMakeFdbks::DoIt(void)
{
	Parent->PutText("Make feedbacks ...");
	Session->GetSubjects()->Apply();
	Session->GetSubjects()->DocumentSharing();
}


//-----------------------------------------------------------------------------
void QMakeAssessments::DoIt(void)
{
	Parent->PutText("Make assessments ...");
	Session->GetSubjects()->Apply();
	Session->GetSubjects()->AddAssessments();
}


//-----------------------------------------------------------------------------
void QComputeAll::DoIt(void)
{
	Parent->PutText("Analyse Documents ...");
	Session->AnalyseDocs(Parent);
	if(GSession::Break())
		return;
	Parent->PutText("Compute Profiles ...");
	Session->CalcProfiles(Parent);
	if(GSession::Break())
		return;
	if(GSession::Break())
		return;
	Parent->PutText("Groups Profiles ...");
	Session->GroupProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QFillMIMETypes::receiveNextMIMEPath(const char* path,RXMLStruct& xml)
{
	DIR* dp;
	struct dirent* ep;
	RString Full;

	dp=opendir(path);
	if(!dp)
		return;
	while((ep=readdir(dp)))
	{
		if(GSession::Break())
			return;
		if((!strcmp(ep->d_name,"."))||(!strcmp(ep->d_name,"..")))
			continue;
		Full=RString(path)+"/"+ep->d_name;
		if(ep->d_type==DT_DIR)
			receiveNextMIMEPath(Full.Latin1(),xml);
		if((ep->d_type==DT_REG)&&(fnmatch("*.desktop",ep->d_name,0)==0))
		{
			Parent->PutText(ep->d_name);
		}
	}
}

//-----------------------------------------------------------------------------
void QFillMIMETypes::DoIt(void)
{
	RXMLStruct xml;
	receiveNextMIMEPath(Path,xml);
}


//-----------------------------------------------------------------------------
QLoadDictionaries::QListViewItemDict::QListViewItemDict(QListView* parent,GConceptType* dict)
	: QListViewItem(parent,ToQString(dict->GetName())), Dict(dict)
{
	setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xmag.png",KIcon::Small)));
}



//-----------------------------------------------------------------------------
void QLoadDictionaries::DoIt(void)
{
	// Go trough each language and create a Item.
	RCursor<GConceptType> Types(Session->GetConceptTypes());
	for(Types.Start();!Types.End();Types.Next())
	{
		QListViewItem* ptr=new QListViewItemDict(Dicts,Types());
		ptr->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
	}
}



//-----------------------------------------------------------------------------
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg(QWidget* parent,GSession* s,const char* c,bool cancel)
    : QSemiModal(parent,"QSessionProgressDlg",true), GSlot(), btnOk(0), Session(s), Running(false), Cancel(cancel), Canceled(false)
{
	resize(200, 28 );
	setCaption(i18n(c));

	QVBoxLayout* Layout = new QVBoxLayout( this, 11, 6, "QConnectMySQLLayout");

	txtRem = new QLabel( this, "txtRem" );
	txtRem->setGeometry( QRect( 10, 10, 80, 20 ) );
	txtRem->setText(c+QString(" ..."));
	Layout->addWidget(txtRem);

	QFrame* Line = new QFrame( this, "Line1" );
	Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	Layout->addWidget(Line);

	QHBoxLayout* HLayout = new QHBoxLayout( 0, 0, 6, "layout9_2");
	QSpacerItem* spacer5 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	HLayout->addItem( spacer5 );
	btnOk = new QPushButton( this, "buttonOk_2" );
	btnOk->setGeometry( QRect( 260, 50, 80, 22 ) );
	btnOk->setText( i18n( "&OK" ) );
	btnOk->setAutoDefault( TRUE );
	btnOk->setDefault( TRUE );
	btnOk->setEnabled(false);
	HLayout->addWidget(btnOk);
	spacer5 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	HLayout->addItem( spacer5 );
	Layout->addItem(HLayout);
}


//-----------------------------------------------------------------------------
bool QSessionProgressDlg::Run(QSessionThread* task)
{
	if(Cancel)
	{
		btnOk->setText(i18n("&Cancel"));
		btnOk->setEnabled(true);
	}
	KApplication::kApplication()->processEvents();
	task->Set(Session,this);
	Running=true;
	Changed=false;
	Canceled=false;
	Error=false;
	GSession::ResetBreak();
	task->start();
	connect(btnOk,SIGNAL(clicked()),this,SLOT(receiveButton()));
	show();
	while(Running)
	{
		if(Changed)
		{
			txtRem->setText(NewLabel);
			Changed=false;
		}
		KApplication::kApplication()->processEvents();
	}
	if(!Canceled)
	{
		if(!Error)
			txtRem->setText("Finish");
		else
			if(Changed)
			{
				txtRem->setText(NewLabel);
				Changed=false;
			}
	}
	else
		txtRem->setText("Canceled");
	btnOk->setEnabled(true);
	btnOk->setText(i18n("&OK"));
	disconnect(btnOk,SIGNAL(clicked()),this,SLOT(receiveButton()));
	connect(btnOk,SIGNAL(clicked()),this,SLOT(close()));
	exec();
	delete task;

	if(GSession::Break())
	{
		GSession::ResetBreak();
		return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang)
{
	NewLabel=QString("Groups Profiles for '")+ToQString(lang->GetName())+"' ...";
	Changed=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextDoc(const GDoc* doc)
{
	NewLabel=QString("Analyse Doc '")+ToQString(doc->GetName())+"' ...";
	Changed=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextProfile(const GProfile* prof)
{
	NewLabel=QString("Analyse Profile '")+ToQString(prof->GetName())+"' of User '"+ToQString(prof->GetUser()->GetFullName())+"' ...";
	Changed=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextChromosome(size_t id)
{
	NewLabel=QString("Analyse Chromosome ")+QString::number(id)+" ...";
	Changed=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveButton()
{
	if(btnOk)
		btnOk->setEnabled(false);
	GSession::SetBreak();
	Canceled=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::PutText(const char* text)
{
	NewLabel=text;
	Changed=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::PutError(const char* text)
{
	NewLabel=text;
	Changed=true;
	Error=true;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Finish(void)
{
	Running=false;
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
