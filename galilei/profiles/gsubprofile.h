/*

	GALILEI Research Project

	GSubProfile.h

	Subprofile - Header.

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
#ifndef GSubProfileH
#define GSubProfileH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation for a user's subprofile.
* @author Pascal Francq.
* @short Subprofile.
*/
class GSubProfile : public GWeightInfos
{
protected:

	/**
	* Identifier of the subprofile.
	*/
	unsigned int Id;

	/**
	* Owner profile of the subprofile.
	*/
	GProfile* Profile;

	/**
	* Language of the subprofile.
	*/
	GLang* Lang;

	/**
	* Identificator of the corresponding group.
	*/
	unsigned int GroupId;

	/**
	* Date of the attachment of the subprofile into the group.
	*/
	R::RDate Attached;

	/**
	* Date of the update of the profile (new assessments).
	*/
	R::RDate Updated;

	/**
	* Date of last subprofile computing.
	*/
	R::RDate Computed;

	/**
	* Documents assessed by the subprofile.
	*/
	R::RContainer<GFdbk,false,false> Fdbks;

public:

	/**
	* Constructor of the subprofile.
	* @param prof            Profile.
	* @param id              Identifier.
	* @param lang            Language of the subprofile.
	* @param grpid           Group identificator.
	* @param a               Date where it was attached.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GSubProfile(GProfile* prof,unsigned int id,GLang* lang,unsigned int grpid,R::RDate a,R::RDate u,R::RDate c);

	/**
	* Compare two subprofiles by comparing their identificator.
	* @see R::RContainer
	* @param subprofile      Subprofile.
	* @return int
	*/
	int Compare(const GSubProfile& subprofile) const;

	/**
	* Compare two subprofiles by comparing their identificator.
	* @see R::RContainer
	* @param subprofile      Pointer to a subprofile.
	* @return int
	*/
	int Compare(const GSubProfile* subprofile) const;

	/**
	* Compare an identificator of a subprofile with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare the language of a subprofile with another one.
	* @see R::RContainer
	* @param lang            Pointer to a language.
	* @return int
	*/
	int Compare(const GLang* lang) const;

	/**
	* Load information from the current storage.
	*/
	virtual void LoadInfos(void) const;

	/**
	* Insert a document into the list of those used to compute the subprofile.
	* @param fdbk             Pointer to the feedback.
	*/
	void InsertFdbk(GFdbk* fdbk);

	/**
	* Get the identificator of the subprofile.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identificator.
	* @param id             Identificator.
	*/
	void SetId(unsigned int id);

	/**
	* Get the language of the subprofile.
	* @return Pointer to the GLang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the profile of the subprofile.
	* @return Pointer GProfile.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* Verify if the subprofile must be (re-)computed.
	*/
	bool MustCompute(void) const;

	/**
	* Get the group holding the subprofile.
	*/
	unsigned int GetGroupId(void) const {return(GroupId);}

	/**
	* Set the group holding the subprofile.
	* @param groupid         Identificator of the group.
	*/
	void SetGroup(unsigned int groupid);

	/**
	* Get the date of the last attachment.
	* @returns R::RDate.
	*/
	R::RDate GetAttached(void) const;

	/**
	* Get the date of the last update of the subprofile.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Get the date of the last analysis of the subprofile.
	* @returns R::RDate.
	*/
	R::RDate GetComputed(void) const;

	/**
	* Verify if the subprofile must be (re-)grouped.
	* @returns Boolean.
	*/
	bool MustGroup(void) const;

	/**
	* Get the number of common OK document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	* @param sub             Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonOKDocs(const GSubProfile* sub) const;

	/**
	* Get the number of common document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	* @param sub             Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonDocs(const GSubProfile* sub) const;

	/**
	* Get the number of common document with different judgement between two
	* subprofiles. In practice, it computes the number of the common documents
	* of the corresponding profiles which are of the language of the profile.
	* @param sub             Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonDiffDocs(const GSubProfile* sub) const;

	/**
	* Get the number of assessed documents.
	* @returns unsigned int.
	*/
	unsigned int GetNbAssessedDocs(void) const {return(Fdbks.GetNb());}

	/**
	* Get a cursor on the feedback for the profile.
	* @return GFdbkCursor.
	*/
	R::RCursor<GFdbk> GetFdbks(void) const;

	/**
	* Update the subprofile by assigning it a set of information and a language.
	* @param lang             Pointer to the language.
	* @param infos            Pointer to the information.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed);

	/**
	* Clear all the assessments of the subprofile.
	*/
	void ClearFdbks(void);

public:

	/**
	* Destructor of a subprofile.
	*/
	virtual ~GSubProfile(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
