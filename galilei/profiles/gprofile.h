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
#include <rstd/rcursor.h>
#include <rstd/rstring.h>
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GUser;
class GSubProfile;
class GProfDoc;
class GLang;


//-----------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile. In fact, it is a
* container of subprofiles, each subprofile corresponding to a language.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public RStd::RContainer<GSubProfile,unsigned,false,true>
{
	/**
	* The owner of the profile.
	*/
	GUser* User;

	/**
	* Identificator of the profile.
	*/
	unsigned Id;

	/**
	* Name of the profile.
	*/
	RStd::RString Name;

	/**
	* State of the profile.
	*/
	tObjState State;

	/**
	* Documents juged by profile.
	*/
	RContainer<GProfDoc,unsigned,false,true> Fdbks;

	/**
	* Date of Update.
	*/
	RTimeDate::RDate Updated;

	/**
	* Date of last document's analysis.
	*/
	RTimeDate::RDate Computed;

public:

    /**
	* Constructor og GProfile
	* @param usr            User of the profile.
	* @param id             Identificator of the profile.
	* @param name           Name of the profile.
	* @param u              String representing the date of updated.
	* @param a              String representing the date of the last analysis.
	* @param nb             Number of subprofiles.
	* @param nbf            Number of Feedbacks.
	*/
	GProfile(GUser* usr,const unsigned int id,const char* name,const char* u,const char* a,unsigned int nb,unsigned int nbf=100) throw(bad_alloc);

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
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the name of the profile.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the user of the profile.
	* @return Pointer to the user.
	*/
	const GUser* GetUser(void) const {return(User);}

	/**
	* Get the subprofile corresponding to a given language.
	* @param lang           Pointer to the language.
	* @return Pointer to the subprofile.
	*/
	GSubProfile* GetSubProfile(const GLang* lang) const;

	/**
	* Get the date of the last update of the profile's.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetUpdated(void) const {return(&Updated);}

	/**
	* Get the date of the last analysis of the profile.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetComputed(void) const {return(&Computed);}

	/**
	* Return the state of the profile.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the profile.
	* @param state          New state.
	*/
	void SetState(tObjState state);

	/**
	* Tell the profile that its updated is finished.
	*/
	void UpdateFinished(void);

	/**
	* Start the iterator to go trough the documents judged.
	*/
	void DocsStart(void);

	/**
	* Test if the end of the container of documents judged is reached.
	*/
	bool DocsEnd(void) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	void DocsNext(void);

	/**
	* Get the current document judge.
	* @returns Pointer to the current document udge.
	*/
	GProfDoc* GetCurDocs(void);

	/**
	* Add a judgement for this profile.
	* @param j              Judgement.
	*/
	void AddJudgement(GProfDoc* j) throw(bad_alloc);
};


//-----------------------------------------------------------------------------
/**
* The GProfileCursor class provides a way to go trough a set of profiles.
* @short Profiles' Cursor
*/
CLASSCURSOR(GProfileCursor,GProfile,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
