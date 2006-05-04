/*

	GALILEI Research Project

	GUser.h

	User - Header.

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
	* Identificator of the user.
	*/
	unsigned Id;

	/*
	* Name of the user.
	*/
	R::RString Name;

	/*
	* FullName of the user.
	*/
	R::RString FullName;

public:

	/**
	* Construct a user.
	* @param id             Identificator.
	* @param name           Name of the user.
	* @param fullname       FullName of the user.
	* @param nb             Number of profiles.
	*/
	GUser(unsigned int id,const R::RString name,const R::RString fullname,unsigned int nb);

	/**
	* Compare two users by comparing their identificator.
	* @see R::RContainer
	* @param user            User.
	* @return int
	*/
	int Compare(const GUser &user) const;

	/**
	* Compare two users by comparing their identificator.
	* @see R::RContainer
	* @param user            Pointer to a user.
	* @return int
	*/
	int Compare(const GUser *user) const;

	/**
	* Compare the idenfiticator of a user with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare the name of a user with another one.
	* @see R::RContainer
	* @param name            Name
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the identificator of the user.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id);

	/**+
	* Get a cursor over the profiles of user.
	*/
	R::RCursor<GProfile> GetProfiles(void);

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
	* Destructor of a user.
	*/
	virtual ~GUser(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
