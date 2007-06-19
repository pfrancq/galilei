/*

	GALILEI Research Project

	GGroupsDocsSims.h

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
#ifndef GGroupsDocsSimsH
#define GGroupsDocsSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggenericsims.h>


//------------------------------------------------------------------------------
/**
* The GGroupsDocsSims class provides a representation for the similarities between
* groups and profiles.
* @author Pascal Francq and Valery Vandaele
* @short Groups-Documents Similarities.
*/
class GGroupsDocsSims : public GGenericSims
{
public:

	/**
	* Constructor of the similarities between documents and subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	* @param memory      use container to stock sims?
	*/
	GGroupsDocsSims(GFactoryMeasure* fac)
		: GGenericSims(fac,true,true,false) {}

	virtual double Compute(GLang* lang,size_t id1,size_t id2);

	virtual size_t GetMaxId1(GLang* lang) { return(Session->GetNbGroups(lang));}

	/**
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GGroupsDocsSims(void) {}
};


//------------------------------------------------------------------------------
#endif
