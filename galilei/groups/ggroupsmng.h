/*

	GALILEI Research Project

	GGroupsMng.h

	Manager of Groups - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
#ifndef GGroupsMngH
#define GGroupsMngH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupsMng class provides a manager for all the groups of the system.
* @author Pascal Francq
* @short Groups Manager.
*/
class GGroupsMng
{
protected:

	/**
	* Groups handled by the system.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true> Groups;

public:

	/**
	* Constructor of the groups manager.
	* @param g              Number of groups.
	*/
	GGroupsMng(unsigned int g);

protected:

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void) throw(bad_alloc,GException)=0;

public:

	/**
	* Get a cursor over the groups of the system.
	*/
	GGroupsCursor& GetGroupsCursor(void);

	/**
	* Find the groups for a specific language.
	* @param lang           Pointer to the language.
	* @returns Pointer to the group.
	*/
	GGroups* GetGroups(const GLang* lang) const;

	/**
	* Save a group, i.e. save all the information of the subprofiles
	* concerning the groupement.
	* @param grp        Group to save.
	*/
	virtual void Save(GGroup* grp) throw(GException)=0;

	/**
	* Create a new group.
	* @param lang       Language of the group to create.
	* @param grp        Group created.
	*/
	virtual void NewGroup(GLang* lang,GGroup* grp)=0;

	/**
	* Save the groups of the session.
	*/
	virtual void SaveGroups(void)=0;

	/**
	* Delete a group.
	* @param grp        Group to delete.
	*/
	virtual void DeleteGroup(GGroup* grp)=0;

	/**
	* Clear the groups of a given language.
	* @param lang       Language of the groups to delete.
	*/
	void ClearGroups(GLang* lang);

	/**
	* Destructor.
	*/
	virtual ~GGroupsMng(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
