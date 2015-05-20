/*

	GALILEI Research Project

	GProfile.h

	Profile - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <gdescriptionobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile, i.e. a description
* of some knowledge attached to people. This can be an interest, an expertise,
* etc.
*
* Each profile is associated to a set feedbacks on documents.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public GDescriptionObject<GProfile>
{
	/**
	* The owner of the profile.
	*/
	GUser* User;

	/**
	 * Type of the profile.
	 */
	tProfileType Type;

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
	* @param session         Session.
	* @param usr             User of the profile.
	* @param type            Type of the profile.
	* @param name            Name of the profile.
	* @param s               Social?
	*/
	GProfile(GSession* session,GUser* usr,tProfileType type,const R::RString name,bool s);

    /**
	* Construct a profile.
	* @param session         Session.
	* @param usr             User of the profile.
	* @param type            Type of the profile.
	* @param id              Identifier of the profile.
	* @param blockid         Identifier of the block.
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
	GProfile(GSession* session,GUser* usr,tProfileType type,size_t id,size_t blockid,const R::RString name,
			 size_t grpid,R::RDate a,R::RDate u,R::RDate c,bool s,double score,char level,size_t nbf);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GProfile");}

	/**
	 * Give the type of the object.
    * @return otProfile
    */
	static inline tObjType GetType(void) {return(otProfile);}

	/**
	* Compare two profiles by comparing their identifier.
	* @see R::RContainer
	* @param profile         Profile.
	* @return int
	*/
	int Compare(const GProfile& profile) const;

	/**
	 * @return the type of the profile.
	 */
	tProfileType GetProfileType(void) const {return(Type);}

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
	* Get the date of the last update of the profile. It may be the date of the
	* latest document feedback or an update of one of the documents on which some
	* feedback exists.
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
	* Compute the agreement ratio between two profiles, i.e. the ratio between
	* the number of documents assessed as relevant by both profiles, and the
	* total number of documents assessed by both profiles.
	* @param prof            Second profile.
	* @param nbmin           Minimum number of common documents for a non null
	*                        ratio.
	*/
	double GetAgreementRatio(const GProfile* prof,size_t nbmin) const;

	/**
	* Compute the disagreement ratio between two profiles, i.e. the ratio
	* between the number of documents assessed as relevant by one of the
	* profiles and differently by the other one, and the total number of
	* documents assessed by both profiles.
	* @param prof            Second profile.
	* @param nbmin           Minimum number of common documents for a non null
	*                        ratio.
	*/
	double GetDisagreementRatio(const GProfile* prof,size_t nbmin) const;

	/**
	* Get the number of assessed documents.
	*/
	size_t GetNbAssessedDocs(void) const;

	/**
	* Get a Cursor on the feedback for the profile.
	*/
	R::RCursor<GFdbk> GetFdbks(void) const;

	/**
	* Get the feedback of the profile on a specific document.
	* @param docid           Identifier of the document.
	*/
	GFdbk* GetFdbk(size_t docid) const;

private:

	/**
	* Add a feedback to the list of the profile.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param done            Date of the assessment.
	* @return true if it is a new feedback.
	*/
	bool InsertFdbk(size_t docid,tFdbkType fdbk,const R::RDate& done);

	/**
	* Delete a feedback from the list of the profile.
	* @param docid           Identifier of the document.
	*/
	void DeleteFdbk(size_t docid);

	/**
	* Update the representation of the profile once a computation was done. The
	* computed date and the status are updated.
	*
	* If the profile is an internal one, the following steps are done:
	* -# An 'hProfiles[oeAboutToBeUpdated]' notification is send.
	* -# The references are updated.
	* -# Existing in memory description is replaced.
	* -# If necessary, the profile and its description are saved.
	* -# An 'hProfiles[oeUpdated]' notification is send.
	*
	* If the profile is an external one, its description is replaced.
	* @param desc            Description to assign.
	* \warning The description is cleared by this method.
	*/
	void Update(GDescription& desc);

	/**
	 * Clear the information associated with the profile.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void Clear(bool disk);

	/**
	* Clear the assessment of the profile.
	*/
	void ClearFdbks(void);

	/**
	* This method is call by a document when it was modified.
	* @param docid           Identifier of the document.
	*/
	void WasUpdated(size_t docid);

public:

	/**
	* Destructor of the profile. It is removed from its user and community.
	*/
	~GProfile(void);

	friend class GSession;
	friend class GDoc;
	friend class GObjects<GProfile,hProfiles>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
