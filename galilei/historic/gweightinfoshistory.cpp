/*

	GALILEI Research Project

	GWeightInfosHistory.cpp

	List of weighted information entities for history  - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <historic/gweightinfoshistory.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GGWeightInfosHistory
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfosHistory::GWeightInfosHistory(GSubProfile* sub, unsigned int max) throw(std::bad_alloc)
	: GWeightInfos(max)
{
	SubProfile=sub;
	WellGrouped=NewSubProfile=false;
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(const GWeightInfosHistory& giwwh) const
{
	return(SubProfile->GetId()-giwwh.SubProfile->GetId());
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(const GWeightInfosHistory* giwwh) const
{
	 return(SubProfile->GetId()-giwwh->SubProfile->GetId());
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(unsigned int id) const
{
	return(SubProfile->GetId()-id);
}


//------------------------------------------------------------------------------
unsigned int GWeightInfosHistory::GetId(void) const
{
	return(SubProfile->GetId());
}


//------------------------------------------------------------------------------
void GWeightInfosHistory::SetParent(GGroupHistory* p)
{
	Parent=p;
}


//------------------------------------------------------------------------------
void GWeightInfosHistory::SetWellGrouped(bool b)
{
	WellGrouped=b;
}


//------------------------------------------------------------------------------
void GWeightInfosHistory::SetNewSubProfile(bool b)
{
	NewSubProfile=b;
}


//------------------------------------------------------------------------------
GWeightInfosHistory::~GWeightInfosHistory(void)
{
}
