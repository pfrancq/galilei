/*

	GALILEI Research Project

	GStorage.h

	Generic Storage Manager - Header.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rxmltag.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GStorage provides a representation for a storage manager. This storage
* manager is responsible to load and save data.
* @author GALILEI Team
* @short Generic Storage Manager.
*/
class GStorage : public GPlugIn
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

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GStorage(GSession* session,GPlugInFactory* fac);

	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyParams(void);

	/**
	* Compute the number of objects of a given type which are saved.
	* @param type            Type of the objects.
	* @return Number of objects.
	*/
	virtual size_t GetNbSaved(tObjType type)=0;

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
	* Load the Subjects.
	*/
	virtual void LoadSubjects(void)=0;

	/**
	 * Save the subject, including the information on the documents (and which
	 * ones are used) and the profiles assigned to them.
	 * @param subject        Subject to save.
	 */
	virtual void SaveSubject(GSubject* subject)=0;

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename        Name of the file.
	*/
	virtual void ExecuteData(const R::RString& filename)=0;

	/**
	* Clear the information of a given object type from the storage.
	* @param objtype         Type of the object.
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
	* Assign an identifier to a new concept type.
	* @param type            Concept type.
	*/
	virtual void AssignId(GConceptType* type)=0;

	/**
	* Loading all concepts.
	*/
	virtual void LoadConcepts(void)=0;

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
	* Load the name of specific concept.
	* @param id              Identifier of the concept.
	*/
	virtual R::RString LoadConcept(size_t id)=0;

	/**
	* Load the identifier of a specific concept.
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
	 * Save the index information of a given object type for a given concept.
	 * @param concept         Concept.
	 * @param what            Type of the object (Only otDoc supported).
	 * @param indexdocs       Identifier of the block containing the index.
	 */
	virtual void SaveIndex(const GConcept* concept,tObjType what,size_t indexdocs)=0;

	/**
	* Save the references of a given object type for a given concept type. If
	* the number of references is null, the method must reset all the
	* references for the corresponding concepts.
	* @param type            Type of the concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(GConceptType* type,tObjType what,size_t refs)=0;

	/**
	* Load the predicates from the database.
	*/
	virtual void LoadPredicates(void)=0;

	/**
	* Assign an identifier to a new predicate.
	* @param predicate       Predicate.
	*/
	virtual void AssignId(GPredicate* predicate)=0;

	/**
	* Load the statements from the database.
	*/
	virtual void LoadStatements(void)=0;

	/**
	* Assign an identifier to a new statement.
	* @param statement       Statement.
	*/
	virtual void AssignId(GStatement* statement)=0;

protected:

	/**
	* Method that load the classes from where they are stored. This method is
	* called once by the session.
	*/
	virtual void LoadClasses(void)=0;

public:

	/**
	* Assign an identifier to a new class.
	* @param theclass        Class.
	*/
	virtual void AssignId(GClass* theclass)=0;

	/**
	 * Save a class.
	 * @param theclass        Class.
	 */
	virtual void SaveClass(GClass* theclass)=0;

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
	* Method that load a document that is stored.
	* @param docid           Identifier of the document
	*/
	virtual GDoc* LoadDoc(size_t docid)=0;

protected:

	/**
	* Method that load the documents from where they are stored. This method is
	* called once by the session.
	*/
	virtual void LoadDocs(void)=0;

public:

	/**
	* Save a document where it is stored. This method is called after an
	* analysis of a document if the result has to be saved. This method must be
	* overloaded.
	* @param doc             Document to save.
	*/
	virtual void SaveDoc(GDoc* doc)=0;

	//@} Documents


	//-----------------------------------------------------
	/** @name Users/Profiles Methods
	*/
	// @{

protected:

	/**
	* Load the profiles and the users. This method is called once by the
	* session.
	*/
	virtual void LoadUsers(void)=0;

public:

	/**
	* Method that load a user that is stored.
	* @param userid          Identifier of the user.
	*/
	virtual GUser* LoadUser(size_t userid)=0;

	/**
	* Method that load a user that is stored.
	* @param name            Name of the user.
	*/
	virtual GUser* LoadUser(const R::RString name)=0;

	/**
	* Method that load a profile that is stored.
	* @param profileid       Identifier of the profile.
	*/
	virtual GProfile* LoadProfile(size_t profileid)=0;

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identifier of the document.
	*/
	virtual void UpdateProfiles(size_t docid)=0;

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
	* Add a feedback for a given profile and document.
	* @param profid          Identifier of the profile.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate date,R::RDate computed)=0;


	// @} Users/Profiles


	//-----------------------------------------------------
	/** @name Communities Methods
	*/
	// @{

protected:

	/**
	* Load the groups. This method is called once by the session.
	*/
	virtual void LoadCommunities(void)=0;

public:

	/**
	* Method that load a group that is stored.
	* @param groupid         Identifier of the group.
	*/
	virtual GCommunity* LoadCommunity(size_t groupid)=0;

	/**
	* A profile was updated and the corresponding communities must be updated.
	* @param profid          Identifier of the profile.
	*/
	virtual void UpdateCommunities(size_t profid)=0;

	/**
	* Assign an identifier to a new community.
	* @param grp            Community.
	*/
	virtual void AssignId(GCommunity* grp)=0;

	/**
	* Save a community.
	* @param grp            Community.
	*/
	virtual void SaveCommunity(GCommunity* grp)=0;

	/**
	 * Save the relevant documents associated with a given community.
	 * @param docs           List of relevant.
	 */
	virtual void SaveDocs(const GCommunityDocs& docs)=0;

	/**
	 * Load the relevant documents associated with a given community.
	 * @param docs           List of relevant.
	 */
	virtual void LoadDocs(GCommunityDocs& docs)=0;

	// @} Communities


	//-----------------------------------------------------
	/** @name Topics Methods
	*/
	// @{

protected:

	/**
	* Load the topics. This method is called once by the session.
	*/
	virtual void LoadTopics(void)=0;

public:

	/**
	* Method that load a topic that is stored.
	* @param topicid         Identifier of the topic.
	*/
	virtual GTopic* LoadTopic(size_t topicid)=0;

	/**
	* A document was updated and the corresponding topics must be updated.
	* @param docid           Identifier of the document.
	*/
	virtual void UpdateTopics(size_t docid)=0;

	/**
	* Assign an identifier to a new topic.
	* @param top            Topic.
	*/
	virtual void AssignId(GTopic* top)=0;

	/**
	* Save a topic.
	* @param grp            Topic.
	*/
	virtual void SaveTopic(GTopic* grp)=0;

	// @} Topics


	//-----------------------------------------------------
	/** @name Suggestions Methods
	*/
	// @{

	/**
	* Save a list of suggestions.
	* @param sugs            Suggestions.
	* @param max             Maximum number of suggestions to save.
	*/
	virtual void SaveSugs(const GSugs& sugs,size_t max=R::cNoRef)=0;

	/**
	* Load a list of suggestions.
	* @param sugs            Suggestions.
	*/
	virtual void LoadSugs(GSugs& sugs)=0;

	// @} Suggestions


	/**
	* Destruct.
	*/
	virtual ~GStorage(void);

	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_STORAGE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GStorage,plugin,"Storage",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
