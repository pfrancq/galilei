/*

	GALILEI Research Project
	
	GSubjectTree.h

	GSubjectTree - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GSubjectTreeH
#define GSubjectTreeH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rtree.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSubjectTree class provides a representation for a tree of subject
* handled by the system.
* @author GALILEI Team
* @short Tree of the Galilei subject.
*/
class GSubjectTree : public R::RTree<GSubject,true,false>, public GParams
{
protected:

	class GroupScore;
	class GGroupId;
	CLASSCURSOR(GroupScoreCursor,GroupScore,unsigned int)

	/**
	* The Users of the galilei system.
	*/
	GSession* Session;

	/**
	* The percentage of ok documents.
	*/
	double PercOK;

	/**
	* The percentage of ko documents.
	*/
	double PercKO;

	/**
	* The percentage of out of the scope documents compare to Ok documents.
	*/
	double PercH;

	/**
	* The Percentage of documents judged whith error.
	*/
	double PercErr;

	/**
	* The Percentage of Group to use.
	*/
	double PercGrp;

	/**
	* The minimal number of docs in the group to keep it.
	*/
	unsigned int NbMinDocsGrp;

	/**
	* Parameter to initialyse the random function for autocreate jugment.
	* 0 to use randomly, >0 to use whit the same su sequence
	*/
	int Rand;

	/**
	* The maximal number of profiles in a group.
	*/
	unsigned int NbProfMax;

	/**
	* The minimal number of profiles in a group.
	*/
	unsigned int NbProfMin;

	/**
	* The % of profiles who are social	
	*/
	double PercSocial;

	/**
	* Temporary Array of documents.
	*/
	GDoc** Docs;

	/**
	* Number of documents actually in Docs.
	*/
	unsigned int NbDocs;

	/**
	* New documents to judge.
	*/
	R::RContainer<GProfDoc,unsigned,false,false> NewDocs;

	/**
	* The date of today.
	*/
	char today[12];

	/**
	* Container of last added subprofiles.
	*/
	R::RContainer<GSubProfile,unsigned int,false,true> LastAdded;

	/**
	* The number of documents judged by user feeddback.
	*/
	unsigned int NbDocsAssess;

	/**
	* Global Similarity must be used to compute the relevance of documents.
	*/
	bool Global;

	/**
	* Ideal groups created.
	*/
	R::RContainer<GGroups,unsigned int,true,true> IdealGroups;

	/**
	* Precision of the groupement.
	*/
	double Precision;

	/**
	* Recall of the groupement.
	*/
	double Recall;

	/**
	* Total comparaison between for groupment.
	*/
	double Total;

	/**
	* SubProfiles that where changed and must be grouped again.
	*/
	R::RContainer<GroupScore,unsigned int,true,true> GroupsScore;

public:

	/**
	* Constructor.
	*/
	GSubjectTree(GSession* session);

protected:

	/**
	* Assign the values of the parameters to the corresponding variables.
	*/
	void Apply(void);

	/**
	* Choose the subjects that will be used.
	*/
	void ChooseSubjects(void);

	/**
	* Create the set based on the subjects used.
	*/
	void CreateSet(void);

	/**
	* Simulatation of judgments for a profile for a given subject.
	* @param prof           Profile that judges the documents.
	* @param sub            Subject.
	* @param maxDocOK       Maximal OK Documents to judge.
	* @param maxDocKO       Maximal KO Documents to judge.
	* @param maxDocH        Maximal H Documents to judge.
	*/
	void ProfileJudges(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH);

	/**
	* Get the ideal group of the subprofile.
	* @param sub            SubProfile.
	*/
	GGroup* GetIdealGroup(GSubProfile* sub) const;

	/**
	* Compute the Recall and the Precision.
	* @param rec            Receiver of the signals.
	*/
	void ComputeRecallPrecision(GSlot* rec=0);

	/**
	* Compute the Total based on the Adjusted Rand Index (Hubert and Arabie)
	* The total is the mean of the total for each languages pondered by the
	* number of subprofiles by languages.
	* @param rec            Receiver of the signals.
	*/
	void ComputeTotal(GSlot* rec=0);

public:

	/**
	* Create randomly the judgment for all the users.
	* @param parent         The container to assign groupid and parent id.
	* @param groups         The ideal groupment into a GGroups container.
	* @param Save           Save the results.
	*/
	void CreateIdeal(bool Save);

	/**
	* Create new feedback for the different users of the system.
	* @param Save           Save the results.
	*/
	void FdbksCycle(bool Save);

	/**
	* Add assessments to the profiles creates.
	* @param Save           Save the results.
	*/
	void AddAssessments(bool Save);

	/**
	* Add profiles of a new not used topic.
	* @param Save           Save the results.
	* @return true if a not used topic was found.
	*/
	bool AddTopic(bool Save);

	/**
	* Add judgements for some new not used profiles.
	* @param Save           Save the results.
	* @return Number of profiles created.
	*/
	unsigned int AddProfiles(bool Save);

	/**
	* Computed the percentage of correct assignments for the subprofiles last
	* added.
	* @return double;
	*/
	double ComputePercAss(void);

	/**
	* Clear the container of last added subprofiles.
	*/
	void ClearLastAdded(void);

	/**
	* Get a subject.
	* @param id              Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(unsigned int id);

	/**
	* Clear the subjects.
	*/
	void ClearSubjects(void);

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified   Recompute only modified elements or all.
	*/
	void Compare(GSlot* rec=0);

	/**
	* Get the total precision of the groupement.
	*/
	double GetPrecision(void) const {return(Precision);}

	/**
	* Get the total recall of the groupement.
	*/
	double GetRecall(void) const {return(Recall);}

	/**
	* Get the total recall of the groupement.
	*/
	double GetTotal(void) const {return(Total);}

	/**
	* Get the precision of a group.
	* @param grp            Group.
	*/
	double GetPrecision(GGroup* grp);

	/**
	* Get the total recall of a group.
	* @param grp            Group.
	*/
	double GetRecall(GGroup* grp);

	/**
	* Get the ideal groups of the session
	* @return Pointer to R::RContainer<GGroups,unsigned int,true,true>.
	*/
	R::RContainer<GGroups,unsigned int,true,true>* GetIdealGroups(void);

	/**
	* Get a cursor over the ideal groups of the system.
	*/
	GGroupsCursor& GetIdealGroupsCursor(void);

	/**
	* Destructor
	*/
	~GSubjectTree(void);
};


} //-------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
