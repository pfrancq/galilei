/*

	GALILEI Research Project

	KViewThTopics.cpp

	Window to manipulate theoretical topics - Implementation.

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
// include files for R/GALILEI Projects
#include <ggalileiapp.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QProgressDialog>


//------------------------------------------------------------------------------
// include files for current application
#include <kviewidealgroups.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// class KViewIdealGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup>
	KViewIdealGroups<cGroup>::KViewIdealGroups(QGALILEIWin* win)
		: QMdiSubWindow(), Ui_KViewIdealGroups(), Win(win)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
}



//-----------------------------------------------------------------------------
//
// class KViewIdealTopics
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
KViewIdealTopics::KViewIdealTopics(QGALILEIWin* win)
	: KViewIdealGroups<GTopic>(win)
{
	connect(Ideal,SIGNAL(Show(GDoc*)),Win,SLOT(showDoc(GDoc*)));
	connect(Computed,SIGNAL(Show(GDoc*)),Win,SLOT(showDoc(GDoc*)));
	connect(Win,SIGNAL(topicsChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewIdealTopics::update(void)
{
	GMeasure* Compare(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Topics Evaluation"));
	double precision,recall,total;
	Compare->Info(0,&recall);
	Compare->Info(1,&precision);
	Compare->Info(2,&total);
	setWindowTitle("Topics Comparison: "
	               "Precision="+QString::number(precision)+
	               " - Recall="+QString::number(recall)+
	               " - Total="+QString::number(total));
	Ideal->Set(Win->getSession(),QGObjectsList::IdealTopics);
	Computed->Set(Win->getSession(),QGObjectsList::CompTopics);
}


//-----------------------------------------------------------------------------
KViewIdealTopics::~KViewIdealTopics(void)
{
}



//-----------------------------------------------------------------------------
//
// class KViewIdealCommunities
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
KViewIdealCommunities::KViewIdealCommunities(QGALILEIWin* win)
	: KViewIdealGroups<GCommunity>(win)
{
	connect(Ideal,SIGNAL(Show(GProfile*)),Win,SLOT(showProfile(GProfile*)));
	connect(Computed,SIGNAL(Show(GProfile*)),Win,SLOT(showProfile(GProfile*)));
	connect(Win,SIGNAL(communitiesChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewIdealCommunities::update(void)
{
	GMeasure* Compare(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Communities Evaluation"));
	double precision,recall,total;
	Compare->Info(0,&recall);
	Compare->Info(1,&precision);
	Compare->Info(2,&total);
	setWindowTitle("Communities Comparison: "
	               "Precision="+QString::number(precision)+
	               " - Recall="+QString::number(recall)+
	               " - Total="+QString::number(total));
	Ideal->Set(Win->getSession(),QGObjectsList::IdealCommunities);
	Computed->Set(Win->getSession(),QGObjectsList::CompCommunities);
}


//-----------------------------------------------------------------------------
KViewIdealCommunities::~KViewIdealCommunities(void)
{
}



//-----------------------------------------------------------------------------
//
// class KViewIdealClasses
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewIdealClasses::KViewIdealClasses(QGALILEIWin* win)
	: QMdiSubWindow(), Ui_KViewIdealGroups(), Win(win)
{
	// Set up the widget
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);

	// Connect the window and update it
	connect(Ideal,SIGNAL(Show(GClass*)),Win,SLOT(showClass(GClass*)));
	connect(Computed,SIGNAL(Show(GClass*)),Win,SLOT(showClass(GClass*)));
	connect(Win,SIGNAL(classesChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewIdealClasses::update(void)
{
	GMeasure* Compare(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Classes Evaluation"));
	QProgressDialog Progress("Classes Evaluation...", "Abort Evaluation", 0,3, this);
	Progress.setMinimumDuration(0);
	Progress.setWindowModality(Qt::WindowModal);
	double treerank;
	Progress.setValue(0);
	QCoreApplication::processEvents();
	Computed->Set(Win->getSession(),QGObjectsList::Classes);
	if(Progress.wasCanceled())
		return;
	Progress.setValue(1);
	QCoreApplication::processEvents();
	Ideal->Set(Win->getSession(),QGObjectsList::Subjects);
	Progress.setValue(2);
	QCoreApplication::processEvents();
	if(Progress.wasCanceled())
		return;
	Compare->Info(0,&treerank);
	setWindowTitle("Classes Comparison: TreeRank="+QString::number(treerank));
	Progress.setValue(3);
}
