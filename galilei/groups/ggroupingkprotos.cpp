/*

	GALILEI Research Project

	GGroupingKProtos.cpp

	Heuristic using Similarity - Implementation

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include specific files
#include <stdlib.h>
#include <time.h>

//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroupingkprotos.h>
#include <groups/ggroup.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------
//
//  GGroupingKProtos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::GGroupingKProtos(GSession* s) throw(bad_alloc)
	: GGrouping("KProtos",s), Random(true), FindGroupsNumber(false), GroupsNumber(13),
	  ProtosNumber(1), IterNumber(10), NbTests(1), MaxNbGroups(13), MinNbGroups(13)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKProtos::GetSettings(void)
{
	return("");
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::SetSettings(const char* /*s*/)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Run(void) throw(GException)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingKProtos::IsValid(GGroup* /*grp*/)
{
	return(true);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::~GGroupingKProtos(void)
{
}
