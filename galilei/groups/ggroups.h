/*

	GALILEI Research Project

	GGroups.h

	Groups for a given language - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GGroupsH
#define GGroupsH


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ggroups/ggroup.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroups class provides a representation for all the groups of a given
* language.
* @author Pascal Francq
* @short Languages' Groups.
*/
class GGroups : public RStd::RContainer<GGroup,unsigned int,true,false>
{
	/**
	* Language corresponding to the set of groups.
	*/
	GLang* Lang;

public:

	/**
	* Constructor.
	* @param lang           Pointer to the corresponding language.
	*/
	GGroups(GLang* lang) throw(bad_alloc);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroups& groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroups* groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Compute -> Must be in separate class.
	*/
	void Calc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
