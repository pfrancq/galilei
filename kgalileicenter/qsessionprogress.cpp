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
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng, GPostGroupManager* pgmng, GEngineManager* emng)
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
	PGmng=pgmng;
	Emng=emng;
}


//-----------------------------------------------------------------------------
void QLoadSession::DoIt(void)
{
	Parent->PutText("Connect (Loading Dicionnaries/Stoplists) ...");
	Session->Connect(Langs,Umng,Dmng,Lmng,Pmng,Gmng,GCmng,Smng,PDmng,PGmng,Emng);
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
	if(!Session->GetDocAnalyseMng()->GetCurrentMethod())
		throw GException("Error: No Text Analyse method chosen.");
	Session->GetDocAnalyseMng()->GetCurrentMethod()->Analyze(XML,Doc);
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
	Session->GroupingProfiles(Parent,Modified,Save,SaveHistory);
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
	Session->GroupingProfiles(Parent,Modified,Save,SaveHistory);
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
    : QSemiModal(parent,"QSessionProgressDlg",true), GSlot(), Session(s), Task(0)
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
	Task->start();
	exec();
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
	if(!Task)
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
		delete Task;
		Task=0;
	}
	if(!Cancel)
		txtRem->setText("Finish");
	btnOk->setEnabled(true);
	btnOk->setText(i18n("&OK"));
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
