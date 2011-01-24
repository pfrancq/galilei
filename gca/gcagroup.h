/*

	GALILEI Research Project

	GCAGroup.h

	Group - Header.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
#include <rgroupnn.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
* The GCAGroupDoc provides a representation for a group of documents.
* @author Pascal Francq
* @short Documents Group.
*/
class GCAGroupDoc : public R::RGroupNN<GCAGroupDoc,GDoc,GCAChromoDoc>
{
public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GCAGroupDoc(GCAGroupDoc* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	GCAGroupDoc(GCAChromoDoc* owner,const size_t id);
};


//-----------------------------------------------------------------------------
/**
* The GCAGroupProfile provides a representation for a group of profiles.
* @author Pascal Francq
* @short Profiles Group.
*/
class GCAGroupProfile : public R::RGroupNN<GCAGroupProfile,GProfile,GCAChromoProfile>
{
public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GCAGroupProfile(GCAGroupProfile* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	GCAGroupProfile(GCAChromoProfile* owner,const size_t id);
};


//-----------------------------------------------------------------------------
#endif
