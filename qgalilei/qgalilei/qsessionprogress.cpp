/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <QProgressBar>
#include <QProgressDialog>


//-----------------------------------------------------------------------------
// include files for current project
#include <qsessionprogress.h>
#include <qgalilei.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// QSessionProgress
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgress::QSessionProgress(QGALILEIWin* win,const QString& title)
	: Win(win), Title(title)
{
}


//-----------------------------------------------------------------------------
bool QSessionProgress::execute(QSessionProgress& task)
{
	task.OK=false;
	QProgressDialog Dialog(task.Win);
	Dialog.setWindowTitle(task.Title);
	Dialog.setAutoClose(false);
	Dialog.setAutoReset(false);
	Dialog.setWindowModality(Qt::WindowModal);
	QProgressBar* Bar(new QProgressBar(&Dialog));
	Dialog.setBar(Bar);
	Bar->setVisible(false);
	Dialog.setMinimumDuration(0);
	Dialog.setLabelText("Start "+task.Title);
	Dialog.show();
	Dialog.raise();
	Dialog.activateWindow();
	connect(&task,SIGNAL(setLabelText(const QString&)),&Dialog,SLOT(setLabelText(const QString&)));
	connect(&task,SIGNAL(setCancelButtonText(const QString&)),&Dialog,SLOT(setCancelButtonText(const QString&)));
	task.run();
	Dialog.exec();
	return(task.OK);
}


//-----------------------------------------------------------------------------
void QSessionProgress::run(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	try
	{
		DoIt();
		emit setLabelText("Finish");
		emit setCancelButtonText("OK");
		OK=true;
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(GException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		emit setCancelButtonText("Error");
		emit setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		emit setCancelButtonText("Error");
		emit setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		emit setCancelButtonText("Error");
		emit setLabelText(QWidget::trUtf8(e.what()));
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		emit setCancelButtonText("Error");
		emit setLabelText(QWidget::trUtf8("Unknown"));
	}
	QApplication::processEvents();
	//OK=OK&&(!wasCanceled());
	//exec();
	//return(OK);
}


//------------------------------------------------------------------------------
void QSessionProgress::StartJob(const R::RString& job)
{
	WriteStr(job);
}


//------------------------------------------------------------------------------
void QSessionProgress::EndJob(const R::RString& msg)
{
	WriteStr(msg);
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextGroupLang(const GLang* lang)
{
	if(lang)
		WriteStr("Treat "+lang->GetLangName()+" language");
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextConceptType(const GConceptType* type)
{
	if(type)
		WriteStr("Treat concept type '"+type->GetName()+"'");
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextDoc(const GDoc* doc)
{
	if(doc)
	{
		RString DocName;
		if(doc->GetTitle().GetLen()>80)
			DocName=doc->GetTitle().Mid(0,20)+"..."+doc->GetTitle().Mid(doc->GetTitle().GetLen()-57,57);
		else
		{
			if(doc->GetName().GetLen())
				DocName=doc->GetTitle();
			else
			{
				if(doc->GetName().GetLen()>80)
					DocName=doc->GetName().Mid(0,20)+"..."+doc->GetName().Mid(doc->GetName().GetLen()-57,57);
				else
					DocName=doc->GetName();
			}
		}
		WriteStr("Treat document '"+DocName+"'");
	}
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextProfile(const GProfile* prof)
{
	WriteStr("Treat profile '"+prof->GetName()+"' ("+prof->GetUser()->GetFullName()+")");
}


//------------------------------------------------------------------------------
void QSessionProgress::Warning(const R::RString&)
{
	//KMessageBox::information(0,ToQString(msg),"GALILEI Warning");
}


//------------------------------------------------------------------------------
void QSessionProgress::Error(const R::RString& msg)
{
	mThrowGException(msg);
}


//------------------------------------------------------------------------------
void QSessionProgress::Alert(const R::RString&)
{
	//KMessageBox::information(0,ToQString(msg),"GALILEI Alert");
}


//------------------------------------------------------------------------------
void QSessionProgress::WriteStr(const R::RString& str)
{
	// No clue why I have to make the call two times to correctly update the dialog box
	emit setLabelText(ToQString(str));
	QApplication::processEvents();
	emit setLabelText(ToQString(str));
	QApplication::processEvents();
}


//------------------------------------------------------------------------------
void QSessionProgress::Interact(void)
{
	qApp->processEvents();
}



//-----------------------------------------------------------------------------
//
// Tasks
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void QCreateSession::DoIt(void)
{
	WriteStr("Load Concepts, Predicates and Statements ...");
	bool Create(true);
	Session=GALILEIApp->GetSession(Name,Create,this);
	if(Win->getSession()->MustBreak())
		return;
	WriteStr("Load Classes ...");
	Win->getSession()->LoadObjs(pClass);
	WriteStr("Load Topics ...");
	Win->getSession()->LoadObjs(pTopic);
	if(Win->getSession()->MustBreak())
		return;
	WriteStr("Loading Documents ...");
	Win->getSession()->LoadObjs(pDoc);
	if(Win->getSession()->MustBreak())
		return;
	WriteStr("Load Communities ...");
	Win->getSession()->LoadObjs(pCommunity);
	if(Win->getSession()->MustBreak())
		return;
	WriteStr("Load Users/Profiles/Feedbacks ...");
	Win->getSession()->LoadObjs(pUser);
	if(Win->getSession()->MustBreak())
		return;
	WriteStr("Load Subjects ...");
	Win->getSession()->LoadObjs(pSubject);
}


//-----------------------------------------------------------------------------
void QAnalyzeDoc::DoIt(void)
{
	WriteStr("Analze '"+Doc->GetName()+"'...");
	Win->getSession()->AnalyzeDoc(Doc,this);
}


//-----------------------------------------------------------------------------
void QIndexDocs::DoIt(void)
{
	WriteStr("Index Documents ...");
	Win->getSession()->BuildIndex(static_cast<const GDoc*>(0));
}


//-----------------------------------------------------------------------------
void QAnalyzeDocs::DoIt(void)
{
	WriteStr("Analyze Documents ...");
	Win->getSession()->AnalyzeDocs(this);
}


//-----------------------------------------------------------------------------
void QComputeProfiles::DoIt(void)
{
	WriteStr("Compute Profiles ...");
	Win->getSession()->CalcProfiles(this);
}


//-----------------------------------------------------------------------------
void QComputeProfile::DoIt(void)
{
	WriteStr("Compute Profile ...");
	Win->getSession()->CalcProfile(Profile,0,0,this);
}


//-----------------------------------------------------------------------------
void QGroupProfiles::DoIt(void)
{
	WriteStr("Groups Profiles ...");
	Win->getSession()->GroupProfiles(this);
}


//-----------------------------------------------------------------------------
void QGroupDocs::DoIt(void)
{
	WriteStr("Groups Documents ...");
	Win->getSession()->GroupDocs(this);
}


//-----------------------------------------------------------------------------
void QInitSimulation::DoIt(void)
{
	WriteStr("Start a Simulation ...");
	Win->getSession()->GetSimulator()->StartSimulation();
}


//-----------------------------------------------------------------------------
void QCreateIdealCommunities::DoIt(void)
{
	WriteStr("Create Ideal Communities ...");
	Win->getSession()->GetSimulator()->BuildIdealCommunities();
}


//-----------------------------------------------------------------------------
void QCreateIdealTopics::DoIt(void)
{
	WriteStr("Create Ideal Topics ...");
	Win->getSession()->GetSimulator()->BuildIdealTopics();
}


//-----------------------------------------------------------------------------
void QCreateIdealTopicsClasses::DoIt(void)
{
	WriteStr("Create Ideal Topics using classes ...");
	Win->getSession()->GetSimulator()->BuildIdealLeafTopics();
}


//-----------------------------------------------------------------------------
void QCreateIdealClasses::DoIt(void)
{
	WriteStr("Create Ideal Classes ...");
	Win->getSession()->GetSimulator()->BuildIdealClasses();
}


//-----------------------------------------------------------------------------
void QCreateIdealClassesDocs::DoIt(void)
{
	WriteStr("Create Ideal Classes using documents ...");
	Win->getSession()->GetSimulator()->BuildIdealDocsClasses();
}


//-----------------------------------------------------------------------------
void QRepairSubjects::DoIt(void)
{
	WriteStr("Repair Subjects ...");
	Win->getSession()->Repair();
}


//-----------------------------------------------------------------------------
void QFeedbackCycle::DoIt(void)
{
	WriteStr("Make feedbacks ...");
	Win->getSession()->GetSimulator()->ShareDocuments();
}


//-----------------------------------------------------------------------------
void QAssessmentCycle::DoIt(void)
{
	WriteStr("Make assessments ...");
	Win->getSession()->GetSimulator()->AddAssessments();
}


//-----------------------------------------------------------------------------
void QComputeTrust::DoIt(void)
{
	WriteStr("Determine trust ...");
	Win->getSession()->ComputeTrust();
}


//-----------------------------------------------------------------------------
void QComputeSugs::DoIt(void)
{
	WriteStr("Make suggestions ...");
	Win->getSession()->ComputeSugs();
}


//-----------------------------------------------------------------------------
void QRunTool::DoIt(void)
{
	WriteStr("Run tool '"+Tool+"' of '"+List+"'...");
	Win->getSession()->RunTool(Tool,List,this);
}


//-----------------------------------------------------------------------------
void QComputeAll::DoIt(void)
{
	WriteStr("Analyze Documents ...");
	Win->getSession()->AnalyzeDocs(this);
/*	if(wasCanceled())
		return;*/
	WriteStr("Compute Profiles ...");
	Win->getSession()->CalcProfiles(this);
/*	if(wasCanceled())
		return;*/
	WriteStr("Groups Profiles ...");
	Win->getSession()->GroupProfiles(this);
}
