/*

	GALILEI Research Project

	GUser.h

	User - Header.

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
#ifndef GUserH
#define GUserH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// forward class declaration
class GProfile;


//-----------------------------------------------------------------------------
/**
* The GUser class provides a representation for a user.
* @author Pascal Francq
* @short User.
*/
class GUser : public RStd::RContainer<GProfile,unsigned,false,true>
{
	/**
	* Identificator of the user.
	*/
	unsigned Id;

	/*
	* Name of the user.
	*/
	RStd::RString Name;

	/*
	* FullName of the user.
	*/
	RStd::RString FullName;

public:

	/**
	* Construct a GUser.
	* @param id             Identificator.
	* @param name           Name of the user.
	* @param fullname       FullName of the user.
	* @param nb             Number of profiles.
	*/
	GUser(const unsigned int id,const char* name,const char* fullname,const unsigned int nb) throw(bad_alloc);

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GUser &user) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GUser *user) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Get the identificator of the user.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the name of the user.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the full name of the user.
	* @return Pointer to a C String.
	*/
	const char* GetFullName(void) const {return(FullName());}

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------

#endif


