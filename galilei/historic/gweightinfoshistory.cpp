/*

	GALILEI Research Project

	GWeightInfosHistory.cpp

	Weights of a list of words - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <historic/gweightinfoshistory.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGWeightInfosHistory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWeightInfosHistory::GWeightInfosHistory(GSubProfile* sub, unsigned int nb)
	: GWeightInfos(nb)
{
	SubProfile=sub;
	WellGrouped=NewSubProfile=false;
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GWeightInfosHistory::GetSubProfile(void)
{
	return SubProfile;
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GWeightInfosHistory::GetId(void)
{
	return SubProfile->GetId();
}


//-----------------------------------------------------------------------------
void GALILEI::GWeightInfosHistory::SetParent(GGroupHistory* p)
{
	Parent=p;
}


//-----------------------------------------------------------------------------
bool GALILEI::GWeightInfosHistory::IsWellGrouped(void)
{
	if (WellGrouped) return true;
	 else return false;
}


//-----------------------------------------------------------------------------
void GALILEI::GWeightInfosHistory::SetWellGrouped(bool b)
{
	WellGrouped=b;
}


//-----------------------------------------------------------------------------
bool GALILEI::GWeightInfosHistory::IsNewSubProfile(void)
{
	return(NewSubProfile==true);
}


//-----------------------------------------------------------------------------
void GALILEI::GWeightInfosHistory::SetNewSubProfile(bool b)
{
	NewSubProfile=b;
}


//-----------------------------------------------------------------------------
GGroupHistory* GALILEI::GWeightInfosHistory::GetParent(void)
{
	return Parent;
}


//-----------------------------------------------------------------------------
int GALILEI::GWeightInfosHistory::Compare(const GWeightInfosHistory& giwwh) const
{
	return(SubProfile->GetId()-giwwh.SubProfile->GetId());
}


//-----------------------------------------------------------------------------
 int GALILEI::GWeightInfosHistory::Compare(const GWeightInfosHistory* giwwh) const
 {
	 return(SubProfile->GetId()-giwwh->SubProfile->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GWeightInfosHistory::Compare(unsigned int id) const
{
	return(SubProfile->GetId()-id);
}


//-----------------------------------------------------------------------------
GALILEI::GWeightInfosHistory::~GWeightInfosHistory(void)
{
}

