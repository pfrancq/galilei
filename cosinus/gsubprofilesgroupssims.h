/*

	GALILEI Research Project

	GProfilesGroupsSimsCosinus.h

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
#ifndef GSubProfilesGroupsSimsH
#define GSubProfilesDocsSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggenericsims.h>


//------------------------------------------------------------------------------
/**
* The GCommunitysDocsSims class provides a representation for the similarities between
* groups and profiles.
* @author Pascal Francq and Valery Vandaele
* @short Groups-Documents Similarities.
*/
class GProfilesGroupsSims : public GGenericSims
{
public:

	/**
	* Constructor of the similarities between documents and subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	* @param memory      use container to stock sims?
	*/
	GProfilesGroupsSims(GFactoryMeasure* fac)
		: GGenericSims(fac,false,true,true) {}

	virtual double Compute(size_t id1,size_t id2);

	virtual size_t GetMaxId1(void) { return(Session->GetNbProfiles());}

	/**
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GProfilesGroupsSims(void) {}
};


//------------------------------------------------------------------------------
#endif
