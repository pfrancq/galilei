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
	: QProgressDialog(win), Win(win)
{
	setWindowTitle(title);
	setAutoClose(false);
	setAutoReset(false);
	setWindowModality(Qt::WindowModal);
	setMinimumSize(380,0);
}


//-----------------------------------------------------------------------------
bool QSessionProgress::run(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	setValue(0);
	show();
	QApplication::processEvents();
	bool OK(false);
	try
	{
		DoIt();
		setLabelText("Finish");
		setCancelButtonText("OK");
		setValue(maximum());
		OK=true;
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(GException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		setCancelButtonText("Error");
		setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		setCancelButtonText("Error");
		setLabelText(QWidget::trUtf8(e.GetMsg()));
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		setCancelButtonText("Error");
		setLabelText(QWidget::trUtf8(e.what()));
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		setCancelButtonText("Error");
		setLabelText(QWidget::trUtf8("Unknown"));
	}
	OK=OK&&(!wasCanceled());
	exec();
	return(OK);
}


//------------------------------------------------------------------------------
void QSessionProgress::StartJob(const R::RString& job)
{
	setLabelText(ToQString(job));
}


//------------------------------------------------------------------------------
void QSessionProgress::EndJob(const R::RString& msg)
{
	setLabelText(ToQString(msg));
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextGroupLang(const GLang* lang)
{
	if(lang)
		setLabelText("Treat "+ToQString(lang->GetLangName())+" language");
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextConceptType(const GConceptType* type)
{
	if(type)
		setLabelText("Treat concept type '"+ToQString(type->GetName())+"'");
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextDoc(const GDoc* doc)
{
	if(doc)
	{
		QString DocName;
		if(doc->GetTitle().GetLen()>80)
			DocName=ToQString(doc->GetTitle().Mid(0,20))+"..."+ToQString(doc->GetTitle().Mid(doc->GetTitle().GetLen()-57,57));
		else
		{
			if(doc->GetName().GetLen())
				DocName=ToQString(doc->GetTitle());
			else
			{
				if(doc->GetName().GetLen()>80)
					DocName=ToQString(doc->GetName().Mid(0,20))+"..."+ToQString(doc->GetName().Mid(doc->GetName().GetLen()-57,57));
				else
					DocName=ToQString(doc->GetName());
			}
		}
		setLabelText("Treat document '"+DocName+"'");
	}
}


//-----------------------------------------------------------------------------
void QSessionProgress::NextProfile(const GProfile* prof)
{
	if(prof)
		setLabelText("Treat profile '"+ToQString(prof->GetName())+"' ("+ToQString(prof->GetUser()->GetFullName())+")");
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
	setLabelText(ToQString(str));
}


//------------------------------------------------------------------------------
void QSessionProgress::Interact(void)
{
	QCoreApplication::processEvents();
}



//-----------------------------------------------------------------------------
//
// Tasks
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void QCreateSession::DoIt(void)
{
	setRange(0,7);
	setValue(0);
	setLabelText("Load Concepts, Predicates and Statements ...");
	Session=GALILEIApp->GetSession(Name,true);
	setValue(1);
	if(Win->getSession()->MustBreak())
		return;
	setLabelText("Load Classes ...");
	Win->getSession()->LoadObjs(pClass);
	setValue(2);
	setLabelText("Load Topics ...");
	Win->getSession()->LoadObjs(pTopic);
	setValue(3);
	if(Win->getSession()->MustBreak())
		return;
	setLabelText("Loading Documents ...");
	Win->getSession()->LoadObjs(pDoc);
	setValue(4);
	if(Win->getSession()->MustBreak())
		return;
	setLabelText("Load Communities ...");
	Win->getSession()->LoadObjs(pCommunity);
	setValue(5);
	if(Win->getSession()->MustBreak())
		return;
	setLabelText("Load Users/Profiles/Feedbacks ...");
	Win->getSession()->LoadObjs(pUser);
	setValue(6);
	if(Win->getSession()->MustBreak())
		return;
	setLabelText("Load Subjects ...");
	Win->getSession()->LoadObjs(pSubject);
}


//-----------------------------------------------------------------------------
void QAnalyzeDoc::DoIt(void)
{
	setLabelText("Analze '"+ToQString(Doc->GetName())+"'...");
	Win->getSession()->AnalyzeDoc(Doc,this);
}


//-----------------------------------------------------------------------------
void QIndexDocs::DoIt(void)
{
	setLabelText("Index Documents ...");
	Win->getSession()->BuildIndex(static_cast<const GDoc*>(0));
}


//-----------------------------------------------------------------------------
void QAnalyzeDocs::DoIt(void)
{
	setLabelText("Analyze Documents ...");
	Win->getSession()->AnalyzeDocs(this);
}


//-----------------------------------------------------------------------------
void QComputeProfiles::DoIt(void)
{
	setLabelText("Compute Profiles ...");
	Win->getSession()->CalcProfiles(this);
}


//-----------------------------------------------------------------------------
void QComputeProfile::DoIt(void)
{
	setLabelText("Compute Profile ...");
	Win->getSession()->CalcProfile(Profile,0,0,this);
}


//-----------------------------------------------------------------------------
void QGroupProfiles::DoIt(void)
{
	setLabelText("Groups Profiles ...");
	Win->getSession()->GroupProfiles(this);
}


//-----------------------------------------------------------------------------
void QGroupDocs::DoIt(void)
{
	setLabelText("Groups Documents ...");
	Win->getSession()->GroupDocs(this);
}


//-----------------------------------------------------------------------------
void QInitSimulation::DoIt(void)
{
	setLabelText("Start a Simulation ...");
	Win->getSession()->GetSimulator()->StartSimulation();
}


//-----------------------------------------------------------------------------
void QCreateIdealCommunities::DoIt(void)
{
	setLabelText("Create Ideal Communities ...");
	Win->getSession()->GetSimulator()->BuildIdealCommunities();
}


//-----------------------------------------------------------------------------
void QCreateIdealTopics::DoIt(void)
{
	setLabelText("Create Ideal Topics ...");
	Win->getSession()->GetSimulator()->BuildIdealTopics();
}


//-----------------------------------------------------------------------------
void QCreateIdealTopicsClasses::DoIt(void)
{
	setLabelText("Create Ideal Topics using classes ...");
	Win->getSession()->GetSimulator()->BuildIdealLeafTopics();
}


//-----------------------------------------------------------------------------
void QCreateIdealClasses::DoIt(void)
{
	setLabelText("Create Ideal Classes ...");
	Win->getSession()->GetSimulator()->BuildIdealClasses();
}


//-----------------------------------------------------------------------------
void QCreateIdealClassesDocs::DoIt(void)
{
	setLabelText("Create Ideal Classes using documents ...");
	Win->getSession()->GetSimulator()->BuildIdealDocsClasses();
}


//-----------------------------------------------------------------------------
void QRepairSubjects::DoIt(void)
{
	setLabelText("Repair Subjects ...");
	Win->getSession()->Repair();
}


//-----------------------------------------------------------------------------
void QFeedbackCycle::DoIt(void)
{
	setLabelText("Make feedbacks ...");
	Win->getSession()->GetSimulator()->ShareDocuments();
}


//-----------------------------------------------------------------------------
void QAssessmentCycle::DoIt(void)
{
	setLabelText("Make assessments ...");
	Win->getSession()->GetSimulator()->AddAssessments();
}


//-----------------------------------------------------------------------------
void QComputeTrust::DoIt(void)
{
	setLabelText("Determine trust ...");
	Win->getSession()->ComputeTrust();
}


//-----------------------------------------------------------------------------
void QComputeSugs::DoIt(void)
{
	setLabelText("Make suggestions ...");
	Win->getSession()->ComputeSugs();
}


//-----------------------------------------------------------------------------
void QRunTool::DoIt(void)
{
	setLabelText("Run tool '"+ToQString(Tool)+"' of '"+ToQString(List)+"'...");
	Win->getSession()->RunTool(Tool,List,this);
}


//-----------------------------------------------------------------------------
void QComputeAll::DoIt(void)
{
	setLabelText("Analyze Documents ...");
	Win->getSession()->AnalyzeDocs(this);
	if(wasCanceled())
		return;
	setLabelText("Compute Profiles ...");
	Win->getSession()->CalcProfiles(this);
	if(wasCanceled())
		return;
	setLabelText("Groups Profiles ...");
	Win->getSession()->GroupProfiles(this);
}

