/*

	GALILEI Research Project

	GStorage.h

	Generic Storage Manager - Header.

	Copyright 2003-2005 by the Université libre de Bruxelles.

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
#ifndef GStorageH
#define GStorageH


//------------------------------------------------------------------------------
// include files for R
#include <rstring.h>
#include <rmysql.h>
#include <rxmltag.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// API VERSION
#define API_STORAGE_VERSION "2.0"


//-----------------------------------------------------------------------------
/**
* This class represent a XML tag containing a storage command.
* @author Pascal Francq
* @short Generic XML Storage tag.
*/
class GStorageTag : public R::RXMLTag
{
public:

	/**
	* Construct a XML Tag.
	* @param cmd             Name of the cmd.
	*/
	GStorageTag(const RString& cmd);
};


//-----------------------------------------------------------------------------
/**
* This class represents a generic command that a storage must execute.
* @author Pascal Francq
* @short Generic Storage Command.
*/
class GStorageCmd
{
	/**
	* Name of the command.
	*/
	R::RString Cmd;

	/**
	* Name of the storage manager supported.
	*/
	R::RString Storage;

public:

	/**
	* Constructor of a command.
	* @param cmd             Name of the command.
	* @param storage         Name of the storage.
	*/
	GStorageCmd(const R::RString cmd,const R::RString storage);

	/**
	* Compare two commands.
	* @param cmd             Command to compare to.
	*/
	int Compare(const GStorageCmd& cmd) const;

	/**
	* Compare a command with a name.
	* @param cmd             Name to compare to.
	*/
	int Compare(const R::RString& cmd) const;

	/**
	* Get the name of the command.
	*/
	R::RString GetName(void) const;

	/**
	* Get the name of the storage.
	*/
	R::RString GetStorage(void) const;

	/**
	* Connect to a storage.
	* @param storage         Pointer to the storage.
	*/
	virtual void Connect(GStorage* storage);

	/**
	* Disconnect to the storage.
	* @param storage         Pointer to the storage.
	*/
	virtual void Disconnect(GStorage* storage);

	/**
	* Run the command.
	* @param storage         Pointer to the command.
	* @param inst            XML tag representing the instruction.
	* @param caller          Caller of the function.
	*/
	virtual void Run(GStorage* storage,const GStorageTag& inst,void* caller)=0;

	/**
	* Destructor.
	*/
	virtual ~GStorageCmd(void);
};


//-----------------------------------------------------------------------------
/**
* The GStorage provides a representation for a storage manager. This storage
* manager is responsible to load and save data.
* @author GALILEI Team
* @short Generic Storage Manager.
*/
class GStorage : public GPlugin<GFactoryStorage>
{
protected:

	/**
	* All the objects must be loaded (true) or only those which are modified
	* (false).
	*/
	bool LoadAll;

	/**
	* Date used to filter when load from the storage.
	*/
	R::RDate Filter;

	/**
	* Is there a filter (Filter date is not the current one).
	*/
	bool Filtering;

	/**
	* All objects are in memory.
	*/
	bool AllMemory;

	/**
	* All available commands.
	*/
	R::RContainer<GStorageCmd,false,true> Commands;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GStorage(GFactoryStorage* fac);

	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

	/**
	* Get a string representing the world stored. This string must be a valid
	* directory name since it is used everywhere.
	*/
	virtual RString GetWorld(void) const=0;

	/**
	* Initialize the access to the storage.
	*/
	virtual void InitAccess(void)=0;

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Compute the number of objects of a given type which are saved.
	* @param type            Type of the objects.
	* @return Number of objects.
	*/
	virtual unsigned int GetNbSaved(tObjType type)=0;

	/**
	* Get a pointer to a structure that should represent the storage. It can be
	* used to give access to some internal methods specific to a given storage.
	* It should be used carefully.
	*/
	virtual void* GetInfos(void);

	/**
	* Are all the object loaded or just some of them (modified or because
	* filters).
	*/
	bool IsAllInMemory(void) const;

	/**
	* Using a date for filtering.
	*/
	bool UseFiltering(void) const;

	/**
	*  Get the filtering date.
	*/
	R::RDate GetFilter(void) const;

	/**
	* Verify if a given command is supported by the storage.
	* @param cmd             Name of the command.
	*/
	bool IsCmdSupported(const R::RString cmd) const;

	/**
	* Execute a command.
	* @param inst            XML Tag representing a command and its parameters.
	* @param caller          Caller of the function.
	*/
	void ExecuteCmd(const GStorageTag& inst,void* caller);

	/**
	* Insert a new command in the storage. The command is inserted if :
	* -# The name of the storage must be compatible with the one of the command.
	* -# Another command with the same name does not exist.
	* @param                 cmd Command to insert.
	*/
	void InsertCmd(GStorageCmd* cmd);
	
	/**
	* Create a dummy table to store different kid of
	* data.
	* @param name            Name of the dummy object.
	*/
	virtual void CreateDummy(R::RString name)=0;

	/**
	* Delete all the data of a given dummy table.
	* @ param name           Name of the dummy object.
	*/
	virtual void ClearDummy(R::RString name)=0;

	/**
	* Add a dummy entry into  a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	*/
	virtual void AddDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid)=0;

	/**
	* Select a dummy entry from a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	* @param filter          Filter to set the fields on which the select is done.
	*/
	virtual R::RQuery* SelectDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter)=0;

	/**
	* Clear a dummy entry from a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	* @param filter          Filter to set the fields on which the select is done.
	*/
	virtual void ClearDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter)=0;

	/**
	* Load the Subjects.
	*/
	virtual void LoadSubjects(void)=0;

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename        Name of the file.
	*/
	virtual void ExecuteData(const R::RString& filename)=0;

	/**
	* Prepare a given number of suggestions for a test. If the name is null,
	* all the tests must be deleted.
	* @param name            Name of the test.
	*/
	virtual void CreateSugs(const R::RString& name)=0;

	/**
	* Get the different tests that were made.
	* @param res             Container that will hold the names.
	*/
	virtual void GetSugsTests(R::RContainer<R::RString,true,true>& res)=0;

	/**
	* Clear the information of a given object type from the storage.
	*/
	virtual void Clear(tObjType objtype)=0;

	//@} General methods


	//-----------------------------------------------------
	/** @name Knowledge Methods
	*/
	// @{

	/**
	* Load the concept types from the database.
	*/
	virtual void LoadConceptTypes(void)=0;

	/**
	* Load the relation types from the database.
	*/
	virtual void LoadRelationTypes(void)=0;

	/**
	* Load the relation from the database.
	*/
	virtual void LoadRelations(void)=0;

	/**
	* Loading the all concepts of given concept type.
	* @param type            Type of concepts.
	*/
	virtual void LoadConcepts(GConceptType* type)=0;

	/**
	* Assign an identifier to a new concept of a given type.
	* @param concept         Concept.
	*/
	virtual void AssignId(GConcept* concept)=0;

	/**
	* Delete a concept.
	* @param concept         Concept.
	*/
	virtual void DeleteConcept(GConcept* concept)=0;

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param id              Identificator of the object.
	*/
	virtual void LoadInfos(R::RContainer<GWeightInfo,false,true>& infos,tObjType type,size_t id)=0;
	
	/**
	* Load the name of specific concept.
	* @param id              Idenfificator of the concept.
	* @param type            Type of the concept.
	*/
	virtual R::RString LoadConcept(unsigned int id,GConceptType* type)=0;

	/**
	* Load the identificator of a specific concept.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	virtual size_t LoadConcept(const R::RString name,GConceptType* type)=0;

	/**
	* Save a concept in the database.
	* @param concept         Concept.
	*/
	virtual void SaveConcept(GConcept* concept)=0;

	/**
	* Save the references of a given object type for a given concept.
	* @param concept         Concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).	
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(const GConcept* concept,tObjType what,size_t refs)=0;

	/**
	* Save the references of a given object type for a given concept type.
	* @param type            Type of the concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(GConceptType* type,tObjType what,size_t refs)=0;

	// @} Knowledge Methods


	//-----------------------------------------------------
	/** @name Documents Methods
	*/
	// @{

	/**
	* Assign an identifier to a new document.
	* @param doc             Document.
	*/
	virtual void AssignId(GDoc* doc)=0;

	/**
	* Load an indexer.
	* @param indexer         Pointer to the indexer.
	* @param langs           Pointer to the langs.
	*/
	virtual void LoadIndexer(GIndexer* &indexer,GLangManager* langs)=0;

	/**
	* Method that load a document that is stored.
	* @param docid           Identificator of the document
	*/
	virtual GDoc* LoadDoc(unsigned int docid)=0;

	/**
	* Method that load the documents from where they are stored. This method
	* must be overloaded.
	*/
	virtual void LoadDocs(void)=0;

	/**
	* Save a document where it is stored. This method is called after an
	* analsyis of a document if the result has to be saved. This method must be
	* overloaded.
	* @param doc             Document to save.
	*/
	virtual void SaveDoc(GDoc* doc)=0;

	//@} Documents


	//-----------------------------------------------------
	/** @name Users/Profiles/Subprofiles Methods
	*/
	// @{

	/**
	* Load the subprofiles (and the profiles and the users).
	*/
	virtual void LoadUsers(void)=0;

	/**
	* Method that load a user that is stored.
	* @param userid          Identificator of the user.
	*/
	virtual GUser* LoadUser(unsigned int userid)=0;

	/**
	* Method that load a user that is stored.
	* @param name            Name of the user.
	*/
	virtual GUser* LoadUser(const R::RString name)=0;

	/**
	* Method that load a profile that is stored.
	* @param profileid       Identificator of the profile.
	*/
	virtual GProfile* LoadProfile(unsigned int profileid)=0;

	/**
	* Method that load a subprofile that is stored.
	* @param subprofileid    Identificator of the subprofile.
	*/
	virtual GSubProfile* LoadSubProfile(unsigned int subprofileid)=0;

	/**
	* Load the list of suggestions for the profiles for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res)=0;

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identificator of the document.
	* @param lang            Language of the document.
	*/
	virtual void UpdateProfiles(unsigned int docid,GLang* lang)=0;

	/**
	* Assign an identifier to a new user.
	* @param user            Pointer to the user.
	*/
	virtual void AssignId(GUser* user)=0;

	/**
	* Assign an identifier to a new profile.
	* @param p               Pointer to the profile.
	*/
	virtual void AssignId(GProfile* p)=0;

	/**
	* Save a user.
	* @param user            User to save.
	*/
	virtual void SaveUser(GUser* user)=0;

	/**
	* Save a profile.
	* @param prof            Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof)=0;

	/**
	* Save profiles in history
	* @param historicID      Identificator of the historic.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicID)=0;

	/**
	* Assign an identifier to a new subprofile.
	* @param sub             Subprofile.
	*/
	virtual void AssignId(GSubProfile* sub)=0;

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub             Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub)=0;

	/**
	* Add a suggestion for a given profile and a given test.
	* @param name            Name of the test.
	* @param profileid       Identificator of the profile.
	* @param docid           Identificator of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsProfile(const R::RString& name,unsigned int profileid,unsigned int docid,unsigned int rank)=0;

	/**
	* Add an assessment for a given profile and document.
	* @param p               Identificator of the profile.
	* @param d               Identificator of the document.
	* @param lang            Language of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(unsigned int p,unsigned int d,GLang* lang,tDocAssessment assess,R::RDate date,R::RDate computed)=0;


	// @} Users/Profiles/Subprofiles


	//-----------------------------------------------------
	/** @name Groups Methods
	*/
	// @{

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void)=0;

	/**
	* Method that load a group that is stored.
	* @param lang            Language of the group.
	* @param groupid         Identificator of the group.
	*/
	virtual GGroup* LoadGroup(GLang* lang,unsigned int groupid)=0;

	/**
	* Load an historic groups.
	* @param historicID      Identificator of the historic.
	* @return Pointer to a historic group.
	*/
	virtual GGroupsHistory* LoadAnHistoricGroups(unsigned int historicID)=0;

	/**
	* Load the historic groups.
	* @param mindate         Date of the first historic to load.
	* @param maxdate         Date of the last historic to load.
	*/
	virtual void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate)=0;

	/**
	* Load the list of suggestions for the groups for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsGroups(const R::RString& name,R::RContainer<GSugs,true,false>& res)=0;

	/**
	* Returns the number of historic groups stored in the database.
	*/
	virtual unsigned int GetHistorySize(void)=0;

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	virtual void UpdateGroups(unsigned int subid)=0;

	/**
	* Assign an identifier to a new group.
	* @param grp            Group.
	*/
	virtual void AssignId(GGroup* grp)=0;

	/**
	* Save the groups of the session.
	* @param lang            Define the language of the groups to save.
	*/
	virtual void SaveGroups(GLang* lang)=0;

	/**
	* Save the groups in history.
	*/
	virtual void SaveGroupsHistory(void)=0;

	/**
	* Add a suggestion for a given community and a given test.
	* @param name            Name of the test.
	* @param groupid         Identificator of the community.
	* @param docid           Identificator of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsGroup(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank)=0;

	// @} Groups

	/**
	* Destructor.
	*/
	virtual ~GStorage(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryStorage represent a factory for a given storage.
* @author Pascal Francq
* @short Generic Storage Factory.
*/
class GFactoryStorage : public GFactoryPlugin<GFactoryStorage,GStorage,GStorageManager>
{

public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryStorage(GStorageManager* mng,const char* n,const char* f);

	/**
	* Create a plugin. Call the method 'InitAccess' from GStorage.
	*/
	virtual void Create(void);

	/**
	* Create a plugin. If there is a current session, generate an exception.
	*/
	virtual void Create(GSession* session);

	/**
	* Update a database.
	*/
	virtual void UpdateSchema(const R::RXMLStruct& schema)=0;
};


//------------------------------------------------------------------------------
/**
* The GStorageManager class provides a representation for a manager
* responsible to manage all storages.
* @author Pascal Francq
* @short Storages Manager.
*/
class GStorageManager : public GPluginManager<GStorageManager,GFactoryStorage,GStorage>
{
private:

	/**
	* All available commands.
	*/
	R::RContainer<GStorageCmd,false,true> Commands;

public:

	/**
	* Construct the storages manager.
	*/
	GStorageManager(void);

	/**
	* Signal that a plug-in was enabled.
	* @param plug            Plug-in enabled.
	*/
	virtual void EnablePlugIn(GStorage* plug);

	/**
	* Signal that a plug-in was disabled.
	* @param plug            Plug-in disabled.
	*/
	virtual void DisablePlugIn(GStorage* plug);


	/**
	* Add a specific command
	*/
	bool InsertCmd(GStorageCmd* cmd);

	/**
	* Destructor of the storages manager.
	*/
	virtual ~GStorageManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_DBFACTORY(manager,factory,genericplugin,plugin,lib,API,name)    \
	class TheFactory : public factory                                          \
{                                                                              \
	static factory* Inst;                                                      \
	TheFactory(manager* mng,const char* l) : factory(mng,name,l)               \
	{                                                                          \
		plugin::CreateParams(this);                                            \
	}                                                                          \
	virtual ~TheFactory(void) {}                                               \
public:                                                                        \
	static factory* CreateInst(manager* mng,const char* l)                     \
	{                                                                          \
		if(!Inst)                                                              \
			Inst = new TheFactory(mng,l);                                      \
		return(Inst);                                                          \
	}                                                                          \
	virtual const char* GetAPIVersion(void)      const {return(API);}          \
	virtual void UpdateSchema(const R::RXMLStruct& schema);                    \
	virtual genericplugin* NewPlugIn(void)                                     \
	{                                                                          \
		return(new plugin(this));                                              \
	}                                                                          \
	virtual void DeletePlugIn(genericplugin* plug)                             \
	{                                                                          \
		delete plug;                                                           \
	}                                                                          \
};                                                                             \
factory* TheFactory::Inst = 0;                                                 \
                                                                               \
extern "C" factory* FactoryCreate(manager* mng,const char* l)                  \
{                                                                              \
	return(TheFactory::CreateInst(mng,l));                                     \
}                                                                              \
extern "C" const char* LibType(void)                                           \
{                                                                              \
	return(lib);                                                               \
}


//-------------------------------------------------------------------------------
#define CREATE_STORAGE_FACTORY(name,plugin)\
	CREATE_DBFACTORY(GStorageManager,GFactoryStorage,GStorage,plugin,"Storage",API_STORAGE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
