/*

	GALILEI Research Project

	GDocProfSim.h

	List of SubProfiles for a given Language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GDocProfSimsH
#define GDocProfSimsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocProfSims class provides a representation for a set of Similarity between Documents and Profiles of
* a given language.
* @author Pascal Francq , Valery Vandaele
* @short SubProfiles.
*/
class GDocProfSims : public R::RContainer<GDocProfSim,unsigned int,true,true>
{

public:

	/**
	* Constructor of Users
	* @param lang           Language of the document and subprofile.
	* @param s              Initial number of profilesSim.
	*/
	GDocProfSims(unsigned int s) throw(bad_alloc);


	/**
	* Insert a DocProfSims in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertDocProfSim(GDocProfSim* p) throw(bad_alloc);

	/**
	* Get a cursor over the profilesSims of the system.
	*/
	GDocProfSimCursor& GetProfilesSimCursor(GLang* l);

public:

	/**
	* Destructor.
	*/
	virtual ~GDocProfSims(void);
};


//-----------------------------------------------------------------------------
/**
* The GDocProfSimCursor class provides a way to go trough a set of profilesSim
* of a given language.
* @short ProfilesSims Cursor.
*/
CLASSCURSOR(GDocProfSimsCursor,GDocProfSims,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif



 
