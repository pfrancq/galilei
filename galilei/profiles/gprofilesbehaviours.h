/*

	GALILEI Research Project

	GSubProfiles.h

	Agreement and DisAgreement Ratios for subprofiles - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		david Wartel (dwartel@ulb.ac.be).

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
#ifndef GProfilesBehavioursH
#define GProfilesBehavioursH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GProfilesBehaviours class manage the agreement and disagreeemnt ratio
*for subprofiles
* @author David Wartel
* @short SubProfiles agreement and disagreement ratio.
*/

//-----------------------------------------------------------------------------
//
//   class GProfilesBehaviours
//
//-----------------------------------------------------------------------------
class GProfilesBehaviours : public R::RContainer<GProfilesBehaviour,unsigned int,true,true>
{
public:

	/**
	* Constructor of Users
	* @param lang           Language of the subprofile.
	* @param s              Initial number of profilesSim.
	*/
	GProfilesBehaviours(unsigned int s) throw(bad_alloc);


	/**
	* Insert a ProfilesSims in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertProfilesBehaviour(GProfilesBehaviour* p) throw(bad_alloc);

	/**
	* Get a cursor over the profilesSims of the system.
	*/
	GProfilesBehaviourCursor& GetProfilesBehaviourCursor(GLang* l);

public:

	/**
	* Destructor.
	*/
	virtual ~GProfilesBehaviours(void);

};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
