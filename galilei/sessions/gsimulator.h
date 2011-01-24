/*

	GALILEI Research Project

	GSimulator.h

	Simulator - Header.

	Copyright 2002-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef GSimulatorH
#define GSimulatorH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rconfig.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSimulator class provides a simulator that can be used for validation
* purposes.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subjects.
*/
class GSimulator
{
protected:

	/**
	 * The Session.
	 */
	GSession* Session;

	/**
	 * Are the subject manually selected or automatically.
	 */
	bool ManualSubjects;

	/**
	 * Number of subjects to randomly select.
	 */
	double NbSubjects;

	/**
	 * Number of subjects is relative or absolute.
	 */
	bool RelSubjects;

    /**
     * Maximal depth of the subjects to be treated.
     */
	size_t MaxDepth;

    /**
     * Minimal number of documents in a subject to use it.
     */
	size_t NbMinDocsSubject;

    /**
     * Number of documents to select for each used subject.
     */
	double NbDocsPerSubject;

    /**
     * Number of documents per subject is relative or absolute.
     */
	bool PercNbDocsPerSubject;

	/**
	 * A same document is selected multiple times.
	 */
	bool MultipleSubjects;

    /**
     * Maximal number of profiles to create in each used subject.
     */
	size_t NbProfMax;

    /**
     * Minimal number of profiles to create in each used subject.
     */
	size_t NbProfMin;

    /**
     * Percentage of profiles that are social, i.e. not alone in a community.
     */
	double PercSocial;

	/**
	 * Number of relevant documents for which an assessment of a profile from
	 * the same subject is simulated.
	 */
	double NbOK;

    /**
     * Number of relevant documents is relative or absolute.
     */
	bool RelOK;

    /**
     * Number of fuzzy relevant documents for which an assessment of a profile
     * from the same subject is simulated.
     */
	double NbKO;

    /**
     * Number of fuzzy relevant documents is relative or absolute.
     */
	bool RelKO;

	/**
	 * Number of irrelevant documents for which an assessment of a profile from
	 * the same subject is simulated.
	 */
	double NbH;

    /**
     * Is the number of irrelevant documents relative or absolute. If relative,
     * it is computed as a percentage of the relevant documents created) or
     * absolute.
     */
	bool RelH;

    /**
     * Number of documents to be assessed during a feedback process.
     */
	size_t NbDocsAssess;

    /**
     * Percentage of documents assessed with an error.
     */
	double PercErr;

    /**
     * Percentage of documents to switch from one subject to another at each
     * step of the degradation.
     */
	double SwitchPerc;

	/**
	 * Create profiles.
	 */
	bool CreateProfiles;

private:

    /**
     * Temporary array of topics.
     */
	GTopic** TmpTopics;

    /**
     * Temporary array of documents.
     */
	GDoc** TmpDocs;

    /**
     * Number of temporary documents.
     */
	size_t NbTmpDocs;

	/**
	 * New documents to assess.
	 */
	R::RContainer<GFdbk,false,false> NewDocs;

	/**
	 * Random generator of switching documents.
	 */
	R::RRandom* SwitchRandom;

	/**
	 * Position of next document to switch.
	 */
	size_t SwitchPos;

	/**
	 * New profiles created during an action of the simulator.
	 */
	R::RContainer<GProfile,false,false> NewProfiles;

public:

	/**
	* Constructor of the simulator.
	* @param session         Session.
	*/
	GSimulator(GSession* session);

	/**
	 * @return The session corresponding to the simulator.
	 */
	GSession* GetSession(void) const {return(Session);}

	/**
	 * Create the configuration parameter related to the simulator.
	 * @param config         Configuration file.
	 */
	static void CreateConfig(R::RConfig* config);

	/**
	* Assign the values of the parameters to the corresponding variables.
	*/
	void ApplyParams(void);

	/**
	* Find a given parameter (template version).
	* @tparam T              Type of the parameter.
	* @param name            Name of the parameter.
	* @return Pointer or null if not found.
	*/
	template<class T> T* FindParam(const R::RString& name)
	{
		return(Session->FindParam<T>(name,"Simulator"));
	}

	/**
	* Start a simulation. A given set of subjects is chosen, then the users
	* are created and the assessments for all the users are randomly created.
	* The results are saved if the corresponding parameter is set in the
	* session.
	* @param create          Must topics be selected (and profiles created).
	*/
	void StartSimulation(bool create=true);

	/**
	* Add a new not used topic.
	* @return true if a not used topic was found.
	*/
	bool AddSubject(void);

	/**
	* Simulate that documents are shared inside the different communities and
	* that the first NbDocsAssess documents are assessed.
	*/
	void ShareDocuments(void);

	/**
	* Simulate that all existing profiles that already have assess some
	* documents, assess randomly NbDocsAssess documents.
	*/
	void AddAssessments(void);

	/**
	* Add a given set of new profiles for an existing subject randomly chosen.
	* @return Number of profiles created.
	*/
	size_t AddProfiles(void);

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
	 * Select manually a given subject.
	 * @param subject        Subject to select.
	 * @param used           Subject used or not.
	 */
	void SetManualUsed(GSubject* subject,bool used);

private:

	/**
	 * Initialize a given subject by creating some profiles and eventually
	 * selecting a set of documents.
	 * @param subject        Subject to select.
	 * @param selectdocs     Must documents be created ?
	 */
	void InitSubject(GSubject* subject,bool selectdocs);

	/**
	* Choose the subjects that will be used.
	*/
	void SelectSubjects(void);

	/**
	* Create a set of assessments for the profiles of the used subjects.
	*/
	void CreateAssessments(void);

	/**
	* Simulation of assessments for a profile on a given subject.
	* @param prof           Profile that assesses the documents.
	* @param sub            Subject.
	* @param max            Maximal documents to assess.
	* @param maxDocsOK      Maximal relevant documents to assess.
	* @param maxDocsKO      Maximal fuzzy relevant documents to assess.
	* @param maxDocsH       Maximal irrelevant documents to assess.
	*/
	void ProfileAssess(GProfile* prof,GSubject* sub,size_t max,size_t maxDocsOK,size_t maxDocsKO,size_t maxDocsH);

	/**
	 * Copy the ideal groups into the current clustering.
	 * @param objtype         Type of the clustering (otProfile or otDoc).
	 * @param grouptype       Type of the groups (otCommunity or otTopic).
	 * @param calc           Method used to compute the groups descriptions.
	 */
	template<class cGroup,class cObj,class cCalc> void CopyIdealGroups(tObjType objtype,tObjType grouptype,cCalc* calc);

	/**
	 * Build a new class corresponding to a given subject.
	 * @param subject        Corresponding subject.
	 * @param parent         Parent class.
	 */
	void BuildClass(GSubject* subject,GClass* parent);

public:

	/**
	 * Build the ideal communities, each community corresponds to a subject
	 * having documents. The descriptions of the communities are computed using
	 * the current plug-in.
	 */
	void BuildIdealCommunities(void);

	/**
	 * Build the ideal topics, each topic corresponds to a subject having
	 * documents. The descriptions of the topics are computing using the
	 * current plug-in.
	 */
	void BuildIdealTopics(void);

	/**
	 * Build the ideal topics, each topic correspond to a "leaf" subject. The
	 * descriptions of the topics are formed using the labels of the subject
	 * and its parents (each label correspond to one information entity, i.e.
	 * "genetic algorithms" is considered as one 'term').
	 */
	void BuildIdealLeafTopics(void);

	/**
	 * Build the ideal classes, each class correspond to a subject.  The
	 * descriptions of the classes are formed using the labels of the subject
	 * and its parents (each label correspond to one information entity, i.e.
	 * "genetic algorithms" is considered as one 'term').
	 */
	void BuildIdealClasses(void);

	/**
	 * Build the ideal classes, each class correspond to a subject.  The
	 * descriptions of the classes are formed using the description of the
	 * documents (for a "leaf" subject) or the children (for a "non-leaf"
	 * subject).
	 */
	void BuildIdealDocsClasses(void);

	/**
	 * Destruct the simulator.
	 */
	~GSimulator(void);
};


} //-------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
