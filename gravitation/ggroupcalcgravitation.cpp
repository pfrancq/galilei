/*

	GALILEI Research Project

	GGroupCalcGravitation.cpp

	Group Description is Gravitational Point Computing Method - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ggroupcalcgravitation.h>
#include <groups/ggroupvector.h>
#include <sessions/gsession.h>
#include <profiles/gsubprofilevector.h>
#include <infos/ginfo.h>
#include <infos/gweightinfo.h>
#include <infos/gweightinfos.h>
#include <sessions/gobjref.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class GGroupCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupCalcGravitation::GGroupCalcGravitation(GFactoryGroupCalc* fac) throw(bad_alloc)
	: GGroupCalc(fac), MaxNonZero(100), Order(0), Vector(0), MaxOrderSize(5000)
{
	Order=new GWeightInfo*[MaxOrderSize];
	Vector=new GWeightInfos(MaxNonZero);
}


//-----------------------------------------------------------------------------
void GGroupCalcGravitation::ApplyConfig(void)
{
	MaxNonZero=Factory->GetUInt("Max Size");
}


//-----------------------------------------------------------------------------
void GGroupCalcGravitation::Connect(GSession* session) throw(GException)
{
	GGroupCalc::Connect(session);
}


//-----------------------------------------------------------------------------
void GGroupCalcGravitation::Disconnect(GSession* session) throw(GException)
{
	GGroupCalc::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GGroupCalcGravitation::Compute(GGroupRef grp) throw(GException)
{
	unsigned int i,j;
	GWeightInfos* Ref;
	GWeightInfo** w;
	GWeightInfo* ins;
	GSubProfileCursor Sub;

	// Clear the Vector.
	Group=dynamic_cast<GGroupVector*>(Session->GetGroup(grp.GetId()));
	Group->RemoveRefs();
	Group->Clear();
	Vector->Clear();

	// If no subprofiles -> No relevant one.
	if(!Group->GetNbSubProfiles()) return;

	// Go through the subprofiles and sum the weigths.
	Sub=Group->GetSubProfilesCursor();
	for(Sub.Start();!Sub.End();Sub.Next())
	{
		// Go trough the words of the current subprofile
		Ref=static_cast<GSubProfileVector*>(Sub());
		for(j=Ref->NbPtr+1,w=Ref->Tab;--j;w++)
		{
			ins=Vector->GetInsertPtr<unsigned int>((*w)->GetId());
			(*ins)+=(*w)->GetWeight();
		}
	}

	// Copy the information of the relevant subprofile to the group.
	if(Vector->NbPtr+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((Vector->NbPtr+1)*1.1);
		Order=new GWeightInfo*[MaxOrderSize];
	}
	memcpy(Order,Vector->Tab,Vector->NbPtr*sizeof(GWeightInfo*));
	if(Vector->NbPtr)
		qsort(static_cast<void*>(Order),Vector->NbPtr,sizeof(GWeightInfo*),GWeightInfos::sortOrder);
	Order[Vector->NbPtr]=0;
	if(MaxNonZero)
	{
		for(i=MaxNonZero+1,w=Order;(--i)&&(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Group->InsertPtr(new GWeightInfo((*w)->GetId(),(*w)->GetWeight()/Group->GetNbSubProfiles()));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Group->InsertPtr(new GWeightInfo((*w)->GetId(),(*w)->GetWeight()/Group->GetNbSubProfiles()));
		}
	}

	// Update the references of the vector.
	Group->UpdateRefs();
}


//------------------------------------------------------------------------------
void GGroupCalcGravitation::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamUInt("Max Size",60));
}


//-----------------------------------------------------------------------------
GGroupCalcGravitation::~GGroupCalcGravitation(void)
{
	if(Order) delete[] Order;
	if(Vector) delete Vector;
}


//------------------------------------------------------------------------------
CREATE_GROUPCALC_FACTORY("Gravitation Method",GGroupCalcGravitation)
