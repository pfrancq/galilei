/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GGroupH
#define GGroupH


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ggroups/gsubprofileref.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a group of sub-profiles. In fact, it is implemented as a
* container of GSubProfileRef.
* @author Pascal Francq
* @short Group.
*/
class GGroup : public RStd::RContainer<GSubProfileRef,unsigned int,true,true>
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
	* Construct a group.
	* @param lang           Language.
	*/
	GGroup(GLang* lang) throw(bad_alloc);

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
	* Returnt he state of the document.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Delete a subprofile from the group.
	* @param sp             SubProfile to delete.
	*/
	void DeleteSubProfile(GSubProfileRef* sp);

	/**
	* Insert a subprofile in the group.
	* @param sp             SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfileRef* sp);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
