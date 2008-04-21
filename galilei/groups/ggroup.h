/*

	GALILEI Research Project

	GGroup.h

	Group - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
* This class represent a virtual community, i.e. a group of profiles.
* @author Pascal Francq
* @short Group.
*/
class GGroup : protected R::RContainer<GProfile,false,true>, public GWeightInfos
{
protected:

	/**
	* Identificator of the group.
	*/
	unsigned int Id;

	/**
	* Is the group a virtual community.
	*/
	bool Community;

	/**
	* Date of the update (profiles have changed).
	*/
	R::RDate Updated;

	/**
	* Date of last profile computing.
	*/
	R::RDate Computed;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id              Identificator.
	* @param com             Community.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GGroup(unsigned int id,bool com,const R::RDate& u,const R::RDate& c);

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
	*/
	virtual void LoadInfos(void) const;

	/**
	 * Look if a given profile is in the group.
	 * @param prof           Profile.µ
	 */
	bool IsIn(const GProfile* prof) const;
	
	/**
	* Delete a profile from the group. If the group is a community, the
	* method modifies the assignation of the profile (the 'Group' pointer).
	* of the profile is set to null).
	* @param prof            Profile to delete.
	*/
	void DeleteProfile(GProfile* prof);

	/**
	* Insert a profile in the group. If the group is a community, the
	* method modifies the assignation of the profile (the 'Group' pointer).
	* of the profile is set to null).
	* @param prof            Profile to insert.
	*/
	void InsertProfile(GProfile* sp);

	/**
	* Insert a profile in the group. This signature is needed by a generic
	* k-Means.
	* @param prof            Profile to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(GProfile* prof);

	 /**
	* Delete all profiles.
	*/
	void DeleteProfiles(void);

	/**
	* Get a cursor over the profiles.
	*/
	R::RCursor<GProfile> GetProfiles(void) const;

	/**
	* Get a cursor over the profiles. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	R::RCursor<GProfile> GetCursor(void) const;

	/**
	* Compute the number of profiles of a given group that are also in the
	* current one.
	* @param subject         Pointer to the subject.
	*/
	unsigned int GetNbProfiles(const GSubject* subject) const;

	/**
	* Get the number of profiles in the group.
	*/
	unsigned int GetNbProfiles(void) const;

	/**
	* Construct the list of all feedbacks of the profiles of a group not
	* already assessed by a given profile. If a document is assessed multiple
	* times differently, most important OK>N>KO>H.
	* @param docs            Documents not assessed.
	* @param prof            Profile.
	*/
	void NotJudgedDocsList(R::RContainer<GFdbk,false,true>* docs, GProfile* prof) const;

	/**
	* Construct the list of all relevant documents of the profiles of a
	* group not already assessed by a given profile and ordered in descending
	* order of their similarity with the chosen profile.
	* @param measure         The measure used to compute the similarities.
	* @param docs            Documents not assessed.
	* @param prof            Profile.
	* @param session         Session.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(GMeasure* measure,R::RContainer<GFdbk,false,false>* docs, GProfile* prof,GSession* session) const;

	/**
	* Compute the relevant profile, i.e. the profile whith the highest
	* average similarity with all the other profiles.
	* @returns Pointer to GProfile representing the relevant one.
	*/
	GProfile* RelevantProfile(void) const;

	/**
	* Compute the relevant profile.
	* @see R::RGroupingKMeans<cGroup, cObj, cGroupData, cGroups>.
	* @returns Pointer to GProfile representing the relevant one.
	*/
	GProfile* RelevantObj(void) const {return(RelevantProfile());}

	/**
	* Compute the sum of the similarities of a given profile to all the
	* others.
	* @param measure         The measure used to compute the similarities.
	* @param prof            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GMeasure* measure,const GProfile* prof) const;

	/**
	* Clear the vector representing the group.
	*/
	void Clear(void);

	/**
	* Update the group by assigning it a set of information.
	* @param infos           Pointer to the information.
	* @param computed        The update is called after a computation (and not
	*                        after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed);

	/**
	* This method is call by a profile when it was modified.
	* @param prof            Profile modified.
	*/
	void HasUpdate(GProfile* prof);

	/**
	* Destructor of a group.
	*/
	virtual ~GGroup(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
