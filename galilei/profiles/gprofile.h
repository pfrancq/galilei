/*

	GALILEI Research Project

	gprofile.h

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



//---------------------------------------------------------------------------
#ifndef GProfileH
#define GProfileH


//-----------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>


//---------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>
#include <gprofiles/guser.h>
using namespace GALILEI ;


//-----------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
namespace GALILEI{

class GUser;
//class GWordCalcs;

//-----------------------------------------------------------------------------
// class GProfile
class GProfile : public RStd::RContainer<GSubProfile,unsigned,true,true>
{
public:

	/**
	* The owner of the profile.
	*/
	GUser *Owner;

	/**
	* Identifier
	*/
	unsigned Id;

	/**
	* name
	*/
	RString Name;

	/**
	* Profile modified since last analyse ? (T/F)
	*/
	bool Modified;
	
	/**
	* Documents juged by profile
	*/
	RContainer<GProfDoc,unsigned,true,true> FdbkDocs;
	

	/**
	* Date of Update
	*/
	time_t	Updated;

	/**
	* Default Constructor.
	*/
    GProfile(GUser *owner) throw(bad_alloc);
	
    /**
	* Constructor og GProfile
	* @param owner          the owner of the profile.
	* @param id             id of the profile.
	* @param name           name of the profile.
	*/
	GProfile(GUser *owner,unsigned int id,RString &name) throw(bad_alloc);
	
	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id);
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile &profile);
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile *profile);

	/**
	* Calculation of the profile
	*/
	void Calc(void);

	/**
	* Load and Save virtual functions.
	*/
//	virtual void Load(void) throw(bad_alloc,GException)=0;
//	virtual void Save(void) throw(bad_alloc,GException)=0;

};
}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------
#endif
