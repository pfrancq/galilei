/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
#include <frontend/kde/rqt.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kapplication.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <docs/gdoc.h>
#include <infos/glang.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <docs/gpostdoc.h>
#include <docs/gdoc.h>
#include <profiles/guser.h>
#include <sessions/gsession.h>
#include <profiles/gprofile.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggrouping.h>
#include <groups/ggroup.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for current project
#include "qsessionprogress.h"



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
	bool cancel=true;

	try
	{
		DoIt();
		cancel=false;
	}
	catch(GException& e)
	{
		Parent->PutText(e.GetMsg());
	}
	catch(RException& e)
	{
		Parent->PutText(e.GetMsg());
	}
	catch(std::bad_alloc)
	{
		Parent->PutText("Memory Error");
	}
	catch(...)
	{
		Parent->PutText("Undefined Error");
	}
	if(GSession::Break())
	{
		Parent->PutText("Operation Cancelled");
		cancel=true;
	}
	Parent->Finish(cancel);
};


//-----------------------------------------------------------------------------
QLoadSession::QLoadSession(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng,GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng, GPostProfileManager* ppmng, GPostGroupManager* pgmng, GEngineManager* emng)
{
	Langs=langs;
	Umng=umng;
	Dmng=dmng;
	Pmng=pmng;
	Gmng=gmng;
	GCmng=gcmng;
	Smng=smng;
	Lmng=lmng;
	PDmng=pdmng;
	PPmng=ppmng;
	PGmng=pgmng;
	Emng=emng;
}


//-----------------------------------------------------------------------------
void QLoadSession::DoIt(void)
{
	Parent->PutText("Loading Dicionnaries/Stoplists ...");
	Session->Connect();
	if(GSession::Break())
		return;
	Parent->PutText("Load Subject Tree ...");
	Session->GetStorage()->LoadSubjectTree(Session);
	if(GSession::Break())
		return;
	Parent->PutText("Loading Documents ...");
	Session->GetStorage()->LoadDocs(Session);
	if(GSession::Break())
		return;
	Parent->PutText("Load Groups ...");
	Session->GetStorage()->LoadGroups(Session);
	if(GSession::Break())
		return;
	Parent->PutText("Load Users/Profiles/SubProfiles ...");
	Session->GetStorage()->LoadUsers(Session);
	if(GSession::Break())
		return;
	Parent->PutText("Load Ideal Groups ...");
	Session->GetStorage()->LoadIdealGroupment(Session);
	if(GSession::Break())
		return;
	Parent->PutText("Load Users Feedbacks ...");
	Session->GetStorage()->LoadFdbks(Session);
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
	RDb Db(Host,User,Pass,DbName,"latin1");
	if(GSession::Break())
		return;

	//Dump database model
	Parent->PutText("Dump Database model");
	path=SQLPath+"DbModel.sql";
	RTextFile fileM(path,Read,"Latin1");

	while((!fileM.Eof())&&(!GSession::Break()))
	{
		line=fileM.GetLine();
		if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
			continue;

		endFound=false;
		while(!fileM.Eof() && !endFound)
		{
			if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
			{
				sql="";
				endFound=true;
				continue;
			}
			sql+=line;
			if(line.Find(';')>=0)
				endFound=true;
			else
				line=fileM.GetLine();
		}
		if(!sql.IsEmpty())
			RQuery Sendquery(Db,sql);

		sql="";
	}
	if(GSession::Break())
		return;

	//Dump stoplists files
	if(UseStopList)
	{
		msg="Dump Database Stoplists";
		Parent->PutText(msg);
		msg+=" for language ";
		DIR* dp;
		struct dirent* ep;
		path="";

		dp=opendir(SQLPath);
		if(dp)
		{
			while((ep=readdir(dp))&&(!GSession::Break()))
			{
				if(strncmp(&ep->d_name[0],"DbStopList",10)) continue;
				msg+=ep->d_name[11];
				msg+=ep->d_name[12];

				path=SQLPath+ep->d_name;
				RTextFile fileS(path,Read,"Latin1");

				Parent->PutText(msg);
				while(!fileS.Eof())
				{
					line=fileS.GetLine();
					if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
						continue;
					RQuery stopquery(Db,line);
				}
				msg=msg.Mid(0,msg.GetLen()-2);
			}
		}
		if(GSession::Break())
			return;
	}

	if(UseUsers)
	{
		//Dump users file
		path=SQLPath +"DbUsers.sql";
		Parent->PutText("Dump Database users");

		RTextFile fileU(path,Read,"Latin1");

		while((!fileU.Eof())&&(!GSession::Break()))
		{
			line=fileU.GetLine();
			if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
				continue;

			endFound=false;
			while(!fileU.Eof() && !endFound)
			{
				if(line.IsEmpty() || line.FindStr("--")>=0 || line.Find('#')>=0)
				{
					sql="";
					endFound=true;
					continue;
				}
				sql+=line;
				if(line.Find(';')>=0)
					endFound=true;
				else
					line=fileU.GetLine();
			}
			if(!sql.IsEmpty())
				RQuery Sendquery(Db,sql);

			sql="";
		}
	}
	if(GSession::Break())
		return;
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
	cout<<"nb topics"<<nbTopicsAtBegin<<endl;
		
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
	
	//Get Nb users
	sSql="SELECT userid FROM users";
	RQuery userId(Db,sSql);
	for(userId.Start();!userId.End();userId.Next())
	{
		usersId.InsertPtr(new RString(userId[0]));
	}

	sSql="SELECT * FROM topics WHERE parent !=0 and topicid>"+itou(nbTopicsAtBegin);
	cout<<sSql<<endl;
	RQuery topics(Db,sSql);
	for(topics.Start();!topics.End();topics.Next())
	{
		sSql="SELECT name FROM topics WHERE topicid="+topics[2];
		RQuery topics2(Db,sSql);

		topics2.Start();
		if((!topics.End())&&topics[0])
		{
			RCursor<RString> userCur(usersId);
			for(userCur.Start();!userCur.End();userCur.Next())
			{
				sSql="INSERT INTO profiles SET description='"+topics2[0]+"/"+topics[1]+"',updated='2004-01-01',userid="+userCur()+",topicid="+topics[0];
				RQuery prof(Db,sSql);
			}
		}
	}
}


//-----------------------------------------------------------------------------
int QFillDB::CreateCategory(RString name,int parentId)
{
	RString sSql("");
	int catId;
	sSql="SELECT topicid FROM topics WHERE name='" +name +"' AND parent=" + itou(parentId);
	RQuery cat(Db,sSql);
	cat.Start();
	if((!cat.End())&&(cat[0]))
		catId=atoi(cat[0]);
	else
	{
		sSql="INSERT INTO topics SET name='"+name+"',langid='en',parent="+itou(parentId);
		RQuery insert(Db,sSql);

		sSql="SELECT topicid FROM topics WHERE name='" +name +"' AND parent=" + itou(parentId);
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
	//Determine mime type of the file
	RString sSql("");
	RString mime("");
	mime=FilterManager->GFilterManager::DetermineMIMEType(path.Latin1());
	if(!mime.IsEmpty())
	{
		sSql="SELECT * FROM htmls WHERE html='"+path+"'";
		RQuery allReadyExists(Db,sSql);
		allReadyExists.Start();
		if(allReadyExists.End())
		{
			//Insert doc in htmls
			sSql="INSERT INTO htmls SET html='"+path+"',updated='2004-01-01',mimetype='"+mime+"',title='"+path+"'";
			RQuery insert(Db,sSql);
			CurrentDocId++;

			//Insert docid and topicid in htmlsbytopics
			sSql="INSERT INTO topicsbyhtmls SET topicid="+itou(parentId)+",htmlid="+itou(CurrentDocId);
			RQuery insert2(Db,sSql);
		}
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
	XML=Session->CreateDocXML(Doc);
}


//-----------------------------------------------------------------------------
void QAnalyzeXML::DoIt(void)
{
	if(!XML)
	{
		Parent->PutText("Creating XML Structure ...");
		XML=Session->CreateDocXML(Doc);
	}
	if(GSession::Break())
		return;
	Parent->PutText("Analysing XML Structure ...");
	if(!(dynamic_cast<GDocAnalyseManager*>(GPluginManager::GetManager("DocAnalyse")))->GetCurrentMethod())
		throw GException("Error: No Text Analyse method chosen.");
	(dynamic_cast<GDocAnalyseManager*>(GPluginManager::GetManager("DocAnalyse")))->GetCurrentMethod()->Analyze(XML,Doc);
}


//-----------------------------------------------------------------------------
void QAnalyzeDocs::DoIt(void)
{
	Parent->PutText("Analyse Documents ...");
	Session->AnalyseDocs(Parent,Modified,Save);
}


//-----------------------------------------------------------------------------
void QComputeProfiles::DoIt(void)
{
	Parent->PutText("Compute Profiles ...");
	Session->CalcProfiles(Parent,Modified,Save,SaveLinks);
}


//-----------------------------------------------------------------------------
void QGroupProfiles::DoIt(void)
{
	Parent->PutText("Groups Profiles ...");
	Session->GroupingProfiles(Parent,Modified,Save);
}


//-----------------------------------------------------------------------------
void QComputePostGroup::DoIt(void)
{
	Parent->PutText("Computing PostGroup ...");
	Session->ComputePostGroup(Parent);
}


//-----------------------------------------------------------------------------
void QExportMatrix::DoIt(void)
{
	Parent->PutText("Export vectors/words matrix...");
	Session->ExportMatrix(Parent,Type,Name,Lang,Label);
}


//-----------------------------------------------------------------------------
void QComputeAll::DoIt(void)
{
	Parent->PutText("Analyse Documents ...");
	Session->AnalyseDocs(Parent,Modified);
	if(GSession::Break())
		return;
	Parent->PutText("Compute Profiles ...");
	Session->CalcProfiles(Parent,Modified,Save,SaveLinks);
	if(GSession::Break())
		return;
	if(GSession::Break())
		return;
	Parent->PutText("Groups Profiles ...");
	Session->GroupingProfiles(Parent,Modified,Save);
}


//-----------------------------------------------------------------------------
void QFillMIMETypes::receiveNextMIMEPath(const char* path,RXMLStruct& xml)
{
//	unsigned int Len;
//	int pos,next;
//	bool Desktop;
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
//			cout<<Full.Latin1()<<endl;
/*			RTextFile File(Full);
			RString Type;
			RString Comment;
			RString MIME;
			RString Patterns;
			RString Encoding;
			RString Line;

			Desktop=false;
			while(!File.Eof())
			{
				if(!strcmp(path,"%"))
					cout<<debug<<endl;
				Line=File.GetLine();
				Len=Line.GetLen();
				if(Len<5) continue;
				if(Line[static_cast<unsigned int>(0)]==RChar('['))
					Desktop=(Line=="[Desktop Entry]");
				if(!Desktop) continue;

				// Read Information
				if(Line.Mid(0,4)=="Type")
					Type=Line.Mid(5,Len-5);
				if(Line.Mid(0,8)=="Encoding")
					Encoding=Line.Mid(9,Len-9);
				if(Line.Mid(0,8)=="Patterns")
					Patterns=Line.Mid(9,Len-9);
				if((Line.Mid(0,7)=="Comment")&&(Line[static_cast<unsigned int>(7)]!=RChar('[')))
					Comment=Line.Mid(8,Len-8);
				if(Line.Mid(0,8)=="MimeType")
					MIME=Line.Mid(9,Len-9);
				if(!strcmp(path,"%"))
					cout<<debug<<endl;
			}
			if((!Patterns.IsEmpty())&&(!MIME.IsEmpty())&&(Type=="MimeType"))
			{
				cout<<"<mimeType description=\""<<Comment.Latin1()<<"\" code=\""<<MIME.Latin1()<<"\"";
				if(!Encoding.IsEmpty())
				{
					cout<<" encoding=\""<<Encoding.Latin1()<<"\"";
				}
				cout<<">"<<endl;
				pos=0;
				while(pos!=-1)
				{
					next=Patterns.Find(';',pos+1);
					if(next>0)
						cout<<"<file type=\""<<Comment.Latin1()<<"\" ext=\""<<Patterns.Mid(pos,next-pos).Latin1()<<"\"/>"<<endl;
					pos=next;
				}
				cout<<"</mimeType"<<endl;
			}*/
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
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg(QWidget* parent,GSession* s,const char* c) throw(std::bad_alloc,RException)
    : QSemiModal(parent,"QSessionProgressDlg",true), GSlot(), Session(s), Task(0), Running(false)
{
	resize(600, 78 );
	setCaption(i18n(c));

	btnOk = new QPushButton( this, "buttonOk_2" );
	btnOk->setGeometry( QRect( 260, 50, 80, 22 ) );
	btnOk->setText( i18n( "&OK" ) );
	btnOk->setAutoDefault( TRUE );
	btnOk->setDefault( TRUE );

	Line = new QFrame( this, "Line1" );
	Line->setGeometry( QRect( 0, 30, 600, 20 ) );
	Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );

	txtRem = new QLabel( this, "txtRem" );
	txtRem->setGeometry( QRect( 10, 10, 580, 20 ) );
	txtRem->setText(c+QString(" ..."));

	connect(btnOk,SIGNAL(clicked()),this,SLOT(receiveButton()));
}


//-----------------------------------------------------------------------------
bool QSessionProgressDlg::Run(QSessionThread* task)
{
	btnOk->setText(i18n("&Cancel"));
	KApplication::kApplication()->processEvents();
	Task=task;
	Task->Set(Session,this);
	Running=true;
	Task->start();
	exec();
	if(Task)
	{
		delete Task;
		Task=0;
	}
	if(GSession::Break())
	{
		GSession::ResetBreak();
		return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Groups Profiles for '")+ToQString(lang->GetName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextDoc(const GDoc* doc) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Analyse Doc '")+ToQString(doc->GetName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextProfile(const GProfile* prof) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Analyse Profile '")+ToQString(prof->GetName())+"' of User '"+ToQString(prof->GetUser()->GetName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextProfileExport(const GProfile* prof) throw(std::bad_alloc,RException)
{
	txtRem->setText("Exporting Profile "+QString::number(prof->GetId())+ToQString(prof->GetName()));
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextGroupExport(const GGroup* grp) throw(std::bad_alloc,RException)
{
	char tmp[100];
	sprintf(tmp,"Exporting  Group %u", grp->GetId());
	txtRem->setText(tmp);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextDocumentExport(const GDoc* doc) throw(std::bad_alloc,RException)
{
	char tmp[100];
	sprintf(tmp,"Exporting  Document %u", doc->GetId());
	txtRem->setText(tmp);
}

//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextChromosome(unsigned int id) throw(std::bad_alloc,RException)
{
	char tmp[50];

	sprintf(tmp,"Analyse Chromosome n%u ...",id);
	txtRem->setText(tmp);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveButton()
{
	// If nothing running -> can close
	if(!Running)
	{
		close();
		return;
	}
	// Ah ah, something runs -> ask to break it
	btnOk->setEnabled(false);
	GSession::SetBreak();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::PutText(const char* text)
{
	txtRem->setText(text);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Begin(void)
{
	txtRem->setText("Cancel");
	btnOk->setEnabled(true);
	show();
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Finish(bool Cancel)
{
	if(Task)
	{
		Running=false;
	}
	if(!Cancel)
		txtRem->setText("Finish");
	btnOk->setEnabled(true);
	btnOk->setText(i18n("&OK"));
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
