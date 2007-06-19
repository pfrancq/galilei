/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2007 by the Université libre de Bruxelles.

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
	* Init the session.
	*/
	void Init(void);

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
	* Analyze a given string to search for variables '%var%' that are then
	* remplace in the string returned.
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
	* @return Pointer to GGroupsHistoryMng.
	*/
	GGroupsHistoryManager* GetGroupsHistoryManager(void) const;

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
	
	/**
	 * Get a cursor over all language specific data. 
	 * @return R::RCursor<GLangData>
	 */
	R::RCursor<GLangData> GetLanguageSpecifics(void);

	/**
	 * Get a pointer to a structure holding language specific data. If the 
	 * structure does not exist, it is created.
	 * @param lang           Language fo which the structure is searched.
	 */
	GLangData* GetLanguageSpecific(GLang* lang);
	
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
	* Get the a poitner to a type of concept.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(unsigned int id,bool null) const;

	/**
	* Get the a poitner to a type of concept.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(const R::RString& name,bool null) const;

	/**
	* Insert a new concept type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	*/
	void InsertConceptType(unsigned int id,const R::RString& name,const R::RString& desc);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data            Data.
	* @param dict            Dictionary.
	*/
	void AssignId(GConcept* data,const GDict* dict);

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
	* @param subjectlang     Language of the subject.
	* @param subjecttypeid   Type of the subject.
	* @param type            Type of the relation.
	* @param objectid        Identifier of the object.
	* @param objectlang      Language of the object.
	* @param objecttypeid    Type of the object.
	* @param weight          Weight of the relation.
	*/
	void InsertRelation(unsigned int id,const R::RString& name,
	                    unsigned int subjectid,GLang* subjectlang,unsigned int subjecttypeid,
	                    unsigned int type,
	                    unsigned int objectid,GLang* objectlang,unsigned int objecttypeid,
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
	* Get the maximum position of documents handled.
	* @returns Number of documents.
	*/
	size_t GetMaxPosDoc(void) const;

	/**
	* Get a cursor on the documents of a given langage.
	* @param lang            Language of the documents.
	*/
	R::RCursor<GDoc> GetDocs(GLang* lang) const;

	/**
	* Get the number of documents handled for a given langage.
	* @param lang            Langage of the documents
	* @returns Number of documents of this language.
	*/
	size_t GetNbDocs(GLang* lang) const;

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
	* Move a document from the container holding all the documents with an
	* unknown language to the container corresponding to the language.
	* @param d               Pointer to the document.
	*/
	void MoveDoc(GDoc* d);

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
	* @param xml             DocXML representation of the document. If the
	*                        pointer is null, the XML structure is created and
	*                        it must be released by the caller.
	* @param doc             Pointer to the document to analyse.
	* @param method          Pointer to the method used to analyze. If null,
	*                        the default method is used.
	* @param newdocs         Pointer to a container of new documents added
	*                        after the analysis of the links.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDoc(GDocXML* &xml,GDoc* doc,GDocAnalyse* method=0,R::RContainer<GDoc,false,true>* newdocs=0,GSlot* rec=0);

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
	/** @name Users/Profiles/Subprofiles Methods
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
	size_t GetProfilesNb(void) const;

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
	* Get a cursor over the subprofiles of the system for a given language.
	* @param lang            Language.
	*/
	R::RCursor<GSubProfile> GetSubProfiles(const GLang* lang) const;

	/**
	* Get the number of subprofiles defined in the system for a given language.
	* @param lang            Language. If null, the total number of subprofiles
	*                        is computed.
	*/
	size_t GetSubProfilesNb(const GLang* lang) const;

	/**
	* Get the highest identificator assign to a subprofile for a given
	* language.
	* @param lang            Language.
	*/
	size_t GetMaxSubProfileId(const GLang* lang) const;

	/**
	* Get a subprofile with a specific identifier.
	* @param id              Identifier.
	* @param lang            Language.
	* @param load            If set to true, the subprofile is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the subprofile does not exist,
	*                        return 0, else an exception is generated.
	*/
	GSubProfile* GetSubProfile(GLang* lang,const unsigned int id,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new subprofile.
	* @param sub             Subprofile.
	*/
	void AssignId(GSubProfile* sub);

	/**
	* Insert a subprofiles in the container.
	* @param sub             Subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* sub);

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
	* @param lang            Language of the document.
	*/
	void UpdateProfiles(unsigned int docid,GLang* lang);

	/**
	* Insert a new Feedback.
	* @param p               Identificator of the profile.
	* @param d               Identificator of the document.
	* @param lang            Language of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	* @param newone          New feedback in the system?
	*/
	void InsertFdbk(unsigned int p,unsigned int d,GLang* lang,tDocAssessment assess,R::RDate date,R::RDate computed,bool newone=false);

	/**
	* Clear all the feedbacks.
	*/
	void ClearFdbks(void);

	// @} Users/Profiles/Subprofiles


	//-----------------------------------------------------
	/** @name Groups Methods
	*/
	// @{

	/**
	* Get a cursor on the groups of a given langage.
	* @param lang            Language of the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroups(GLang* lang);

	/**
	* Get the number of groups of a given langauge handled.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroups(GLang* lang) const;

	/**
	* Get a group corresponding to a given identificator.
	* @param lang            Language.
	* @param id              Identificator of the group.
	* @param load            If set to true, the group is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the group does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GGroup.
	*/
	GGroup* GetGroup(GLang* lang,unsigned int id,bool load=true,bool null=false) const;

	/**
	* Assign an identifier to a new group.
	* @param grp             Group.
	*/
	void AssignId(GGroup* grp);

	/**
	* Insert a group. The group is also stored in the container correspondong to
	* its language.
	* @param grp             Pointer to the group.
	*/
	void InsertGroup(GGroup* grp);

	/**
	* Delete a group.
	* @param grp             Pointer to the group.
	*/
	void DeleteGroup(GGroup* grp);

	/**
	* Clear the groups of a given language.
	* @param lang            Language of the groups to delete.
	*/
	void ClearGroups(GLang* lang);

	/**
	* Groups the subprofile into virtual communities. At the end, all the
	* enabled post-grouping methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupingProfiles(GSlot* rec=0);

	/**
	* Run all the enabled post-grouping methods.
	* @param rec             Receiver of the signals.
	*/
	void DoPostGroups(GSlot* rec=0);

	/**
	* Copy the ideal groupment in the current one.
	*/
	void CopyIdealGroups(void);

	/**
	* load the historic groups.
	*/
	void LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen);

	/**
	* load the historic groups.
	*/
	void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate);

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	void UpdateGroups(unsigned int subid);

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
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
