/*

	GALILEI Research Project

	GEvaluateGrouping.cpp

	Compare a ideal groupement with a computed one - Implementation

	(C) 2002 by David Wartel

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
#include<groups/gevaluategrouping.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gsubprofile.h>
#include<sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GEvaluateGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGrouping::GEvaluateGrouping(RString name, GSession* s,RContainer<GGroups,unsigned int,true,true>* groups, unsigned int i) throw(bad_alloc)
	: Name(name), Id(i), Session(s), Groups(groups), SubProfiles(100,20)
{
	// fill the subprofiles Container
	for (Groups->Start(); !Groups->End(); Groups->Next())
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroup* grp=(*(*Groups)())() ;
			for (grp->Start(); !grp->End(); grp->Next())
				SubProfiles.InsertPtr((*grp)());
		}
}


//-----------------------------------------------------------------------------
GALILEI::GEvaluateGrouping::~GEvaluateGrouping(void)
{

}
