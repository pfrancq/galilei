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
class GUserCursor;
class GProfile;
class GProfileCursor;
class GSubProfile;
class GSubProfileCursor;


//-----------------------------------------------------------------------------
/**
* The GUsers class provides a representation for the users of the system.
* @author Pascal Francq
* @short Users.
*/
class GUsers : public RStd::RContainer<GUser,unsigned,true,true>
{
	/**
	* SubProfiles handled by the system.
	*/
	RStd::RContainer<GProfile,unsigned int,true,true>* Profiles;

	/**
	* SubProfiles handled by the system.
	*/
	RStd::RContainer<GSubProfile,unsigned int,true,true>* SubProfiles;

protected:

	/**
	* State of the users.
	*/
	bool bUsers;

public:

	/**
	* Constructor of Users
	* @param u              Initial number of users.
	* @param p              Initial number of profiles.
	* @param s              Initial number of subprofiles for a given profile.
	*/
	GUsers(unsigned int u,unsigned int p,unsigned int s) throw(bad_alloc);

	/**
	* Get a cursor over the users used in the system.
	*/
	GUserCursor& GetUsersCursor(void);

	/**
	* Get the number of users treated by the system.
	* @returns Number of Users.
	*/
	unsigned int GetNbUsers(void) const {return(NbPtr);}

	/**
	* Insert an user in the container.
	* @param usr            Pointer to the user to insert.
	*/
	void InsertUser(GUser* usr) throw(bad_alloc);

	/**
	* Get a user from the container.
	* @param id             Identificator.
	* @returns Pointer to the corresponding GUser object.
	*/
	GUser* GetUser(unsigned int id);

	/**
	* Verify if the users are loaded.
	* @returns true, if loaded.
	*/
	bool IsUsersLoad(void) const {return(bUsers);}

	/**
	* Create a new profile.
	* @param usr        Pointer to the user of the profile.
	* @param desc       Description of the profile.
	* @returns Pointer to GProfile.
	*/
	virtual GProfile* NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException)=0;

	/**
	* Insert a new profile in the container.
	* @param p              Pointer to the profile to add.
	*/
	void InsertProfile(GProfile* p) throw(bad_alloc);

	/**
	* Get a profile with a specific identifier.
	* @param id         Identifier.
	*/
	GProfile* GetProfile(const unsigned int id) const;

	/**
	* Get a cursor over the profiles of the system.
	*/
	GProfileCursor& GetProfilesCursor(void);

	/**
	* Save a profile.
	* @param prof       Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException)=0;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s) throw(bad_alloc);

	/**
	* Get a subprofile with a specific identifier.
	* @param id         Identifier.
	*/
	GSubProfile* GetSubProfile(const unsigned int id) const;

	/**
	* Get a cursor over the subprofiles of the system.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException)=0;


protected:

	/**
	* Load the Users.
	*/
	virtual void LoadUsers(void) throw(bad_alloc,GException)=0;

public:

	/**
	* Destructor.
	*/
	virtual ~GUsers(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

