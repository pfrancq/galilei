/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gdocanalyze.h>
#include <gdoc.h>
#include <guser.h>
#include <gsession.h>
#include <gprofile.h>
#include <gprofilecalc.h>
#include <ggroupprofiles.h>
#include <ggroup.h>
#include <gsubject.h>
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gfilter.h>
#include <gsimulator.h>
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



//-------------------------------------------------Finish----------------------------
// include files for KDE
/*#include <klocale.h>
#include <kapplication.h>
#include <kiconloader.h>
#include <kmessagebox.h>*/


//-----------------------------------------------------------------------------
// include files for current project
#include <qsessionprogress.h>
#include <qgalilei.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// class All session threads
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionThread::QSessionThread(QGALILEIWin* app)
	: /*QThread(),*/ Parent(0), App(app)
{
}

//-----------------------------------------------------------------------------
void QSessionThread::Set(QSessionProgressDlg* parent)
{
	//Parent=parent;
}


//-----------------------------------------------------------------------------
bool QSessionThread::run(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QProgressDialog progress(App);
	progress.setAutoClose(false);
	progress.setAutoReset(false);
	progress.setMinimumDuration(0);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumSize(380,0);
	progress.show();
	QApplication::processEvents();
	Parent=&progress;
	bool OK(false);
	try
	{
		DoIt();
		progress.setLabelText("Finish");
		progress.setCancelButtonText("OK");
		progress.setValue(Parent->maximum());
		OK=true;
	}
	catch(GException& e)
	{
		progress.setCancelButtonText("Error");
		progress.setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(RException& e)
	{
		progress.setCancelButtonText("Error");
		progress.setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(std::exception& e)
	{
		progress.setCancelButtonText("Error");
		progress.setLabelText(QWidget::trUtf8(e.what()));
	}
	catch(...)
	{
		progress.setCancelButtonText("Error");
		progress.setLabelText(QWidget::trUtf8("Unknown"));
	}
	QApplication::setOverrideCursor(Qt::ArrowCursor);
	OK=OK&&(!Parent->wasCanceled());
	progress.exec();
	return(OK);
}


//-----------------------------------------------------------------------------
void QCreateSession::DoIt(void)
{
	Parent->setWindowTitle("Connect to database '"+ToQString(Name)+"'");
	Parent->setRange(0,7);
	Parent->setValue(0);
	Parent->setLabelText("Load Concepts, Predicates and Statements ...");
	Session=GALILEIApp->GetSession(Name,true);
	Parent->setValue(1);
	if(App->getSession()->MustBreak())
		return;
	Parent->setLabelText("Load Classes ...");
	App->getSession()->LoadObjs(pClass);
	Parent->setValue(2);
	Parent->setLabelText("Load Topics ...");
	App->getSession()->LoadObjs(pTopic);
	Parent->setValue(3);
	if(App->getSession()->MustBreak())
		return;
	Parent->setLabelText("Loading Documents ...");
	App->getSession()->LoadObjs(pDoc);
	Parent->setValue(4);
	if(App->getSession()->MustBreak())
		return;
	Parent->setLabelText("Load Communities ...");
	App->getSession()->LoadObjs(pCommunity);
	Parent->setValue(5);
	if(App->getSession()->MustBreak())
		return;
	Parent->setLabelText("Load Users/Profiles/Feedbacks ...");
	App->getSession()->LoadObjs(pUser);
	Parent->setValue(2);
	if(App->getSession()->MustBreak())
		return;
	Parent->setLabelText("Load Subjects ...");
	App->getSession()->LoadObjs(pSubject);
	//Parent->setValue(7);
}


////-----------------------------------------------------------------------------
//void QAnalyzeDoc::DoIt(void)
//{
//	App->getSession()->AnalyzeDoc(Doc,Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QAnalyzeDocs::DoIt(void)
//{
//	Parent->setLabelText("Analyze Documents ...");
//	App->getSession()->AnalyzeDocs(Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QComputeProfiles::DoIt(void)
//{
//	Parent->setLabelText("Compute Profiles ...");
//	App->getSession()->CalcProfiles(Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QComputeProfile::DoIt(void)
//{
//	Parent->setLabelText("Compute Profile ...");
//	App->getSession()->CalcProfile(Profile,0,0,Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QGroupProfiles::DoIt(void)
//{
//	Parent->setLabelText("Groups Profiles ...");
//	App->getSession()->GroupProfiles(Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QGroupDocs::DoIt(void)
//{
//	Parent->setLabelText("Groups Documents ...");
//	App->getSession()->GroupDocs(Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealSubjects::DoIt(void)
//{
//	Parent->setLabelText("Start a Simulation ...");
//	App->getSession()->GetSimulator()->StartSimulation();
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealCommunities::DoIt(void)
//{
//	Parent->setLabelText("Create Ideal Communities ...");
//	App->getSession()->GetSimulator()->BuildIdealCommunities();
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealTopics::DoIt(void)
//{
//	Parent->setLabelText("Create Ideal Topics ...");
//	App->getSession()->GetSimulator()->BuildIdealTopics();
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealTopicsFromClasses::DoIt(void)
//{
//	Parent->setLabelText("Create Ideal Topics ...");
//	App->getSession()->GetSimulator()->BuildIdealLeafTopics();
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealClasses::DoIt(void)
//{
//	Parent->setLabelText("Create Ideal Classes ...");
//	App->getSession()->GetSimulator()->BuildIdealClasses();
//}
//
//
////-----------------------------------------------------------------------------
//void QCreateIdealDocsClasses::DoIt(void)
//{
//	Parent->setLabelText("Create Ideal Classes ...");
//	App->getSession()->GetSimulator()->BuildIdealDocsClasses();
//}
//
//
////-----------------------------------------------------------------------------
//void QRepairSubjects::DoIt(void)
//{
//	Parent->setLabelText("Repair Subjects ...");
//	App->getSession()->Repair();
//}
//
//
////-----------------------------------------------------------------------------
//void QMakeFdbks::DoIt(void)
//{
//	Parent->setLabelText("Make feedbacks ...");
//	App->getSession()->GetSimulator()->ShareDocuments();
//}
//
//
////-----------------------------------------------------------------------------
//void QMakeAssessments::DoIt(void)
//{
//	Parent->setLabelText("Make assessments ...");
//	App->getSession()->GetSimulator()->AddAssessments();
//}
//
//
////-----------------------------------------------------------------------------
//void QComputeTrust::DoIt(void)
//{
//	Parent->setLabelText("Determine trust ...");
//	App->getSession()->ComputeTrust();
//}
//
//
////-----------------------------------------------------------------------------
//void QComputeSugs::DoIt(void)
//{
//	Parent->setLabelText("Make suggestions ...");
//	App->getSession()->ComputeSugs();
//}
//
//
////-----------------------------------------------------------------------------
//void QRunTool::DoIt(void)
//{
//	Parent->setLabelText("Run tool '"+ToQString(Tool)+"' of '"+ToQString(List)+"'...");
//	App->getSession()->RunTool(Tool,List,Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QComputeAll::DoIt(void)
//{
//	Parent->setLabelText("Analyze Documents ...");
//	App->getSession()->AnalyzeDocs(Parent);
//	if(App->getSession()->MustBreak())
//		return;
//	Parent->setLabelText("Compute Profiles ...");
//	App->getSession()->CalcProfiles(Parent);
//	if(App->getSession()->MustBreak())
//		return;
//	Parent->setLabelText("Groups Profiles ...");
//	App->getSession()->GroupProfiles(Parent);
//}
//
//
////-----------------------------------------------------------------------------
//void QIndexDocs::DoIt(void)
//{
//	Parent->setLabelText("Index Documents ...");
//	App->getSession()->BuildIndex(static_cast<const GDoc*>(0));
//}



//-----------------------------------------------------------------------------
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg(QGALILEIWin* parent,const QString& c,bool cancel)
    : QProgressDialog(parent), GSlot(), Running(false), App(parent)
{
	setWindowTitle(c);
	setAutoReset(false);
	setAutoClose(false);
	if(!cancel)
		setCancelButtonText("Wait");
	setMinimumSize(380,0);
	adjustSize();
}


//-----------------------------------------------------------------------------
bool QSessionProgressDlg::Run(QSessionThread* task)
{
	EndMsg=QString::null;
	Running=true;
	if(task)
		task->Set(this);
	QEventLoop q;
	Loop=&q;
	setModal(true);
	connect(this, SIGNAL(accepted()), this, SLOT(closeMe()));
	connect(this, SIGNAL(rejected()), this, SLOT(closeMe()));
	connect(this, SIGNAL(finished(int)),&q, SLOT(quit()));

	if(task)
	{
//		connect(task,SIGNAL(setValue(int)),this,SLOT(setValue(int)),Qt::QueuedConnection);
//		connect(task,SIGNAL(setRange(int,int)),this,SLOT(setRange(int,int)),Qt::QueuedConnection);
//		connect(task,SIGNAL(finish()),this,SLOT(Finish()),Qt::QueuedConnection);
//		connect(task,SIGNAL(setLabel(const QString&)),this,SLOT(setLabelText(const QString&)),Qt::QueuedConnection);
	}
	Ret=false; // Suppose something goes wrong
	show();
//	if(task)
//		task->start();
	q.exec();
	if(App->getSession()&&App->getSession()->MustBreak())
		App->getSession()->ResetBreak();
	if(task)
		delete task;
	cout<<"OK 2"<<endl;
	return(Ret);
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::StartJob(const R::RString& job)
{
	setLabelText(ToQString(job));
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::EndJob(const R::RString& msg)
{
	EndMsg=ToQString(msg);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang)
{
	if(lang)
		setLabelText("Treat "+ToQString(lang->GetLangName())+" language");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextConceptType(const GConceptType* type)
{
	if(type)
		setLabelText("Treat concept type '"+ToQString(type->GetName())+"'");
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextDoc(const GDoc* doc)
{
	if(doc)
	{
		QString DocName;
		if(doc->GetName().GetLen()>80)
			DocName=ToQString(doc->GetName().Mid(0,20))+"..."+ToQString(doc->GetName().Mid(doc->GetName().GetLen()-57,57));
		else
		{
			if(doc->GetName().GetLen())
				DocName=ToQString(doc->GetName());
			else
			{
				if(doc->GetURI()().GetLen()>80)
					DocName=ToQString(doc->GetURI()().Mid(0,20))+"..."+ToQString(doc->GetURI()().Mid(doc->GetURI()().GetLen()-57,57));
				else
					DocName=ToQString(doc->GetURI()());
			}
		}
		setLabelText("Treat document '"+DocName+"'");
	}
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextProfile(const GProfile* prof)
{
	if(prof)
		setLabelText("Treat profile '"+ToQString(prof->GetName())+"' ("+ToQString(prof->GetUser()->GetFullName())+")");
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::Warning(const R::RString&)
{
	//KMessageBox::information(0,ToQString(msg),"GALILEI Warning");
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::Error(const R::RString& msg)
{
	mThrowGException(msg);
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::Alert(const R::RString&)
{
	//KMessageBox::information(0,ToQString(msg),"GALILEI Alert");
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::WriteStr(const R::RString& str)
{
	setLabelText(ToQString(str));
}


//------------------------------------------------------------------------------
void QSessionProgressDlg::Interact(void)
{
	QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::reject(void)
{
	if(Running)
		App->getSession()->SetBreak();
	else
	{
		if(App->getSession()&&App->getSession()->MustBreak())
			done(0);
		else
			done(1);
	}
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::closeMe(void)
{
	Loop->quit();
	cout<<"OK"<<endl;
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Finish(void)
{
	Ret=true;
	cout<<"Finish"<<endl;
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
