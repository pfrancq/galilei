/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

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
#ifndef GGroupH
#define GGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class represent a group of sub-profiles. In fact, it is implemented as a
* container of GSubProfile.
* @author Pascal Francq
* @short Group.
*/
class GGroup : public R::RContainer<GSubProfile,unsigned int,false,true>
{
protected:

	/**
	* identifier
	*/
	unsigned int Id;

	/**
	* State of the document.
	*/
	tObjState State;

	/**
	* Language of the group.
	*/
	GLang* Lang;

private:

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	* Default Constructor.
	*/
	GGroup(void)throw(bad_alloc);

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	*/
	GGroup(const unsigned int id,GLang* lang) throw(bad_alloc);

	/**
	* Construct a group with an invalid identificator.
	* @param lang           Language.
	*/
	GGroup(GLang* lang) throw(bad_alloc);

	/**
	* Copy constructor for group.
	* @param grp            Group.
	*/
	GGroup(GGroup* grp) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return (0);};

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroup& group) const;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GGroup* group) const;

	/**
	* Verify if the group is empty, i.e. it does not have any subprofiles.
	* @returns bool
	*/
	bool IsEmpty(void) const;

	/**
	* Get the identificator of the group.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Return the state of the group.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}
	
	/**
	* Set the state of the group.
	* @param state          New state.
	*/
	void SetState(tObjState state) {State=state;}

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Delete a subprofile from the group.
	* @param sp             SubProfile to delete.
	*/
	void DeleteSubProfile(GSubProfile* sp);

	/**
	* Insert a subprofile in the group.
	* @param sp             SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfile* sp);

	/**
	* Delete all subprofiles.
	*/
	void DeleteSubProfiles(void);

	/**
	* Get a cursor over the subprofiles.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

	/**
	* Compute the number of subprofiles of grp that are in the current group.
	* @param grp            Group to compare with.
	*/
	unsigned int GetNbSubProfiles(GGroup* grp);

	/**
	* Get the number of subprofiles in the group.
	* @param grp            Group to compare with.
	*/
	unsigned int GetNbSubProfiles(void) const;

	/**
	* Construct the list of all feedbacks of the subprofiles of a group not
	* already judged by a given subprofile. If a document is judged multiple
	* times differently, most important OK>N>KO>H.
	* @param docs           Documents not judged.
	* @param s              Subprofile.
	*/
	void NotJudgedDocsList(R::RContainer<GProfDoc,unsigned,false,true>* docs, GSubProfile* s);

	/**
	* Construct the list of all relevant documents of the subprofiles of a
	* group not already judged by a given subprofile and ordered in descending
	* order of their similarity with the chosen subprofile.
	* @param docs           Documents not judged.
	* @param s              Subprofile.
	* @param global         Global Similarities.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(R::RContainer<GProfDoc,unsigned,false,false>* docs, GSubProfile* s,bool global);

	/**
	* Compute the relevant subprofile, i.e. the subprofiles whith the highest
	* average similarity with all the other subprofiles.
	* @param g              Global similarities to use.
	* @returns Pointer to GSubProfile representing the relevant one.
	*/
	GSubProfile* RelevantSubProfile(bool g) const;

	/**
	* returns the relevant subprofiles (needed for kmeans).
	*/
	GSubProfile* RelevantObj() const {return(RelevantSubProfile(0));}

	/**
	* Compute the sum of the similarities of a given profile to all the others.
	* @param s              Profile used as reference.
	* @param g              Global similarities to use.
	* @returns result.
	*/
	double ComputeSumSim(const GSubProfile* s,bool g) const;

	/**
	* Compute the similarity between a group and a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute the global similarity between a group and a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const;

	/**
	* Compute the similarity between a group and subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the global similarity between a group and a subprofile.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const;

	/**
	* Compute the similarity between groups.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute the global similarity between groups.
	*/
	virtual double SimilarityIFF(const GGroup* grp) const;

	/**
	* Destructor
	*/
	~GGroup(void) throw(GException);
};


//-----------------------------------------------------------------------------
/**
* The GGroupCursor class provides a way to go trough a set of group.
* @short Group Cursor.
*/
CLASSCURSOR(GGroupCursor,GGroup,unsigned int)



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
