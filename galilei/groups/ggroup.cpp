/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

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

#include <stdlib.h>
//include files for GALILEI
#include<ggroup.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------
//
//  GGroup
//
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
GGroup::GGroup(GGroups *owner) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(0),Owner(owner)
{
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
GGroup::GGroup(GGroups *owner,unsigned int id) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(id),Owner(owner)
{
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/

int GGroup::Compare(const unsigned int &id)
{
	return(Id-id);
}

//-----------------------------------------------------------------------------

int GGroup::Compare(const GGroup &group)
{
	return(Id-group.Id);
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
int GGroup::Compare(const GGroup *group)
{
	return(Id-group->Id);
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
bool GGroup::CanAttach(GSubProfile *sub)
{
	for(Start();!End();Next())
		if((*this)()->Similar(sub))
			return(true);
	return(false);
}



