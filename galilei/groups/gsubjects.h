/*

	GALILEI Research Project

	GSubjects.h

	Subjects - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#include <rtree.h>
#include <rvectorint.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSubjects class provides a representation for a set of subjects
* representing ideal groups. Since different subjects may be part of a main
* subject, it is in fact a tree of subjects which are stored.
*
* This class is used for validation purposes.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subjects.
*/
class GSubjects : public R::RTree<GSubject,true,false>, public GParams
{
protected:

	// Internal Classes
	class GroupScore;
	class GGroupId;
	class GroupLang;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Percentage of relevant documents to create.
	*/
	double PercOK;

	/**
	* Percentage of fuzzy relevant documents to create.
	*/
	double PercKO;

	/**
	* Percentage of irrelevant documents to create (Computed as a percentage of
	* the relevant documents created).
	*/
	double PercH;

	/**
	* Percentage of documents assessed whith an error.
	*/
	double PercErr;

	/**
	* Percentage of subjects to use.
	*/
	double PercSubjects;

	/**
	* Minimal number of documents in a subject to use it.
	*/
	unsigned int NbMinDocsSubject;

	/**
	* Maximal number of profiles to create in a subject.
	*/
	unsigned int NbProfMax;

	/**
	* Minimal number of profiles to create in a subject.
	*/
	unsigned int NbProfMin;

	/**
	* Percentage of profiles that are social.
	*/
	double PercSocial;

	/**
	* Temporary Array of documents.
	*/
	GDoc** tmpDocs;

	/**
	* Number of documents actually managed.
	*/
	unsigned int NbDocs;

	/**
	* New documents to assess.
	*/
	R::RContainer<GFdbk,false,false> NewDocs;

	/**
	* Lastest added subprofiles.
	*/
	R::RContainer<GSubProfile,false,true> LastAdded;

	/**
	* Number of documents to be assessed during a feeddback process.
	*/
	unsigned int NbDocsAssess;

	/**
	* Inverse Frequency Factor must be used.
	*/
	bool IFF;

	/**
	* Ideal groups created.
	*/
	GGroups* IdealGroups;

	/**
	* Precision of the clustering.
	*/
	double Precision;

	/**
	* Recall of the clustering.
	*/
	double Recall;

	/**
	* Total comparaison between for the clustering.
	*/
	double Total;

	/**
	* SubProfiles that where changed and must be grouped again.
	*/
	R::RContainer<GroupScore,true,true> GroupsScore;

	/**
	* Subjects for the profiles.
	*/
	R::RContainer<GSubject,false,false> Profiles;

	/**
	* Subjects for the documents.
	*/
	R::RContainer<R::RContainer<GSubject,false,false>,true,false> Docs;

	/**
	* Must the simulation be saved?
	*/
	bool SaveSimulation;

public:

	/**
	* Constructor of the subjects.
	* @param session         Session.
	*/
	GSubjects(GSession* session);

	/**
	* Assign the values of the parameters to the corresponding variables.
	*/
	void Apply(void);

protected:

	/**
	* Choose the subjects that will be used.
	*/
	void ChooseSubjects(void);

	/**
	* Create the set based on the subjects used.
	*/
	void CreateSet(void);

	/**
	* Simulatation of assessments for a profile on a given subject.
	* @param groups         Container of groups associated with a given subject.
	* @param prof           Profile that assesses the documents.
	* @param sub            Subject.
	* @param maxDocsOK      Maximal relevant Documents to assess.
	* @param maxDocsKO      Maximal fuzzy relevant Documents to assess.
	* @param maxDocsH       Maximal irrelevant Documents to assess.
	*/
	void ProfileAssess(R::RContainer<GroupLang,true,true>& groups,GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH);

	/**
	* Get the ideal group of the subprofile.
	* @param sub            SubProfile.
	*/
	GGroup* GetIdealGroup(GSubProfile* sub) const;

	/**
	* Compute the Recall and the Precision.
	*/
	void ComputeRecallPrecision(void);

	/**
	* Compute the Total based on the Adjusted Rand Index (Hubert and Arabie)
	* The total is the mean of the total for each languages pondered by the
	* number of subprofiles by languages.
	*/
	void ComputeTotal(void);

public:

	/**
	* Create randomly the judgment for all the users.
	* @param Save           Save the results.
	*/
	void CreateIdeal(bool Save);

	/**
	* Create new feedback for the different users of the system.
	*/
	void FdbksCycle(void);

	/**
	* Add assessments to the profiles creates.
	*/
	void AddAssessments(void);

	/**
	* Add profiles of a new not used topic.
	* @return true if a not used topic was found.
	*/
	bool AddTopic(void);

	/**
	* Add judgements for some new not used profiles.
	* @return Number of profiles created.
	*/
	unsigned int AddProfiles(void);

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
	void Clear(void);

	/**
	* Compare the clustering.
	*/
	void Compare(void);

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
	* @return Pointer to R::RContainer<GGroups,true,true>.
	*/
	GGroups* GetIdealGroups(void);

	/**
	* Add a given profile to a subject.
	* @param profile        Pointer to the profile.
	* @param subjectid      Identificator of the subject.
	*/
	void InsertProfileSubject(GProfile* profile,unsigned int subjectid);

	/**
	* Get the subject of a given subprofile.
	* @param sub            Pointer to the subprofile.
	*/
	GSubject* GetSubject(GSubProfile* sub);

	/**
	* Add a given document to a subject.
	* @param doc            Pointer to the document.
	* @param subjectid      Identificator of the subject.
	*/
	void InsertDocSubject(GDoc* doc,unsigned int subjectid);

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(GDoc* doc,const GSubject* s);

	/**
	* Look if a document is from a given subject.
	* @param docid          Identificator of the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(unsigned int docid,const GSubject* s);

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(GDoc* doc,const GSubject* s);

	/**
	* Look if a document is in the parent subject.
	* @param docid          Identificator of the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(unsigned int doc,const GSubject* s);

	/**
	* Get a Cursor on the subjects of a given document.
	* @param doc            Pointer to the document.
	*/
	R::RCursor<GSubject> GetSubjects(GDoc* doc);

	/**
	* Get a Cursor on the subjects of a given document.
	* @param docid          Identificator of the document.
	*/
	R::RCursor<GSubject> GetSubjects(unsigned int docid);

	/**
	* Get the number of subjects associated with a given document.
	* @param doc            Pointer to the document.
	*/
	unsigned int GetNbSubjects(GDoc* doc);

	/**
	* Get the number of subjects associated with a given document.
	* @param docid          Identificator of the document.
	*/
	unsigned int GetNbSubjects(unsigned int docid);

	/**
	* Destructor of a subject.
	*/
	~GSubjects(void);
};


} //-------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
