/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GGroupH
#define GGroupH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a virtual community, i.e. a group of sub-profiles.
* @author Pascal Francq
* @short Group.
*/
class GGroup : protected R::RContainer<GSubProfile,false,true>, public GWeightInfos
{
protected:

	/**
	* Identificator of the group.
	*/
	unsigned int Id;

	/**
	* State of the group.
	*/
	tObjState State;

	/**
	* Language of the group.
	*/
	GLang* Lang;

	/**
	* Is the group a virtual community.
	*/
	bool Community;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	* @param com            Community.
	*/
	GGroup(unsigned int id,GLang* lang,bool com) throw(std::bad_alloc);

	/**
	* Construct a group with an invalid identificator.
	* @param lang           Language.
	* @param com            Community.
	*/
	GGroup(GLang* lang,bool com) throw(std::bad_alloc);

private:

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	* Compare two groups by comparing their identificator.
	* @see R::RContainer
	* @param grp             Group.
	* @return int
	*/
	int Compare(const GGroup& grp) const;

	/**
	* Compare two groups by comparing their identificator.
	* @see R::RContainer
	* @param grp             Pointer to a group.
	* @return int
	*/
	int Compare(const GGroup* grp) const;

	/**
	* Compare the idenfiticator of a group with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Verify if the group is defined. By default, a document is suppose to be
	* undefined.
	* @return false.
	*/
	bool IsDefined(void) const;

	/**
	* Verify if the group is empty, i.e. it does not have any subprofiles.
	* @return bool.
	*/
	bool IsEmpty(void) const;

	/**
	* Look if the group is a community.
	* @return bool.
	*/
	bool IsCommunity(void) const {return(Community);}

	/**
	* Get the identificator of the group.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) throw(GException);

	/**
	* Get a Cursor on the weighted information entities.
	*/
	R::RCursor<GWeightInfo> GetWeightInfoCursor(void);

	/**
	* Return the state of the group.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the group.
	* @param state          New state.
	*/
	void SetState(tObjState state);

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Look if a given subprofile is contained in the group.
	* @param sp             SubProfile to Search.
	* @return true if the subprofile is contained, false else.
	*/
	bool IsIn(const GSubProfile* sp) const;

	/**
	* Delete a subprofile from the group. If the group is a community, the
	* method modifies the assignation of the subprofile (the 'Group' pointer).
	* of the subprofile is set to null).
	* @param sp             SubProfile to delete.
	*/
	void DeleteSubProfile(GSubProfile* sp) throw(std::bad_alloc);

	/**
	* Insert a subprofile in the group. If the group is a community, the
	* method modifies the assignation of the subprofile (the 'Group' pointer).
	* of the subprofile is set to null).
	* @param sp             SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfile* sp) throw(std::bad_alloc);

	/**
	* Insert a subprofile in the group. This signature is needed by a generic
	* k-Means.
	* @param sp             SubProfile to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(GSubProfile* sp) throw(std::bad_alloc);

	 /**
	* Delete all subprofiles.
	*/
	void DeleteSubProfiles(void) throw(std::bad_alloc);

	/**
	* Get a cursor over the subprofiles.
	*/
	R::RCursor<GSubProfile> GetSubProfilesCursor(void);

	/**
	* Get a cursor over the subprofiles. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	R::RCursor<GSubProfile> GetCursor(void);

	/**
	* Compute the number of subprofiles of a given group that are also in the
	* current one.
	* @param grp            Group.
	*/
	unsigned int GetNbSubProfiles(const GGroup* grp) const;

	/**
	* Get the number of subprofiles in the group.
	*/
	unsigned int GetNbSubProfiles(void) const;

	/**
	* Construct the list of all feedbacks of the subprofiles of a group not
	* already assessed by a given subprofile. If a document is assessed multiple
	* times differently, most important OK>N>KO>H.
	* @param docs           Documents not assessed.
	* @param s              Subprofile.
	*/
	void NotJudgedDocsList(R::RContainer<GFdbk,false,true>* docs, GSubProfile* s) const throw(std::bad_alloc);

	/**
	* Construct the list of all relevant documents of the subprofiles of a
	* group not already assessed by a given subprofile and ordered in descending
	* order of their similarity with the chosen subprofile.
	* @param docs           Documents not assessed.
	* @param s              Subprofile.
	* @param session        Session.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(R::RContainer<GFdbk,false,false>* docs, GSubProfile* s,GSession* session) const throw(std::bad_alloc);

	/**
	* Compute the relevant subprofile, i.e. the subprofiles whith the highest
	* average similarity with all the other subprofiles.
	* @param iff             Use the Inverse Frequency Factor.
	* @returns Pointer to GSubProfile representing the relevant one.
	*/
	GSubProfile* RelevantSubProfile(bool iff) const;

	/**
	* Compute the relevant subprofile
	* @see R::RGroupingKMeans<cGroup, cObj, cGroupData, cGroups>.
	* @returns Pointer to GSubProfile representing the relevant one.
	*/
	GSubProfile* RelevantObj(void) const {return(RelevantSubProfile(true));}

	/**
	* Compute the sum of the similarities of a given subprofile to all the
	* others.
	* @param s              Subprofile used as reference.
	* @param iff            Use the Inverse Frequency Factor.
	* @returns result.
	*/
	double ComputeSumSim(const GSubProfile* s,bool iff) const;

	/**
	* Compute the similarity between a group and a document.
	* @param doc             Pointer to a document.
	*/
	double Similarity(const GDoc* doc) const;

	/**
	* Compute a similarity between a group and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute the similarity between a group and subprofile.
	* @param sub             Pointer to a subprofile.
	*/
	double Similarity(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a group and a subprofile using a Inverse
	* Frequence Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute the similarity between groups.
	* @param grp             Pointer to a group.
	*/
	double Similarity(const GGroup* grp) const;

	/**
	* Compute a similarity between two groups using a Inverse Frequence Factor
	* (IFF).
	* @param grp             Pointer to a group.
	*/
	double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Clear the vector representing the group.
	*/
	void Clear(void);

	/**
	* Update the group by assigning it a set of information and a language.
	* @param infos            Pointer to the information.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed);

	/**
	* This method is call by a subprofile when it was modified (either because it
	* as analyzed for the first time or because the content has changed).
	* @param id               Identificator of the subprofile.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	*/
	void HasUpdate(unsigned int id,bool computed);

	/**
	* Destructor of a group.
	*/
	virtual ~GGroup(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
