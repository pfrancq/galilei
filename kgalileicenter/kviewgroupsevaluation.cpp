/*

	GALILEI Research Project

	KViewGroupsEvaluation.cpp

	Window to manipulate theoritical groups - Implementation.

	(C) 2001 by David Wartel

	Version $Revision$

	Last Modify: $Date$

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
// include files for GALILEI
#include <sessions/gsession.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <groups/ggrouping.h>
#include <groups/gevaluategrouping.h>
#include <groups/gevaluategroupingcalinski.h>
#include <groups/gevaluategroupingvariance.h>
#include <groups/gevaluategroupingintramininter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewgroupsevaluation.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
//
// class KViewGroupsEvaluation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGroupsEvaluation::KViewGroupsEvaluation(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	methods= new GEvaluateGroupingMethods();

	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/kmultiple.png"));
	setCaption("Clustering Evaluation");

	evaluation = new QListView(this);
	evaluation->resize(size());
	evaluation->addColumn("Criterion");
	evaluation->addColumn("Value");

	RegisterEvaluationMethods();
	Run();
}


//-----------------------------------------------------------------------------
void KViewGroupsEvaluation::RegisterEvaluationMethods(void)
{
	RContainer<GGroups,unsigned int,true,true>* groups;
	groups= new RContainer<GGroups,unsigned int,true,true> (2,2);
	GSession* session=Doc->GetSession();
	GGroupsCursor& Cur=session->GetGroupsCursor();
	
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		groups->InsertPtr((Cur)());
	}

	//register the methods
//	methods->RegisterEvaluationMethod(new GEvaluateGroupingCalinski(session, groups));
//	methods->RegisterEvaluationMethod(new GEvaluateGroupingVariance(session, groups));
//	methods->RegisterEvaluationMethod(new GEvaluateGroupingIntraMinInter(session, groups));
	
}



//-----------------------------------------------------------------------------
void KViewGroupsEvaluation::Run(void)
{
	GEvaluateGroupingCursor& cur= methods->GetEvaluateGroupingCursor();
	for (cur.Start(); !cur.End(); cur.Next())
		evaluation->insertItem(new QListViewItem (evaluation, QString(cur()->GetName()), QString(dtoa(cur()->Run()))));

}


//-----------------------------------------------------------------------------
void KViewGroupsEvaluation::update(unsigned int cmd)
{
	if(cmd) return;
}



//-----------------------------------------------------------------------------
void KViewGroupsEvaluation::resizeEvent(QResizeEvent *)
{
	evaluation->resize(size());
}


//-----------------------------------------------------------------------------
KViewGroupsEvaluation::~KViewGroupsEvaluation(void)
{
}
