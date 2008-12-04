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
// include files for R/GALILEI
#include <rqt.h>
#include <rdir.h>
#include <ggalileiapp.h>
#include <rxmlfile.h>
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
// include files for Qt
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QLayout>



//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kapplication.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <qsessionprogress.h>



//-----------------------------------------------------------------------------
//
// class All session threads
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionThread::QSessionThread(void)
	: QThread(), Parent(0)
{
}

//-----------------------------------------------------------------------------
void QSessionThread::Set(QSessionProgressDlg* parent)
{
	Parent=parent;
}


//-----------------------------------------------------------------------------
void QSessionThread::run(void)
{
	try
	{
		DoIt();
		emit finish();
		if(GSession::Break())
			Parent->setLabelText("Canceled");
		else
			Parent->setLabelText("Finish");
	}
	catch(GException& e)
	{
		Parent->setLabelText(e.GetMsg());
	}
	catch(RException& e)
	{
		Parent->setLabelText(e.GetMsg());
	}
	catch(std::exception& e)
	{
		Parent->setLabelText(e.what());
	}
	catch(...)
	{
		Parent->setLabelText("Undefined Error");
	}
	Parent->Running=false;
	Parent->setButtonText(i18n("OK"));
}


//-----------------------------------------------------------------------------
QLoadSession::QLoadSession(void)
{
}


//-----------------------------------------------------------------------------
void QLoadSession::DoIt(void)
{
	Parent->setLabelText("Load Topics ...");
	GALILEIApp->GetSession()->LoadTopics();
	if(GSession::Break())
		return;
	Parent->setLabelText("Loading Documents ...");
	GALILEIApp->GetSession()->LoadDocs();
	if(GSession::Break())
		return;
	Parent->setLabelText("Load Communities ...");
	GALILEIApp->GetSession()->LoadCommunities();
	if(GSession::Break())
		return;
	Parent->setLabelText("Load Users/Profiles/Feedbacks ...");
	GALILEIApp->GetSession()->LoadUsers();
}


//-----------------------------------------------------------------------------
void QCreateDB::DoIt(void)
{
	// Create the database
	Parent->setLabelText("Database structure created");
	RDb::CreateMySQLDatabase(Host,User,Pass,Name);
	auto_ptr<RDb> Db(RDb::Get(RDb::MySQL,Name,Host,User,Pass,"utf-8"));
	if(GSession::Break())
		return;

 	// Construct tables
 	Parent->setLabelText("Dump Database model");
 	RunSQL(SchemaURL+"DbModel.sql",Db);
	if(GSession::Break())
		return;

 	// Import stoplists
 	RCursor<GLang> Langs=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
 	for(Langs.Start();!Langs.End();Langs.Next())
 	{
 		Parent->setLabelText(ToQString("Import stoplist for "+Langs()->GetName()));
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
void QImportDocs::DoIt(void)
{
	FilterManager=GALILEIApp->GetManager<GFilterManager>("Filter");
	Subjects=GALILEIApp->GetSession()->GetSubjects(true);
	CurDepth=0;
	ParseDir(Dir,Parent);
	GALILEIApp->GetSession()->GetStorage()->SaveSubjects();
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
			GSubject* Subject;
			RString cat(Files()->GetFileName());

			// Find parent topic
			if(parent.IsEmpty())
				Subject=Subjects->GetTop();
			else
			{
				Subject=Subjects->GetNode(parent);
				cat=parent+"/"+cat;
			}

			if(CurDepth<=Depth)
				Subjects->InsertNode(Subject,new GSubject(Subjects->GetNbNodes(),cat,false));
			ParseDir(Files()->GetURI(),cat);
		}
		else
		{
			// Must be a normal document
			GSubject* Subject=Subjects->GetNode(parent);
			GDoc* doc=new GDoc(Files()->GetURI(),Files()->GetURI(),cNoRef,0,DefaultMIME,cNoRef,RDate::Null,RDate::GetToday(),RDate::Null,0,0);
			GALILEIApp->GetSession()->InsertDoc(doc);
			if(Subject)
				Subject->Insert(doc);
		}
	}
	CurDepth--;
}


//-----------------------------------------------------------------------------
void QCreateDocXML::DoIt(void)
{
	Parent->setLabelText("Creating XML Structure ...");
	bool Native;
	if(!GALILEIApp->GetSession()->GetDocXML(Doc,XML,Native))
		throw GException("XML file could not be created");
}


//-----------------------------------------------------------------------------
void QAnalyzeXML::DoIt(void)
{
	GALILEIApp->GetSession()->AnalyseDoc(Doc,true,0,Parent);
}


//-----------------------------------------------------------------------------
void QAnalyzeDocs::DoIt(void)
{
	Parent->setLabelText("Analyze Documents ...");
	GALILEIApp->GetSession()->AnalyseDocs(false,Parent);
}


//-----------------------------------------------------------------------------
void QPostAnalyzeDocs::DoIt(void)
{
	Parent->setLabelText("Do Post-Documents Methods ...");
	GALILEIApp->GetSession()->DoPostDocs(Parent);
}


//-----------------------------------------------------------------------------
void QComputeProfiles::DoIt(void)
{
	Parent->setLabelText("Compute Profiles ...");
	GALILEIApp->GetSession()->CalcProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QPostComputeProfiles::DoIt(void)
{
	Parent->setLabelText("Do Post-Profiles Methods ...");
	GALILEIApp->GetSession()->DoPostProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QComputeProfile::DoIt(void)
{
	Parent->setLabelText("Compute Profile ...");
	GALILEIApp->GetSession()->CalcProfile(Profile,Parent);
}


//-----------------------------------------------------------------------------
void QGroupProfiles::DoIt(void)
{
	Parent->setLabelText("Groups Profiles ...");
	GALILEIApp->GetSession()->GroupProfiles(Parent);
}


//-----------------------------------------------------------------------------
void QPostGroupProfiles::DoIt(void)
{
	Parent->setLabelText("Do Post-Community Methods ...");
	GALILEIApp->GetSession()->DoPostCommunity(Parent);
}


//-----------------------------------------------------------------------------
void QGroupDocs::DoIt(void)
{
	Parent->setLabelText("Groups Documents ...");
	GALILEIApp->GetSession()->GroupDocs(Parent);
}


//-----------------------------------------------------------------------------
void QPostGroupDocs::DoIt(void)
{
	Parent->setLabelText("Do Post-Topic Methods ...");
	GALILEIApp->GetSession()->DoPostTopic(Parent);
}


//-----------------------------------------------------------------------------
void QCreateIdealSubjects::DoIt(void)
{
	Parent->setLabelText("Start a Simulation ...");
	GALILEIApp->GetSession()->GetSubjects()->Apply();
	GALILEIApp->GetSession()->GetSubjects()->StartSimulation();
}


//-----------------------------------------------------------------------------
void QCreateIdealCommunities::DoIt(void)
{
	Parent->setLabelText("Create Ideal Communities ...");
	GALILEIApp->GetSession()->BuildGroupsFromIdeal(otCommunity);
}


//-----------------------------------------------------------------------------
void QCreateIdealTopics::DoIt(void)
{
	Parent->setLabelText("Create Ideal Topics ...");
	GALILEIApp->GetSession()->BuildGroupsFromIdeal(otTopic);
}


//-----------------------------------------------------------------------------
void QMakeFdbks::DoIt(void)
{
	Parent->setLabelText("Make feedbacks ...");
	GALILEIApp->GetSession()->GetSubjects()->Apply();
	GALILEIApp->GetSession()->GetSubjects()->DocumentSharing();
}


//-----------------------------------------------------------------------------
void QMakeAssessments::DoIt(void)
{
	Parent->setLabelText("Make assessments ...");
	GALILEIApp->GetSession()->GetSubjects()->Apply();
	GALILEIApp->GetSession()->GetSubjects()->AddAssessments();
}


//-----------------------------------------------------------------------------
void QComputeAll::DoIt(void)
{
	Parent->setLabelText("Analyse Documents ...");
	GALILEIApp->GetSession()->AnalyseDocs(Parent);
	if(GSession::Break())
		return;
	Parent->setLabelText("Compute Profiles ...");
	GALILEIApp->GetSession()->CalcProfiles(Parent);
	if(GSession::Break())
		return;
	if(GSession::Break())
		return;
	Parent->setLabelText("Groups Profiles ...");
	GALILEIApp->GetSession()->GroupProfiles(Parent);
}



//-----------------------------------------------------------------------------
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg(QWidget* parent,const QString& c,bool cancel)
    : KProgressDialog(parent,c,""), GSlot(), Running(false)
{
	showCancelButton(cancel);
	setAllowCancel(cancel);
	setAutoClose(false);
	progressBar()->setVisible(false);
	adjustSize();
}


//-----------------------------------------------------------------------------
bool QSessionProgressDlg::Run(QSessionThread* task)
{
	Running=true;
	task->Set(this);
	task->start();
	const int res = KProgressDialog::exec();
	if(GSession::Break())
		GSession::ResetBreak();
	delete task;
	return(res);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang)
{
	setLabelText(QString("Groups Profiles for '")+ToQString(lang->GetName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextDoc(const GDoc* doc)
{
	setLabelText(QString("Analyze Doc '")+ToQString(doc->GetName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextProfile(const GProfile* prof)
{
	setLabelText(QString("Analyze Profile '")+ToQString(prof->GetName())+"' of User '"+ToQString(prof->GetUser()->GetFullName())+"' ...");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextChromosome(size_t id)
{
	setLabelText(QString("Analyze Chromosome ")+QString::number(id)+" ...");
}



//-----------------------------------------------------------------------------
void QSessionProgressDlg::reject(void)
{
	if(Running)
		GSession::SetBreak();
	else
	{
		if(GSession::Break())
			done(0);
		else
			done(1);
	}
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
