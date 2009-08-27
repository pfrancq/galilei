/*

	GALILEI Research Project

	GProfile.h

	Profile - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <galilei.h>
#include <gweightinfosobj.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFdbk class provides a representation for assessment on a document.
* @author Pascal Francq
* @short Document Assessment.
*/
class GFdbk
{
	/**
	* Identifier of the document assessed.
	*/
	size_t DocId;

	/**
	* Assessment of the profile.
	*/
	tDocAssessment Fdbk;

	/**
	* When the assessment was made.
	*/
	R::RDate When;

	/**
	* Date of the last computation of the document when it was modified.
	*/
	R::RDate Computed;

public:

	/**
	* Constructor.
	* @param docid           Identifier of the document.
	* @param fdbk            Assessment.
	* @param when            Date.
	* @param computed        Last computation.
	*/
	GFdbk(size_t docid,tDocAssessment fdbk,const R::RDate& when,const R::RDate& computed);

	/**
	* Compare two assessments to order them using the document identifier.
	* @see R::RContainer
	* @param fdbk            Assessment.
	* @return int
	*/
	int Compare(const GFdbk& fdbk) const;

	/**
	* Compare two assessments to order them using the document identifier.
	* @see R::RContainer
	* @param fdbk            Pointer to an assessment.
	* @return int
	*/
	int Compare(const GFdbk* fdbk) const;

	/**
	* Compare the document assessed with another document using their
	* identifiers.
	* @see R::RContainer
	* @param id              Identifier of the document.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* New assessment.
	* @param fdbk            Assessment.
	* @param date            Date.
	*/
	void NewFdbk(tDocAssessment fdbk,const R::RDate& date);

	/**
	* Get the assessment for the document.
	* @returns Profile's assessment.
	*/
	tDocAssessment GetFdbk(void) const {return(Fdbk);}

	/**
	* Get the identifier of the document assessed.
	* @returns size_t.
	*/
	size_t GetDocId(void) const {return(DocId);}

	/**
	* Get the date of the assessment on the document.
	* @returns R::RDate.
	*/
	R::RDate GetWhen(void) const;

	/**
	* Get the date of last computation of the document.
	* @returns R::RDate.
	*/
	R::RDate GetComputed(void) const;

	/**
	* Must the assessment be used to compute a profile.
	* @param profile         profile.
	*/
	bool MustUse(const GProfile* profile) const;

	/**
	* The document assessed was updated.
	*/
	void HasUpdate(void);

	/**
	* Create an erroneous assessment with a given percentage. The percentage
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
	* @param fdbk            Original assessment.
	* @param PercErr         Percentage of error.
	* @param rand            Pointer to the random number generator to use.
	* @returns tDocAssessment
	*/
	static tDocAssessment ErrorJudgment(tDocAssessment fdbk,double PercErr,R::RRandom* rand);

	/*
	* Destruct the feedback.
	*/
	~GFdbk(void);
};


//------------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public GWeightInfosObj
{
protected:

	/**
	* The owner of the profile.
	*/
	GUser* User;

	/**
	* Documents assessed by profile.
	*/
	R::RContainer<GFdbk,true,true> Fdbks;

	/**
	* Determine if the profile is social, i.e. prefer to be grouped with
	* someone also if the similarity is not evident rather to stay alone.
	*/
	bool Social;

	/**
	* Date of the update of the profile (new assessments).
	*/
	R::RDate Updated;

	/**
	* Date of last profile computing.
	*/
	R::RDate Computed;

	/**
	* Identifier of the corresponding community.
	*/
	size_t GroupId;

	/**
	* Date of the attachment of the profile into the community.
	*/
	R::RDate Attached;

	/**
	 * Confidence score (to maximize).
	 */
	double Score;

	/**
	 * Confidence Level (to maximize).
	 */
	char Level;

public:

    /**
	* Construct a new profile.
	* @param usr             User of the profile.
	* @param name            Name of the profile.
	* @param s               Social?
	*/
	GProfile(GUser* usr,const R::RString name,bool s);

    /**
	* Construct a profile.
	* @param usr             User of the profile.
	* @param id              Identifier of the profile.
	* @param blockid        Identifier of the block.
	* @param name            Name of the profile.
	* @param grpid           Community identifier.
	* @param a               Date where it was attached.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	* @param s               Social?
	* @param score           Score of the profile.
	* @param level           Level of the profile.
	* @param nbf             Number of Feedbacks.
	*/
	GProfile(GUser* usr,size_t id,size_t blockid,const R::RString name,size_t grpid,R::RDate a,R::RDate u,R::RDate c,bool s,double score,char level,size_t nbf);

	/**
	* Compare two profiles by comparing their identifier.
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
	* Compare an identifier of a profile with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	 * Set the name of the profile.
	 * @param name           Name of the profile.
	 * @return
	 */
	void SetName(const R::RString& name);

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
	* @param social          Social value.
	*/
	void SetSocial(bool social);

	/**
	* Get the date of the last update of the profile.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const {return(Updated);}

	/**
	* Get the date of the last analysis of the profile.
	* @returns R::RDate.
	*/
	R::RDate GetComputed(void) const {return(Computed);}

	/**
	* Verify if the profile must be (re-)computed.
	*/
	bool MustCompute(void) const;

	/**
	* Get the community holding the profile.
	*/
	size_t GetGroupId(void) const {return(GroupId);}

	/**
	* Set the community holding the profile.
	* @param groupid         Identifier of the group.
	*/
	void SetGroup(size_t groupid);

	/**
	* Get the date of the last attachment.
	* @returns R::RDate.
	*/
	R::RDate GetAttached(void) const;

	/**
	 * The confidence score represent an absolute measure of the confidence of
	 * a given profile.
	 * @return the confidence score of the profile.
	 */
	double GetConfidenceScore(void) const {return(Score);}

	/**
	 * The confidence level represent a relative measure of the confidence of a
	 * given profile in comparison to all other profiles.
	 * @return the confidence level of the profile.
	 */
	char GetConfidenceLevel(void) const {return(Level);}

	/**
	 * Set the confidence score and level of the profile.
	 * @param score          Confidence level.
	 * @param level          Confidence score.
	 */
	void SetConfidence(double score,char level);

	/**
	* Get the number of common OK document between two profiles.
	* @param prof            Pointer to a profile.
	*/
	size_t GetCommonOKDocs(const GProfile* prof) const;

	/**
	* Get the number of common document between two profiles.
	* @param prof            Pointer to a profile.
	*/
	size_t GetCommonDocs(const GProfile* prof) const;

	/**
	* Get the number of common document with different assessments between two
	* profiles.
	* @param prof            Pointer to a profile.
	*/
	size_t GetCommonDiffDocs(const GProfile* prof) const;

	/**
	* Get the number of assessed documents.
	*/
	size_t GetNbAssessedDocs(void) const;

	/**
	* Get a Cursor on the feedback for the profile.
	*/
	R::RCursor<GFdbk> GetFdbks(void) const;

	/**
	* Insert an assessment to the list of the profile.
	* @param docid           Identifier of the document.
	* @param assess          Assessment.
	* @param date            Date.
	* @param update          Last update of the document.
	*/
	void InsertFdbk(size_t docid,tDocAssessment assess,const R::RDate& date,const R::RDate& update);

	/**
	* Delete an assessment from the list of the profile.
	* @param docid           Identifier of the document.
	*/
	void DeleteFdbk(size_t docid);

	/**
	* Assign a new description to the profile.
	* @param infos            Pointer to the information.
	* \warning The container infos is cleared by this method.
	*/
	void Update(GWeightInfos& infos);

	/**
	* Clear the assessment of the profile.
	*/
	void ClearFdbks(void);

	/**
	* Get the feedback of the profile on a specific document.
	* @param docid           Identifier of the document.
	*/
	GFdbk* GetFdbk(size_t docid) const;

	/**
	* This method is call by a document when it was modified.
	* @param docid           Identifier of the document.
	*/
	void HasUpdate(size_t docid);

public:

	/**
	* Destructor of the profile.
	*/
	~GProfile(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
