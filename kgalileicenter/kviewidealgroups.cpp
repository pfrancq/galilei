/*

	GALILEI Research Project

	KViewThTopics.cpp

	Window to manipulate theoretical topics - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#include <gsubjects.h>


//------------------------------------------------------------------------------
// include files for current application
#include <kviewidealgroups.h>
#include <kgalileicenter.h>



//-----------------------------------------------------------------------------
//
// class KViewIdealGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup>
	KViewIdealGroups<cGroup>::KViewIdealGroups(void)
		: QMdiSubWindow(), Ui_KViewIdealGroups()
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
KViewIdealTopics::KViewIdealTopics(void)
	: KViewIdealGroups<GTopic>()
{
	connect(Ideal,SIGNAL(Show(GDoc*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showDoc(GDoc*)));
	connect(Computed,SIGNAL(Show(GDoc*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showDoc(GDoc*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(topicsChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewIdealTopics::update(void)
{
	GMeasure* Compare(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Topics Evaluation"));
	if(!Compare)
		throw GException("'Topics Evaluation' is not a valid evaluation measure");
	double precision,recall,total;
	Compare->Info(0,&recall);
	Compare->Info(1,&precision);
	Compare->Info(2,&total);
	setWindowTitle("Clustering Comparison: "
	               "Precision="+QString::number(precision)+
	               " - Recall="+QString::number(recall)+
	               " - Total="+QString::number(total));
	Ideal->Set(QGObjectsList::IdealTopics);
	Computed->Set(QGObjectsList::CompTopics);
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
KViewIdealCommunities::KViewIdealCommunities(void)
	: KViewIdealGroups<GCommunity>()
{
	connect(Ideal,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
	connect(Computed,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(communitiesChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewIdealCommunities::update(void)
{
	GMeasure* Compare(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Communities Evaluation"));
	if(!Compare)
		throw GException("'Communities Evaluation' is not a valid evaluation measure");
	double precision,recall,total;
	Compare->Info(0,&recall);
	Compare->Info(1,&precision);
	Compare->Info(2,&total);
	setWindowTitle("Clustering Comparison: "
	               "Precision="+QString::number(precision)+
	               " - Recall="+QString::number(recall)+
	               " - Total="+QString::number(total));
	Ideal->Set(QGObjectsList::IdealCommunities);
	Computed->Set(QGObjectsList::CompCommunities);
}


//-----------------------------------------------------------------------------
KViewIdealCommunities::~KViewIdealCommunities(void)
{
}
