/*

	GALILEI Research Project

	GTopicCalcGravitation.cpp

	Group Description is Gravitational Point Computing Method - Implementation.

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
#include <gtopic.h>
#include <gsession.h>
#include <gdoc.h>
#include <gweightinfo.h>
#include <gweightinfos.h>


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
GTopicCalcGravitation::GTopicCalcGravitation(GFactoryTopicCalc* fac)
	: GTopicCalc(fac), Infos(5000,2500), MaxNonZero(100), Order(0), Vector(5000), MaxOrderSize(5000)
{
	Order=new GWeightInfo*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::ApplyConfig(void)
{
	MaxNonZero=Factory->GetUInt("Max Size");
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::Connect(GSession* session)
{
	GTopicCalc::Connect(session);
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::Disconnect(GSession* session)
{
	GTopicCalc::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::Compute(GTopic* grp)
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

	// If no documents -> No relevant one.
	if(!grp->GetNbObjs()) return;

	// Go through the documents and sum the weights.
	RCursor<GDoc> Prof(grp->GetObjs());
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		// Go trough the words of the current document
		RCursor<GWeightInfo> Cur(Prof()->GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			ins=Vector.GetInsertPtr(*Cur());
			(*ins)+=Cur()->GetWeight();
		}
	}

	// Copy the information of the relevant document to the topic.
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

	// Update the topic.
	grp->Update(&Infos,true);
}


//------------------------------------------------------------------------------
void GTopicCalcGravitation::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Max Size",60));
}


//-----------------------------------------------------------------------------
GTopicCalcGravitation::~GTopicCalcGravitation(void)
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
CREATE_TOPICCALC_FACTORY("Gravitation Method for Topics",GTopicCalcGravitation)
