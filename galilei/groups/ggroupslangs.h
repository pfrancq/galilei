/*

	GALILEI Research Project

	GGroupsLangs.h

	All Groups - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GGroupsLangsH
#define GGroupsLangsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


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
* The GGroupsLangs provides a representation for all the groups.
* @author Pascal Francq
* @short Groups.
*/
class GGroupsLangs : public RStd::RContainer<GGroups,unsigned int,true,true>
{
public:
	/**
	* Constructor.
	*/
	GGroupsLangs(GSession* session) throw(bad_alloc);

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
