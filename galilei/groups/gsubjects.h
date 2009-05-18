/*

	GALILEI Research Project

	GSubjects.h

	Subjects - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2004 by Julien Lamoral.
	Copyright 2002-2004 by David Wartel.
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gdebugobject.h>


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
class GSubjects : public GDebugObject, public R::RTree<GSubjects,GSubject,true>
{
protected:

	template<class cGroup> class GroupScore;
	class Intern;

	/**
	* Internal data.
	*/
	Intern* Data;

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

	/**
	* Initialize all the subjects (reset the profiles association).
	*/
	void ReInit(void);

protected:

	/**
	 * Select a subject.
	 * @param subject        Subject to select.
	 * @param createdocs     Must documents be created ?
	 */
	void SelectSubject(GSubject* subject,bool createdocs);

	/**
	* Choose the subjects that will be used.
	*/
	void ChooseSubjects(void);

	/**
	* Create the set based on the subjects used.
	*/
	void CreateSet(void);

	/**
	* Simulation of assessments for a profile on a given subject.
	* @param prof           Profile that assesses the documents.
	* @param sub            Subject.
	* @param maxDocsOK      Maximal relevant Documents to assess.
	* @param maxDocsKO      Maximal fuzzy relevant Documents to assess.
	* @param maxDocsH       Maximal irrelevant Documents to assess.
	*/
	void ProfileAssess(GProfile* prof,GSubject* sub,size_t maxDocsOK,size_t maxDocsKO,size_t maxDocsH);

	/**
	* Compute the Recall and the Precision for a given clustering.
	* @param type            Type of the clustering.
	* @param grp             Cursor of the score of the different groups.
	* @param recall          Total recall.
	* @param precision       Total precision.
	*/
	template<class cGroup,class cObj> void ComputeRecallPrecision(tObjType type,R::RCursor<GroupScore<cGroup> >& grp,double& recall,double& precision);

	/**
	* Compute the Total for a given clustering based on the Adjusted Rand Index
	* (Hubert and Arabie).
	* @param objtype         Type of the clustering (otProfile or otDoc).
	* @param grouptype       Type of the groups (otCommunity or otTopic).
	* @param grps            Groups computed.
	* @param total           Total result.
	*/
	template<class cGroup,class cObj> void ComputeTotal(tObjType objtype,tObjType grouptype,R::RCursor<cGroup>& grps,double& total);

	/**
	* Compute the J measure for a given clustering.
	* @param objtype         Type of the clustering (otProfile or otDoc).
	* * @param grouptype       Type of the groups (otCommunity or otTopic).
	* @param grps            Groups computed.
	* @param J               J Measure.
	*/
	template<class cGroup,class cObj> void ComputeJ(tObjType objtype,tObjType grouptype,R::RCursor<cGroup>& grps,double& J);

public:

	/**
	 * Perform a degradation. The ideal clustering is used as the current one,
	 * and a list of objects to assign to another group is determined.
	 * @param what           Describe what to do:
	 *                       -# Initialize the degradation (what=0).
	 *                       -# Make the ideal clustering the current one
	 *                          (what=1). Eventually, randomly selected topics
	 *                          are merged or split.
	 *                       -# Take a given percentage of objects and assign
	 *                          them to a "wrong" group (what=2).
	 * @param nb             Number of clusters to merge (nb<0) or to split
	 *                       (nb>0) (only if what==1).
	 *
	 * The J measure and the adjusted Rand index are then computed (except for
	 * what=0).
	 */
	void PerformDegradation(char what,int nb);

	/**
	* Start a simulation. A given set of subjects is chosen, then the users
	* are created and the assessments for all the users are randomly created.
	* The results are saved if the corresponding parameter is set in the
	* session.
	*/
	void StartSimulation(void);

	/**
	* Simulate that documents are shared inside the different communities and
	* that the first NbDocsAssess documents are assessed.
	*/
	void DocumentSharing(void);

	/**
	* Simulate that existing profiles assess randomly NbDocsAssess chosen
	* documents.
	*/
	void AddAssessments(void);

	/**
	* Add profiles of a new not used topic.
	* @return true if a not used topic was found.
	*/
	bool AddTopic(void);

	/**
	* Add assessments for some new not used profiles.
	* @return Number of profiles created.
	*/
	size_t AddProfiles(void);

	/**
	* Computed the percentage of correct assignments for the profiles last
	* added.
	* @return double;
	*/
	double ComputePercAss(void);

	/**
	* Clear the container of last added profiles.
	*/
	void ClearLastAdded(void);

	/**
	* Get a subject.
	* @param id              Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(size_t id);

	/**
	* Clear the subjects.
	*/
	void Clear(void);

	/**
	* Compare the clustering.
	* @param type            Type of clustering to compare.
	*/
	void Compare(tObjType type);

	/**
	* Get the total precision of the clustering.
	*/
	double GetPrecision(tObjType type) const;

	/**
	* Get the total recall of the clustering.
	*/
	double GetRecall(tObjType type) const;

	/**
	* Get the total recall of the clustering.
	*/
	double GetTotal(tObjType type) const;

	/**
	* Get the J measure of the clustering.
	*/
	double GetJ(tObjType type) const;

	/**
	* Get the precision of a community.
	* @param com            Community.
	*/
	double GetPrecision(GCommunity* com) const;

	/**
	* Get the total recall of a community.
	* @param com            Community.
	*/
	double GetRecall(GCommunity* com) const;

	/**
	* Get the ideal group of the profile.
	* @param prof            Profile.
	*/
	GSubject* GetIdealGroup(GProfile* prof) const;

	/**
	* Get the ideal group of the document.
	* @param doc            Document.
	*/
	GSubject* GetIdealGroup(GDoc* doc) const;

	/**
	* Compute the number of ideal groups of a given type (otProfile or otDoc)
	* for a given topic (and its sub-topics).
	* @param type            Type.
	*/
	size_t GetNbIdealGroups(tObjType type) const;

	/**
	* Get the precision of a topic.
	* @param top            Topic.
	*/
	double GetPrecision(GTopic* top) const;

	/**
	* Get the total recall of a topic.
	* @param top            Topic.
	*/
	double GetRecall(GTopic* top) const;

	/**
	* Compute the number of topics (and its sub-topics) containing a document.
	*/
	size_t GetNbTopicsDocs(void) const;

	/**
	* Add a given profile to a subject.
	* @param profile        Pointer to the profile.
	* @param subjectid      Identifier of the subject.
	*/
	void InsertProfileSubject(GProfile* profile,size_t subjectid);
	/**
	* Get the subject of a given profile.
	* @param prof            Pointer to the profile.
	*/
	GSubject* GetSubject(GProfile* prof);

	/**
	* Add a given document to a subject.
	* @param doc            Pointer to the document.
	* @param subjectid      Identifier of the subject.
	* @param used           Document currently used.
	*/
	void InsertDocSubject(GDoc* doc,size_t subjectid,bool used);

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(GDoc* doc,const GSubject* s);

	/**
	* Look if a document is from a given subject.
	* @param docid          Identifier of the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(size_t docid,const GSubject* s);

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(GDoc* doc,const GSubject* s);

	/**
	* Look if a document is in the parent subject.
	* @param docid          Identifier of the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(size_t docid,const GSubject* s);

	/**
	* Get a Cursor on the subjects of a given document.
	* @param doc            Pointer to the document.
	*/
	R::RCursor<GSubject> GetSubjects(GDoc* doc);

	/**
	* Get a Cursor on the subjects of a given document.
	* @param docid          Identifier of the document.
	*/
	R::RCursor<GSubject> GetSubjects(size_t docid);

	/**
	* Get the number of subjects associated with a given document.
	* @param doc            Pointer to the document.
	*/
	size_t GetNbSubjects(GDoc* doc);

	/**
	* Get the number of subjects associated with a given document.
	* @param docid          Identifier of the document.
	*/
	size_t GetNbSubjects(size_t docid);

	/**
	 * Build some debugging information as a string. A string is given as
	 * parameter to allow the building of several outputs.
	 * @param info           Description of the information needed.
	 */
	virtual R::RString GetDebugInfo(const R::RString& info);

	/**
	* Destruct the subjects.
	*/
	~GSubjects(void);
};


} //-------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
