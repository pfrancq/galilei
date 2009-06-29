/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <gindexer.h>
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
void QCreateSession::DoIt(void)
{
	Parent->setLabelText("Load Concepts, Predicates and Statements ...");
	Session=GALILEIApp->CreateSession();
	if(GSession::Break())
		return;
	Parent->setLabelText("Load Classes ...");
	GALILEIApp->GetSession()->LoadClasses();
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
void QIndexDocs::DoIt(void)
{
	Parent->setLabelText("Index Documents ...");
	GALILEIApp->GetSession()->BuildRefs(otDoc,Parent);
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
	QEventLoop q;
	setModal(true);
	connect(this, SIGNAL(accepted()), &q, SLOT(quit()));
	connect(this, SIGNAL(rejected()), &q, SLOT(quit()));
	connect(task, SIGNAL(finish()), this, SLOT(Finish()));
	Ret=false; // Suppose something goes wrong
	show();
	task->start();
	q.exec();
	if(GSession::Break())
		GSession::ResetBreak();
	delete task;
	//return(res);
	return(Ret);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang)
{
	if(lang)
		setLabelText("Treat "+ToQString(lang->GetName())+" language");
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
				if(doc->GetURL()().GetLen()>80)
					DocName=ToQString(doc->GetURL()().Mid(0,20))+"..."+ToQString(doc->GetURL()().Mid(doc->GetURL()().GetLen()-57,57));
				else
					DocName=ToQString(doc->GetURL()());
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
void QSessionProgressDlg::Finish(void)
{
	Ret=true;
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
