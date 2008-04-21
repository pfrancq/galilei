/*

	GALILEI Research Project

	GWeightInfosHistory.cpp

	List of weighted information entities for history  - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		David Wartel (dwartel@ulb.ac.be).

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
#include <gweightinfoshistory.h>
#include <gprofile.h>
#include <gweightinfo.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GGWeightInfosHistory
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfosHistory::GWeightInfosHistory(GProfile* prof, unsigned int max)
	: GWeightInfos(max), Profile(prof), WellGrouped(false), NewSubProfile(false) 
{	
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(const GWeightInfosHistory& giwwh) const
{
	return(Profile->GetId()-giwwh.Profile->GetId());
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(const GWeightInfosHistory* giwwh) const
{
	 return(Profile->GetId()-giwwh->Profile->GetId());
}


//------------------------------------------------------------------------------
int GWeightInfosHistory::Compare(unsigned int id) const
{
	return(Profile->GetId()-id);
}


//------------------------------------------------------------------------------
unsigned int GWeightInfosHistory::GetId(void) const
{
	return(Profile->GetId());
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
void GWeightInfosHistory::SetNewProfile(bool b)
{
	NewSubProfile=b;
}


//------------------------------------------------------------------------------
GWeightInfosHistory::~GWeightInfosHistory(void)
{
}
