/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2005 by the Université libre de Bruxelles.

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
#include <gparams.h>
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
	* @param str             Storage manager.
	* @param maxdocs         Maximum number of documents to allocate. If zero,
	*                        everything is allocated.
	* @param maxsubprofiles  Maximum number of subprofiles to allocate. If
	*                        zero, everything is allocated.
	* @param maxgroups       Maximum number of groups to allocate. If zero,
	*                        everything is allocated.
	*/
	GSession(GStorage* str,unsigned int maxdocs=0,unsigned int maxsubprofiles=0,unsigned int maxgroups=0);

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
	* Must the objects be saved after computation.
	* @param objtype        Type of the object.
	*/
	bool MustSave(tObjType objtype) const;

	/**
	* Must only the modified objects be computed.
	* @param objtype        Type of the object.
	*/
	bool ComputeModified(tObjType objtype) const;

	/**
	* Must the objects be saved.
	* @param objtype        Type of the object.
	* @param save           Save the objects after computation?
	*/
	void SetSave(tObjType objtype,bool save=true);

	/**
	* Must only the modified objects be computed.
	* @param objtype        Type of the object.
	* @param modified       Computed only modified objects?
	*/
	void SetComputeModified(tObjType objtype,bool modified=true);

	/**
	* Connect the session to managers.
	*/
	void Connect(void);

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
	* Set the manager for the similarities between (sub)profiles.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GProfilesSimsManager* sims);

	/**
	* Get the manager for the similarities between (sub)profiles.
	*/
	GProfilesSimsManager* GetProfilesSims(void) const;

	/**
	* Set the manager for the similarities between (sub)profiles/documents.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GProfilesDocsSimsManager* sims);

	/**
	* Get the manager for the similarities between (sub)profiles/documents.
	*/
	GProfilesDocsSimsManager* GetProfilesDocsSims(void) const;

	/**
	* Set the manager for the similarities between groups/documents.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GGroupsDocsSimsManager* sims);

	/**
	* Get the manager for the similarities between groups/documents.
	*/
	GGroupsDocsSimsManager* GetGroupsDocsSims(void) const;

	/**
	* Set the manager for the similarities between groups/profiles.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GProfilesGroupsSimsManager* sims);

	/**
	* Get the manager for the similarities between profiles/documents.
	*/
	GProfilesGroupsSimsManager* GetProfilesGroupsSims(void) const;

	/**
	* Set the slot for the session.
	*/
	void SetSlot(GSlot* slot);

	/**
	* Get the slot of the session.
	*/
	GSlot* GetSlot(void) const;

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict);

	/**
	* Assign an identifier to a new group.
	* @param grp            Group.
	*/
	virtual void AssignId(GGroup* grp);

	/**
	* Assign an identifier to a new document.
	* @param doc            Document.
	*/
	virtual void AssignId(GDoc* doc);

	/**
	* Assign an identifier to a new subprofile.
	* @param sub             Subprofile.
	*/
	virtual void AssignId(GSubProfile* sub);

	/**
	* Get a cursor on all the documents.
	*/
	R::RCursor<GDoc> GetDocs(void) const;

	/**
	* Get a cursor on the documents of a given langage.
	* @param lang            Language of the documents.
	*/
	R::RCursor<GDoc> GetDocs(GLang* lang) const;

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
	unsigned int FillDocs(GDoc** docs);

	/**
	* Get the number of documents handled.
	* @returns Number of documents.
	*/
	unsigned int GetNbDocs(void) const;

	/**
	* Get the maximum position of documents handled.
	* @returns Number of documents.
	*/
	unsigned int GetMaxPosDoc(void) const;

	/**
	* Get the number of documents handled for a given langage.
	* @param lang          Langage of the documents
	* @returns Number of documents of this language.
	*/
	unsigned int GetNbDocs(GLang* lang) const;

	/**
	* Get an identificator that can be assigned to a new document.
	* @return unsigned int
	*/
	unsigned int GetNewId(void) const;

	/**
	* Insert a document. The document is stored in the different containers.
	* @param d               Pointer to the document.
	*/
	void InsertDoc(GDoc* d);

	/**
	* Move a document from the container holding all the documents with an
	* unknow language to the container corresponding to the language. This
	* method supposes that the documents was previously in the unkown container.
	* @param d               Pointer to the document.
	*/
	void MoveDoc(GDoc* d);

	/**
	* Get a document corresponding to a given identificator.
	* @param id              Identificator of the document.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	* @return Pointer to GDoc.
	*/
	GDoc* GetDoc(unsigned int id,bool load=true) const;

	/**
	* Get a document corresponding to a given URL.
	* @param url        URL of the document.
	* @return Pointer to GDoc or null if the document does no exist.
	*/
	GDoc* GetDoc(const char* url);

	/**
	* Clear all the documents.
	*/
	void ClearDocs(void);

	/**
	* Create a XML structure with the content of a document. The structure
	* created has to be desallocate by the caller.
	* @param doc             Document to analyse.
	* @return Pointer to a GDocXML structure.
	*/
	GDocXML* CreateDocXML(GDoc* doc);

	/**
	* Analyse the documents. At the end, all the enabled post-docs methods are
	* run.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseDocs(GSlot* rec=0);

	/**
	* Analyse the documents. At the end, all the enabled post-docs methods are
	* run.
	* @param rec             Receiver for the signals.
	*/
	void AnalyseNewDocs(GSlot* rec=0);

	/**
	* run post-grouping methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePostDoc(GSlot* rec);

	/**
	* Send a Query to the Meta engine selected. The pages are researched, ranked and return
	* @param &keyWords      The set of keywords to form the query
	* @param &results       The set of results returned by the meta engine
	*/
	void QueryMetaEngine(R::RContainer<R::RString,true,false> &keyWords);

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
	* Get an identificator that can be assigned to a new object.
	* @param obj             Object type (otProfile,otSubProfile,otUser).
	* @return unsigned int
	*/
	size_t GetNewId(tObjType obj);

	/**
	* Insert an user in the container.
	* @param usr             Pointer to the user to insert.
	*/
	void InsertUser(GUser* usr);

	/**
	* Get a user from the container.
	* @param id              Identificator.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	*/
	GUser* GetUser(unsigned int id,bool load=true) const;

	/**
	* Insert a new profile in the container.
	* @param p               Pointer to the profile to add.
	*/
	void InsertProfile(GProfile* p);

	/**
	* Get a profile with a specific identifier.
	* @param id              Identifier.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	*/
	GProfile* GetProfile(const unsigned int id,bool load=true) const;

	/**
	* Get a cursor over the profiles of the system.
	*/
	R::RCursor<GProfile> GetProfiles(void) const;

	/**
	* Get the number of profiles defined in the system.
	*/
	size_t GetProfilesNb(void) const;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s);

	/**
	* Get a subprofile with a specific identifier.
	* @param id              Identifier.
	* @param lang            Language. If null, the subprofile is searched in
	*                        all languages.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	*/
	GSubProfile* GetSubProfile(const unsigned int id,const GLang* lang,bool load=true) const;

	/**
	* Get a cursor over the subprofiles of the system for a given language.
	* @param lang           Language.
	*/
	R::RCursor<GSubProfile> GetSubProfiles(const GLang* lang) const;

	/**
	* Compute the (sub)profiles.
	* @param rec            Receiver for the signals.
	*/
	void CalcProfiles(GSlot* rec);

	/**
	* Compute a profile.
	* @param rec            Receiver for the signals.
	* @param profile        Profile to compute.
	*/
	void CalcProfile(GSlot* rec,GProfile* profile);

	/**
	* run pre-profiling methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePreProfile(GSlot* rec);

	/**
	* run post-profiling methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePostProfile(GSlot* rec);

	/**
	* Groups the subprofile into virtual communities. At the end, all the
	* enabled post-grouping methods are called.
	* @param rec            Receiver of the signals.
	*/
	void GroupingProfiles(GSlot* rec);

	/**
	* run post-grouping methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePostGroup(GSlot* rec);

	/**
	* Insert a new Feedback.
	* @param p                Identificator of the profile.
	* @param d                Identificator of the document.
	* @param assess           Feedback.
	* @param date             Date on the last feedback.
	* @param update           Date on the last update of the document.
	*/
	void InsertFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date,R::RDate update);

	/**
	* Clear all the feedbacks.
	*/
	void ClearFdbks(void);

	/**
	* Get a cursor on all the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroups(void) const;

	/**
	* Get a cursor on the groups of a given langage.
	* @param lang            Language of the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroups(GLang* lang);

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
	* Get the group where the given subprofile is attached.
	* @param sub             Subprofile used.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	* @returns Pointer to the group.
	*/
	GGroup* GetGroup(const GSubProfile* sub,bool load=true) const;

	/**
	* Get a group corresponding to a given identificator.
	* @param id              Identificator of the group.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	* @return Pointer to GGroup.
	*/
	GGroup* GetGroup(unsigned int id,bool load=true) const;

	/**
	* Get the number of groups handled.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroups(void) const;

	/**
	* Get the number of groups of a given langauge handled.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroups(GLang* lang) const;

	/**
	* Clear the groups of a given language.
	* @param lang       Language of the groups to delete.
	*/
	void Clear(GLang* lang);

	/**
	* Copy the ideal groupment in the current one.
	*/
	void CopyIdealGroups(void);

	/**
	* Get a cursor over the filters of the system.
	*/
	R::RCursor<GFactoryFilter> GetFiltersCursor(void);

	/**
	* Run a "program" for this session.
	* @param rec            Slot that receive information.
	* @param filename       Name of the file.
	*/
	void RunPrg(GSlot* rec,const char* filename);

	/**
	* Run the filter on the docments
	* @param nbdocs         The number of docs where the word must be present
	* @param nboccurs       The minimal occurs for the word in a document
	*/
	void DocsFilter(int nbdocs,int nboccurs);

	/**
	* Get The Current RandomSeed.
	* @return The current RandomSeed.
	*/
	int GetCurrentRandom(void) const;

	/**
	* Set The Current RandomSeed.
	* @param rand             The current RandomSeed.
	*/
	void SetCurrentRandom(int rand);

	/**
	* Get The Current Random Value.
	* @param max              The max Random Value.
	* @return The current Random value * max.
	*/
	int GetCurrentRandomValue(unsigned int max);

	/**
	* Get the random number generator.
	* @returns Pointer to RMath::RRandom;
	*/
	R::RRandom* GetRandom(void) const;

	/**
	* load the historic groups.
	*/
	void LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen);

	/**
	* load the historic groups.
	*/
	void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate);

	/**
	* Re-init the session (clear all containers).
	*/
	void ReInit(void);

	/**
	* Export the vectors/words matrix
	* @param rec            Slot that can receive ionformation.
	* @param type           type of export ("Profiles", "Documents" or "Groups")
	* @param filename       export file name
	* @param lang           lang of the export
	* @param label          display words id aned vectors id ?
	*/
	virtual void ExportMatrix(GSlot* rec,const char* type, const char* filename, GLang* lang, bool label);

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
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identificator of the document.
	*/
	void UpdateProfiles(unsigned int docid);

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	void UpdateGroups(unsigned int subid);

	/**
	* Destructor.
	*/
	virtual ~GSession(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
