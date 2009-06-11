/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <genginedoc.h>
#include <gindexer.h>
#include <gontology.h>
#include <gclass.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a generic GALILEI session.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession : public R::RConfig, public GIndexer, public GOntology
{
	class Intern;

	/*
	* Internal data.
	*/
	Intern* Data;

	/**
	 * Classes organizing the concepts.
	 */
	GClasses Classes;

	/**
	 * Determine if all the classes were loaded.
	 */
	bool ClassesLoaded;

public:

	/**
	* Constructor.
	* @param slot            Pointer to the slot.
	* @param debug           Pointer to a debugger structure.
	* @param maxdocs         Maximum number of documents to allocate. If zero,
	*                        everything is allocated.
	* @param maxprofiles     Maximum number of profiles to allocate. If
	*                        zero, everything is allocated.
	* @param maxgroups       Maximum number of groups to allocate. If zero,
	*                        everything is allocated.
	*/
	GSession(GSlot* slot=0,R::RDebug* debug=0,size_t maxdocs=0,size_t maxprofiles=0,size_t maxgroups=0);

	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

	/**
	 * Apply the internal configuration. This method must be called to take
	 * changes in the configuration into account.
	 */
	void Apply(void);

	/**
	* Force some objects to be re-computed even if they are updated.
	* @param type            Type of the objects. Only 'otDocs', 'otUsers' and
	*                        'otGroups' are allowed.
	*/
	void ForceReCompute(tObjType type);

	/**
	* Re-init the session (clear all containers).
	*/
	void ReInit(void);

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
	* Get the subjects defined.
	* @param load            If true, the subjects are loaded from the
	*                        database if necessary.
	*/
	GSubjects* GetSubjects(bool load=true) const;

	/**
	* Set the slot for the session.
	* @param slot            Pointer to the slot.
	*/
	void SetSlot(GSlot* slot);

	/**
	* Get the slot of the session.
	*/
	GSlot* GetSlot(void) const;

	/**
	* Set the object managing the debug output.
	* @param debug           Pointer to the "debugger".
	*/
	void SetDebug(R::RDebug* debug);

	/**
	* Get a pointer over the object managing the debug output.
	*/
	R::RDebug* GetDebug(void) const;

private:

	/**
	 * Add a object given debugging information.
	 * @param debug          Object.
	 */
	static void AddDebugObject(const GDebugObject* debug);

	/**
	 * Remove a object given debugging information.
	 * @param debug          Object.
	 */
	static void RemoveDebugObject(const GDebugObject* debug);

public:

	/**
	 * @return a cursor on all debug objects.
	 */
	R::RCursor<GDebugObject> GetDebugObjects(void) const;

	/**
	 * Set if a particular debug object should be activated.
	 * @param name
	 * @param active
	 */
	void SetActiveDebugObject(const R::RString& name,bool active);

	/**
	 * Build some debugging information concerning an object with a given name.
	 * @param name           Name of the object.
	 * @param info           Description of the information needed.
	 */
	virtual void DebugInfo(const R::RString& name,const R::RString& info);

	/**
	* Set The Current RandomSeed.
	* @param rand            Current RandomSeed.
	*/
	void SetCurrentRandom(int rand);

	/**
	* Get The Current RandomSeed.
	* @return The current RandomSeed.
	*/
	int GetCurrentRandom(void) const;

	/**
	* Get The Current Random Value.
	* @param max             Max Random Value.
	* @return The current Random value * max.
	*/
	int GetCurrentRandomValue(size_t max);

	/**
	* Get the random number generator.
	* @returns Pointer to RMath::RRandom;
	*/
	R::RRandom* GetRandom(void) const;

	//@} General methods


	//-----------------------------------------------------
	/** @name Knowledge Methods
	*/
	// @{

	/**
	* Method that load the classes from where they are stored.
	*/
	void LoadClasses(void);

	/**
	* Get a cursor on all the classes.
	*/
	R::RCursor<GClass> GetClasses(void) const {return(R::RCursor<GClass>(Classes.GetNodes()));}

	/**
	* Get a cursor on the top classes.
	*/
	R::RCursor<GClass> GetTopClasses(void) const {return(R::RCursor<GClass>(Classes.GetTopNodes()));}

	/**
	 * Search for a class with a given identifier.
	 * @param id             Identifier.
	 * @param null           If true, 0 is returned if the class is not found,
	 *                       else an exception is generated.
	 * @return Pointer to the class.
	 */
	GClass* GetClass(size_t id,bool null);

	/**
	 * Insert a class in the tree of classes.
	 * @param parent         Parent class (0 if root class).
	 * @param id             Identifier (or cNoRef if a new class must be
	 *                       created).
	 * @param blockid        Identifier of the block containing the description.
	 * @param name           Name of the class (if RString::Null, the name is
	 *                       "Class id").
	 * @return Pointer to the class.
	 */
	GClass* InsertClass(GClass* parent,size_t id=R::cNoRef,size_t blockid=0,const R::RString& name=R::RString::Null);

	/**
	 * Assign a specific description to a given class. If necessary, the
	 * description is saved.
	 * @param theclass       The class.
	 * @param infos          Vector to assign.
	 */
	void AssignInfos(GClass* theclass,R::RContainer<GWeightInfo,false,true>& infos);

	//@} Knowledge methods

	//-----------------------------------------------------
	/** @name Generic Methods
	*/
	// @{

	/**
	* Get the number of elements of a given type.
	* @param type            Type of the elements.
	* @returns Number of elements.
	*/
	size_t GetNbElements(tObjType type) const;

	/**
	 * Get the highest identifier for the elements of a given type.
	 * @param type            Type of the elements.
	 * @returns Highest identifier.
	 */
	size_t GetMaxElementId(tObjType type) const;

	/**
	* Get an untyped pointer to a given element of a given type, or null if the
	* identifier does not correspond to an element.
	*
	* The resulting pointer should be cast:
	* @code
	* GDoc* Doc=static_cast<GDoc*>(Session->GetElement(otDoc,4));
	* GTopic* Topic=static_cast<GTopic*>(Session->GetElement(otTopic,4));
	* @endcode
	* @param type            Type of the element.
	* @param id              Identifier of the element.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	void* GetElement(tObjType type,size_t id,bool null=false) const;

	/**
	 * Clear the groups containing a given type of elements.
	 * @param type            Type of the element.
	 */
	void ClearGroups(tObjType type);

	/**
	 * Create a new "blank" group for a given type of elements.
	 * @param type            Type of the element.
	 * @param name            Name of the group.
	 */
	void* NewGroup(tObjType type,const R::RString& name);

	/**
	 * Insert a group of elements of a given type in the system.
	 * @param ptr            Pointer to the group.
	 * @param type           Type of the element.
	 */
	void InsertGroup(void* ptr,tObjType type);

	//@} General methods

	//-----------------------------------------------------
	/** @name Documents Methods
	*/
	// @{

	/**
	* Method that load the documents from where they are stored.
	*/
	void LoadDocs(void);

	/**
	* Get a cursor on all the documents.
	*/
	R::RCursor<GDoc> GetDocs(void) const;

	/**
	* Get the number of documents handled.
	* @returns Number of documents.
	*/
	size_t GetNbDocs(void) const;

	/**
	 * Get the highest identifier for a document.
	 * @returns Highest identifier.
	 */
	size_t GetMaxDocId(void) const;

	/**
	* Get the maximum position of documents handled.
	* @returns Number of documents.
	*/
	size_t GetMaxPosDoc(void) const;

	/**
	* Fill a given array with all the documents. The array must be created and
	* must be large enough to hold all the documents. It can contained null
	* pointers.
	* @see This method is used in GSubjects to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data (including the null pointers) copied in the
	* array.
	*/
	size_t FillDocs(GDoc** docs);

	/**
	* Get a document corresponding to a given identifier.
	* @param id              Identifier of the document.
	* @param load            If set to true, the document is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	GDoc* GetDoc(size_t id,bool load=true,bool null=false) const;

	/**
	* Get a document corresponding to a given URL.
	* @param url             URL of the document.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	GDoc* GetDoc(const char* url,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new document.
	* @param doc             Document.
	*/
	void AssignId(GDoc* doc);

	/**
	* Insert a document. The document is stored in the different containers.
	* @param d               Pointer to the document.
	*/
	void InsertDoc(GDoc* d);

	/**
	* Clear all the documents.
	*/
	void ClearDocs(void);

	/**
	* Analyze the documents. At the end, all the enabled post-documents methods
	* are run.
	* @param ram             Must the description of each document be maintain
	*                        in RAM.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDocs(bool ram=false,GSlot* rec=0);

	/**
	* Analyze a document.
	* @param doc             Pointer to the document to analyze.
	* @param ram             Must the description of the document be maintain
	*                        in RAM.
	*
	* @param method          Pointer to the method used to analyze. If null,
	*                        the default method is used.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDoc(GDoc* doc,bool ram=true,GDocAnalyse* method=0,GSlot* rec=0);

	/**
	 * Get the XML structure corresponding to a document. If the document is
	 * not a XML one, a filter is searched.
	 * @param doc            Document.
	 * @param xml            XML struct that will be hold the result.
	 * @param native         Native document.
	 * @return True if the XML structure could be created.
	 */
	bool GetDocXML(GDoc* doc,R::RXMLStruct* xml,bool& native);

	/**
	* Run all the enabled post-documents computing methods.
	* @param rec             Receiver for the signals.
	*/
	void DoPostDocs(GSlot* rec=0);

	/**
	* Send a Query to the Meta engine selected. The pages are researched and ranked.
	* @param keyWords        The set of keywords to form the query.
	*/
	void QueryMetaEngine(R::RContainer<R::RString,true,false>& keyWords);

	//@} Documents


	//-----------------------------------------------------
	/** @name Users/Profiles Methods
	*/
	// @{

	/**
	* Load the profiles and the users.
	*/
	void LoadUsers(void);

	/**
	* Get a cursor over the users used in the system.
	*/
	R::RCursor<GUser> GetUsers(void) const;

	/**
	* Get the number of users treated by the system.
	* @returns Number of Users.
	*/
	size_t GetNbUsers(void) const;

	/**
	* Get a user.
	* @param name            Name of the user.
	* @param load            If set to true, the user is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the user does not exist,
	*                        return 0, else an exception is generated.
	*/
	GUser* GetUser(const R::RString name,bool load=true,bool null=false) const;

	/**
	* Get a user.
	* @param id              Identifier.
	* @param load            If set to true, the user is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the user does not exist,
	*                        return 0, else an exception is generated.
	*/
	GUser* GetUser(size_t id,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new user.
	* @param user            Pointer to the user.
	*/
	void AssignId(GUser* user);

	/**
	* Insert an user.
	* @param user            Pointer to the user to insert.
	*/
	void InsertUser(GUser* user);

	/**
	* Get a cursor over the profiles of the system.
	*/
	R::RCursor<GProfile> GetProfiles(void) const;

	/**
	* Get the number of profiles defined in the system.
	*/
	size_t GetNbProfiles(void) const;

	/**
	* Get the highest identifier assign to a profile.
	*/
	size_t GetMaxProfileId(void) const;

	/**
	* Get a profile with a specific identifier.
	* @param id              Identifier.
	* @param load            If set to true, the profile is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the profile does not exist,
	*                        return 0, else an exception is generated.
	*/
	GProfile* GetProfile(const size_t id,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new profile.
	* @param p               Pointer to the profile.
	*/
	void AssignId(GProfile* p);

	/**
	* Insert a new profile.
	* @param p               Pointer to the profile to add.
	*/
	void InsertProfile(GProfile* p);

	/**
	* Compute the profiles.
	* @param rec             Receiver for the signals.
	*/
	void CalcProfiles(GSlot* rec=0);

	/**
	* Compute a profile.
	* @param rec             Receiver for the signals.
	* @param profile         Profile to compute.
	*/
	void CalcProfile(GProfile* profile,GSlot* rec=0);

	/**
	* Run all enabled post-profiles methods.
	* @param rec             Receiver for the signals.
	*/
	void DoPostProfiles(GSlot* rec=0);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identifier of the document.
	*/
	void UpdateProfiles(size_t docid);

	/**
	* Insert a new Feedback.
	* @param profid          Identifier of the profile.
	* @param docid           Identifier of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	* @param newone          New feedback in the system?
	*/
	void InsertFdbk(size_t profid,size_t docid,tDocAssessment assess,R::RDate date,R::RDate computed,bool newone=false);

	/**
	* Clear all the feedbacks.
	*/
	void ClearFdbks(void);

	// @} Users/Profiles

	//-----------------------------------------------------
	/** @name Groups Methods
	*/
	// @{

private:

	/**
	 * Copy the ideal groups into the current clustering.
	 * @param objtype         Type of the clustering (otProfile or otDoc).
	 * @param grouptype       Type of the groups (otCommunity or otTopic).
	 * @param calc           Method used to compute the groups descriptions.
	 */
	template<class cGroup,class cObj,class cCalc> void CopyIdealGroups(tObjType objtype,tObjType grouptype,cCalc* calc);

public:

	/**
	* Build a type of clustering (otTopic or otCommunity) from the ideal one
	* based on the documents and the profiles assigned to the subject.
	* @param type            Type.
	*/
	void BuildGroupsFromIdeal(tObjType type);

	// @} Groups


	//-----------------------------------------------------
	/** @name Communities Methods
	*/
	// @{

	/**
	* Load the groups. This method is called once by the session.
	*/
	void LoadCommunities(void);

	/**
	* Get a cursor on the communities.
	*/
	R::RCursor<GCommunity> GetCommunities(void);

	/**
	* Get the number of communities.
	*/
	size_t GetNbCommunities(void) const;

	/**
	* Get a community corresponding to a given identifier.
	* @param id              Identifier of the community.
	* @param load            If set to true, the community is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the community does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GCommunity.
	*/
	GCommunity* GetCommunity(size_t id,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new community.
	* @param com             Community.
	*/
	void AssignId(GCommunity* com);

	/**
	* Insert a community.
	* @param com             Community.
	*/
	void InsertCommunity(GCommunity* com);

	/**
	* Delete a community.
	* @param com             Community.
	*/
	void DeleteCommunity(GCommunity* com);

	/**
	* Clear the communities.
	*/
	void ClearCommunities(void);

	/**
	* Groups the profiles into communities. At the end, all the enabled
	* post-community computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupProfiles(GSlot* rec=0);

	/**
	* Run all the enabled post-community computing methods.
	* @param rec             Receiver of the signals.
	*/
	void DoPostCommunity(GSlot* rec=0);

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

	// @} Communities


	//-----------------------------------------------------
	/** @name Topics Methods
	*/
	// @{

	/**
	* Load the topics.
	*/
	void LoadTopics(void);

	/**
	* Get a cursor on the topics.
	*/
	R::RCursor<GTopic> GetTopics(void);

	/**
	* Get the number of topics.
	*/
	size_t GetNbTopics(void) const;

	/**
	 * Get the maximal identifier of a topic.
	 */
	size_t GetMaxTopicId(void) const;

	/**
	* Get the maximum position of topics handled.
	* @returns Number of topics.
	*/
	size_t GetMaxPosTopic(void) const;

	/**
	* Get a topic corresponding to a given identifier.
	* @param id              Identifier of the topic.
	* @param load            If set to true, the topic is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the topic does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GTopic.
	*/
	GTopic* GetTopic(size_t id,bool load=true,bool null=false) const;

	/**
	* Fill a given array with all the topics. The array must be created and
	* must be large enough to hold all the topics. It can contained null
	* pointers.
	* @see This method is used in GSubjects to degrade the ideal clustering.
	* @param topics          Pointer to the array.
	* @returns Size of the data (including the null pointers) copied in the
	* array.
	*/
	size_t FillTopics(GTopic** topics);

	/**
	* Assign an identifier to a new topic.
	* @param top             Topic.
	*/
	void AssignId(GTopic* top);

	/**
	* Insert a topic.
	* @param topic           Topic.
	*/
	void InsertTopic(GTopic* topic);

	/**
	* Delete a topic.
	* @param top             Topic.
	*/
	void DeleteTopic(GTopic* top);

	/**
	* Clear the topics.
	*/
	void ClearTopics(void);

	/**
	* Groups the documents into topics. At the end, all the enabled post-topic
	* computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupDocs(GSlot* rec=0);

	/**
	* Run all the enabled post-topic computing methods.
	* @param rec             Receiver of the signals.
	*/
	void DoPostTopic(GSlot* rec=0);

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

	// @} Topics


	//-----------------------------------------------------
	/**
	* Get a pointer to the unique session of the process.
	* @return Pointer to a GSession.
	*/
	static GSession* Get(void);

	/**
	 * Look if a session is current connected.
	 */
	static bool IsConnected(void);

	/**
	* See if the session must break.
	*/
	static bool Break(void);

	/**
	* Ask to session to break as soon as possible. The method ResetBreak should
	* be called to allow the session to do something again.
	*/
	static void SetBreak(void);

	/**
	* Reset the break on a session. This method must be called after a SetBreak
	* to allow the session to do something again.
	*/
	static void ResetBreak(void);

	/**
	* Destruct the session.
	*/
	virtual ~GSession(void);

	friend class GDebugObject;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
