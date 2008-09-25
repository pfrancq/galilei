/*

	Genetic Community Algorithm

	GCA.cpp

	Main - Implementation.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// includes files for GCA
#include <gca.h>
#include <gcaobj.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// GCAParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAParams::GCAParams(void)
{
}



//-----------------------------------------------------------------------------
//
// GCAMaxRatio
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
int GCAMaxRatio::sortOrder(const void* a,const void* b)
{
	double af=(*((GCAMaxRatio**)(a)))->Ratio;
	double bf=(*((GCAMaxRatio**)(b)))->Ratio;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//-----------------------------------------------------------------------------
//
// GCAMaxRatios
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
GCAMaxRatios::GCAMaxRatios(size_t max)
	: RContainer<GCAMaxRatio,true,false>(max>4?max/4:4)
{
}



//-----------------------------------------------------------------------------
//
// class CGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CGroup::CGroup(CGroups* owner,size_t id)
	: R::RGroup<CGroup,GCAObj,CGroups>(owner,id)
{
}



//-----------------------------------------------------------------------------
//
// class CGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CGroups::CGroups(R::RCursor<GCAObj> objs,size_t max)
	: R::RGroups<CGroup,GCAObj,CGroups>(objs,max)
{
}
