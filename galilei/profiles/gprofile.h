/*

	GALILEI Research Project

	GProfile.h

	Profile - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#include <rstd/rdate.h>


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
class GSubProfileCursor;
class GProfDoc;
class GProfDocCursor;
class GLang;


//-----------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile. In fact, it is a
* container of subprofiles, each subprofile corresponding to a language.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public R::RContainer<GSubProfile,unsigned,false,true>
{
protected:

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
	R::RString Name;

	/**
	* Documents juged by profile.
	*/
	R::RContainer<GProfDoc,unsigned,false,true> Fdbks;

//	/**
//	* Date of Update.
//	*/
//	R::RDate Updated;
//
//	/**
//	* Date of last document's analysis.
//	*/
//	R::RDate Computed;

	/**
	* Determine if the profile is social, i.e. prefer to be grouped with
	* someone also if the similarity is not evident rather to stay alone.
	*/
	bool Social;

#if GALILEITEST
	/**
	* Subject of the profile.
	*/
	GSubject* Subject;
#endif

public:

    /**
	* Constructor og GProfile
	* @param usr            User of the profile.
	* @param id             Identificator of the profile.
	* @param name           Name of the profile.
	* @param s              Social?
	* @param u              String representing the date of updated.
	* @param a              String representing the date of the last analysis.
	* @param nb             Number of subprofiles.
	* @param nbf            Number of Feedbacks.
	*/
	GProfile(GUser* usr,unsigned int id,const char* name,bool s,unsigned int nb,unsigned int nbf=100) throw(bad_alloc);

	/**
	* Clear The Fdbks Container
	*/
	void ClearFdbks (void);

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int& id) const;

	/**
	* Comparaison function
	*/
	int Compare(const GProfile& profile) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile* profile) const;

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
	* Look if the profile is social.
	* @return boolean value.
	*/
	bool IsSocial(void) const {return(Social);}

	/**
	* set if the profile is social.
	* @param social         social  value.
	*/
	void SetSocial(bool social) {Social=social;}

	/**
	* Get the subprofile corresponding to a given language.
	* @param lang           Pointer to the language.
	* @return Pointer to the subprofile.
	*/
	GSubProfile* GetSubProfile(const GLang* lang) const;

	/**
	* Get the subprofile corresponding to a given language. If not found, insert
	* it.
	* @param lang           Pointer to the language.
	* @param s                  Session.
	* @return Pointer to the subprofile.
	*/
	GSubProfile* GetInsertSubProfile(GLang* lang,GSession* s);

	/**
	* Get the feedback of the profile on a specific document.
	* @param doc            Pointer to the document.
	* return Pointer to the feedback or 0 if the document wasn't judged by the
	*        profile.
	*/
	GProfDoc* GetFeedback(const GDoc* doc) const;

	/**
	* Get the number of judged documents of a given language.
	* @param lang           Pointer to the language.
	* @returns unsigned int.
	*/
	unsigned int GetNbJudgedDocs(const GLang* lang) const;

	/**
	* Get a Cursor on the feedback for the profile.
	* @return GProfDocCursor.
	*/
	GProfDocCursor& GetProfDocCursor(void);

	/**
	* Get a Cursor on the subprofiles.
	* @return GSubProfileCursor.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

	/**
	* Add a judgement for this profile.
	* @param j              Judgement.
	* @param s              Session.
	*/
	void AddJudgement(GProfDoc* j,GSession* s) throw(bad_alloc);

	/**
	* Store the profdoc in the feedbacks of the subprofile
	* coressponding to the lang of the profdoc.
	* this function is called when at least one of the two lang is defined
	* and the two lang are different.
	* @param s                  Session.
	*/
	void DispatchFdbks(GProfDoc* profdoc, GLang* oldlang,GSession* s);


#if GALILEITEST

	/**
	* Set the Subject of the subprofile.
	* @param s              Subject.
	*/
	void SetSubject(GSubject* s);

	/**
	* Get the subject of the subprofile.
	*/
	GSubject* GetSubject(void) const;

#endif
};


//-----------------------------------------------------------------------------
/**
* The GProfileCursor class provides a way to go trough a set of profiles.
* @short Profiles Cursor
*/
CLASSCURSOR(GProfileCursor,GProfile,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
