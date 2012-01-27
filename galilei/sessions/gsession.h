/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GSession_H
#define GSession_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rconfig.h>
#include <rrandom.h>
#include <rindexfile.h>
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdocretrieved.h>
#include <gsubject.h>
#include <gobjects.h>
#include <gclasses.h>
#include <gvector.h>
#include <gdocanalyze.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession : virtual public GKB, public R::RConfig, GObjects<GDoc>, GObjects<GTopic>, GObjects<GUser>, GObjects<GProfile>, GObjects<GCommunity>, public GClasses
{
public:

	// Explicit use of methods from GObjects<GDoc>
   using GObjects<GDoc>::Clear;
	using GObjects<GDoc>::DoCreateIndex;
	using GObjects<GDoc>::DoCreateStruct;
	using GObjects<GDoc>::LoadObjs;
	using GObjects<GDoc>::GetObj;
	using GObjects<GDoc>::GetObjs;
	using GObjects<GDoc>::GetNbObjs;
	using GObjects<GDoc>::InsertObj;
	using GObjects<GDoc>::DeleteObj;
	using GObjects<GDoc>::AssignId;
	using GObjects<GDoc>::GetMaxObjId;
	using GObjects<GDoc>::LoadDesc;
	using GObjects<GDoc>::SaveDesc;
	using GObjects<GDoc>::FlushDesc;
	using GObjects<GDoc>::LoadStruct;
	using GObjects<GDoc>::SaveStruct;
	using GObjects<GDoc>::FlushStruct;
	using GObjects<GDoc>::UpdateIndex;
	using GObjects<GDoc>::LoadIndex;
   using GObjects<GDoc>::BuildIndex;

	// Explicit use of methods from GObjects<GTopic>
   using GObjects<GTopic>::Clear;
	using GObjects<GTopic>::LoadObjs;
	using GObjects<GTopic>::GetObj;
	using GObjects<GTopic>::GetObjs;
	using GObjects<GTopic>::GetNbObjs;
	using GObjects<GTopic>::InsertObj;
	using GObjects<GTopic>::DeleteObj;
	using GObjects<GTopic>::AssignId;
	using GObjects<GTopic>::GetMaxObjId;
	using GObjects<GTopic>::LoadDesc;
	using GObjects<GTopic>::SaveDesc;
	using GObjects<GTopic>::FlushDesc;
	using GObjects<GTopic>::UpdateIndex;
   using GObjects<GTopic>::BuildIndex;

	// Explicit use of methods from GObjects<GUser>
   using GObjects<GUser>::Clear;
	using GObjects<GUser>::LoadObjs;
	using GObjects<GUser>::GetObj;
	using GObjects<GUser>::GetObjs;
	using GObjects<GUser>::GetNbObjs;
	using GObjects<GUser>::InsertObj;
	using GObjects<GUser>::DeleteObj;
	using GObjects<GUser>::AssignId;
	using GObjects<GUser>::GetMaxObjId;

	// Explicit use of methods from GObjects<GProfile>
   using GObjects<GProfile>::Clear;
	using GObjects<GProfile>::LoadObjs;
	using GObjects<GProfile>::GetObj;
	using GObjects<GProfile>::GetObjs;
	using GObjects<GProfile>::GetNbObjs;
	using GObjects<GProfile>::InsertObj;
	using GObjects<GProfile>::DeleteObj;
	using GObjects<GProfile>::AssignId;
	using GObjects<GProfile>::GetMaxObjId;
	using GObjects<GProfile>::LoadDesc;
	using GObjects<GProfile>::SaveDesc;
	using GObjects<GProfile>::FlushDesc;
	using GObjects<GProfile>::UpdateIndex;
   using GObjects<GProfile>::BuildIndex;

	// Explicit use of methods from GObjects<GCommunity>
   using GObjects<GCommunity>::Clear;
	using GObjects<GCommunity>::LoadObjs;
	using GObjects<GCommunity>::GetObj;
	using GObjects<GCommunity>::GetObjs;
	using GObjects<GCommunity>::GetNbObjs;
	using GObjects<GCommunity>::InsertObj;
	using GObjects<GCommunity>::DeleteObj;
	using GObjects<GCommunity>::AssignId;
	using GObjects<GCommunity>::GetMaxObjId;
	using GObjects<GCommunity>::LoadDesc;
	using GObjects<GCommunity>::SaveDesc;
	using GObjects<GCommunity>::FlushDesc;
	using GObjects<GCommunity>::UpdateIndex;
   using GObjects<GCommunity>::BuildIndex;

	// Explicit use of methods from GClasses
   using GClasses::Clear;
	using GClasses::LoadObjs;
	using GClasses::GetObj;
	using GClasses::GetObjs;
	using GClasses::GetNbObjs;
	using GClasses::InsertObj;
	using GClasses::DeleteObj;
	using GClasses::AssignId;
	using GClasses::GetMaxObjId;
	using GClasses::LoadDesc;
	using GClasses::SaveDesc;
	using GClasses::FlushDesc;
	using GClasses::UpdateIndex;

private :

	/**
	 * Identifier of the session.
	 */
	size_t Id;

	/**
	 * Name of the session.
	 */
	R::RString Name;

	/**
	 * Specify if the configuration file is a valid one.
	 */
	bool ValidConfigFile;

    /**
     * Log file for the session.
     */
	 R::RTextFile Log;

    /**
     * Current seek for the session.
     */
	int CurrentRandom;

    /**
     * Random number generator.
     */
	R::RRandom Random;

	/**
	 * External break.
	 */
	bool ExternBreak;

	/**
	 * The simulator (if any).
	 */
	GSimulator* Simulator;

	/**
	 * Limit the clustering of the documents to the selected ones.
	 */
	bool ClusterSelectedDocs;

	/**
	 * Subjects.
	 */
	GSubjects Subjects;

	/**
	 * Analyze of the document.
	 */
	GDocAnalyze DocAnalyze;

public:

	/**
	* Constructor.
	* @param id              Identifier.
	* @param name            Name of the session.
	*/
	GSession(size_t id,const R::RString& name);
	/**
	 * @return the identifier of the session.
	 */
	inline size_t GetId(void) const {return(Id);}

	/**
	 * @return the name of the session.
	 */
	inline R::RString GetName(void) const {return(Name);}

	/**
	* See if the session must break.
	*/
	bool MustBreak(void);

	/**
	* Reset the break on a session. This method must be called after a SetBreak
	* to allow the session to do something again.
	*/
	void ResetBreak(void);

	/**
	* Ask to session to break as soon as possible. The method ResetBreak should
	* be called to allow the session to do something again.
	*/
	void SetBreak(void);

	/**
	* Get The Current RandomSeed.
	* @return The current RandomSeed.
	*/
	inline int GetCurrentRandom(void) const {return(CurrentRandom);}

	/**
	* Get a current Random Value.
	* @param max             Max Random Value.
	* @return The current Random value * max.
	*/
	inline size_t GetCurrentRandomValue(size_t max) {return(Random.GetValue(max));}

	/**
	* Get the random number generator.
	* @returns Pointer to RMath::RRandom;
	*/
	inline R::RRandom& GetRandom(void) {return(Random);}

	/**
	 * Apply the internal configuration. This method must be called to take
	 * changes in the configuration into account.
	 */
	void ApplyConfig(void);

	/**
	* Verify if the results of the computations must be saved.
	*/
	bool MustSaveResults(void) const {return(SaveResults);}

	/**
	* Set The Current RandomSeed.
	* @param rand            Current RandomSeed.
	*/
	void SetCurrentRandom(int rand);

	/**
	* Set if the results of the computation must be saved or not.
	* @param save            Save the objects after computation?
	*/
	void SetSaveResults(bool save=true) {SaveResults=save;}

	/** @name Computing Methods
	 *  Methods used to launch different computations on the session.
	 */
	//@{
	/**
	* Analyze a document.
	* @param doc             Pointer to the document to analyze.
	* @param ram             Must the description of the document be maintain
	*                        in RAM.
	* @param rec             Receiver for the signals.
	*/
	void AnalyzeDoc(GDoc* doc,bool ram=true,GSlot* rec=0);

	/**
	* Analyze the documents. At the end, all the enabled post-documents methods
	* are run.
	* @param ram             Must the description of each document be maintain
	*                        in RAM.
	* @param rec             Receiver for the signals.
	*/
	void AnalyzeDocs(bool ram=false,GSlot* rec=0);

	/**
	* Groups the documents into topics. At the end, all the enabled post-topic
	* computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupDocs(GSlot* rec=0);

	/**
	* Compute a profile.
	* @param profile         Profile to compute.
	* @param method          Profile Computation method.
	* @param linkcalc        Link Computation method.
	* @param rec             Receiver for the signals.
	*/
	void CalcProfile(GProfile* profile,GProfileCalc* method=0,GLinkCalc* linkcalc=0,GSlot* rec=0);

	/**
	* Compute the profiles.
	* @param rec             Receiver for the signals.
	*/
	void CalcProfiles(GSlot* rec=0);

	/**
	* Groups the profiles into communities. At the end, all the enabled
	* post-community computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupProfiles(GSlot* rec=0);

	/**
	* Run all the enabled suggestions computing methods.
	* @param rec             Receiver of the signals.
	*/
	void ComputeSugs(GSlot* rec=0);

	/**
	* Run all the enabled trust computing methods.
	* @param rec             Receiver of the signals.
	*/
	void ComputeTrust(GSlot* rec=0);

	/**
	 * Run a specific tool.
	 * @param name           Name of the tool.
	 * @param list           List of the tool.
	 * @param slot           Slot.
	 * @param need           If set to true and the tool doesn't exist, it
	 *                       generates an exception.
	 */
	void RunTool(const R::RString& name,const R::RString& list,GSlot* slot=0,bool need=true);
	//@}

	/**
	 * Compare two session regarding their identifiers.
	 * @param session        Session to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const GSession& session) const;

	/**
	 * Compare two session regarding their identifiers.
	 * @param session        Session to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const GSession* session) const;

	/**
	 * Compare the identifier of the session with a given one.
	 * @param id             Identifier to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(size_t id) const;

	/**
	 * Compare the name of the session with a given one.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const;

	/**
	 * Reset a type of objects. In practice, the objects are destroyed, their
	 * references removed and delete the corresponding files.
	 * @param type           Type of the objects.
	 */
	void Reset(tObjType type);

   /**
	 * Reset a file associated with a type of objects.
	 * @param type           Type of the objects.
	 * @param meta           Meta-type (may be index or references).
	 */
	void ResetFile(tObjType type,tObjType meta);

	/**
	* Force some objects to be re-computed even if they are updated.
	* @param type            Type of the objects.
	*/
	void ForceReCompute(tObjType type);

	/**
	* Fill a given array with all the subjects. The array must be created and
	* must be large enough to hold all the subjects.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param subjects        Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t FillSubjects(GSubject** subjects);

	/**
	* Get the number of elements of a given type.
	* @param type            Type of the elements.
	* @returns Number of elements.
	*/
	size_t GetNbObjs(tObjType type) const;

	/**
	 * Get the highest identifier for the elements of a given type.
	 * @param type            Type of the elements.
	 * @returns Highest identifier.
	 */
	size_t GetMaxObjId(tObjType type) const;

	/**
	 * Get the maximum position of the objects of a given type handled.
	 * @param type            Type of the elements.
	 * @returns Maximum position occupied by the elements.
	 */
	size_t GetMaxObjPos(tObjType type) const;

	/**
	 * Find an object of a given type and with a particular identifier.
	 * @param type           Type. Only the objects managed.
	 * @param id             Identifier.
	 * @param null           If set to true, if the document does not exist,
	 *                       return 0, else an exception is generated.
	 * @return Pointer to the object.
	 */
	GObject* GetObj(tObjType type,size_t id,bool null=false);

	/**
	 * Fill an array of pointers with the elements of a given type.
	 * @param type           Type of the elements.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated.
	 * @return number of elements in the array (including null pointers).
	 */
	size_t GetObjs(tObjType type,GObject** &tab,bool alloc);

	/**
	 * Get the depth of the tree formed by all the subjects.
	 */
	size_t GetMaxDepth(void) const;

	/**
	* Compute the number of ideal groups of a given type (otProfile or otDoc)
	* for a given topic (and its sub-topics).
	* @param type            Type.
	*/
	size_t GetNbIdealGroups(tObjType type) const;

	/**
	* Get the number of subjects associated with a given document.
	* @param doc            Pointer to the document.
	*/
	size_t GetNbSubjects(GDoc* doc) const;

	/**
	* Compute the number of topics (and its sub-topics) containing a document.
	*/
	size_t GetNbTopicsDocs(void) const;

	/**
	 * @return Simulator associated with the session.
	 */
	GSimulator* GetSimulator(void) const;

	/**
	* Get the subject of a given community.
	* @param com             Pointer to the community.
	*/
	const GSubject* GetSubject(GCommunity* com) const;

	/**
	* Get the subject of a given document.
	* @param doc             Pointer to the document.
	*/
	const GSubject* GetSubject(GDoc* doc) const;

	/**
	* Get a subject.
	* @param id              Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(size_t id) const;

	/**
	* Get a pointer to a given subject.
	* @param name            Name of the subject.
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	GSubject* GetSubject(const R::RString& name) const;

	/**
	* Get the subject of a given profile.
	* @param prof            Pointer to the profile.
	*/
	const GSubject* GetSubject(GProfile* prof) const;

	/**
	* Get a cursor over all the nodes of the tree.
	*/
	R::RCursor<GSubject> GetSubjects(void) const;

	/**
	* Get a Cursor on the subjects of a given document.
	* @param doc            Pointer to the document.
	*/
	R::RCursor<GSubject> GetSubjects(GDoc* doc) const;

	/**
	* Get a cursor over the child subjects of a given subject.
	* @param subject            Parent subject. If null, the top subjects are returned.
	*/
	R::RCursor<GSubject> GetSubjects(const GSubject* subject) const;

	/**
	* @return a cursor over the top nodes.
	*/
	R::RCursor<GSubject> GetTopSubjects(void) const;

	/**
	 * Get the cost of the Up operations to move a token from a node to another
	 * one.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define a similarity measure between two trees based on the different
	 * costs of up operations of the nodes of each tree.
	 * @param u              First node.
	 * @param v              Second node.
	 * @return Cost of the up operations.
	 */
	double GetUpOperationsCost(const GSubject* u,const GSubject* v) const;

	/**
	 * Initialize the session.
	 */
	void Init(void);

	/**
	* Analyze a given string to search for variables of the type '%%var%%'. These
	* variables are then replaced with the corresponding value. Actually, the
	* known variables are:
	* - %%world% : Name of the session.
	*
	* @param str             String to analyze.
	*/
	R::RString AnalyzeString(const R::RString& str);

	/**
	* Fill a given array with all the selected documents. The array must be
	* created and must be large enough to hold all the documents.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t FillSelectedDocs(GDoc** docs);

	/**
	* Add a given document to a subject.
	* @param doc            Pointer to the document.
	* @param subjectid      Identifier of the real subject.
	* @param usedid         Identifier of the subject used.
	*/
	void Insert(GDoc* doc,size_t subjectid,size_t usedid);

	/**
	* Add a given profile to a subject.
	* @param profile        Pointer to the profile.
	* @param subjectid      Identifier of the subject.
	*/
	void Insert(GProfile* profile,size_t subjectid);

	/**
	* Insert a subject and attached it to a parent. If the parent is null,
	* the subject is considered as one of the top subject of the tree.
	* @param to              Parent where the subject is to attached.
	* @param subject         Subject to insert.
	*/
	void Insert(GSubject* to,GSubject* subject);

	/**
	* Add a new feedback of a given profile on a given document.
	* @param profid          Identifier of the profile.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	* @param newone          New feedback in the system?
	*/
	void InsertFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate date,R::RDate computed,bool newone=false);

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(GDoc* doc,const GSubject* s) const;

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(GDoc* doc,const GSubject* s) const;

	/**
	 * Load the subjects.
	 */
	void LoadSubjects(void) const;

	/**
	* Send a query to the meta-engine selected. The documents are researched and
	* ranked.
	* @param query           String representing the query.
	*/
	void RequestMetaEngine(const R::RString query);

	/**
	* Re-initialize the session (clear all containers).
	*/
	void ReInit(void);

	/**
	 * Set the descriptions of the subjects.
	 * @param type           TYpe of the subjects.
	 */
	void SetDescType(tSubjectDesc type);

	/**
	 * Test if the subjects are well-formed.
	 */
	void TestSubjects(void);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param prof            Profile.
	*/
	void UpdateCommunity(GProfile* prof);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param profid          Identifier of the profile.
	*/
	void UpdateCommunity(size_t profid);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identifier of the document.
	*/
	void UpdateProfiles(size_t docid);

	/**
	* A document was updated and the corresponding topic must be updated.
	* @param doc             Document.
	*/
	void UpdateTopic(GDoc* doc);

	/**
	* A document was updated and the corresponding topic must be updated.
	* @param docid           Identifier of the document.
	*/
	void UpdateTopic(size_t docid);

private:

	/**
	 * Verify if at least one vector is defined in the container.
	 * @param vectors        Vectors.
	 * @return true if the container is defined.
	 */
	bool IsDefined(const R::RContainer<GVector,true,true>& vectors);

public:

	/**
	 * Assign a specific description to a given class. If necessary, the
	 * description is saved.
	 * @param theclass       The class.
	 * @param desc           Description.
	 */
	void Assign(GClass* theclass,GDescription& desc);

	/**
	* Destruct the session.
	*/
	virtual ~GSession(void);

	friend class GDebugObject;
	friend class GSimulator;
	friend class GPlugIn;
	friend class GPlugInList;
	friend class GPlugInManager;
	friend class GConceptType;
	friend class GGALILEIApp;
	friend class GSubject;
	friend class GDocAnalyze;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
