/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GGroupH
#define GGroupH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* This class represent a virtual community, i.e. a group of sub-profiles.
* @author Pascal Francq
* @short Group.
*/
class GGroup : protected R::RContainer<GSubProfile,unsigned int,false,true>
{
protected:

	/**
	* identificator of the group.
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
	GGroup(unsigned int id,GLang* lang,bool com) throw(bad_alloc);

	/**
	* Construct a group with an invalid identificator.
	* @param lang           Language.
	* @param com            Community.
	*/
	GGroup(GLang* lang,bool com) throw(bad_alloc);

private:

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return (0);};

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
	virtual bool IsDefined(void) const;

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
	void DeleteSubProfile(GSubProfile* sp) throw(bad_alloc);

	/**
	* Insert a subprofile in the group. If the group is a community, the
	* method modifies the assignation of the subprofile (the 'Group' pointer).
	* of the subprofile is set to null).
	* @param sp             SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfile* sp) throw(bad_alloc);

	/**
	* Insert a subprofile in the group. This signature is needed by a generic
	* k-Means.
	* @param sp             SubProfile to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(GSubProfile* sp) throw(bad_alloc);

	 /**
	* Delete all subprofiles.
	*/
	void DeleteSubProfiles(void) throw(bad_alloc);

	/**
	* Get a cursor over the subprofiles.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

	/**
	* Get a cursor over the subprofiles. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	GSubProfileCursor& GetCursor(void);

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
	void NotJudgedDocsList(R::RContainer<GProfDoc,unsigned,false,true>* docs, GSubProfile* s) const throw(bad_alloc);

	/**
	* Construct the list of all relevant documents of the subprofiles of a
	* group not already assessed by a given subprofile and ordered in descending
	* order of their similarity with the chosen subprofile.
	* @param docs           Documents not assessed.
	* @param s              Subprofile.
	* @param global         Global Similarities.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(R::RContainer<GProfDoc,unsigned,false,false>* docs, GSubProfile* s,bool global) const throw(bad_alloc);

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
	* Add a given information to the group.
	* @param info            Pointer to the information.
	*/
	virtual void AddInfo(GWeightInfo* info) throw(bad_alloc);

	/**
	* Compute the similarity between a group and a document.
	* @param doc             Pointer to a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute a similarity between a group and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute the similarity between a group and subprofile.
	* @param sub             Pointer to a subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a group and a subprofile using a Inverse
	* Frequence Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute the similarity between groups.
	* @param grp             Pointer to a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute a similarity between two groups using a Inverse Frequence Factor
	* (IFF).
	* @param grp             Pointer to a group.
	*/
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Destructor of a group.
	*/
	~GGroup(void) throw(GException);
};


//------------------------------------------------------------------------------
/**
* The GGroupCursor class provides a way to go trough a set of group.
* @short Group Cursor.
*/
CLASSCURSOR(GGroupCursor,GGroup,unsigned int)



}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
