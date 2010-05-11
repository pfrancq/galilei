/*

	GALILEI Research Project

	Gravitation_Community.cpp

	Community Description Computing Method - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gcommunity.h>
#include <gsession.h>
#include <gprofile.h>
#include <gweightinfo.h>
#include <gweightinfos.h>


//-----------------------------------------------------------------------------
//include files for current project
#include "gravitation_community.h"
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class GCommunityCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCommunityCalcGravitation::GCommunityCalcGravitation(GSession* session,GPlugInFactory* fac)
	: GCommunityCalc(session,fac), MaxNonZero(100), Order(0), Vector(5000), MaxOrderSize(5000)
{
	Order=new const GWeightInfo*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::ApplyConfig(void)
{
	MaxNonZero=FindParam<RParamValue>("Max Size")->GetUInt();
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::Compute(const GCommunity* grp)
{
	size_t i;
	GWeightInfo* ins;
	const GWeightInfo** w;

	// Clear the Vectors.
	Vector.Clear();
	Infos.Clear();

	// If no profiles -> No relevant one.
	if(!grp->GetNbObjs()) return;

	// Go through the profiles and sum the weights.
	RCursor<GProfile> Prof(grp->GetObjs());
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		// Go trough the words of the current profile
		RCursor<GWeightInfo> Cur(Prof()->GetVector().GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			ins=Vector.GetInfo(Cur());
			(*ins)+=Cur()->GetWeight();
		}
	}

	// Copy the information of the relevant profile to the community.
	if(Vector.GetNb()+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<size_t>((static_cast<double>(Vector.GetNb())+1)*1.1);
		Order=new const GWeightInfo*[MaxOrderSize];
	}
	Vector.GetTab(Order);
	if(Vector.GetNb())
		qsort(static_cast<void*>(Order),Vector.GetNb(),sizeof(GWeightInfo*),GWeightInfos::SortOrder);
	Order[Vector.GetNb()]=0;
	if(MaxNonZero)
	{
		for(i=MaxNonZero+1,w=Order;(--i)&&(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Infos.InsertInfo(new GWeightInfo((*w)->GetConcept(),(*w)->GetWeight()/static_cast<double>(grp->GetNbObjs())));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Infos.InsertInfo(new GWeightInfo((*w)->GetConcept(),(*w)->GetWeight()/static_cast<double>(grp->GetNbObjs())));
		}
	}
}


//------------------------------------------------------------------------------
void GCommunityCalcGravitation::CreateConfig(void)
{
	InsertParam(new RParamValue("Max Size",60));
}


//-----------------------------------------------------------------------------
GCommunityCalcGravitation::~GCommunityCalcGravitation(void)
{
	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_COMMUNITYCALC_FACTORY("Gravitation Method for Communities","Gravitation Method for Communities",GCommunityCalcGravitation)
