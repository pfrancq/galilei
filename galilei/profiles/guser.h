/*

	GALILEI Research Project

	GUser.h

	User - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#ifndef GUserH
#define GUserH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GUser class provides a representation for a user.
* @author Pascal Francq
* @short User.
*/
class GUser : public R::RContainer<GProfile,false,true>
{
	/**
	* Identifier of the user.
	*/
	size_t Id;

	/**
	* Name of the user.
	*/
	R::RString Name;

	/**
	* FullName of the user.
	*/
	R::RString FullName;

public:

	/**
	* Construct a user.
	* @param id             Identifier.
	* @param name           Name of the user.
	* @param fullname       FullName of the user.
	* @param nb             Number of profiles.
	*/
	GUser(size_t id,const R::RString name,const R::RString fullname,size_t nb=5);

	/**
	* Compare two users by comparing their identifier.
	* @see R::RContainer
	* @param user            User.
	* @return int
	*/
	int Compare(const GUser &user) const;

	/**
	* Compare two users by comparing their identifier.
	* @see R::RContainer
	* @param user            Pointer to a user.
	* @return int
	*/
	int Compare(const GUser *user) const;

	/**
	* Compare the identifier of a user with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Compare the name of a user with another one.
	* @see R::RContainer
	* @param name            Name
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the identifier of the user.
	* @return size_t.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(size_t id);

	/**
	 * Get a profile with a given name.
	 * @param name           Name of the profile.
	 * @return Pointer to the profile or null if not found.
	 */
	GProfile* GetProfile(const R::RString& name) const;

	/**
	* Get a cursor over the profiles of user.
	*/
	R::RCursor<GProfile> GetProfiles(void);

	/**
	 * Get the number of profiles.
	 */
	size_t GetNbProfiles(void) const;

	/**
	* Get the name of the user.
	* @return RString.
	*/
	R::RString GetName(void) const;

	/**
	* Get the full name of the user.
	* @return Pointer to a C String.
	*/
	R::RString GetFullName(void) const;

	/**
	 * Set the name of the user.
	 * @param name           Name of the user.
	 * @param fullname       Full name of the user. If null string, the name is
	 *                       used.
	 * @return
	 */
	void SetName(const R::RString& name,const R::RString& fullname=R::RString::Null);

	/**
	* Destruct a user.
	*/
	virtual ~GUser(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
