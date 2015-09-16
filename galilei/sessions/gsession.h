/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <rtree.h>
#include <robject.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdocfragment.h>
#include <gsubjects.h>
#include <gobjects.h>
#include <gclasses.h>
#include <gvector.h>
#include <gdocanalyze.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * A message associated to a session when its send a notification.
 * @short Session Message.
 */
class GSessionMsg
{
	/**
	 * The session sending the notification.
	 */
	GSession* Session;

	/**
	 * Type of the objects concerned by the message send.
	 */
	tObjType Type;

	/**
	 * Were the object deleted or just their description cleared.
	 */
	bool DeleteObjs;

public:

	/**
	 * Construct a session message. The meta type associated is supposed to be
	 * the session.
    * @param session        Session.
    * @param type           Type of the objects.
    */
	GSessionMsg(GSession* session,tObjType type);

	/**
	 * Construct a session message.
    * @param session        Session.
    * @param type           Type of the objects.
    * @param del            Specify if the object were delete.
    */
	GSessionMsg(GSession* session,tObjType type,bool del);

	/**
    * @return the session associated to the message.
    */
	GSession* GetSession(void) const {return(Session);}

	/**
    * @return the object type associated to the message.
    */
	tObjType GetType(void) const {return(Type);}

	/**
    * Specify if the objects were deleted.
    */
	bool GetDeleteObjs(void) const {return(DeleteObjs);}
};


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session.
*
* The session generates several notifications, including 'Reset' and 'ResetFile'
* and 'ForceReCompute'.
* @short Generic Session.
*/
class GSession : public R::RObject,                    // Session receives and sends messages
	              virtual public GKB,                   // It has a knowledge base
	              public R::RConfig,                    // It has a configuration file
	              GObjects<GDoc,hDocs>,                 // It manages documents
	              GObjects<GTopic,hTopics>,             // It manages topics
	              GObjects<GUser,hUsers>,               // It manages users
	              GObjects<GProfile,hProfiles>,         // It manages profiles
	              GObjects<GCommunity,hCommunities>,    // It manages communities
	              public GClasses,                      // It manages classes
	              public GSubjects                      // It manages subjects
{
public:

	// Explicit use of methods from GKB
	using GKB::InsertObj;
	using GKB::DeleteObj;
	using GKB::GetObj;
	using GKB::GetObjs;
	using GKB::GetNbObjs;
	using GKB::GetMaxObjId;

	// Explicit use of methods from GObjects<GDoc,eCreateDoc>
	using GObjects<GDoc,hDocs>::Clear;
	using GObjects<GDoc,hDocs>::DoCreateIndex;
	using GObjects<GDoc,hDocs>::DoCreateTree;
	using GObjects<GDoc,hDocs>::LoadObjs;
	using GObjects<GDoc,hDocs>::GetObj;
	using GObjects<GDoc,hDocs>::GetObjs;
	using GObjects<GDoc,hDocs>::GetNbObjs;
	using GObjects<GDoc,hDocs>::InsertObj;
	using GObjects<GDoc,hDocs>::DeleteObj;
	using GObjects<GDoc,hDocs>::AssignId;
	using GObjects<GDoc,hDocs>::GetMaxObjId;
	using GObjects<GDoc,hDocs>::GetMaxObjPos;
	using GObjects<GDoc,hDocs>::LoadDesc;
	using GObjects<GDoc,hDocs>::SaveDesc;
	using GObjects<GDoc,hDocs>::FlushDesc;
	using GObjects<GDoc,hDocs>::LoadTree;
	using GObjects<GDoc,hDocs>::SaveTree;
	using GObjects<GDoc,hDocs>::FlushTree;
	using GObjects<GDoc,hDocs>::UpdateIndex;
	using GObjects<GDoc,hDocs>::LoadIndex;
	using GObjects<GDoc,hDocs>::BuildIndex;
	using GObjects<GDoc,hDocs>::FindDepthFirstRecord;
	using GObjects<GDoc,hDocs>::FindRecord;
	using GObjects<GDoc,hDocs>::FindParentRecord;
	using GObjects<GDoc,hDocs>::FindNearestRecord;
	using GObjects<GDoc,hDocs>::FindRootRecord;
	using GObjects<GDoc,hDocs>::FindFirstChildRecord;
	using GObjects<GDoc,hDocs>::GetMaxPosRecord;
	using GObjects<GDoc,hDocs>::GetMaxSyntacticPosRecord;
	using GObjects<GDoc,hDocs>::GetMinPosRecord;
	using GObjects<GDoc,hDocs>::GetMaxPosRecord;
	using GObjects<GDoc,hDocs>::GetNbTotalChildRecords;

	// Explicit use of methods from GObjects<GTopic,eCreateTopic>
	using GObjects<GTopic,hTopics>::Clear;
	using GObjects<GTopic,hTopics>::DoCreateIndex;
	using GObjects<GTopic,hTopics>::LoadObjs;
	using GObjects<GTopic,hTopics>::GetObj;
	using GObjects<GTopic,hTopics>::GetObjs;
	using GObjects<GTopic,hTopics>::GetNbObjs;
	using GObjects<GTopic,hTopics>::InsertObj;
	using GObjects<GTopic,hTopics>::DeleteObj;
	using GObjects<GTopic,hTopics>::AssignId;
	using GObjects<GTopic,hTopics>::GetMaxObjId;
	using GObjects<GTopic,hTopics>::GetMaxObjPos;
	using GObjects<GTopic,hTopics>::LoadDesc;
	using GObjects<GTopic,hTopics>::SaveDesc;
	using GObjects<GTopic,hTopics>::FlushDesc;
	using GObjects<GTopic,hTopics>::UpdateIndex;
	using GObjects<GTopic,hTopics>::BuildIndex;

	// Explicit use of methods from GObjects<GUser,eCreateUser>
	using GObjects<GUser,hUsers>::Clear;
	using GObjects<GUser,hUsers>::LoadObjs;
	using GObjects<GUser,hUsers>::GetObj;
	using GObjects<GUser,hUsers>::GetObjs;
	using GObjects<GUser,hUsers>::GetNbObjs;
	using GObjects<GUser,hUsers>::InsertObj;
	using GObjects<GUser,hUsers>::DeleteObj;
	using GObjects<GUser,hUsers>::AssignId;
	using GObjects<GUser,hUsers>::GetMaxObjId;
	using GObjects<GUser,hUsers>::GetMaxObjPos;

	// Explicit use of methods from GObjects<GProfile,eCreateProfile>
	using GObjects<GProfile,hProfiles>::Clear;
	using GObjects<GProfile,hProfiles>::DoCreateIndex;
	using GObjects<GProfile,hProfiles>::LoadObjs;
	using GObjects<GProfile,hProfiles>::GetObj;
	using GObjects<GProfile,hProfiles>::GetObjs;
	using GObjects<GProfile,hProfiles>::GetNbObjs;
	using GObjects<GProfile,hProfiles>::InsertObj;
	using GObjects<GProfile,hProfiles>::DeleteObj;
	using GObjects<GProfile,hProfiles>::AssignId;
	using GObjects<GProfile,hProfiles>::GetMaxObjId;
	using GObjects<GProfile,hProfiles>::GetMaxObjPos;
	using GObjects<GProfile,hProfiles>::LoadDesc;
	using GObjects<GProfile,hProfiles>::SaveDesc;
	using GObjects<GProfile,hProfiles>::FlushDesc;
	using GObjects<GProfile,hProfiles>::UpdateIndex;
	using GObjects<GProfile,hProfiles>::BuildIndex;

	// Explicit use of methods from GObjects<GCommunity,eCreateCommunity>
	using GObjects<GCommunity,hCommunities>::Clear;
	using GObjects<GCommunity,hCommunities>::DoCreateIndex;
	using GObjects<GCommunity,hCommunities>::LoadObjs;
	using GObjects<GCommunity,hCommunities>::GetObj;
	using GObjects<GCommunity,hCommunities>::GetObjs;
	using GObjects<GCommunity,hCommunities>::GetNbObjs;
	using GObjects<GCommunity,hCommunities>::InsertObj;
	using GObjects<GCommunity,hCommunities>::DeleteObj;
	using GObjects<GCommunity,hCommunities>::AssignId;
	using GObjects<GCommunity,hCommunities>::GetMaxObjId;
	using GObjects<GCommunity,hCommunities>::GetMaxObjPos;
	using GObjects<GCommunity,hCommunities>::LoadDesc;
	using GObjects<GCommunity,hCommunities>::SaveDesc;
	using GObjects<GCommunity,hCommunities>::FlushDesc;
	using GObjects<GCommunity,hCommunities>::UpdateIndex;
	using GObjects<GCommunity,hCommunities>::BuildIndex;

	// Explicit use of methods from GClasses
	using GClasses::Clear;
	using GClasses::DoCreateIndex;
	using GClasses::LoadObjs;
	using GClasses::GetObj;
	using GClasses::GetObjs;
	using GClasses::GetNbObjs;
	using GClasses::InsertObj;
	using GClasses::DeleteObj;
	using GClasses::AssignId;
	using GClasses::GetMaxObjId;
	using GClasses::GetMaxObjPos;
	using GClasses::LoadDesc;
	using GClasses::SaveDesc;
	using GClasses::FlushDesc;
	using GClasses::UpdateIndex;
	using GClasses::GetUpOperationsCost;

	// Explicit use of methods from GSubjects
	using GSubjects::Clear;
	using GSubjects::LoadObjs;
	using GSubjects::GetObj;
	using GSubjects::GetObjs;
	using GSubjects::GetNbObjs;
	using GSubjects::InsertObj;
	using GSubjects::GetMaxDepth;
	using GSubjects::GetUpOperationsCost;

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
	 * Analyze of the document.
	 */
	GDocAnalyze DocAnalyze;

	/**
	 * Is the analyzer reserved ?
	 */
	bool AnalyzerReserved;

public:

	/**
	* Constructor.
	* @param id              Identifier.
	* @param name            Name of the session.
	*/
	GSession(size_t id,const R::RString& name);

	/**
	* This method is call when no handler is founded for a notification send by
	* the session. Actually, it does nothing.
	* @param notification    Notification.
	*/
	virtual void HandlerNotFound(const R::RNotification& notification);

	/** @name Session General Methods
	 *  General methods related to the session.
	 */
	//@{
	/**
	 * @return the identifier of the session.
	 */
	inline size_t GetId(void) const {return(Id);}

	/**
	 * @return the name of the session.
	 */
	inline R::RString GetName(void) const {return(Name);}

	/**
	 * Compare two session regarding their identifiers.
	 * @param session        Session to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const GSession& session) const;

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
    * @return the log file current used.
    */
	R::RTextFile& GetLog(void) {return(Log);}

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

	/**
	* Analyze a given string to search for variables of the type '%%var%%'. These
	* variables are then replaced with the corresponding value. Actually, the
	* known variables are:
	* - %%world% : Name of the session.
	*
	* @param str             String to analyze.
	*/
	R::RString AnalyzeString(const R::RString& str);

	//@}

	/** @name Computing Methods
	 *  Methods used to launch different computations on the session.
	 */
	//@{
	/**
	 * Reserve a document analyzer. While it is not released, it cannot be used
	 * by the session.
    * @return a pointer to a GDocAnalyze.
    */
	GDocAnalyze* ReserveAnalyzer(void);

	/**
	 * Release an analyzer.
    * @param analyzer       Analyzer to release.
    */
	void ReleaseAnalyzer(GDocAnalyze* analyzer);

	/**
	* Analyze a document.
	*
	* If multiple analyzers are managed by the session, one is randomly chosen.
	* @param doc             Pointer to the document to analyze.
	* @param rec             Receiver for the signals.
	* @param force           Force the analysis of the document?
	* @param download        Try to download locally the document?
	* @exception When no analyzer is available.
	*/
	void AnalyzeDoc(GDoc* doc,GSlot* rec=0,bool force=false,bool download=true);

	/**
	* Analyze the documents. At the end, all the enabled post-documents methods
	* are run.
	*
	* All enabled analyzers managed by the session are used.
	* @param rec             Receiver for the signals.
	* @exception When no analyzer is available.
	*/
	void AnalyzeDocs(GSlot* rec=0);

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

	/**
	* Send a query to the meta-engine selected. The documents are researched and
	* ranked.
	* @param query           String representing the query.
	*/
	void RequestMetaEngine(const R::RString& query);
	//@}

	/** @name Informational Methods
	 *  Methods that provides some information on the session.
	 */
	//@{
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
	 * @param null           If set to true, if the object does not exist,
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
	 * Verify that a given set of containers can hold a given size of objects.
	 * This method can be useful before a given set of elements is loaded in
	 * order to avoid a constant increase of the corresponding containers.
    * @param type           Type of the container.
	 * @param max            Maximum number of objects.
    */
	void VerifyTab(tObjType type,size_t max);

	/**
	 * @return Simulator associated with the session.
	 */
	GSimulator* GetSimulator(void) const;
	//@}

	/** @name Update Methods
	 *  Methods used to update the session.
	 */
	/**
	* Re-initialize the subjects for a new validation process. In practice, the
	* methods deselects all the documents, and delete the users, the feedbacks,
	* the profiles, the communities, the topics and the classes. Eventually, the
	* document descriptions are cleared too.
	*
	* The method sends several hReInit notifications.
   * @param obj            Pseudo-parameter.
   * @param cleardocs      Should the documents descriptions be cleared.
	*/
	void ReInit(const GSubject* obj,bool cleardocs);

	/**
	 * Re-initialize the documents. In practice, the method deletes the topics
	 * and the classes since they depend directly from the documents
	 * descriptions. If the documents must be deleted, the method also deletes
	 * the feedbacks.
	 *
	 * The method sends several hReInit notifications.
    * @param obj            Pseudo-parameter.
    * @param del            Should the documents be deleted or just their
	 *                       descriptions cleared.
    */
	void ReInit(const GDoc* obj,bool del);

	/**
	 * Re-initialize the users. In practice, the method deletes the users, the
	 * profiles, the communities and the feedbacks.
	 *
	 * The method sends several hReInit notifications.
    * @param obj            Pseudo-parameter.
    */
	void ReInit(const GUser* obj);

	/**
	 * Re-initialize the profiles. In practice, the method deletes the
	 * communities since they depend directly from the profiles descriptions. If
	 * the profiles must be deleted, the method also deletes the feedbacks.
	 *
	 * The method sends several hReInit notifications.
    * @param obj            Pseudo-parameter.
    * @param del            Should the profiles be deleted or just their
	 *                       descriptions cleared.
    */
	void ReInit(const GProfile* obj,bool del);

	/**
	 * Delete the feedbacks.
	 *
	 * The method sends a hReInit notification.
    * @param obj            Pseudo-parameter.
    */
	void ReInit(const GFdbk* obj);

	/**
	 * Delete the topics.
	 *
	 * The method sends a hReInit notification.
    * @param obj            Pseudo-parameter.
    */
	void ReInit(const GTopic* obj);

	/**
	 * Delete the classes.
	 *
	 * The method sends a hReInit notification.
    * @param obj            Pseudo-parameter.
    */
	void ReInit(const GClass* obj);

	/**
	 * Delete the communities.
	 *
	 * The method sends a hReInit notification.
    * @param obj            Pseudo-parameter.
    */
	void ReInit(const GCommunity* obj);

	/**
	* Add a new feedback of a given profile on a given document.
	* @param prof            Profile.
	* @param doc             Document.
	* @param fdbk            Feedback type.
	* @param done            Date on the last feedback.
	* @param load            Is the feedback loaded from the storage or not. It
	*                        should be always true if the method is call from a
	*                        GStorage child class.
	* @return true if it is a new feedback.
	*/
	bool InsertFdbk(GProfile* prof,GDoc* doc,tFdbkType fdbk,R::RDate done,bool load=false);

	/**
	* Add a new feedback of a given profile on a given document.
	* @param profid          Identifier of the profile.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param done            Date on the last feedback.
	* @param load            Is the feedback loaded from the storage or not. It
	*                        should be always true if the method is call from a
	*                        GStorage child class.
	* @return true if it is a new feedback.
	*/
	bool InsertFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate done,bool load=false);

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
	 * Set the name of a user.
	 * @param user           User.
	 * @param name           Name of the user.
	 * @param fullname       Full name of the user. If null string, the name is
	 *                       used.
	 */
	void SetName(GUser* user,const R::RString& name,const R::RString& fullname=R::RString::Null);

	/**
	 * Set the name of a profile.
	 * @param prof            Profile.
    * @param name            Name.
    */
	void SetName(GProfile* profile,const R::RString& name);

	/**
	* Set if a profile is social.
	* @param prof            Profile.
	* @param social          Social value.
	*/
	void SetSocial(GProfile* profile,bool social);

	/**
	 * Set the confidence score and level of a profile.
	 * @param prof            Profile.
	 * @param score          Confidence level.
	 * @param level          Confidence score.
	 */
	void SetConfidence(GProfile* profile,double score,char level);

	/**
	* A document was updated and the profiles that have made a feedback on it
	* must be updated.
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

	//@}

private:

	/**
	 * Initialize the session.
	 */
	void Init(void);

	/**
	 * Emit a hReInit notification for a given object type.
	 * @param type           Type of the objects.
	 * @param del            Were the objects deleted ?
    */
	inline void EmitReInit(tObjType type,bool del);

	/**
	 * Verify if at least one vector is defined in the container.
	 * @param vectors        Vectors.
	 * @return true if the container is defined.
	 */
	bool IsDefined(const R::RContainer<GVector,true,true>& vectors);

public:

	/**
	* Destruct the session.
	*/
	virtual ~GSession(void);

	friend class GGALILEIApp;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
