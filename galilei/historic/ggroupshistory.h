/*

	GALILEI Research Project

	GGroupsHistory.h

	History of Groups for a given language - Header.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
* The GGroupsHistoryManager class manages the history of subprofiles groupments.
* @author David Wartel
* @short Histories of Subprofiles Groupment.
*/
class GGroupsHistoryManager : public R::RContainer<GGroupsHistory,true,true>
{
public :

	/**
	* Constructor.
	* @param max             Maximal number of word created at initialisation.
	*/
	GGroupsHistoryManager(unsigned int max) throw(std::bad_alloc);

	/**
	* Get a cursor on the history of all groupments.
	* @return GGroupCursor.
	*/
	GGroupsHistoryCursor GetGroupsHistoryCursor(void);

	/**
	* Check which groups are modified.
	*/
	void CheckModifiedGroups(unsigned int minGen)  throw(std::bad_alloc);

	/**
	* Check which subprofiles are correctly grouped.
	*/
	void CheckWellGroupedSubProfs(void)  throw(std::bad_alloc);

	/**
	* Check which subprofiles are new.
	*/
	void CheckNewProfiles(void)  throw(std::bad_alloc);

	/**
	* Creates relationship between historic groups.
	* #param maxgen          Identificators of the last history to use.
	*/
	void CreateGroupsRelationship(unsigned int maxgen) throw(std::bad_alloc);

	/**
	* Insert a history of a groupment.
	* @param gh              History of a groupment.
	*/
	void InsertGroupsHistory(GGroupsHistory* gh)  throw(std::bad_alloc);

	/**
	*   Destructor.
	*/
	~GGroupsHistoryManager(void);
};


//------------------------------------------------------------------------------
/**
* The GGroupsHistory class provides a representation for a history of a given
* subprofiles groupment.
* @author David Wartel
* @short History of a Subprofiles Groupment.
*/
class GGroupsHistory : public R::RContainer<GGroupHistory,true,true>
{
	/**
	* Identificator of the history.
	*/
	unsigned int Id;

	/**
	* Date.
	*/
	R::RDate Date;

	/**
	* Manager that handles all histories of groupment.
	*/
	GGroupsHistoryManager* Manager;

public :

	/**
	* Constructor of a history of a groupment.
	* @param id              Identificator of the historic.
	* @param date            Date of the history.
	*/
	GGroupsHistory(unsigned int id, R::RString date) throw(std::bad_alloc);

	/**
	* Get the identificator of the groupment.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Get the date where this groupment was computed.
	* @return R::RDate.
	*/
	R::RDate GetDate(void) const;

	/**
	* Get the history of a subprofile corresponding to a given identificator.
	* @param id              Identificator.
	* @return Pointer to a GWeightInfosHistory.
	*/
	GWeightInfosHistory* GetSubProfile(unsigned int id) throw(GException);

	/**
	* Compare two histories of groupment by comparing their identificator.
	* @see R::RContainer
	* @param groups          History of groupment.
	* @return int
	*/
	int Compare(const GGroupsHistory& groups) const;

	/**
	* Compare two histories of groupment by comparing their identificator.
	* @see R::RContainer
	* @param groups          Pointer to a history of groupment.
	* @return int
	*/
	int Compare(const GGroupsHistory* groups) const;

	/**
	* Compare the identificator of a history of groupment with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(unsigned int id) const;

	/**
	* Set the manager of the history.
	* @param m               Pointer to the manager.
	*/
	void SetManager(GGroupsHistoryManager* m);

	/**
	* Check which groups are modified.
	*/
	void CheckModifiedGroups(unsigned int minGen) throw(std::bad_alloc);

	/**
	* Set the main topic associated to each history of groupment.
	*/
	void SetGroupsSubject(void) throw(std::bad_alloc);

	/**
	* Check which subprofiles are correctly grouped.
	*/
	void CheckWellGroupedSubProfs(void) throw(std::bad_alloc);

	/**
	* Check which subprofiles are new.
	*/
	void CheckNewProfiles(void) throw(std::bad_alloc);

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
CLASSCURSOR(GGroupsHistoryCursor,GGroupsHistory);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
