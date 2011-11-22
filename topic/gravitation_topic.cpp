/*

	GALILEI Research Project

	Gravitation_Topic.cpp

	Topic Description Computing Method - Implementation.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gtopic.h>
#include <gsession.h>
#include <gdoc.h>
#include <gconceptref.h>


//-----------------------------------------------------------------------------
//include files for current project
#include "gravitation_topic.h"
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class GTopicCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTopicCalcGravitation::GTopicCalcGravitation(GSession* session,GPlugInFactory* fac)
	: GTopicCalc(session,fac), MaxNonZero(100), Order(0), MaxOrderSize(5000), Internal(20)
{
	Order=new const GConceptRef*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::ApplyConfig(void)
{
	MaxNonZero=FindParam<RParamValue>("Max Size")->GetUInt();
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::Compute(const GTopic* grp)
{
	size_t i;
	GConceptRef* ins;
	const GConceptRef** w;

	// Clear the Vectors.
	Vectors.Clear();
	Internal.Clear();

	// If no documents -> No relevant one.
	if(!grp->GetNbObjs()) return;

	// Go through the documents and sum the weights.
	RCursor<GDoc> Doc(grp->GetObjs());
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		// Go to the vectors of each profile
		RCursor<GVector> Vector(Doc()->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			GVector* Ins(Internal.GetInsertPtr(Vector()->GetConcept()));

			// Go trough the concepts of the current vector
			RCursor<GConceptRef> Cur(Vector()->GetRefs());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				ins=Ins->GetRef(Cur()->GetConcept());
				(*ins)+=Cur()->GetWeight();
			}
		}
	}

	// Copy the information of the relevant profile to the community.
	RCursor<GVector> Vector(Internal);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		if(Vector()->GetNb()+1>MaxOrderSize)
		{
			if(Order) delete[] Order;
			MaxOrderSize=static_cast<size_t>((static_cast<double>(Vector.GetNb())+1)*1.1);
			Order=new const GConceptRef*[MaxOrderSize];
		}
		Vector()->GetTab(Order);
		if(Vector()->GetNb())
			qsort(static_cast<void*>(Order),Vector()->GetNb(),sizeof(GConceptRef*),GVector::SortOrder);
		Order[Vector()->GetNb()]=0;
		GVector* Ins(Vectors.GetInsertPtr(Vector()->GetConcept()));
		if(MaxNonZero)
		{
			for(i=MaxNonZero+1,w=Order;(--i)&&(*w);w++)
			{
				if((*w)->GetWeight()>0)
					Ins->InsertRef(new GConceptRef((*w)->GetConcept(),(*w)->GetWeight()*(*w)->GetConcept()->GetIF(otDoc)/static_cast<double>(grp->GetNbObjs())));
			}
		}
		else
		{
			for(w=Order;(*w);w++)
			{
				if((*w)->GetWeight()>0)
					Ins->InsertRef(new GConceptRef((*w)->GetConcept(),(*w)->GetWeight()*(*w)->GetConcept()->GetIF(otDoc)/static_cast<double>(grp->GetNbObjs())));
			}
		}
	}
}


//------------------------------------------------------------------------------
void GTopicCalcGravitation::CreateConfig(void)
{
	InsertParam(new RParamValue("Max Size",60));
}


//-----------------------------------------------------------------------------
GTopicCalcGravitation::~GTopicCalcGravitation(void)
{
	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_TOPICCALC_FACTORY("Gravitation Method for Topics","Gravitation Method for Topics",GTopicCalcGravitation)
