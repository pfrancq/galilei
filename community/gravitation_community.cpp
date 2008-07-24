/*

	GALILEI Research Project

	GCommunityCalcGravitation.cpp

	Community Description is Gravitational Point Computing Method - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
GCommunityCalcGravitation::GCommunityCalcGravitation(GFactoryCommunityCalc* fac)
	: GCommunityCalc(fac), Infos(5000,2500), MaxNonZero(100), Order(0), Vector(5000), MaxOrderSize(5000)
{
	Order=new GWeightInfo*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::ApplyConfig(void)
{
	MaxNonZero=Factory->GetUInt("Max Size");
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::Connect(GSession* session)
{
	GCommunityCalc::Connect(session);
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::Disconnect(GSession* session)
{
	GCommunityCalc::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::Compute(GCommunity* grp)
{
	size_t i;
	GWeightInfo* ins;
	GWeightInfo** w;

	// Clear the Vector.
	Vector.Clear();
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	// If no sprofiles -> No relevant one.
	if(!grp->GetNbObjs()) return;

	// Go through the sprofiles and sum the weights.
	RCursor<GProfile> Prof(grp->GetObjs());
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		// Go trough the words of the current profile
		RCursor<GWeightInfo> Cur(Prof()->GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			ins=Vector.GetInsertPtr(*Cur());
			(*ins)+=Cur()->GetWeight();
		}
	}

	// Copy the information of the relevant profile to the community.
	if(Vector.GetNb()+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<size_t>((Vector.GetNb()+1)*1.1);
		Order=new GWeightInfo*[MaxOrderSize];
	}
	Vector.GetTab(Order);
	if(Vector.GetNb())
		qsort(static_cast<void*>(Order),Vector.GetNb(),sizeof(GWeightInfo*),GWeightInfos::sortOrder);
	Order[Vector.GetNb()]=0;
	if(MaxNonZero)
	{
		for(i=MaxNonZero+1,w=Order;(--i)&&(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Infos.InsertPtr(new GWeightInfo((*w)->GetConcept(),(*w)->GetWeight()/grp->GetNbObjs()));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Infos.InsertPtr(new GWeightInfo((*w)->GetConcept(),(*w)->GetWeight()/grp->GetNbObjs()));
		}
	}

	// Update the community.
	grp->Update(&Infos,true);
}


//------------------------------------------------------------------------------
void GCommunityCalcGravitation::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Max Size",60));
}


//-----------------------------------------------------------------------------
GCommunityCalcGravitation::~GCommunityCalcGravitation(void)
{
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_COMMUNITYCALC_FACTORY("Gravitation Method for Communities",GCommunityCalcGravitation)
