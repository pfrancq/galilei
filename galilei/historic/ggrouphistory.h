/*

	GALILEI Research Project

	GGroupHistory.h

	History of Group for a given language - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be)..

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
#ifndef GGroupHistoryH
#define GGroupHistoryH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a historical group of sub-profiles. In fact, it is
* implemented as aa  container of GSubProfile.
* @author David Wartel
* @short Historical Group.
*/
class GGroupHistory : public R::RContainer<GWeightInfosHistory,unsigned int,false,true>
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
	* Is the group modified since previous step.
	*/
	bool Modified;

	/**
	* Subject associated to the group.
	*/
	GSubject* Subject;

	/**
	* Parent.
	*/
	GGroupsHistory* Parent;

	/**
	* Container of childrens.
	*/
	R::RContainer<GGroupHistory, unsigned int, false,true>* Childrens;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	*/
	GGroupHistory(const unsigned int id,GLang* lang, GGroupsHistory* grps) throw(std::bad_alloc);

	/**
	* get the id
	*/
	unsigned int GetId(void);

	/**
	* returns the lang of the historical group
	*/
	GLang* GetLang(void){return Lang;}

	/**
	* Add a historic profile to the group.
	*/
	void AddSubProfile(GWeightInfosHistory* giwwh);

	/**
	* set the value of the "modified" parameter..
	*/
	void SetModified(bool b);

	/**
	* get the value of the "modified" parameter..
	*/
	bool GetModified(void);

	/**
	* set the subject of the group..
	*/
	void SetSubject(GSubject* sub);

	/**
	* get the value of the "modified" parameter..
	*/
	GSubject*  GetSubject(void);

	/**
	* insert a child into the container of children
	*/
	 void InsertChildren(GGroupHistory* grp);

	 /**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroupHistory& grouph) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroupHistory* grouph) const;;

	/**
	* cursor on childrens.
	*/
	GGroupHistoryCursor& GetChildrenCursor(void);

	/**
	* get the parent of the group
	*/
	GGroupsHistory* GetParent(void) {return(Parent);}

	/**
	* Destructor
	*/
	~GGroupHistory(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupHistoryCursor class provides a way to go trough a set of GGroupHistory.
* @short GroupHistory Cursor
*/
CLASSCURSOR(GGroupHistoryCursor,GGroupHistory,unsigned int)



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
