/*

	GALILEI Research Project

	GProfDoc.h

	Profile's Assessment on a document - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#ifndef GProfDocH
#define GProfDocH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfDoc class provides a representation for assessment of a given profile
* on a given document.
* @author Pascal Francq
* @short Profile's Assessment.
*/
class GProfDoc
{
	/**
	* Document assessed.
	*/
	GDoc* Doc;

	/**
	* Profile.
	*/
	GProfile* Profile;

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
	* @param doc            Pointer to the document assessed.
	* @param prof           Pointer to the profile.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	GProfDoc(GDoc* doc,GProfile* prof,tDocAssessment fdbk,const char* date);

	/**
	* Constructor.
	* @param doc            Pointer to the document assessed.
	* @param prof           Pointer to the profile.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	GProfDoc(GDoc* doc,GProfile* prof,tDocAssessment fdbk,R::RDate& date);

	/**
	* Compare two assessements to order them using the document identificator as
	* first index and the sybprofile identificator as the second one.
	* @see R::RContainer
	* @param profdoc        Assessement.
	* @return int
	*/
	int Compare(const GProfDoc &profdoc) const;

	/**
	* Compare two assessements to order them using the document identificator as
	* first index and the sybprofile identificator as the second one.
	* @see R::RContainer
	* @param profdoc        Pointer to an assessement.
	* @return int
	*/
	int Compare(const GProfDoc *profdoc) const;

	/**
	* Compare the document assessed with another document using their
	* identificators.
	* @see R::RContainer
	* @param doc             Pointer to a document.
	* @return int
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Update the assessment on a given date.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	void UpdateFdbk(tDocAssessment fdbk,const char* date);

	/**
	* Update the assessment on a given date.
	* @param fdbk           Assessment.
	* @param date           Date.
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
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	* Get the profile that has assessed the document.
	* @returns Pointer to the profile.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* Get the date of the last assessment of the profile on the document.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Look if the assessment is considered to be treated. If the assessment was
	* made (or updated) later than the last computing of the subprofile of the
	* language corresponding to the document, this assessment is considered to
	* be not treated.
	* @return bool
	*/
	bool FdbkTreated(void) const;

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
	~GProfDoc(void);
};


//------------------------------------------------------------------------------
/**
* The GProfDocCursor class provides a way to go trough a set of feedbacks.
* @short Feedbacks Cursor
*/
CLASSCURSOR(GProfDocCursor,GProfDoc,unsigned int);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
