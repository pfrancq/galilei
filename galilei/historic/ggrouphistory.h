/*

	GALILEI Research Project

	GGroupHistory.h

	History of a given group - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be)..

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
#ifndef GGroupHistoryH
#define GGroupHistoryH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a history for a given group of subprofiles.
* @author David Wartel
* @short History of a Group.
*/
class GGroupHistory : public R::RContainer<GWeightInfosHistory,false,true>
{
protected:

	/**
	* Identificator.
	*/
	unsigned int Id;

	/**
	* Language of the group.
	*/
	GLang* Lang;

	/**
	* Was the group modified since previous step.
	*/
	bool Modified;

	/**
	* Main topic associated with this group.
	*/
	GSubject* Subject;

	/**
	* History of groupment holding this group.
	*/
	GGroupsHistory* Parent;

	/**
	* List of groups of the next step to be formed started from this group.
	*/
	R::RContainer<GGroupHistory,false,true> Childrens;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	* @param grps           Parent.
	*/
	GGroupHistory(const unsigned int id,GLang* lang, GGroupsHistory* grps) throw(std::bad_alloc);

	/**
	* Get the identificator of the group.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Get the lang of the group.
	* @return Pointer to GLang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Add a history of a subprofile into the group.
	* @param giwwh           Pointer to a history of a subprofile.
	*/
	void AddSubProfile(GWeightInfosHistory* giwwh);

	/**
	* Set if the groups is considered as modified.
	* @param b               Modified?
	*/
	void SetModified(bool b);

	/**
	* Is the group modified?
	*/
	bool IsModified(void) const {return(Modified);}

	/**
	* Set the main topic of the group.
	* @param sub             Pointer to the topic.
	*/
	void SetSubject(GSubject* sub);

	/**
	* Get the topic of the group.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(void) const {return(Subject);}

	/**
	* Insert a group derived from the current one.
	* @param grp             POinter to a child group.
	*/
	void InsertChildren(GGroupHistory* grp);

	/**
	* Compare two historied of groups by comparing their identificator.
	* @see R::RContainer
	* @param grouph          History of group.
	* @return int
	*/
	int Compare(const GGroupHistory& grouph) const;

	/**
	* Compare two groups by comparing their identificator.
	* @see R::RContainer
	* @param grouph          Pointer to a history of group.
	* @return int
	*/
	int Compare(const GGroupHistory* grouph) const;

	/**
	* Compare the idenfiticator of a history of group with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Get a cursor on all children groups.
	*/
	R::RCursor<GGroupHistory> GetChildrens(void);

	/**
	* Get the groupment containing the group.
	* @return Pointer to GGroupsHistory.
	*/
	GGroupsHistory* GetParent(void) const {return(Parent);}

	/**
	* Destructor of history of group.
	*/
	~GGroupHistory(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
