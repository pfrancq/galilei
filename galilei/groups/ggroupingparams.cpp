/*

	GALILEI Research Project

	GGroupingParams.cpp

	Generic Parameters for a Grouping Method - Implementation

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <groups/ggroupingparams.h>
#include <groups/ggrouping.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupingParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingParams::GGroupingParams(const char* n) throw(bad_alloc)
	: GroupingName(n)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupingParams::Compare(const GGroupingParams* grp) const
{
	return(GroupingName.Compare(grp->GroupingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupingParams::Compare(const GGroupingParams& grp) const
{
	return(GroupingName.Compare(grp.GroupingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupingParams::Compare(const GGrouping* grp) const
{
	return(GroupingName.Compare(grp->GetGroupingName()));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupingParams::Compare(const char* name) const
{
	return(GroupingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingParams::~GGroupingParams(void)
{
}
