/*

	GALILEI Research Project

	GSubProfile.h

	Sub-Profile - Header.

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
#ifndef GSubProfileH
#define GSubProfileH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class represents a sub-profile for a specific language. The sub-profiles
* are ordered by identificator. A sub-profile is in reality a container of
* sub-profile descriptions.
* @author Pascal Francq.
* @short Sub-Profile.
*/
class GSubProfile
{
protected:

	/**
	* Identifier of the subprofile.
	*/
	unsigned Id;

	/**
	* Owner profile of the subprofile.
	*/
	GProfile* Profile;

	/**
	* Language of the subprofile.
	*/
	GLang* Lang;

	/**
	* Corresponding Group.
	*/
	GGroup* Group;

	/**
	* State of the subprofile.
	*/
	tObjState State;

	/**
	* Date of attachment to the group.
	*/
	R::RDate Attached;

	/**
	* Date of Update.
	*/
	R::RDate Updated;

	/**
	* Date of last document's analysis.
	*/
	R::RDate Computed;

	/**
	* Documents juged by the subprofile.
	*/
	R::RContainer<GProfDoc,unsigned,false,true> Fdbks;

#if GALILEITEST
	/**
	* Subject of the subprofile.
	*/
	GSubject* Subject;
#endif

public:

	/**
	* Constructor of the subprofile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const=0;

	/**
	* Create a new profile.
	* @param GProfile*      Pointer to the profile.
	* @param unsigned int   Identifier.
	* @param GLang*         Language of the subprofile.
	* @param GGroup*        Pointer to the group.
	* @param const char*    C String representing the date where it was attached.
	*/
	static GSubProfile* NewSubProfile(GProfile*,unsigned int,GLang*,GGroup*,const char*) {return(0);}

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GSubProfile& subprofile) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GSubProfile* subprofile) const;

	/*
	* add a judgement for the subprofile.
	*/
	void AddJudgement(GProfDoc* j) throw(bad_alloc);

	/**
	* Remove a judgement for this subprofile.
	* @param j              Judgement.
	*/
	void RemoveJudgement(GProfDoc* j) throw(bad_alloc);

	/**
	* Method called when the profiles feedbacks were cleared.
	*/
	virtual void ClearFdbks(void);

	/**
	* Get the identifier of the subprofile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* returns the state of the subprofile
	*/
	tObjState GetState(void) const {return State;}

	/*
	* set the state of the subprofile
	*/
	void SetState(tObjState state);

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the corresponding profile.
	* @return Pointer to the profile.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const {return(false);};

	/**
	* Get the corresponding group.
	* @returns Pointer to GGroup.
	*/
	GGroup* GetGroup(void) const {return(Group);}

	/**
	* Set the group for the subprofile.
	* @params grp           Group where to attached.
	*/
	void SetGroup(GGroup* grp);

	/**
	* Get the date of the last attachment.
	* @returns Pointer to date.
	*/
	const R::RDate* GetAttached(void) const {return(&Attached);}

	/**
	* Get the date of the last update of the subprofile.
	* @returns Pointer to date.
	*/
	const R::RDate* GetUpdated(void) const {return(&Updated);}

	/**
	* Get the date of the last analysis of the subprofile.
	* @returns Pointer to date.
	*/
	const R::RDate* GetComputed(void) const {return(&Computed);}

	/**
	* See if the subprofiles was updated until the last attachment to a group.
	* @returns Boolean.
	*/
	bool IsUpdated(void) const;

	/**
	* Get the number of common OK document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonOKDocs(const GSubProfile* prof);

	/**
	* Get the number of common document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonDocs(const GSubProfile* prof) ;

	/**
	* Get the number of common document with different judgement between two
	* subprofiles. In practice, it computes the number of the common documents
	* of the corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonDiffDocs(const GSubProfile* prof);

	/**
	* Get the number of judged documents.
	* @returns unsigned int.
	*/
	unsigned int GetNbJudgedDocs(void) const {return (Fdbks.NbPtr);}

	/**
	* Get the feedback of the subprofile on a specific document.
	* @param doc            Pointer to the document.
	* return Pointer to the feedback or 0 if the document wasn't judged by the
	*        profile.
	*/
	GProfDoc* GetFeedback(const GDoc* doc) const;

	/**
	* Get a Cursor on the feedback for the profile.
	* @return GProfDocCursor.
	*/
	GProfDocCursor& GetProfDocCursor(void);

	/**
	* Compute the similarity between a subprofile and a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute the global similarity between a subprofile and a document.
	*/
	virtual double GlobalSimilarity(const GDoc* doc) const;

	/**
	* Compute the similarity between subprofiles.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the global similarity between subprofiles.
	*/
	virtual double GlobalSimilarity(const GSubProfile* sub) const;

	/**
	* Compute the similarity between a subprofile and a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute the global similarity between a subprofile and a group.
	*/
	virtual double GlobalSimilarity(const GGroup* grp) const;

	virtual GIWordWeightCursor& GetVectorCursor(void)=0;

	/**
	* Tell the subprofile that its updated is finished.
	*/
	void UpdateFinished(void);

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

	/**
	*Destructor
	*/
	virtual ~GSubProfile(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfileCursor class provides a way to go trough a set of subprofiles.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GSubProfileCursor,GSubProfile,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

