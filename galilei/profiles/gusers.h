/*

	GALILEI Research Project

	GUsers.h

	Users - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GUsersH
#define GUsersH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/guser.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GUsers class provides a representation for the users of the system.
* @author Pascal Francq
* @short Users.
*/
class GUsers : public RStd::RContainer<GUser,unsigned,true,true>
{
public:

	/**
	* Constructor of Users
	* @param nb				number of User in Users
	*/
	GUsers(unsigned int nb) throw(bad_alloc);
	
	/**
	* Reach one subprofile from its identifier.
	*/
	GSubProfile* GetSubProfile(unsigned int profileid) const;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

