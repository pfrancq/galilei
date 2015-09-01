/*

	GALILEI Research Project

	GUser.h

	User - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GUser class provides a representation for a user.
* @short User.
*/
class GUser : public GObject, public R::RContainer<GProfile,false,true>
{
	/**
	* FullName of the user.
	*/
	R::RString FullName;

public:

	/**
	* Construct a user.
	* @param session        Session.
	* @param id             Identifier.
	* @param name           Name of the user.
	* @param fullname       FullName of the user.
	* @param nb             Number of profiles.
	*/
	GUser(GSession* session,size_t id,const R::RString& name,const R::RString& fullname,size_t nb=5);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GUser");}

	/**
	 * Give the type of the object.
    * @return otUser
    */
	static inline tObjType GetType(void) {return(otUser);}

	/**
	* Compare two users by comparing their identifier.
	* @see R::RContainer
	* @param user            User.
	* @return int
	*/
	int Compare(const GUser &user) const;

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
	* Get the full name of the user.
	* @return Pointer to a C String.
	*/
	R::RString GetFullName(void) const;

	/**
	* Destruct a user. Its profiles are also deleted.
	*/
	virtual ~GUser(void);

	friend class GSession;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
