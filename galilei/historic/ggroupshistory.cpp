/*

	GALILEI Research Project

	GGroupsHistory.cpp

	History of Groups for a given language - Implementation.

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
// include files for GALILEI
#include <historic/ggroupshistory.h>
#include <historic/giwordsweightshistory.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGroupsHistory
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GGroupsHistory::GGroupsHistory(GLang* lang, unsigned int id)
	: RContainer<GGroupHistory,unsigned int,true,true>(20,10)
{
	Lang=lang;
	Id=id;
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupsHistory::GetId(void)
{
	return Id;
}


//-----------------------------------------------------------------------------
GIWordsWeightsHistory* GALILEI::GGroupsHistory::GetSubProfile(unsigned int id)
{
	GGroupHistory* grp;

	for (Start(); !End(); Next())
	{
		grp=(*this)();
		for (grp->Start(); !grp->End(); grp->Next())
		 	if ((*grp)()->GetId()==id)
		  		return((*grp)());
	}
	return(0);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(const GGroupsHistory& groups) const
{
	return(Id-groups.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(const GGroupsHistory* groups) const
{
	return(Id-groups->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupsHistory::~GGroupsHistory(void)
{
}


