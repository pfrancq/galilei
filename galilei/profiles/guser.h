/*

	GALILEI Research Project

	guser.h

	Basic Information - Implementation.

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
//include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gusers.h>
#include <gprofiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------
// forward declaration
class GUsers;
class GProfile;


//-----------------------------------------------------------------------------
// class GUser

class GUser : public RStd::RContainer<GProfile,unsigned,true,true>
{
public:

	GUsers *Owner;
	/**
	* Identifier
	*/
	unsigned Id;
	
	/*
	* Name
	*/
	RString Name;
	
	/**
	* Number of session
	*/
	unsigned SessNum;    // Number of session

	/**
	* Construct a GUser
	* @param owner			parent owners
	*/
	GUser(GUsers *owner) throw(bad_alloc);
	
	/** comparaison functions
	*/
	int Compare(const unsigned int id);
	
	/** comparaison functions
	*/
	int Compare(const GUser &user);
	
	/** comparaison functions
	*/
	int Compare(const GUser *user);
	
	/** virtual Load function
	*/
//	virtual void Load(void) throw(bad_alloc,GException)=0;

};

}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif


