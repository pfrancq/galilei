/*

	GALILEI Research Project

	GGroupCalcGravitation.cpp

	Group Description is Gravitational Point Computing Method - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroupcalcgravitation.h>
#include <groups/ggroupvector.h>
#include <sessions/gsession.h>
#include <profiles/gsubprofilevector.h>
#include <infos/giword.h>
#include <infos/giwordlist.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GGroupCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupCalcGravitation::GGroupCalcGravitation(GSession* session) throw(bad_alloc)
	: GGroupCalc("Gravitational Point",session), MaxNonZero(60), Order(0),
	  MaxOrderSize(5000), Vector(0)
{
	Order=new GIWordWeight*[MaxOrderSize];
	Vector=new GIWordsWeights(60);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcGravitation::Compute(GGroup* grp)
{
	unsigned int i,j;
	GSubProfile** ptr;
	GIWordsWeights* Desc=static_cast<GGroupVector*>(grp)->GetVector();
	GIWordsWeights* Ref;
	GIWordWeight** w;
	GIWordWeight* ins;

	// Clear the Vector.
	(static_cast<GGroupVector*>(grp))->RemoveRefs();
	Desc->Clear();
	Vector->Clear();

	// If no subprofiles -> No relevant one.
	if(!grp->NbPtr) return;

	// Go through the subprofiles and sum the weigths.
	for(i=grp->NbPtr+1,ptr=grp->Tab;--i;ptr++)
	{
		// Go trough the words of the current subprofile
		Ref=static_cast<GSubProfileVector*>(*ptr)->GetVector();
		for(j=Ref->NbPtr+1,w=Ref->Tab;--j;w++)
		{
			ins=Vector->GetInsertPtr<unsigned int>((*w)->GetId());
			ins->AddWeight((*w)->GetWeight());
		}
	}

	// Copy the information of the relevant subprofile to the group.
	if(Vector->NbPtr+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((Vector->NbPtr+1)*1.1);
		Order=new GIWordWeight*[MaxOrderSize];
	}
	memcpy(Order,Vector->Tab,Vector->NbPtr*sizeof(GIWordWeight*));
	qsort(static_cast<void*>(Order),Vector->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
	Order[Vector->NbPtr]=0;
	if(MaxNonZero)
	{
		for(i=MaxNonZero+1,w=Order;(--i)&&(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight((*w)->GetId(),(*w)->GetWeight()/grp->NbPtr));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight((*w)->GetId(),(*w)->GetWeight()/grp->NbPtr));
		}
	}

	// Update the references of the vector.
	(static_cast<GGroupVector*>(grp))->UpdateRefs();
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupCalcGravitation::GetSettings(void)
{
	static char tmp[300];

	sprintf(tmp,"%u",MaxNonZero);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcGravitation::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u",&MaxNonZero);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalcGravitation::~GGroupCalcGravitation(void)
{
	if(Order) delete[] Order;
	if(Vector) delete Vector;
}
