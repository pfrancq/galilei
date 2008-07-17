/*

	GALILEI Research Project

	GCommunitysHistory.h

	History of Groups for a given language - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
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
#ifndef GCommunitiesHistoryH
#define GCommunitiesHistoryH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gcommunityhistory.h>

//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GCommunitiesHistoryManager class manages the history of subprofiles groupments.
* @author David Wartel
* @short Histories of Subprofiles Groupment.
*/
class GCommunitiesHistoryManager : public R::RContainer<GCommunitiesHistory,true,true>
{
	/**
	* Pointer to the session.
	*/
	GSession* Session;

public :

	/**
	* Constructor.
	* @param session         Session.
	* @param max             Number of steps to handle.
	*/
	GCommunitiesHistoryManager(GSession* session,size_t max);

	/**
	* Get a cursor on the history of all groupments.
	*/
	R::RCursor<GCommunitiesHistory> GetCommunitiesHistory(void);

	/**
	* Check which groups are modified.
	*/
	void CheckModifiedCommunities(size_t minGen);

	/**
	* Check which profiles are correctly grouped.
	*/
	void CheckWellGroupedProfs(void);

	/**
	* Check which profiles are new.
	*/
	void CheckNewProfiles(void);

	/**
	* Creates relationship between historic groups.
	* #param maxgen          Identificators of the last history to use.
	*/
	void CreateCommunitiesRelationship(size_t maxgen);

	/**
	* Insert a history of a groupment.
	* @param gh              History of a groupment.
	*/
	void InsertCommunitiesHistory(GCommunitiesHistory* gh);

	/**
	* Get the associated session.
	*/
	GSession* GetSession(void) const;

	/**
	*   Destructor.
	*/
	~GCommunitiesHistoryManager(void);
};


//------------------------------------------------------------------------------
/**
* The GCommunitysHistory class provides a representation for a history of a given
* subprofiles groupment.
* @author David Wartel
* @short History of a Subprofiles Groupment.
*/
class GCommunitiesHistory : public R::RContainer<GCommunityHistory,true,true>
{
	/**
	* Identificator of the history.
	*/
	size_t Id;

	/**
	* Date.
	*/
	R::RDate Date;

	/**
	* Manager that handles all histories of groupment.
	*/
	GCommunitiesHistoryManager* Manager;

public :

	/**
	* Constructor of a history of a groupment.
	* @param id              Identificator of the historic.
	* @param date            Date of the history.
	*/
	GCommunitiesHistory(size_t id, R::RString date);

	/**
	* Get the identificator of the groupment.
	* @return size_t.
	*/
	size_t GetId(void) const {return(Id);}

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
	GWeightInfosHistory* GetProfile(size_t id);

	/**
	* Compare two histories of groupment by comparing their identificator.
	* @see R::RContainer
	* @param groups          History of groupment.
	* @return int
	*/
	int Compare(const GCommunitiesHistory& groups) const;

	/**
	* Compare two histories of groupment by comparing their identificator.
	* @see R::RContainer
	* @param groups          Pointer to a history of groupment.
	* @return int
	*/
	int Compare(const GCommunitiesHistory* groups) const;

	/**
	* Compare the identificator of a history of groupment with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(size_t id) const;

	/**
	* Set the manager of the history.
	* @param m               Pointer to the manager.
	*/
	void SetManager(GCommunitiesHistoryManager* m);

	/**
	* Check which groups are modified.
	*/
	void CheckModifiedCommunities(size_t minGen);

	/**
	* Set the main topic associated to each history of groupment.
	*/
	void SetCommunitiesSubject(void);

	/**
	* Check which profiles are correctly grouped.
	*/
	void CheckWellGroupedProfs(void);

	/**
	* Check which subprofiles are new.
	*/
	void CheckNewProfiles(void);

	/**
	*   Destructor
	*/
	~GCommunitiesHistory(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
