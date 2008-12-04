/*

	GALILEI Research Project

	GStorageMySQL.h

	Storage Manager using a MySQL Database - Header.

	Copyright 2001-2008 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GStorageMySQLH
#define GStorageMySQLH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <memory.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rdb.h>
using namespace std;
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstorage.h>
#include <galilei.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
/**
* The GStorageMySQL class provides a representation for a storage manager using
* a MySQL Database as store medium.
* @author Pascal Francq
* @short MySQL-based Storage Manager.
*/
class GStorageMySQL : public GStorage
{
	/**
	* Database.
	*/
	auto_ptr<R::RDb> Db;

	/**
	 * Host of the database.
	 */
	RString Host;

	/**
	 * User of the database.
	 */
	RString User;

	/**
	 * Password needed to access.
	 */
	RString Password;

	/**
	 * Name of the database.
	 */
	RString Database;

	/**
	 * Encoding of the database.
	 */
	RString Encoding;

	/**
	 * Directory where to binary files are stored.
	 */
	RString Dir;

	/**
	 * Root dir for the documents.
	 */
	RString DirDocs;

	/**
	 * Root dir for the profiles.
	 */
	RString DirProfiles;

	/**
	 * Root dir for the communities.
	 */
	RString DirCommunities;

	/**
	 * Root dir for the topics.
	 */
	RString DirTopics;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GStorageMySQL(GFactoryStorage* fac);

protected:

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyze.
	* @returns Number of rows.
	*/
	virtual size_t GetCount(R::RString tbl);

	/**
	* Count the maximal value for a field of a table.
	* @param tbl            Table to analyze.
	* @param fld            Field to analyze.
	* @returns Maximal value.
	*/
	size_t GetMax(R::RString tbl,R::RString fld);

	/**
	* Transform a MySQL date into a C string, in particular when the
	* parameter is null, it chooses '1970-01-01'.
	* @param date           MySQL date.
	* @return string.
	*/
	R::RString GetMySQLToDate(R::RString date);

	/**
	 * Return a SQL string for a given char.
	 * @param i              The number.
	 */
	inline R::RString Num(char i) {return("'"+R::RString::Number(i)+"'");}

	/**
	 * Return a SQL string for a given unsigned int.
	 * @param i              The number.
	 */
	inline R::RString Num(size_t i) {return("'"+R::RString::Number(i)+"'");}

	/**
	 * Return a SQL string for a given double.
	 * @param d              The number.
	 */
	R::RString Num(double d);

	/**
	 * Return a double from a string.
	 */
	double atof(const R::RString& str);

	/**
	 * Return a SQL string for a given language.
	 */
	inline R::RString Lang(GLang* lang) {return("'"+R::RString(lang->GetCode())+"'");}

public:

	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

	/**
	* Get a string representing the world stored. This string must be a valid
	* directory name since it is used everywhere.
	*/
	virtual R::RString GetWorld(void) const;

	/**
	* Initialize the access to the storage.
	*/
	virtual void InitAccess(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Compute the number of objects of a given type which are saved.
	* @param type            Type of the objects.
	* @return Number of objects.
	*/
	virtual size_t GetNbSaved(tObjType type);

	/**
	* Get a pointer to a structure that should represent the storage. It can be
	* used to give access to some internal methods specific to a given storage.
	* It should be used carefully.
	*/
	virtual void* GetInfos(void);

	/**
	* Load the Subjects.
	*/
	virtual void LoadSubjects(void);

	/**
	 * Save the subjects, including the information on the documents and the
	 * profiles assigned to them.
	 */
	virtual void SaveSubjects(void);

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename        Name of the file.
	*/
	virtual void ExecuteData(const R::RString& filename);

	/**
	* Prepare a given number of suggestions for a test. If the name is null,
	* all the tests must be deleted.
	* @param name            Name of the test.
	*/
	virtual void CreateSugs(const R::RString& name);

	/**
	* Get the different tests that were made.
	* @param res             Container that will hold the names.
	*/
	virtual void GetSugsTests(R::RContainer<R::RString,true,true>& res);

	/**
	* Clear the information of a given object type from the storage.
	*/
	virtual void Clear(tObjType objtype);

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param id              Identifier of the object.
	*/
	virtual void LoadInfos(GWeightInfos& infos,tObjType type,size_t id);

	/**
	* Save the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param id              Identifier of the object.
	*/
	virtual void SaveInfos(GWeightInfos& infos,tObjType type,size_t id);


	//@} General methods


	//-----------------------------------------------------
	/** @name Knowledge Methods
	*/
	// @{

	/**
	* Load the concept types from the database.
	*/
	virtual void LoadConceptTypes(void);

	/**
	* Assign an identifier to a new concept type.
	* @param type            Concept type.
	*/
	virtual void AssignId(GConceptType* type);

	/**
	* Load the relation types from the database.
	*/
	virtual void LoadRelationTypes(void);

	/**
	* Load the relation from the database.
	*/
	virtual void LoadRelations(void);

	/**
	* Loading the all concepts of given concept type.
	* @param type            Type of concepts.
	*/
	virtual void LoadConcepts(GConceptType* type);

	/**
	* Loading the all concepts of given concept type.
	* @param concept         Concept.
	*/
	virtual void AssignId(GConcept* concept);

	/**
	* Delete a data from the dictionary.
	* @param concept         Concept.
	*/
	virtual void DeleteConcept(GConcept* concept);

	/**
	* Load the name of specific concept.
	* @param id              Identifier of the concept.
	* @param type            Type of the concept.
	*/
	virtual R::RString LoadConcept(size_t id,GConceptType* type);

	/**
	* Load the identifier of a specific concept.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	virtual size_t LoadConcept(const R::RString name,GConceptType* type);

	/**
	* Save a concept in the database.
	* @param concept         Concept.
	*/
	virtual void SaveConcept(GConcept* concept);

	/**
	* Save the references of a given object type for a given concept.
	* @param concept         Concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(const GConcept* concept,tObjType what,size_t refs);

	/**
	* Save the references of a given object type for a given concept type.
	* @param type            Type of the concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(GConceptType* type,tObjType what,size_t refs);

	// @} Knowledge Methods


	//-----------------------------------------------------
	/** @name Documents Methods
	*/
	// @{

	/**
	* Assign an identifier to a new document.
	* @param doc             Document.
	*/
	virtual void AssignId(GDoc* doc);

	/**
	* Load an indexer.
	* @param indexer         Pointer to the indexer.
	* @param langs           Pointer to the langs.
	*/
	virtual void LoadIndexer(GIndexer* &indexer,GLangManager* langs);

	/**
	* Method that load a document that is stored.
	* @param docid           Identifier of the document
	*/
	virtual GDoc* LoadDoc(size_t docid);

	/**
	 * Method that load the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param doc            Document.
	 */
	virtual void LoadStruct(GDocStruct& docstruct,GDoc* doc);

	/**
	 * Method that save the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param doc            Document.
	 */
	virtual void SaveStruct(GDocStruct& docstruct,GDoc* doc);

	/**
	* Method that load the documents from where they are stored. This method
	* must be overloaded.
	*/
	virtual void LoadDocs(void);

	/**
	* Save a document where it is stored. This method is called after an
	* analysis of a document if the result has to be saved. This method must be
	* overloaded.
	* @param doc             Document to save.
	*/
	virtual void SaveDoc(GDoc* doc);

	//@} Documents


	//-----------------------------------------------------
	/** @name Users/Profiles Methods
	*/
	// @{

	/**
	* Load the profiles (and the profiles and the users).
	*/
	virtual void LoadUsers(void);

	/**
	* Method that load a user that is stored.
	* @param userid          Identifier of the user.
	*/
	virtual GUser* LoadUser(size_t userid);

	/**
	* Method that load a user that is stored.
	* @param name            Name of the user.
	*/
	virtual GUser* LoadUser(const R::RString name);

	/**
	* Method that load a profile that is stored.
	* @param profileid       Identifier of the profile.
	*/
	virtual GProfile* LoadProfile(size_t profileid);

	/**
	* Load the list of suggestions for the profiles for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identifier of the document.
	*/
	virtual void UpdateProfiles(size_t docid);

	/**
	* Assign an identifier to a new user.
	* @param user            Pointer to the user.
	*/
	virtual void AssignId(GUser* user);

	/**
	* Assign an identifier to a new profile.
	* @param p               Pointer to the profile.
	*/
	virtual void AssignId(GProfile* p);

	/**
	* Save a user.
	* @param user            User to save.
	*/
	virtual void SaveUser(GUser* user);

	/**
	* Save a profile.
	* @param prof            Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof);

	/**
	* Save profiles in history
	* @param sub            Profile to save.
	* @param historicID     Identifier of the historic.
	*/
	virtual void SaveProfileInHistory(GProfile* sub, size_t historicID);

	/**
	* Save profiles in history
	* @param historicID      Identifier of the historic.
	*/
	virtual void SaveHistoricProfiles(size_t historicID);

	/**
	* Add a suggestion for a given profile and a given test.
	* @param name            Name of the test.
	* @param profileid       Identifier of the profile.
	* @param docid           Identifier of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsProfile(const R::RString& name,size_t profileid,size_t docid,size_t rank);

	/**
	* Add an assessment for a given profile and document.
	* @param p               Identifier of the profile.
	* @param d               Identifier of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(size_t p,size_t d,tDocAssessment assess,R::RDate date,R::RDate computed);


	// @} Users/Profiles


	//-----------------------------------------------------
	/** @name Communities Methods
	*/
	// @{

	/**
	* Load the groups.
	*/
	virtual void LoadCommunities(void);

	/**
	* Method that load a group that is stored.
	* @param groupid         Identifier of the group.
	*/
	virtual GCommunity* LoadCommunity(size_t groupid);

	/**
	* Load the list of suggestions for the groups for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsCommunities(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* A profile was updated and the corresponding groups must be updated.
	* @param subid           Identifier of the profile.
	*/
	virtual void UpdateCommunities(size_t subid);

	/**
	* Assign an identifier to a new group.
	* @param grp            Group.
	*/
	virtual void AssignId(GCommunity* grp);

	/**
	* Save the groups of the session.
	*/
	virtual void SaveCommunities(void);

	/**
	* Add a suggestion for a given community and a given test.
	* @param name            Name of the test.
	* @param groupid         Identifier of the community.
	* @param docid           Identifier of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsCommunity(const R::RString& name,size_t groupid,size_t docid,size_t rank);

	// @} Communities


	//-----------------------------------------------------
	/** @name Topics Methods
	*/
	// @{

	/**
	* Load the topics.
	*/
	virtual void LoadTopics(void);

	/**
	* Method that load a topic that is stored.
	* @param topicid         Identifier of the topic.
	*/
	virtual GTopic* LoadTopic(size_t groupid);

	/**
	* A document was updated and the corresponding topics must be updated.
	* @param docid           Identifier of the document.
	*/
	virtual void UpdateTopics(size_t docid);

	/**
	* Assign an identifier to a new topic.
	* @param topic            Topic.
	*/
	virtual void AssignId(GTopic* grp);

	/**
	* Save the topics of the session.
	*/
	virtual void SaveTopics(void);

	// @} Topics


	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destruct.
	*/
	virtual ~GStorageMySQL(void);
};


//------------------------------------------------------------------------------
#endif
