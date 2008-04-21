/*

	GALILEI Research Project

	GProfilesGroupsSimsCosinus.cpp

	Similarities between documents and groups - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gsubprofilesgroupssims.h>


//------------------------------------------------------------------------------
//
// class GProfilesGroupsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double GProfilesGroupsSims::Compute(size_t id1,size_t id2)
{
	GProfile* sub=Session->GetProfile(id1,true,false);
	GGroup* grp=Session->GetGroup(id2,true,false);
	return(sub->SimilarityIFF2(*grp,otProfile,otGroup));	
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles/Groups Similarities","Cosinus Method",GProfilesGroupsSims)

