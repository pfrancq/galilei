/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

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
#ifndef GGroupHistoryH
#define GGroupHistoryH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <historic/giwordsweightshistory.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupHistoryCursor class provides a way to go trough a set of GGroupHistory.
* @short GroupHistory Cursor
*/
CLASSCURSOR(GGroupHistoryCursor,GGroupHistory,unsigned int)


//-----------------------------------------------------------------------------
/**
* This class represent a historic group of sub-profiles. In fact, it is
*  implemented as aa  container of GSubProfile.
* @author David Wartel
* @short GroupHistory.
*/
class GGroupHistory : public R::RContainer<GIWordsWeightsHistory,unsigned int,false,true>
{

protected:

	/**
	* identifier
	*/
	unsigned int Id;

	/**
	* Language of the group.
	*/
	GLang* Lang;

	/**
	* modified in comparison with the same group at step s-1 ?
	*/
	bool Modified ;

	/**
	* dominant subject of the group;
	*/
	GSubject* Subject;

	/**
	* parent
	*/
	GGroupsHistory* Parent;

	/**
	* container of childrens.
	*/
	R::RContainer<GGroupHistory, unsigned int, false,true>* Childrens;

	/**
	* container of parents.
	*/
	R::RContainer<GGroupHistory, unsigned int, false,true>* Parents;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	*/
	GGroupHistory(const unsigned int id,GLang* lang, GGroupsHistory* grps) throw(bad_alloc);

	/**
	* get the id
	*/
	unsigned int GetId(void);

	/**
	* Add a historic profile to the group.
	*/
	void AddSubProfile(GIWordsWeightsHistory* giwwh);

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
	* insert a parent itno the container of parents
	*/
	 void InsertParent(GGroupHistory* grp);

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
	* cursor on parents.
	*/
	GGroupHistoryCursor& GetParentsCursor(void);

	/**
	* get the parent of the group
	*/
	GGroupsHistory* GetParent(void) {return(Parent);}

	/**
	* Destructor
	*/
	~GGroupHistory(void);
};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
