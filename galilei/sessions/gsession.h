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
#include <gusers.h>
#include <gdocs.h>
#include <genginedoc.h>
#include <ggroups.h>
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
class GSession : public GDocs, public GUsers, public GGroups
{
protected:

	/**
	* Pointer to a tree of subjects used for testing purposes.
	*/
	GSubjects* Subjects;

	/**
	* Hitoric of grouping Manager
	*/
	GGroupsHistoryManager* GroupsHistoryMng;

	/**
	* Similarities between the subprofiles.
	*/
	GProfilesSimsManager* ProfilesSims;

	/**
	*  Similarities between documents and subprofiles.
	*/
	GProfilesDocsSimsManager* ProfilesDocsSims;

	/**
	*  Similarities between documents and groupss.
	*/
	GGroupsDocsSimsManager* GroupsDocsSims;

	/**
	* Current seek for this session.
	*/
	int CurrentRandom;

	/**
	* Random number generator
	*/
	R::RRandom* Random;

	/**
	* Storage manager.
	*/
	GStorage* Storage;

	/**
	* Pointer representing the unique session of a process.
	*/
	static GSession* Session;

	/**
	* If this variable is set to true, the session should stop as soon as
	* possible what it currently does.
	*/
	static bool ExternBreak;

	/**
	* Handlers of GALILEi signals.
	*/
	static R::RContainer<GSignalHandler,false,false> Handlers;

	/**
	* Must the documents be saved after computed?
	*/
	bool SaveDocs;

	/**
	* Must the subprofiles be saved after computed?
	*/
	bool SaveSubProfiles;

	/**
	* Must the groups be saved after computed?
	*/
	bool SaveGroups;

	/**
	* Compute only the modified documents.
	*/
	bool ComputeModifiedDocs;

	/**
	* Compute only the modified subprofiles.
	*/
	bool ComputeModifiedSubProfiles;

	/**
	* Use the modified groups.
	*/
	bool ComputeModifiedGroups;

	/**
	* Slot for the session.
	*/
	GSlot* Slot;

public:

	/**
	* Constructor.
	* @param str             Storage manager.
	* @param savedocs        Save documents after computation.
	* @param modifieddocs    Computed only modified documents.
	* @param savesubprofiles Save subprofiles after computation.
	* @param modifiedsubprofiles Computed only modified subprofiles.
	* @param savegroups      Save groups after computation.
	* @param modifiedgroups  Use modified groups.
	*/
	GSession(GStorage* str,bool savedocs,bool modifieddocs,bool savesubprofiles,bool modifiedsubprofiles,bool savegroups,bool modifiedgroups);

	/**
	* Get a pointer to the unique session of the process.
	* @return Pointer to a GSession.
	*/
	static GSession* Get(void) {return(Session);}

	/**
	* See if the session must break.
	*/
	static bool Break(void) {return(ExternBreak);}

	/**
	* Ask to session to break as soon as possible. The method ResetBreak should
	* be called to allow the session to do something again.
	*/
	static void SetBreak(void) {ExternBreak=true;}

	/**
	* Reset the break on a session. This method must be called after a SetBreak
	* to allow the session to do something again.
	*/
	static void ResetBreak(void) {ExternBreak=false;}

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
	GGroupsHistoryManager* GetGroupsHistoryManager(void) {return(GroupsHistoryMng);}

	/**
	* Get the storage manager.
	* @return Pointer to GStorage.
	*/
	GStorage* GetStorage(void) const {return(Storage);}

	/**
	* Get the subjects defined.
	* @return Pointer to GSubjects.
	*/
	GSubjects* GetSubjects(void);

	/**
	* Set the manager for the similarities between (sub)profiles.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GProfilesSimsManager* sims);

	/**
	* Get the manager for the similarities between (sub)profiles.
	*/
	GProfilesSimsManager* GetProfilesSims(void) const {return(ProfilesSims);}

	/**
	* Set the manager for the similarities between (sub)profiles/documents.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GProfilesDocsSimsManager* sims);

	/**
	* Get the manager for the similarities between (sub)profiles/documents.
	*/
	GProfilesDocsSimsManager* GetProfilesDocsSims(void) const {return(ProfilesDocsSims);}

	/**
	* Set the manager for the similarities between groups/documents.
	* @param sims            Pointer to the manager.
	*/
	void SetSims(GGroupsDocsSimsManager* sims);

	/**
	* Get the manager for the similarities between groups/documents.
	*/
	GGroupsDocsSimsManager* GetGroupsDocsSims(void) const {return(GroupsDocsSims);}

	/**
	* Set the slot for the session.
	*/
	void SetSlot(GSlot* slot);

	/**
	* Get the slot of the session.
	*/
	GSlot* GetSlot(void) const {return(Slot);}

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
	void GroupingProfiles(GSlot* rec)  throw(GException);

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
	*/
	void InsertFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date);

	/**
	* Clear all the feedbacks.
	*/
	void ClearFdbks(void);

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
	int GetCurrentRandom(void) {return(CurrentRandom);};

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
	R::RRandom* GetRandom(void) const {return(Random);}

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
	* Destructor.
	*/
	virtual ~GSession(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
