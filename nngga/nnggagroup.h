/*

	GALILEI Research Project

	NNGGAGroup.h

	Group - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef NNGGAGroupH
#define NNGGAGroupH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupnn.h>


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <gca_nngga.h>


//-----------------------------------------------------------------------------
/**
* The NNGGAGroupDoc provides a representation for a group of documents.
* @author Pascal Francq
* @short Documents Group.
*/
class NNGGAGroupDoc : public R::RGroupNN<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>
{
public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	NNGGAGroupDoc(NNGGAGroupDoc* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	NNGGAGroupDoc(NNGGAChromoDoc* owner,const size_t id);

	friend class R::RGroups<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>;
};


//-----------------------------------------------------------------------------
/**
* The NNGGAGroupProfile provides a representation for a group of profiles.
* @author Pascal Francq
* @short Profiles Group.
*/
class NNGGAGroupProfile : public R::RGroupNN<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>
{
public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	NNGGAGroupProfile(NNGGAGroupProfile* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	NNGGAGroupProfile(NNGGAChromoProfile* owner,const size_t id);

	friend class R::RGroups<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>;
};


//-----------------------------------------------------------------------------
#endif
