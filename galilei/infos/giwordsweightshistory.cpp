/*

	GALILEI Research Project

	GIWordsWeightsHistory.cpp

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
#include <infos/giwordsweightshistory.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GGIWordsWeightsHistory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordsWeightsHistory::GIWordsWeightsHistory(GSubProfile* sub, unsigned int nb)
	: GIWordsWeights(nb)
{
	SubProfile=sub;
	WellGrouped=NewSubProfile=false;
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GIWordsWeightsHistory::GetSubProfile(void)
{
	return SubProfile;
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GIWordsWeightsHistory::GetId(void)
{
	return SubProfile->GetId();
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeightsHistory::SetParent(GGroupHistory* p)
{
	Parent=p;
}


//-----------------------------------------------------------------------------
bool GALILEI::GIWordsWeightsHistory::IsWellGrouped(void)
{
	if (WellGrouped) return true;
	 else return false;
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeightsHistory::SetWellGrouped(bool b)
{
	WellGrouped=b;
}


//-----------------------------------------------------------------------------
bool GALILEI::GIWordsWeightsHistory::IsNewSubProfile(void)
{
	return(NewSubProfile==true);
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeightsHistory::SetNewSubProfile(bool b)
{
	NewSubProfile=b;
}


//-----------------------------------------------------------------------------
GGroupHistory* GALILEI::GIWordsWeightsHistory::GetParent(void)
{
	return Parent;
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordsWeightsHistory::Compare(const GIWordsWeightsHistory& giwwh) const
{
	return(SubProfile->GetId()-giwwh.SubProfile->GetId());
}


//-----------------------------------------------------------------------------
 int GALILEI::GIWordsWeightsHistory::Compare(const GIWordsWeightsHistory* giwwh) const
 {
	 return(SubProfile->GetId()-giwwh->SubProfile->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordsWeightsHistory::Compare(unsigned int id) const
{
	return(SubProfile->GetId()-id);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordsWeightsHistory::~GIWordsWeightsHistory(void)
{
}

