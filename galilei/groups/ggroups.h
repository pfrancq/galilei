/*

	GALILEI Research Project

	GGroups.h

	Groups for a given language - Header.

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
#ifndef GGroupsH
#define GGroupsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroups class provides a representation for all the groups of a given
* language. The GGroups are ordered by languages.
* @author Pascal Francq
* @short Languages' Groups.
*/
class GGroups : public RStd::RContainer<GGroup,unsigned int,true,true>
{
	/**
	* Language corresponding to the set of groups.
	*/
	GLang* Lang;

public:

	/**
	* Constructor.
	* @param lang           Pointer to the corresponding language.
	*/
	GGroups(GLang* lang) throw(bad_alloc);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroups& groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroups* groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the group where the given subprofile is attached.
	* @param sub            Subprofile used.
	* @returns Pointer to the group.
	*/
	GGroup* GetGroup(const GSubProfile* sub);

	/**
	* Get the group with a given identificator. If the group doesn't exist
	* -> create it.
	* @param id             Identificator.
	* @returns Pointer to the group.
	*/
	GGroup* GetGroup(unsigned int id);

	/**
	* Get a cursor over the groups.
	*/
	GGroupCursor& GetGroupCursor(void);

	/**
	* Create a new group.
	* @returns Pointer to the group.
	*/
	GGroup* NewGroup(void) throw(bad_alloc);

	/**
	* Delete a group.
	* @param grp            Group to delete.
	*/
	void DeleteGroup(GGroup* grp);
};


//-----------------------------------------------------------------------------
/**
* The GGroupsCursor class provides a way to go trough a set of groups.
* @short Groups Cursor
*/
CLASSCURSOR(GGroupsCursor,GGroups,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
