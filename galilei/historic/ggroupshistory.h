/*

	GALILEI Research Project

	GGroupsHistory.h

	History of Groups for a given language - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GGroupsHistoryH
#define GGroupsHistoryH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <historic/ggrouphistory.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupsHistoryManager class manage the history of groument for the session
* @author David Wartel
* @short Languages GroupsHistory.
*/
class GGroupsHistoryManager : public R::RContainer<GGroupsHistory,unsigned int,true,true>
{
public :

	/**
	* Constructor
	*/
	GGroupsHistoryManager(unsigned int i);

	/*
	* insert a groupshistory
	*/
	void InsertGroupsHistory(GGroupsHistory* gh);

	/**
	* Get a cursor on all the hsitoric groups.
	* @return GGroupCursor.
	*/
	GGroupsHistoryCursor& GetGroupsHistoryCursor(void);

	/**
	* check the modified groups
	*/
	void CheckModifiedGroups(unsigned int minGen);

	/**
	*  check the well grouped subprofiles.
	*/
	void CheckWellGroupedSubProfs(void);

	/**
	*  check for new profiles.
	*/
	void CheckNewProfiles(void);

	/**
	*   Destructor
	*/
	~GGroupsHistoryManager(void){};
};


//-----------------------------------------------------------------------------
/**
* The GGroupsHistory class provides a representation for all the historic groups of a given
* language. The GGroupsHistory are ordered by IDs.
* @author David Wartel
* @short Languages GroupsHistory.
*/
class GGroupsHistory : public R::RContainer<GGroupHistory,unsigned int,true,true>
{

private :

	/**
	*historic  identificator
	*/
	unsigned int Id;

	/**
	*  historic date
	*/
	R::RDate Date;

	/*
	* manager
	*/
	GGroupsHistoryManager* Manager;

public :

	/**
	* constructor
	* @param id             id of the historic.
	* @param date           date of the historic.
	*/
	GGroupsHistory(unsigned int id, R::RString date);

	/**
	* get the id
	*/
	unsigned int GetId(void);

	/**
	* get the date
	*/
	R::RDate GetDate(void) {return Date;}

	/**
	*  returns the subprofiles with Id=id;
	* @param id             id to found.
	*/
	GWeightInfosHistory* GetSubProfile(unsigned int id);

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroupsHistory& groups) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroupsHistory* groups) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(unsigned int id) const ;

	/**
	* set the manager
	*/
	void SetManager(GGroupsHistoryManager* m) {Manager=m;}


	/**
	* check the modified groups
	*/
	void CheckModifiedGroups(unsigned int minGen);

	/**
	*  set the main subject to each historic group
	*/
	void SetGroupsSubject(void);

	/**
	*  check the well grouped subprofiles.
	*/
	void CheckWellGroupedSubProfs(void);

	/**
	*  check for new profiles.
	*/
	void CheckNewProfiles(void);

	/**
	*   Destructor
	*/
	~GGroupsHistory(void);
};


//------------------------------------------------------------------------------
/**
* The GGroupCursor class provides a way to go trough a set of group.
* @short Group Cursor.
*/
CLASSCURSOR(GGroupsHistoryCursor,GGroupsHistory,unsigned int)

}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
