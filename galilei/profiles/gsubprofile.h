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
#include <sessions/galilei.h>
#include <infos/gweightinfos.h>


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
	* @param grp             Group.
	* @param a               String representing the date where it was attached.
	* @param u               String representing the date of the last updated.
	* @param c               String representing the date of the last computation.
	*/
	GSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,const char* u,const char* c);

	/**
	* Constructor of the subprofile.
	* @param session         Session.
	* @param prof            Profile.
	* @param lang            Language of the subprofile.
	*/
	GSubProfile(GSession* session,GProfile* prof,GLang* lang);

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

proxy:

	/**
	* Insert a document into the list of those used to compute the subprofile.
	* @param fddk             Pointer to the feedback.
	*/
	void InsertFdbk(GFdbk* fdbk) throw(std::bad_alloc);

	/**
	* Get the identificator of the subprofile.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identificator.
	* @param id             Identificator.
	*/
	void SetId(unsigned int id) throw(GException);

	/**
	* Get the state of the subprofile.
	* @return tObjState.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the subprofile
	* @param state           New state of the subprofile.
	*/
	void SetState(tObjState state);

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
	* Verify if the subprofile is defined, i.e. it has a list of information
	* entities.
	*/
	bool IsDefined(void) const;

	/**
	* Verify if the subprofile can be computed, i.e. when some documents are
	* associated to it.
	*/
	bool CanCompute(void);

	/**
	* Get the group holding the subprofile.
	* @returns Pointer to GGroup.
	*/
	GGroup* GetGroup(void) const {return(Group);}

	/**
	* Set the group holding the subprofile.
	* @param grp            Pointer to the group.
	*/
	void SetGroup(GGroup* grp);

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
	* See if the subprofile was updated until the last time he was inserted into
	* a group.
	* @returns Boolean.
	*/
	bool IsUpdated(void) const;

	/**
	* Look if a given document has the same description since the last time it
	* was used to compute the subprofile.
	*/
	bool SameDescription(const GFdbk* fdbk) const;

	/**
	* Get the number of common OK document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	* @param prof            Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonOKDocs(const GSubProfile* prof);

	/**
	* Get the number of common document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	* @param prof            Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonDocs(const GSubProfile* prof) ;

	/**
	* Get the number of common document with different judgement between two
	* subprofiles. In practice, it computes the number of the common documents
	* of the corresponding profiles which are of the language of the profile.
	* @param prof            Pointer to a subprofile.
	* @return unsigned int.
	*/
	unsigned int GetCommonDiffDocs(const GSubProfile* prof);

	/**
	* Get the number of assessed documents.
	* @returns unsigned int.
	*/
	unsigned int GetNbAssessedDocs(void) const {return(Fdbks.NbPtr);}

	/**
	* Get a cursor on the feedback for the profile.
	* @return GFdbkCursor.
	*/
	R::RCursor<GFdbk> GetFdbks(void);

	/**
	* Get a Cursor on the weighted information entities.
	*/
	R::RCursor<GWeightInfo> GetWeightInfoCursor(void);

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	* Compute the similarity between a subprofile and a document.
	* @param doc             Pointer to a document.
	*/
	double Similarity(const GDoc* doc) const;

	/**
	* Compute the similarity between a subprofile and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute the similarity between a subprofile and a document.
	* @param doc             Pointer to a proxy on a document.
	*/
	double Similarity(const GDocProxy* doc) const;

	/**
	* Compute the similarity between a subprofile and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a proxy on a document.
	*/
	double SimilarityIFF(const GDocProxy* doc) const throw(GException);

	/**
	* Compute the similarity between subprofiles.
	* @param sub             Pointer to a subprofile.
	*/
	double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the similarity between subprofiles using a Inverse Frequence
	* Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute the similarity between a subprofile and a group.
	* @param grp             Pointer to a group.
	*/
	double Similarity(const GGroup* grp) const;

	/**
	* Compute the similarity between a subprofile and a group using a Inverse
	* Frequence Factor (IFF).
	* @param grp             Pointer to a group.
	*/
	double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Update the subprofile by assigning it a set of information and a language.
	* @param infos            Pointer to the information.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed);

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
