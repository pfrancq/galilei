/*

	GALILEI Research Project

	GGroupingRandom.cpp

	Random Heuristic for grouping Subprofiles - Implementation

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
//include files for GALILEI
#include<groups/ggroupingrandom.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GGroupingRandom
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingRandom::GGroupingRandom(GSession* s) throw(bad_alloc)
	: GGrouping("Random Heuristic",s), Random(12345)
{
}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingRandom::GetSettings(void)
{
	return("");
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingRandom::SetSettings(const char*)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingRandom::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingRandom::IsValid(GGroup*)
{
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingRandom::Run(void) throw(GException)
{
	GSubProfile* s;
	GGroup* g;
	unsigned int idx;

	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
	{
		s=SubProfiles();
		if(s->GetGroup()) continue;  // If already grouped -> next one.

		// Choose randomly a group index
		idx=static_cast<unsigned int>((Groups->NbPtr+1)*Random.Value());

		// Find the group.
		if(idx<Groups->NbPtr)
			g=Groups->Tab[idx];
		else
			g=NewGroup(Groups->GetLang());

		// Attach the subprofile to the group found/created.
		g->InsertSubProfile(s);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingRandom::~GGroupingRandom(void)
{
}
