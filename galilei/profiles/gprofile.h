/*

	GALILEI Research Project

	GProfile.h

	Profile - Implementation.

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
#ifndef GProfileH
#define GProfileH


//-----------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class GUser;


//-----------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile. In fact, it is a
* container of subprofiles, each subprofile corresponding to a language.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public RStd::RContainer<GSubProfile,unsigned,true,true>
{
	/**
	* The owner of the profile.
	*/
	GUser* Owner;

	/**
	* Identificator of the profile.
	*/
	unsigned Id;

	/**
	* Name of the profile.
	*/
	RStd::RString Name;

	/**
	* Profile modified since last analyse ? (T/F)
	*/
	bool Modified;

public:

	/**
	* Documents juged by profile
	*/
	RContainer<GProfDoc,unsigned,true,true> FdbkDocs;

	/**
	* Date of Update
	*/
	time_t	Updated;

	/**
	* Constructor.
	* @param owner          User of the profile.
	*/
    GProfile(GUser *owner) throw(bad_alloc);
	
    /**
	* Constructor og GProfile
	* @param owner          User of the profile.
	* @param id             Identificator of the profile.
	* @param name           Name of the profile.
	*/
	GProfile(GUser *owner,const unsigned int id,const char* name) throw(bad_alloc);
	
	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile &profile) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile *profile) const;

	/**
	* Get the identificator of the profile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Get the name of the profile.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the user of the profile.
	* @return Pointer to the user.
	*/
	const GUser* GetUser(void) const {return(Owner);}

	/**
	* Test if the profile was modified.
	* @returns Boolean.
	*/
	bool IsModified(void) const {return(Modified);}

	/**
	* Start the iterator to go trough the documents judged.
	*/
	inline void DocsStart(void)
		{FdbkDocs.Start();}

	/**
	* Test if the end of the container of documents judged is reached.
	*/
	inline bool DocsEnd(void) const
		{return(FdbkDocs.End());}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void DocsNext(void)
		{FdbkDocs.Next();}

	/**
	* Get the current document judge.
	* @returns Pointer to the current document udge.
	*/
	GProfDoc* GetCurDocs(void) {return(FdbkDocs());}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
