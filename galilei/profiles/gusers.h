/*

	GALILEI Research Project

	GUsers.h

	Users - Implementation.

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
#ifndef GUsersH
#define GUsersH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GUser;
class GSubProfile;


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
	* @param nb             Number of User in Users
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

