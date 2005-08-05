/*

	GALILEI Research Project

	GGroupCalcGravitation.cpp

	Group Description is Gravitational Point Computing Method - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#include <ggroup.h>
#include <gsession.h>
#include <gsubprofile.h>
#include <ginfo.h>
#include <gweightinfo.h>
#include <gweightinfos.h>
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
	: GGroupCalc(fac), Infos(5000,2500), MaxNonZero(100), Order(0), Vector(5000), MaxOrderSize(5000)
{
	Order=new GWeightInfo*[MaxOrderSize];
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
void GGroupCalcGravitation::Compute(GGroup* grp) throw(GException)
{
	unsigned int i;
	GWeightInfo* ins;
	GWeightInfo** w;
	RCursor<GSubProfile> Sub;

	// Clear the Vector.
	Vector.Clear();
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	// If no subprofiles -> No relevant one.
	if(!grp->GetNbSubProfiles()) return;

	// Go through the subprofiles and sum the weigths.
	Sub=grp->GetSubProfiles();
	for(Sub.Start();!Sub.End();Sub.Next())
	{
		// Go trough the words of the current subprofile
		RCursor<GWeightInfo> Cur(Sub()->GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			ins=Vector.GetInsertPtr<unsigned int>(Cur()->GetId());
			(*ins)+=Cur()->GetWeight();
		}
	}

	// Copy the information of the relevant subprofile to the group.
	if(Vector.GetNb()+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((Vector.GetNb()+1)*1.1);
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
				Infos.InsertPtr(new GWeightInfo((*w)->GetId(),(*w)->GetWeight()/grp->GetNbSubProfiles()));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Infos.InsertPtr(new GWeightInfo((*w)->GetId(),(*w)->GetWeight()/grp->GetNbSubProfiles()));
		}
	}

	// Update the group.
	grp->Update(grp->GetLang(),&Infos,true);
}


//------------------------------------------------------------------------------
void GGroupCalcGravitation::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamUInt("Max Size",60));
}


//-----------------------------------------------------------------------------
GGroupCalcGravitation::~GGroupCalcGravitation(void)
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
CREATE_GROUPCALC_FACTORY("Gravitation Method",GGroupCalcGravitation)
