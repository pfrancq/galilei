/*

	GALILEI Research Project

	GProfDoc.h

	Profile's Judgement of a document - Header.

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
#ifndef GProfDocH
#define GProfDocH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfDoc class provides a representation for assessment of a given profile
* on a given document.
* @author Pascal Francq
* @short Profile Assessment.
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
	* Feedback of the profile.
	*/
	tDocJudgement Fdbk;

	/**
	* Last Updated of this feedback.
	*/
	R::RDate Updated;

public:

	/**
	* Constructor.
	* @param doc            Pointer to the document judged.
	* @param prof           Pointer to the profile.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
    GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,const char* date);

	/**
	* Constructor.
	* @param doc            Pointer to the document judged.
	* @param prof           Pointer to the profile.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,R::RDate date);

	/**
	* Compare mathod used by R::RContainer.
	*/
	int Compare(const GProfDoc &profdoc) const;

	/**
	* Compare mathod used by R::RContainer.
	*/
	int Compare(const GProfDoc *profdoc) const;

	/**
	* Compare mathod used by R::RContainer for the document part.
	* @param doc            Pointer to the document.
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Update the assessment on a given date.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	void UpdateFdbk(tDocJudgement fdbk,const char* date);

	/**
	* Update the assessment on a given date.
	* @param fdbk           Assessment.
	* @param date           Date.
	*/
	void UpdateFdbk(tDocJudgement fdbk,R::RDate date);

	/**
	* Get the assessment for the document.
	* @returns Profile's feedback.
	*/
	tDocJudgement GetFdbk(void) const;

	/**
	* Get the document assessed.
	* @returns Pointer to the document.
	*/
	GDoc* GetDoc(void) const;

	/**
	* Get the profile that has assessed.
	* @returns Pointer to the profile.
	*/
	GProfile* GetProfile(void) const;

	/**
	* Get the date of the last assessment of the profile on the document.
	* @returns Pointer to a date.
	*/
	const R::RDate* GetUpdated(void) const;

	/**
	* Look if the assessment is considered to be treated. If the assessment was
	* made (or updated) later than the last computing of the subprofile of the
	* language corresponding to the document, this assessment is considered to
	* be not treated.
	* @return bool
	*/
	bool FdbkTreated(void) const;

	/**
	* Get the similarity between the document and the profile. In particular, if
	* the profile does not have a subprofile of the language of the document,
	* the similarity is null.
	* @return double.
	*/
	double Similarity(void);

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
	* @returns tDocJudgement
	*/
	static tDocJudgement ErrorJudgment(tDocJudgement fdbk,double PercErr,R::RRandom* rand);

	/*
	* Destructor
	*/
	~GProfDoc(void);
};


//-----------------------------------------------------------------------------
/**
* The GProfDocCursor class provides a way to go trough a set of feedbacks.
* @short Feedbacks Cursor
*/
CLASSCURSOR(GProfDocCursor,GProfDoc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
