/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#include <ggrouping.h>
#include <ggroup.h>
#include <gsubjects.h>
#include <ggalileiapp.h>
#include <gdict.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;


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
	catch(std::bad_alloc)
	{
		Parent->PutError("Memory Error");
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
	Parent->PutText("Loading Dicionnaries/Stoplists ...");
	if(GSession::Break())
		return;
	Parent->PutText("Loading Documents ...");
	Session->GetStorage()->LoadDocs();
	if(GSession::Break())
		return;
	Parent->PutText("Load Groups ...");
	Session->GetStorage()->LoadGroups();
	if(GSession::Break())
		return;
	Parent->PutText("Load Users/Profiles/Feedbacks/SubProfiles ...");
	Session->GetStorage()->LoadUsers();
}


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	RString line("");
	RString sql("");
	RString msg("");
	RString path("");
	bool endFound=false;

	//Qsession progres to view progression
	Parent->PutText("Database structure created");
	RDb::CreateDatabase(Host,User,Pass,DbName);
	RDb Db(Host,User,Pass,DbName,"utf-8");
 	if(GSession::Break())
 		return;

 	// Construct tables
 	Parent->PutText("Dump Database model");
 	path=SQLPath+"/DbModelMySQL.sql";
 	RTextFile file(path,"utf-8");
	file.Open(RIO::Read);

	while((!file.Eof())&&(!GSession::Break()))
	{
		line=file.GetLine();
		if(line.IsEmpty() || line.FindStr("/*!")>=0 || line.FindStr("--")>=0 || line.Find('#')>=0)
			continue;

		endFound=false;
		while(!file.Eof() && !endFound)
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
			RQuery Sendquery(Db,sql);

		sql="";
	}

	if(GSession::Break())
		return;


// 	if(UseUsers)
// 	{
// 		//Dump users file
// 		path=SQLPath +"DbUsers.sql";
// 		Parent->PutText("Dump Database users");
//
// 		RTextFile fileU(path,"Latin1");
//
// 		fileU.Open(RIO::Read);
// 		while((!fileU.Eof())&&(!GSession::Break()))
// 		{
// 			line=fileU.GetLine();
// 			if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
// 				continue;
//
// 			endFound=false;
// 			while(!fileU.Eof() && !endFound)
// 			{
// 				if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
// 				{
// 					sql="";
// 					endFound=true;
// 					continue;
// 				}
// 				sql+=line;
// 				if(line.Find(';')>=0)
// 					endFound=true;
// 				else
// 					line=fileU.GetLine();
// 			}
// 			if(!sql.IsEmpty())
// 				RQuery Sendquery(Db,sql);
//
// 			sql="";
// 		}
// 	}
	if(GSession::Break())
		return;
}


//-----------------------------------------------------------------------------
void QImportStopLists::DoIt(void)
{
 	RDir Dir(SQLPath);
 	Dir.Open(RIO::Read);
 	RCursor<RFile> Files=Dir.GetEntries();
 	for(Files.Start();!Files.End();Files.Next())
	{
		int pos=Files()->GetFileName().FindStr("StopList");
		if(pos==-1) continue;
		RTextFile Stop(Files()->GetName(),"utf-8");
		Stop.Open(RIO::Read);
		GLang* lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn(Files()->GetFileName().Mid(9,2));
		Parent->PutText("Import stoplist for "+lang->GetName());
		GDict* dic=lang->GetDict();
		while(!Stop.Eof())
		{
			RString stop=Stop.GetLine();
			GConcept w(cNoRef,lang,stop,2,0,0,0);
			dic->InsertConcept(&w);
		}
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
		Session->InsertFdbk(profile->GetId(),doc->GetId(),0,docass,RDate::GetToday(),RDate::null,true);
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
					Session->InsertDoc(doc=new GDoc(text,text,cNoRef,0,"text/html",RDate::GetToday(),RDate::null));
				break;
			case 2:
				user=Session->GetUser(text,true,true);
				if(!user)
					Session->InsertUser(user=new GUser(cNoRef,text,text));
				break;
			case 3:
				profile=user->GetPtr(text,false);
				if(!profile)
					Session->InsertProfile(profile=new GProfile(user,cNoRef,text,true));
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
void QFillDB::DoIt(void)
{
	Parent->PutText("Try to connect database");

	//Init
	DIR* dp;
	struct dirent* ep;
	RContainer<RString,true,false> usersId(10,5);
	RString path("");
	RString sSql("");
	RString tmp("");
	bool found=false;
	int catId;
	int parentId=0;
	int nbTopicsAtBegin=0;
	unsigned int profileid;

	//Connect to DB
	Db = new RDb(Host,User,Pass,DbName,"latin1");

	Parent->PutText("Find documents and fill categories");

	//Find ID of last inserted document
	sSql="SELECT count(*) FROM htmls";
	RQuery nbDoc(Db,sSql);
	nbDoc.Start();
	if((!nbDoc.End())&&(nbDoc[0]))
		CurrentDocId=atoi(nbDoc[0]);

	sSql="SELECT count(*) FROM topics";
	RQuery nbTopic(Db,sSql);
	nbTopic.Start();
	if((!nbTopic.End())&&(nbTopic[0]))
		nbTopicsAtBegin=atoi(nbTopic[0]);

	if(!ParentName.IsEmpty())
	{
		tmp=ParentName;
		while(tmp.Find('/')>-1)
		{
			int id=tmp.Find('/');
			parentId= CreateCategory(tmp.Mid(0,id),parentId);
			tmp=tmp.Mid(id+1);
		}
		if(!tmp.IsEmpty())
		{
			parentId= CreateCategory(tmp,parentId);
		}
	}


	//Parse directory containing files
	dp=opendir(CatDirectory);
	if(dp)
	{
		//Parse all sub directories and insert docs in categories
		while((ep=readdir(dp))&&(!GSession::Break()))
		{
			if((ep->d_type==DT_DIR)&&(ep->d_name[0] != '.'))
			{
				found=true;
				catId= CreateCategory(ep->d_name,parentId);
				path= CatDirectory+ep->d_name;
				ParseDocDir(path,catId,1);
			}
			else if((!ParentName.IsEmpty())&&(ep->d_type==DT_REG))
			{
				path=CatDirectory+ ep->d_name;
				InsertDocument(path,parentId);
			}
		}
		if(GSession::Break())
			return;
		//If no categories are found -> stop
		if(!found)
		{
			throw new GException("Fill Database : no category detected");
		}
	}

	//Create profiles
	Parent->PutText("create Profiles");
	profileid=0;

	//Get Nb users
	sSql="SELECT userid FROM users";
	RQuery userId(Db,sSql);
	for(userId.Start();!userId.End();userId.Next())
	{
		usersId.InsertPtr(new RString(userId[0]));
	}

	sSql="SELECT * FROM topics WHERE  topicid>"+RString::Number(nbTopicsAtBegin);
//	sSql="SELECT * FROM topics WHERE parent !=0 and topicid>"+RString::Number(nbTopicsAtBegin);
	RQuery topics(Db,sSql);
	for(topics.Start();!topics.End();topics.Next())
	{
//		sSql="SELECT name FROM topics WHERE topicid="+topics[2];
//		RQuery topics2(Db,sSql);

//		topics2.Start();
		if((!topics.End())&&topics[0])
		{
			RCursor<RString> userCur(usersId);
			for(userCur.Start();!userCur.End();userCur.Next())
			{
				sSql="INSERT INTO profiles SET description='"+topics[1]+"',updated='2004-01-01',userid="+(*userCur())+",topicid="+topics[0];
//				sSql="INSERT INTO profiles SET description='"+topics2[0]+"/"+topics[1]+"',updated='2004-01-01',userid="+userCur()+",topicid="+topics[0];
				RQuery prof(Db,sSql);
				sSql="INSERT INTO subprofiles SET langid='en', attached='2001-01-01', profileid="+RString::Number(++profileid);
				RQuery subprof(Db,sSql);
			}
		}
	}
}


//-----------------------------------------------------------------------------
int QFillDB::CreateCategory(RString name,int parentId)
{
	RString sSql("");
	int catId;
	sSql="SELECT topicid FROM topics WHERE name='" +name +"' AND parent=" + RString::Number(parentId);
	RQuery cat(Db,sSql);
	cat.Start();
	if((!cat.End())&&(cat[0]))
		catId=atoi(cat[0]);
	else
	{
		sSql="INSERT INTO topics SET name='"+name+"',langid='en',parent="+RString::Number(parentId);
		RQuery insert(Db,sSql);

		sSql="SELECT topicid FROM topics WHERE name='" +name +"' AND parent=" + RString::Number(parentId);
		RQuery get(Db,sSql);
		get.Start();
		if(get[0])
			catId=atoi(get[0]);
		else
			throw new GException("Cannot insert new category in database");
	}
	sSql= "Parse documents for category : "+name;
	Parent->PutText(sSql);
	return catId;
}


//-----------------------------------------------------------------------------
void QFillDB::InsertDocument(RString path,int parentId)
{
	RString sSql("");
	
	sSql="SELECT * FROM htmls WHERE html='"+path+"'";
	RQuery allReadyExists(Db,sSql);
	allReadyExists.Start();
	if(allReadyExists.End())
	{
		//Insert doc in htmls
		sSql="INSERT INTO htmls SET html='"+path+"',updated='2004-01-01',mimetype=NULL,title='"+path+"'";
		RQuery insert(Db,sSql);
		CurrentDocId++;

		// Insert docid and topicid in htmlsbytopics
		sSql="INSERT INTO topicsbyhtmls SET topicid="+RString::Number(parentId)+",htmlid="+RString::Number(CurrentDocId);
		RQuery insert2(Db,sSql);
	}
}


//-----------------------------------------------------------------------------
void QFillDB::ParseDocDir(RString path,int parentId, int level)
{
	DIR* dp;
	struct dirent* ep;
	int catId=0;
	RString newPath("");
	RString sSql("");
	dp=opendir(path);
	if(dp)
	{
		while((ep=readdir(dp))&&(!GSession::Break()))
		{
			if((ep->d_type==DT_DIR)&&(ep->d_name[0] != '.'))
			{
				if(level<Depth)
					catId= CreateCategory(ep->d_name,parentId);
				else
					catId=parentId;
				newPath=path+"/"+ep->d_name;
				ParseDocDir(newPath,catId,level+1);
			}
			else if(ep->d_type==DT_REG)
			{
				// Choose right mime type
				newPath=path+"/"+ep->d_name;
				InsertDocument(newPath,parentId);
			}
		}
	}
	if(GSession::Break())
		return;
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
	XML=GALILEIApp->GetManager<GFilterManager>("Filter")->CreateDocXML(Doc);
}


//-----------------------------------------------------------------------------
void QAnalyzeXML::DoIt(void)
{
	Session->AnalyseDoc(XML,Doc,0,0,Parent);
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
	Session->GroupingProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QPostGroupProfiles::DoIt(void)
{
	Parent->PutText("Do Post-Groups Methods ...");
	Session->DoPostGroups(Parent);
}


//-----------------------------------------------------------------------------
void QCreateIdealGroups::DoIt(void)
{
	Parent->PutText("Create Ideal Groups ...");
	Session->GetSubjects()->Apply();
	Session->GetSubjects()->CreateIdeal();
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
	Session->GroupingProfiles(Parent);
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
QLoadDictionnaries::QListViewItemDict::QListViewItemDict(QListViewItem* parent,GDict* dict,GSession* session)
	: QListViewItem(parent,ToQString(session->GetConceptType(dict->GetType(),false)->GetName())), Dict(dict)
{
	setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xmag.png",KIcon::Small)));
}



//-----------------------------------------------------------------------------
void QLoadDictionnaries::DoIt(void)
{
	// Go trough each language and create a Item.
	R::RCursor<GLang> CurLang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
	for(CurLang.Start(); !CurLang.End(); CurLang.Next())
	{
		QListViewItemType* ptr=new QListViewItemType(Dicts,ToQString(CurLang()->GetName()));
		ptr->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));

		// Go trough each dictionnary and create an item
		RCursor<GDict> Dicts(CurLang()->GetDicts());
		for(Dicts.Start();!Dicts.End();Dicts.Next())
			new QListViewItemDict(ptr,Dicts(),Session);
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
void QSessionProgressDlg::NextChromosome(unsigned int id)
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
