/*

	GALILEI Research Project

	GGrouping.cpp

	Generic Grouping Method - Implementation

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggrouping.h>
#include <glang.h>
#include <ggroupcalc.h>
#include <ggroup.h>
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
//  GGrouping
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGrouping::GGrouping(GFactoryGrouping* fac)
	: GPlugin<GFactoryGrouping>(fac), Profiles(100,50)
{
}


//-----------------------------------------------------------------------------
void GGrouping::Grouping(GSlot*,bool save)
{
	// How to compute the groups
	GGroupCalc* CalcDesc=GALILEIApp->GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod();
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
		R::RCursor<GGroup> Groups(Session->GetGroups());
		for(Groups.Start();!Groups.End();Groups.Next())
			CalcDesc->Compute(Groups());
	}

	// Save if necessary
	if(save)
	{
		Session->GetStorage()->SaveGroups();
		R::RCursor<GGroup> Groups(Session->GetGroups());
		for(Groups.Start();!Groups.End();Groups.Next())
			Groups()->SetState(osSaved);
	}	
}


//------------------------------------------------------------------------------
GGrouping::~GGrouping(void)
{
}



//------------------------------------------------------------------------------
//
// class GGroupingManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupingManager::GGroupingManager(void)
	: GPluginManager<GGroupingManager,GFactoryGrouping,GGrouping>("Grouping",API_GROUPING_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
GGroupingManager::~GGroupingManager(void)
{
}
