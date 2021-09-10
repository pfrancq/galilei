/*

	GALILEI Research Project

	Gravitation_Community.cpp

	Community Description Computing Method - Implementation.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GCommunityCalc(session,fac), GDescriptionFilter(),
	  LMax(60), LMin(0), Method(0),
	  Profiles(session)
{
}


//------------------------------------------------------------------------------
void GCommunityCalcGravitation::Init(void)
{
	GCommunityCalc::Init();
	Weighting=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0);
	InsertObserver(HANDLER(GCommunityCalcGravitation::HandleCurrentPlugIn),hCurrentPlugIn,GALILEIApp->GetManager("Measures")->GetPlugInList("Features Evaluation"));
}


//------------------------------------------------------------------------------
void GCommunityCalcGravitation::HandleCurrentPlugIn(const R::RNotification& notification)
{
	Weighting=dynamic_cast<GMeasure*>(GetData<GPlugIn*>(notification));
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::ApplyConfig(void)
{
	LMax=FindParam<RParamValue>("LMax")->GetUInt();
	LMin=FindParam<RParamValue>("LMin")->GetUInt();
	RString MethodParam(FindParam<RParamValue>("Method")->Get());
	if(MethodParam=="Centroid Method")
		Method=1;
	else if(MethodParam=="Prototype Method")
		Method=2;
	else
		Method=0;
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::ComputeCentroid(const GCommunity* grp)
{
	// Clear the descriptions
	Description.Clear();
	Profiles.Clear();
	Internal.Clear();

	// If no profiles -> No relevant one
	if(!grp->GetNbObjs())
		return;

	// Go through the profiles and sum the description
	RCursor<GProfile> Prof(grp->GetObjs());
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		Internal+=(*Prof())();
		Profiles.InsertDescription(&(*Prof())());
	}

	// Multiply by the if factors and divided by the number of profiles
	//Internal.MultiplyIF(Profiles);
	Weighting->Measure(1,&Internal,otProfile);
	Internal/=Profiles.GetNb();

	// Compute the description
	CopyFilter(Internal,Description,LMax,LMin);
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::ComputePrototype(const GCommunity* grp)
{
	GProfile* Prototype(grp->RelevantObj());
	if(!Prototype)
		Description.Clear();
	else
		Description=(*Prototype)();
}


//-----------------------------------------------------------------------------
void GCommunityCalcGravitation::Compute(const GCommunity* grp)
{
	if(!Weighting)
		mThrowGException("No plug-in selected for \"Features Evaluation\"");

	switch(Method)
	{
		case 1: ComputeCentroid(grp); break;
		case 2: ComputePrototype(grp); break;
		default: mThrowGException("No valid community description computing method");
	}
}


//------------------------------------------------------------------------------
void GCommunityCalcGravitation::CreateConfig(void)
{
	InsertParam(new RParamValue("LMax",60));
	InsertParam(new RParamValue("LMin",0));
	InsertParam(new RParamValue("Method","Centroid Method"));
}


//-----------------------------------------------------------------------------
GCommunityCalcGravitation::~GCommunityCalcGravitation(void)
{
}


//------------------------------------------------------------------------------
CREATE_COMMUNITYCALC_FACTORY("Gravitation Method for Communities","Gravitation Method for Communities",GCommunityCalcGravitation)
