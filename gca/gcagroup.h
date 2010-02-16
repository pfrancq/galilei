/*

	GALILEI Research Project

	GCAGroup.h

	Group - Header.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GCAGroupH
#define GCAGroupH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupsc.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
* The RGroupBP provides a representation for a group for the IR Problem.
* @author Pascal Francq
* @short IR Group.
*/
class GCAGroup : public R::RGroupSC<GCAGroup,GCAObj,GCAChromo>
{
public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GCAGroup(GCAGroup* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	GCAGroup(GCAChromo* owner,const size_t id);

	/**
	* Destruct the group.
	*/
	virtual ~GCAGroup(void);
};


//-----------------------------------------------------------------------------
#endif
