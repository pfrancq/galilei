/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GGroupingH
#define GGroupingH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroups/ggroups.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;


//-----------------------------------------------------------------------------
/**
* The GGrouping provides a representation for a generic method to group some
* subprofiles.
* @author Pascal Francq
* @short Generic Grouping.
*/
class GGrouping
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Pointer to groups to form.
	*/
	GGroups* Groups;

	/**
	* Sub-Profiles to group.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true> SubProfiles;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGrouping(GSession* s) throw(bad_alloc);

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

protected:

	/**
	* Make the grouping for a specific Language. Groups is a pointer to the
	* groups to form and SubProfiles contains all the subprofiles for a given
	* language. This variables must be set before calling this function. This
	* is done by the Grouping method.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Make the groups.
	*/
	void Grouping(void);

	/**
	* Destructor.
	*/
	virtual ~GGrouping(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
