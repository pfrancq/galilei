/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

	(C) 2001 by P. Francq.

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
#ifndef GGroupH
#define GGroupH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GLang;
class GSubProfile;
class GSubProfileCursor;


//-----------------------------------------------------------------------------
/**
* This class represent a group of sub-profiles. In fact, it is implemented as a
* container of GSubProfileRef.
* @author Pascal Francq
* @short Group.
*/
class GGroup : public RStd::RContainer<GSubProfile,unsigned int,false,true>
{
	/**
	* identifier
	*/
	unsigned int Id;

	/**
	* State of the document.
	*/
	tObjState State;

	/**
	* Language of the group.
	*/
	GLang* Lang;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	*/
	GGroup(const unsigned int id,GLang* lang) throw(bad_alloc);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroup& group) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroup* group) const;

	/**
	* Get the identificator of the group.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Return the state of the group.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}
	
	/**
	* Set the state of the group.
	* @param state          New state.
	*/
	void SetState(tObjState state) {State=state;}

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Delete a subprofile from the group.
	* @param sp             SubProfile to delete.
	*/
	void DeleteSubProfile(GSubProfile* sp);

	/**
	* Insert a subprofile in the group.
	* @param sp             SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfile* sp);

	/**
	* Delete all subprofiles.
	*/
	void DeleteSubProfiles(void);

	/**
	* Get a cursor over the subprofiles.
	*/
	GSubProfileCursor& GetSubProfileCursor(void);

	/**
	* Compute the number of subprofiles of grp that are in the current group.
	* @param grp            Group to compare with.
	*/
	unsigned int GetNbSubProfiles(GGroup* grp);
};


//-----------------------------------------------------------------------------
/**
* The GGroupCursor class provides a way to go trough a set of group.
* @short Group' Cursor
*/
CLASSCURSOR(GGroupCursor,GGroup,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
