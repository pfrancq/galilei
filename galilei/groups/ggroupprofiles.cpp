/*

	GALILEI Research Project

	GGroupProfiles.cpp

	Generic Profiles Grouping Method - Implementation

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroupprofiles.h>
#include <glang.h>
#include <gcommunitycalc.h>
#include <gcommunity.h>
#include <gprofile.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <gslot.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// defines
//#define GROUP_PROFILES_NOT_DEFINED



//------------------------------------------------------------------------------
//
//  GGroupProfiles
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupProfiles::GGroupProfiles(GFactoryGroupProfiles* fac)
	: GPlugin<GFactoryGroupProfiles>(fac), Profiles(100,50)
{
}


//-----------------------------------------------------------------------------
void GGroupProfiles::Grouping(GSlot*,bool save)
{
	// How to compute the groups
	GCommunityCalc* CalcDesc=GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc")->GetCurrentMethod();
	Profiles.Clear();
	RCursor<GProfile> cur(Session->GetProfiles());
	for(cur.Start();!cur.End();cur.Next())
	{
		#ifdef GROUP_PROFILES_NOT_DEFINED
		if(cur()->IsDefined()||cur()->GetNbJudgedDocs())
			Profiles.InsertPtr(cur());
		#else
		if(cur()->IsDefined())
			Profiles.InsertPtr(cur());
		#endif
	}

	// Make the grouping
	Run();

	// Compute the description of the groups and Save the information.
	if(CalcDesc)
	{
		R::RCursor<GCommunity> Groups(Session->GetCommunities());
		for(Groups.Start();!Groups.End();Groups.Next())
			CalcDesc->Compute(Groups());
	}

	// Save if necessary
	if(save)
	{
		Session->GetStorage()->SaveCommunities();
		R::RCursor<GCommunity> Groups(Session->GetCommunities());
		for(Groups.Start();!Groups.End();Groups.Next())
			Groups()->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
GGroupProfiles::~GGroupProfiles(void)
{
}



//------------------------------------------------------------------------------
//
// class GGroupProfilesManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupProfilesManager::GGroupProfilesManager(void)
	: GPluginManager<GGroupProfilesManager,GFactoryGroupProfiles,GGroupProfiles>("GroupProfiles",API_GROUPPROFILES_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
GGroupProfilesManager::~GGroupProfilesManager(void)
{
}
