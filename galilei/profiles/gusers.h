/*

	GALILEI Research Project

	gusers.h

	Basic Information - Implementation.

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
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/guser.h>
//#include <gprofiles/gprofile.h>
#include <gprofiles/gsubprofile.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class GSession;


//-----------------------------------------------------------------------------
// class GUsers
class GUsers : public RStd::RContainer<GUser,unsigned,true,true>
{
public:
	GSession *Session;

	/**
	* Constructor of Users
	* @param nb				number of User in Users
	* @param session		opened session
	*/
	GUsers(unsigned int nb,GSession *session) throw(bad_alloc);
	
//	template<class hprofile> GProfile* GetProfile(unsigned int userid,unsigned int profileid,const char* name);
	
	/** reach one subprofile from its identifier
	*/
	GSubProfile* GetSubProfile(unsigned int profileid);
	
	/** vitual Load function
	*/
	//virtual void Load(void) throw(bad_alloc,GException)=0;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

