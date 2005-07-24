/*

	GALILEI Research Project

	GUsers.h

	Users - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GUsersH
#define GUsersH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gsubprofile.h>
#include <gprofile.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GUsers class provides a representation for the users of the system.
* @author Pascal Francq
* @short Users.
*/
class GUsers : public R::RContainer<GUser,true,true>
{
	// Internal class
	class GSubProfiles;

	/**
	* Profiles handled by the system.
	*/
	R::RContainer<GProfile,true,true> Profiles;

	/**
	* SubProfiles handled by the system.
	*/
	R::RContainer<GSubProfiles,true,true> SubProfiles;

public:

	/**
	* Constructor of Users
	* @param u               Initial number of users.
	* @param p               Initial number of profiles.
	*/
	GUsers(unsigned int u,unsigned int p);

	/**
	* Get a cursor over the users used in the system.
	*/
	R::RCursor<GUser> GetUsersCursor(void) const;

	/**
	* Get the number of users treated by the system.
	* @returns Number of Users.
	*/
	size_t GetNbUsers(void) const {return(GetNb());}

	/**
	* Get an identificator that can be assigned to a new object.
	* @param obj             Object type (otProfile,otSubProfile,otUser).
	* @return unsigned int
	*/
	size_t GetNewId(tObjType obj);

	/**
	* Insert an user in the container.
	* @param usr             Pointer to the user to insert.
	*/
	void InsertUser(GUser* usr);

	/**
	* Get a user from the container.
	* @param id              Identificator.
	* @returns Pointer to the corresponding GUser object.
	*/
	GUser* GetUser(unsigned int id) const;

	/**
	* Insert a new profile in the container.
	* @param p               Pointer to the profile to add.
	*/
	void InsertProfile(GProfile* p);

	/**
	* Get a profile with a specific identifier.
	* @param id              Identifier.
	*/
	GProfile* GetProfile(const unsigned int id) const;

	/**
	* Get a cursor over the profiles of the system.
	*/
	R::RCursor<GProfile> GetProfilesCursor(void) const;

	/**
	* Get the number of profiles defined in the system.
	*/
	size_t GetProfilesNb(void) const;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s);

	/**
	* Get a subprofile with a specific identifier.
	* @param id             Identifier.
	*/
	GSubProfile* GetSubProfile(const unsigned int id) const;

	/**
	* Get a subprofile with a specific identifier.
	* @param id             Identifier.
	* @param lang           Language.
	*/
	GSubProfile* GetSubProfile(const unsigned int id,const GLang* lang) const;

	/**
	* Get a cursor over the subprofiles of the system for a given language.
	* @param lang           Language.
	*/
	R::RCursor<GSubProfile> GetSubProfilesCursor(const GLang* lang) const;

	/**
	* Clear all the users.
	*/
	void ClearUsers(void);

	/**
	* Destructor of the users.
	*/
	virtual ~GUsers(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
