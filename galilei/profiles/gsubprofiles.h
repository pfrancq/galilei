/*

	GALILEI Research Project

	GSubProfiles.h

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
#ifndef GSubProfilesH
#define GSubProfilesH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSubProfiles class provides a representation for a set of subprofiles of
* a given language.
* @author Pascal Francq
* @short SubProfiles.
*/
class GSubProfiles : public RStd::RContainer<GSubProfile,unsigned int,true,true>
{
protected:

	/**
	* Language of the subprofiles.
	*/
	GLang* Lang;

public:

	/**
	* Constructor of Users
	* @param lang           Language of the subprofile.
	* @param s              Initial number of subprofiles.
	*/
	GSubProfiles(GLang* lang,unsigned int s) throw(bad_alloc);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfiles& s) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfiles* s) const;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s) throw(bad_alloc);

	/**
	* Get a subprofile with a specific identifier.
	* @param id         Identifier.
	*/
	GSubProfile* GetSubProfile(const unsigned int id) const;

	/**
	* Get a cursor over the subprofiles of the system.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

public:

	/**
	* Destructor.
	*/
	virtual ~GSubProfiles(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfilesCursor class provides a way to go trough a set of subprofiles
* of a given language.
* @short SubProfiles Cursor.
*/
CLASSCURSOR(GSubProfilesCursor,GSubProfiles,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
