/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2008 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#ifndef GSessionH
#define GSessionH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <genginedoc.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a generic GALILEI session.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession
{
	class Intern;

	/*
	* Internal data.
	*/
	Intern* Data;

	/**
	* Handlers of GALILEi signals.
	*/
	static R::RContainer<GSignalHandler,false,false> Handlers;

public:

	/**
	* Constructor.
	* @param slot            Pointer to the slot.
	* @param debug           Pointer to a debugger structure.
	* @param maxdocs         Maximum number of documents to allocate. If zero,
	*                        everything is allocated.
	* @param maxsubprofiles  Maximum number of subprofiles to allocate. If
	*                        zero, everything is allocated.
	* @param maxgroups       Maximum number of groups to allocate. If zero,
	*                        everything is allocated.
	*/
	GSession(GSlot* slot=0,R::RDebug* debug=0,unsigned int maxdocs=0,unsigned int maxsubprofiles=0,unsigned int maxgroups=0);
	
	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

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
	* Verify if the results of the computations must be saved.
	*/
	bool MustSaveResults(void) const;

	/**
	* Set if the results of the computation must be saved or not.
	* @param save            Save the objects after computation?
	*/
	void SetSaveResults(bool save=true);

	/**
	* Get the historic groups manager.
	* @return Pointer to GCommunitysHistoryMng.
	*/
	GCommunitiesHistoryManager* GetCommunitiesHistoryManager(void) const;

	/**
	* Get the storage manager.
	* @return Pointer to GStorage.
	*/
	GStorage* GetStorage(void) const;

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
	void AddDebugObject(const GDebugObject* debug);

	/**
	 * Remove a object given debugging information.
	 * @param debug          Object.
	 */
	void RemoveDebugObject(const GDebugObject* debug);
	
public:
	
	/**
	 * Build some debugging information concerning an object with a given name.
	 * @param name           Name of the object.	 
	 * @param info           Description of the information needed.
	 */
	virtual R::RString GetDebugInfo(const R::RString& name,const R::RString& info);
	
	/**
	 * Put debugging information concerning an object with a given name in a
	 * text file.
	 * @param file           RTextFile where to write.
	 * @param name           Name of the object. 
	 * @param info           Description of the information needed.
	 */
	void PutDebugInfo(R::RTextFile& file,const R::RString& name,const R::RString& info);
	
	/**
	* Run a "program" for this session.
	* @param rec             Slot that receive information.
	* @param filename        Name of the file.
	*/
	void RunPrg(GSlot* rec,const char* filename);

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
	int GetCurrentRandomValue(unsigned int max);

	/**
	* Get the random number generator.
	* @returns Pointer to RMath::RRandom;
	*/
	R::RRandom* GetRandom(void) const;
	
	//@} General methods

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
	 * Get an untyped pointer to an element of a given type.
	 * @param type            Type of the element.
	 * @returns Highest identifier.
	 */	
	void* GetElement(tObjType type,size_t id) const;
	
	//@} General methods
	
	//-----------------------------------------------------
	/** @name Knowledge Methods
	*/
	// @{

	/**
	* Get all concept types defined.
	* @return RCursor over GConceptType.
	*/
	R::RCursor<GConceptType> GetConceptTypes(void) const;

	/**
	* Get the a pointer to a type of concept.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(unsigned int id,bool null) const;

	/**
	* Get the a pointer to a type of concept.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(const R::RString& name,bool null) const;
	
	/**
	* Get the a pointer to a type of concept. If the concept type doesn't
	* exist, it is created.
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetInsertConceptType(const R::RString& name,const R::RString& desc);

	/**
	* Insert a new concept type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param refdocs         Number of documents referenced.
	* @param refsubprofiles  Number of subprofiles referenced.
	* @param refgroups       Number of groups referenced.
	*/
	void InsertConceptType(unsigned int id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refsubprofiles,size_t refgroups);

	/**
	* Assign an identifier to a new concept type.
	* @param type            Concept type.
	*/
	void AssignId(GConceptType* type);
	
	/**
	* Assign an identifier to a new concept.
	* @param concept         Concept.
	*/
	void AssignId(GConcept* concept);

	/**
	* Get all relation types defined.
	* @return RCursor over GRelationType.
	*/
	R::RCursor<GRelationType> GetRelationTypes(void) const;

	/**
	* Get the a poitner to a type of relation.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GRelationType
	*/
	GRelationType* GetRelationType(unsigned int id,bool null) const;

	/**
	* Get the a poitner to a type of relation.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GRelationType
	*/
	GRelationType* GetRelationType(const R::RString& name,bool null) const;

	/**
	* Insert a new relation type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	*/
	void InsertRelationType(unsigned int id,const R::RString& name,const R::RString& desc);

	/**
	* Insert a new relation.
	* @param id              Identifier of the relation.
	* @param name            Name of the relation.
	* @param subjectid       Identifier of the subject.
	* @param subjecttypeid   Type of the subject.
	* @param type            Type of the relation.
	* @param objectid        Identifier of the object.
	* @param objecttypeid    Type of the object.
	* @param weight          Weight of the relation.
	*/
	void InsertRelation(unsigned int id,const R::RString& name,
	                    unsigned int subjectid,unsigned int subjecttypeid,
	                    unsigned int type,
	                    unsigned int objectid,unsigned int objecttypeid,
	                    double weight);

	/**
	* Get a relation.
	* @param id              Identifier of the relation.
	* @param type            Type of the relation.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a relation.
	*/
	GRelation* GetRelation(unsigned int id,unsigned int type,bool null);

	/**
	* Get a set of relations corresponding to a set of criteria and put them
	* into a container.
	* @param rel             Container where the relations will be inserted.
	*                        The container is not cleared by this method.
	* @param subject         Pointer to the concept that is the subject of
	*                        a relation. If null, all concepts are used.
	* @param type            Type of the relation. If cNoRef, all types are
	*                        used.
	* @param object          Pointer of the concept that is the object of
	*                        a relation. If null, all concepts are used.
	* @param sym             Symetric search?
	*/
	void GetRelations(R::RContainer<GRelation,false,false>& rel,GConcept* subject,unsigned int type,GConcept* object,bool sym);

	//@} Knowledge methods

	//-----------------------------------------------------
	/** @name Documents Methods
	*/
	// @{

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
	* Fill a given array with all the documents of a given language. The array
	* must be created and must be large enough to hold all the documents. It
	* can contained null pointers.
	* @see This method is used in GSubjects to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data (including the null pointers) copied in the
	* array.
	*/
	size_t FillDocs(GDoc** docs);

	/**
	* Get a document corresponding to a given identificator.
	* @param id              Identificator of the document.
	* @param load            If set to true, the document is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	GDoc* GetDoc(unsigned int id,bool load=true,bool null=false) const;

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
	* Analyse the documents. At the end, all the enabled post-docs methods are
	* run.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDocs(GSlot* rec=0);

	/**
	* Analyse a document.
	* @param doc             Pointer to the document to analyse.
	* @param method          Pointer to the method used to analyze. If null,
	*                        the default method is used.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDoc(GDoc* doc,GDocAnalyse* method=0,GSlot* rec=0);

	/**
	* Run all the enabled post-docs methods.
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
	* @param id              Identificator.
	* @param load            If set to true, the user is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the user does not exist,
	*                        return 0, else an exception is generated.
	*/
	GUser* GetUser(unsigned int id,bool load=true,bool null=false) const;

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
	* Get the highest identificator assign to a profile.
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
	GProfile* GetProfile(const unsigned int id,bool load=true,bool null=false) const;

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
	* @param docid           Identificator of the document.
	*/
	void UpdateProfiles(unsigned int docids);

	/**
	* Insert a new Feedback.
	* @param profid          Identificator of the profile.
	* @param docid           Identificator of the document.
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

	/**
	* Get a cursor on the communities.
	*/
	R::RCursor<GCommunity> GetCommunities(void);

	/**
	* Get the number of communities.
	*/
	size_t GetNbCommunities(void) const;

	/**
	* Get a group corresponding to a given identificator.
	* @param id              Identificator of the community.
	* @param load            If set to true, the community is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the group does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GCommunity.
	*/
	GCommunity* GetCommunity(unsigned int id,bool load=true,bool null=false) const;

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
	* Clear the groups.
	*/
	void ClearCommunities(void);

	/**
	* Groups the subprofile into virtual communities. At the end, all the
	* enabled post-grouping methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupProfiles(GSlot* rec=0);

	/**
	* Run all the enabled post-community computinng methods.
	* @param rec             Receiver of the signals.
	*/
	void DoPostCommunity(GSlot* rec=0);

	/**
	* Copy the ideal communities as the current one.
	*/
	void CopyIdealCommunities(void);

	/**
	* Load the historic Communities.
	*/
	void LoadHistoricCommunitiesById(unsigned int mingen, unsigned int maxgen);

	/**
	* Load the historic Communities.
	*/
	void LoadHistoricCommunitiesByDate(R::RString mindate, R::RString maxdate);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param prof            Profile.
	*/
	void UpdateCommunity(GProfile* prof);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param profid          Identificator of the profile.
	*/
	void UpdateCommunity(size_t profid);

	// @} Groups


	//-----------------------------------------------------
	/**
	* Get a pointer to the unique session of the process.
	* @return Pointer to a GSession.
	*/
	static GSession* Get(void);

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
	* Add a handler to the list of handlers.
	* @param handler         Pointer to the handler.
	*/
	static void AddHandler(GSignalHandler* handler) {Handlers.InsertPtr(handler);}

	/**
	* Delete a handler from the list of handlers.
	* @param handler         Pointer to the handler.
	*/

	static void DeleteHandler(GSignalHandler* handler) {Handlers.DeletePtr(*handler);}

	/**
	* Emit a signal for a given object.
	* @param O               Class of the object.
	* @param obj             Pointer to the object.
	* @param event           Event.
	*/
	template<class O> static void Event(O* obj, tEvent event)
	{
		if(!obj) return;
		R::RCursor<GSignalHandler> Handlers(GSession::Handlers);
		for(Handlers.Start();!Handlers.End();Handlers.Next())
			Handlers()->Event(obj,event);
	}

	/**
	* Destructor.
	*/
	virtual ~GSession(void);
	
	friend class GDebugObject;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
