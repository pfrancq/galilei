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
	* Language of the group.
	*/
	GLang* Lang;

	/**
	* Is the group a virtual community.
	*/
	bool Community;

	/**
	* Date of the update (profiles have changed).
	*/
	R::RDate Updated;

	/**
	* Date of last subprofile computing.
	*/
	R::RDate Computed;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id              Identificator.
	* @param lang            Language.
	* @param com             Community.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GGroup(unsigned int id,GLang* lang,bool com,const R::RDate& u,const R::RDate& c);

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
	* @param id              Identifier.
	*/
	void SetId(unsigned int id);

	/**
	* Load information from the current storage.
	* @param infos           Container hold
	*/
	virtual void LoadInfos(void) const;

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Look if a given subprofile is contained in the group.
	* @param sp              SubProfile to Search.
	* @return true if the subprofile is contained, false else.
	*/
	bool IsIn(const GSubProfile* sp) const;

	/**
	* Delete a subprofile from the group. If the group is a community, the
	* method modifies the assignation of the subprofile (the 'Group' pointer).
	* of the subprofile is set to null).
	* @param sp              SubProfile to delete.
	*/
	void DeleteSubProfile(GSubProfile* sp);

	/**
	* Insert a subprofile in the group. If the group is a community, the
	* method modifies the assignation of the subprofile (the 'Group' pointer).
	* of the subprofile is set to null).
	* @param sp              SubProfile to insert.
	*/
	void InsertSubProfile(GSubProfile* sp);

	/**
	* Insert a subprofile in the group. This signature is needed by a generic
	* k-Means.
	* @param sp              SubProfile to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(GSubProfile* sp);

	 /**
	* Delete all subprofiles.
	*/
	void DeleteSubProfiles(void);

	/**
	* Get a cursor over the subprofiles.
	*/
	R::RCursor<GSubProfile> GetSubProfilesCursor(void) const;

	/**
	* Get a cursor over the subprofiles. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	R::RCursor<GSubProfile> GetCursor(void) const;

	/**
	* Compute the number of subprofiles of a given group that are also in the
	* current one.
	* @param grp             Group.
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
	* @param docs            Documents not assessed.
	* @param s               Subprofile.
	*/
	void NotJudgedDocsList(R::RContainer<GFdbk,false,true>* docs, GSubProfile* s) const;

	/**
	* Construct the list of all relevant documents of the subprofiles of a
	* group not already assessed by a given subprofile and ordered in descending
	* order of their similarity with the chosen subprofile.
	* @param docs            Documents not assessed.
	* @param s               Subprofile.
	* @param session         Session.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(R::RContainer<GFdbk,false,false>* docs, GSubProfile* s,GSession* session) const;

	/**
	* Compute the relevant subprofile, i.e. the subprofiles whith the highest
	* average similarity with all the other subprofiles.
	* @returns Pointer to GSubProfile representing the relevant one.
	*/
	GSubProfile* RelevantSubProfile(void) const;

	/**
	* Compute the relevant subprofile
	* @see R::RGroupingKMeans<cGroup, cObj, cGroupData, cGroups>.
	* @returns Pointer to GSubProfile representing the relevant one.
	*/
	GSubProfile* RelevantObj(void) const {return(RelevantSubProfile());}

	/**
	* Compute the sum of the similarities of a given subprofile to all the
	* others.
	* @param s               Subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(const GSubProfile* s) const;

	/**
	* Clear the vector representing the group.
	*/
	void Clear(void);

	/**
	* Update the group by assigning it a set of information and a language.
	* @param lang            Pointer to the language.
	* @param infos           Pointer to the information.
	* @param computed        The update is called after a computation (and not
	*                        after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed);

	/**
	* This method is call by a subprofile when it was modified.
	* @param sub             Subprofile modified.
	*/
	void HasUpdate(GSubProfile* sub);

	/**
	* Destructor of a group.
	*/
	virtual ~GGroup(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
