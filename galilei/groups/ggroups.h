/*

	GALILEI Research Project

	GGroups.h

	Groups - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GGroupsH
#define GGroupsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroups class provides a manager for all the groups managed by a GALILEI
* session.
* @author Pascal Francq.
* @short Session Groups.
*/
class GGroups : protected R::RContainer<GGroup,true,true>
{
protected:

	// Internal Class
	class GGroupsLang;
	class GFreeId;

	/**
	* Groups ordered by language and identificator.
	*/
	R::RContainer<GGroupsLang,true,true> GroupsLang;

	/**
	* List of all "free" identificators that could be used for a group.
	*/
	R::RContainer<GFreeId,true,true> FreeIds;

public:

	/**
	* Constructor.
	* @param g              Number of groups.
	*/
	GGroups(unsigned int g);

	/**
	* Get a cursor on all the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroupsCursor(void);

	/**
	* Get a cursor on the groups of a given langage.
	* @param lang            Language of the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroupsCursor(GLang* lang) throw(GException);

	/**
	* Insert a group. The group is also stored in the container correspondong to
	* its language.
	* @param grp             Pointer to the group.
	*/
	void InsertGroup(GGroup* grp) throw(std::bad_alloc);

	/**
	* Delete a group.
	* @param grp             Pointer to the group.
	*/
	void DeleteGroup(GGroup* grp) throw(std::bad_alloc);

	/**
	* Get the group where the given subprofile is attached.
	* @param sub            Subprofile used.
	* @returns Pointer to the group.
	*/
	GGroup* GetGroup(const GSubProfile* sub) throw(GException);

	/**
	* Get a group corresponding to a given identificator.
	* @param id         Identificator of the group.
	* @return Pointer to GGroup.
	*/
	GGroup* GetGroup(unsigned int id) throw(std::bad_alloc);

	/**
	* Get the number of groups handled.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroups(void) const {return(NbPtr);}

	/**
	* Get the number of groups of a given langauge handled.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroups(GLang* lang) const;

	/**
	* Get an identificator that can be assigned to a new group. The returned
	* identificator is considered as reserved.
	* @return unsigned int
	*/
	unsigned int GetNewId(void) const;

	/**
	* Clear the groups of a given language.
	* @param lang       Language of the groups to delete.
	*/
	void Clear(GLang* lang);

	/**
	* Clear all the groups.
	*/
	void ClearGroups(void) throw(GException);

	/**
	* Destructor of the groups.
	*/
	virtual ~GGroups(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
