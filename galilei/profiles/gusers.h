/*

	GALILEI Research Project

	GUsers.h

	Users - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GUsersH
#define GUsersH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GUsers class provides a representation for the users of the system.
* @author Pascal Francq
* @short Users.
*/
class GUsers : public R::RContainer<GUser,unsigned,true,true>
{
	// Internal class
	class GSubProfiles;

	/**
	* Profiles handled by the system.
	*/
	R::RContainer<GProfile,unsigned int,true,true>* Profiles;

	/**
	* SubProfiles handled by the system.
	*/
	R::RContainer<GSubProfiles,unsigned int,true,true>* SubProfiles;

public:

	/**
	* Constructor of Users
	* @param u               Initial number of users.
	* @param p               Initial number of profiles.
	*/
	GUsers(unsigned int u,unsigned int p) throw(bad_alloc);

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
	* @param usr             Pointer to the user to insert.
	*/
	void InsertUser(GUser* usr) throw(bad_alloc);

	/**
	* Get a user from the container.
	* @param id              Identificator.
	* @returns Pointer to the corresponding GUser object.
	*/
	GUser* GetUser(unsigned int id) const;

	/**
	* Create a new profile.
	* @param usr             Pointer to the user of the profile.
	* @param desc            Description of the profile.
	* @returns Pointer to GProfile.
	*/
	virtual GProfile* NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException)=0;

	/**
	* Create a new subprofile.
	* @param prof             Pointer to the profile.
	* @param lang             Language of the subprofile.
	* @returns Pointer to GSubProfile.
	*/
	virtual GSubProfile* NewSubProfile(GProfile* prof,GLang* lang) throw(bad_alloc,GException)=0;

	/**
	* Insert a new profile in the container.
	* @param p               Pointer to the profile to add.
	*/
	void InsertProfile(GProfile* p) throw(bad_alloc);

	/**
	* Get a profile with a specific identifier.
	* @param id              Identifier.
	*/
	GProfile* GetProfile(const unsigned int id) const;

	/**
	* Get a cursor over the profiles of the system.
	*/
	GProfileCursor& GetProfilesCursor(void);

	/**
	* Get the number of profiles defined in the system.
	*/
	unsigned int GetProfilesNb(void) const;

	/**
	* Save a profile.
	* @param prof           Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException)=0;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s) throw(bad_alloc,GException);

	/**
	* Get a subprofile with a specific identifier.
	* @param id             Identifier.
	*/
	GSubProfile* GetSubProfile(const unsigned int id) const throw(GException);

	/**
	* Get a subprofile with a specific identifier.
	* @param id             Identifier.
	* @param lang           Language.
	*/
	GSubProfile* GetSubProfile(const unsigned int id,GLang* lang) const throw(GException);

	/**
	* Get a cursor over the subprofiles of the system for a given language.
	* @param lang           Language.
	*/
	GSubProfileCursor& GetSubProfilesCursor(GLang* lang) throw(GException);

	/**
	* Clear all the subprofiles assignement.
	*/
	void ClearSubProfilesGroups(void) throw(GException);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException)=0;

	/**
	* Load the Users.
	*/
	virtual void LoadUsers(bool wg,bool w) throw(bad_alloc,GException)=0;

	/**
	* Clear all the users, profiles and subprofiles.
	*/
	void Clear(void) throw(GException);

	/**
	* Destructor of the users.
	*/
	virtual ~GUsers(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
