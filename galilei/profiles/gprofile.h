/*

	GALILEI Research Project

	GProfile.h

	Profile - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#ifndef GProfileH
#define GProfileH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFdbk class provides a representation for assessment on a document.
* @author Pascal Francq
* @short Documment Assessment.
*/
class GFdbk
{
	/**
	* Document assessed.
	*/
	GDocProxy* Doc;

	/**
	* Assessment of the profile.
	*/
	tDocAssessment Fdbk;

	/**
	* Last update of this assessment.
	*/
	R::RDate Updated;

public:

	/**
	* Constructor.
	* @param id               Identificator of the document.
	* @param fdbk             Assessment.
	* @param updated          Date.
	*/
	GFdbk(unsigned int id,tDocAssessment fdbk,R::RDate& updated);

	/**
	* Compare two assessements to order them using the document identificator.
	* @see R::RContainer
	* @param fdbk             Assessement.
	* @return int
	*/
	int Compare(const GFdbk& fdbk) const;

	/**
	* Compare two assessements to order them using the document identificator.
	* @see R::RContainer
	* @param fdbk             Pointer to an assessement.
	* @return int
	*/
	int Compare(const GFdbk* fdbk) const;

	/**
	* Compare the document assessed with another document using their
	* identificators.
	* @see R::RContainer
	* @param doc              Identificator of the document.
	* @return int
	*/
	int Compare(unsigned int id) const;

	/**
	* Update the assessment on a given date.
	* @param fdbk             Assessment.
	* @param date             Date.
	*/
	void UpdateFdbk(tDocAssessment fdbk,R::RDate date);

	/**
	* Get the assessment for the document.
	* @returns Profile's assessment.
	*/
	tDocAssessment GetFdbk(void) const {return(Fdbk);}

	/**
	* Get the document assessed.
	* @returns Pointer to the document.
	*/
	GDocProxy* GetDoc(void) const {return(Doc);}

	/**
	* Get the date of the assessment on the document.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Create an erronous assessment with a given percentage. The percentage
	* represents the number of assessments that will be changed in comparison to
	* the original.
	*
	* The changed assessment depends on the original assessment :
	* - If the original assessment is relevant, the changed assessment has a
	*   probability of 0.75 to be fuzzy relevant and a probability of 0.25 to be
	*   irrelevant.
	* - If the original assessment is fuzzy relevant, the changed assessment has
	*   a probability of 0.5 to be relevant and a probability of 0.5 to be
	*   irrelevant.
	* - If the original assessment is irrelevant, the changed assessment has a
	*   probability of 0.75 to be fuzzy relevant and a probability of 0.25 to be
	*   relevant.
	* @param fdbk           Original assessment.
	* @param PercErr        Percentage of error.
	* @param rand           Pointer to the random number generator to use.
	* @returns tDocAssessment
	*/
	static tDocAssessment ErrorJudgment(tDocAssessment fdbk,double PercErr,R::RRandom* rand);

	/*
	* Destructor
	*/
	~GFdbk(void);
};


//------------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile. In fact, it is a
* container of subprofiles, each subprofile corresponding to a language.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public R::RContainer<GSubProfile,false,true>
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
	* Documents assessed by profile.
	*/
	R::RContainer<GFdbk,true,true> Fdbks;

	/**
	* Determine if the profile is social, i.e. prefer to be grouped with
	* someone also if the similarity is not evident rather to stay alone.
	*/
	bool Social;

public:

    /**
	* Constructor of a profile.
	* @param usr            User of the profile.
	* @param id             Identificator of the profile.
	* @param name           Name of the profile.
	* @param s              Social?
	* @param nb             Number of subprofiles.
	* @param nbf            Number of Feedbacks.
	*/
	GProfile(GUser* usr,unsigned int id,const char* name,bool s,unsigned int nb,unsigned int nbf=100);

	/**
	* Compare two profiles by comparing their identificator.
	* @see R::RContainer
	* @param profile         Profile.
	* @return int
	*/
	int Compare(const GProfile& profile) const;

	/**
	* Compare a name with the one of a profile.
	* @see R::RContainer
	* @param name            Name to compare.
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Compare an identificator of a profile with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

proxy:

	/**
	* Get the identificator of the profile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id)  throw(GException);

	/**
	* Get the name of the profile.
	* @return RString.
	*/
	R::RString GetName(void) const;

	/**
	* Get the user of the profile.
	* @return Pointer to the user.
	*/
	GUser* GetUser(void) const {return(User);}

	/**
	* Look if the profile is social.
	* @return boolean value.
	*/
	bool IsSocial(void) const {return(Social);}

	/**
	* Set if the profile is social.
	* @param social         social  value.
	*/
	void SetSocial(bool social);

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
	* Get the number of assessed documents of a given language.
	* @param lang           Pointer to the language.
	* @returns unsigned int.
	*/
	unsigned int GetNbAssessedDocs(const GLang* lang) const;

	/**
	* Get the number of assessed documents.
	* @returns unsigned int.
	*/
	unsigned int GetNbAssessedDocs(void) const;

	/**
	* Get a Cursor on the feedback for the profile.
	* @return GFdbkCursor.
	*/
	R::RCursor<GFdbk> GetFdbks(void);

	/**
	* Get a Cursor on the subprofiles.
	* @return RCursor<GSubProfile>.
	*/
	R::RCursor<GSubProfile> GetSubProfilesCursor(void);

	/**
	* Insert an assessment to the list of the profile.
	* @param id               Identificator of the document.
	* @param assess           Assessment.
	* @param date             Date.
	*/
	void InsertFdbk(unsigned int id,tDocAssessment assess,R::RDate date) throw(std::bad_alloc);

	/**
	* Delete an assessment from the list of the profile.
	* @param id               Identificator of the document.
	*/
	void DeleteFdbk(unsigned int id) throw(std::bad_alloc);

	/**
	* Clear the assessment of the profile.
	*/
	void ClearFdbks(void);

	/**
	* Get the feedback of the profile on a specific document.
	* @param id               Identificator of the document.
	*/
	GFdbk* GetFdbk(unsigned int id) const;

	/**
	* This method is call by a document when it was modified (either because it
	* as analyzed for the first time or because the content has changed).
	* @param id               Identificator of the document.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	*/
	void HasUpdate(unsigned int id,bool computed);

	/**
	* Update the profile. In practice, it constructs for each subprofile the
	* list of assessed documents from the corresponding language.
	* subprofiles.
	*/
	void Update(void);

public:

	/*
	* Destructor of the profile.
	*/
	~GProfile(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
