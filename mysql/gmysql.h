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
#include <rmysql.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstorage.h>
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


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
	* MySQL Database.
	*/
	std::auto_ptr<R::RDb> Db;

	R::RString Host;
	R::RString User;
	R::RString Password;
	R::RString Database;
	R::RString Encoding;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GStorageMySQL(GFactoryStorage* fac);

protected:

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	virtual unsigned int GetCount(R::RString tbl);

	/**
	* Count the maximal value for a field of a table.
	* @param tbl            Table to analyse.
	* @param fld            Field to analyse.
	* @returns Maximal value.
	*/
	unsigned int GetMax(R::RString tbl,R::RString fld);

	/**
	* Transform a MySQL date into a C string, in particular when the
	* parameter is null, it chooses '1970-01-01'.
	* @param date           MySQL date.
	* @return string.
	*/
	R::RString GetMySQLToDate(R::RString date);

	/**
	 * Return a SQL string for a given unsigned int.
	 * @param i              The number.
	 */
	inline R::RString Num(unsigned int i) {return("'"+R::RString::Number(i)+"'");}

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
	virtual unsigned int GetNbSaved(tObjType type);

	/**
	* Get a pointer to a structure that should represent the storage. It can be
	* used to give access to some internal methods specific to a given storage.
	* It should be used carefully.
	*/
	virtual void* GetInfos(void);

	/**
	* Create a dummy table to store different kid of
	* data.
	* @param name            Name of the dummy object.
	*/
	virtual void CreateDummy(R::RString name);

	/**
	* Delete all the data of a given dummy table.
	* @ param name           Name of the dummy object.
	*/
	virtual void ClearDummy(R::RString name);

	/**
	* Add a dummy entry into  a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	*/
	virtual void AddDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid);

	/**
	* Select a dummy entry from a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	* @param filter          Filter to set the fields on which the select is done.
	*/
	virtual R::RQuery* SelectDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter);

	/**
	* Clear a dummy entry from a dummy table;
	* @param name            Name of the dummy table.
	* @param id              Identificator of the dymmy entry.
	* @param desc            Description of the dymmy entry.
	* @param parentid        Identificator of the parent.
	* @param filter          Filter to set the fields on which the select is done.
	*/
	virtual void ClearDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter);

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
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param id              Identificator of the object.
	*/
	virtual void LoadInfos(R::RContainer<GWeightInfo,false,true>& infos,tObjType type,size_t id);

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
	* Delete a data from the dictionnary.
	* @param concept         Concept.
	*/
	virtual void DeleteConcept(GConcept* concept);

	/**
	* Load the name of specific concept.
	* @param id              Idenfificator of the concept.
	* @param type            Type of the concept.
	*/
	virtual R::RString LoadConcept(unsigned int id,GConceptType* type);

	/**
	* Load the identificator of a specific concept.
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
	* @param docid           Identificator of the document
	*/
	virtual GDoc* LoadDoc(unsigned int docid);

	/**
	 * Load a specific node.
	 * @param nodes          Query on all nodes of the corresponding document.
	 * @param content        Query on all content of the corresponding document.
	 * @param doc            Document.
	 * @param docstruct      Structure to load.
	 * @param parent         Parent node.
	 */
	void LoadNode(R::RQuery& nodes,R::RQuery& content,const GDoc* doc,GDocStruct* docstruct,GDocStructNode* parent);

	/**
	 * Method that load the structure of a document.
	 * @param doc            Document.
	 */
	virtual GDocStruct* LoadStruct(const GDoc* doc);

	/**
	 * Save a specific node.
	 * @param node           Node to save.
	 * @param topcontent     Id of the first content.
	 * @param nbcontent      Total number of content.
	 * @return Identifier of the first subnode.
	 */
	size_t SaveNode(GDocStructNode* node,size_t& topcontent,size_t& nbcontent);

	/**
	 * Method that save the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param docid          Identifier of the document.
	 */
	virtual void SaveStruct(GDocStruct* docstruct,size_t docid);

	/**
	* Method that load the documents from where they are stored. This method
	* must be overloaded.
	*/
	virtual void LoadDocs(void);

	/**
	* Save a document where it is stored. This method is called after an
	* analsyis of a document if the result has to be saved. This method must be
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
	* Load the subprofiles (and the profiles and the users).
	*/
	virtual void LoadUsers(void);

	/**
	* Method that load a user that is stored.
	* @param userid          Identificator of the user.
	*/
	virtual GUser* LoadUser(unsigned int userid);

	/**
	* Method that load a user that is stored.
	* @param name            Name of the user.
	*/
	virtual GUser* LoadUser(const R::RString name);

	/**
	* Method that load a profile that is stored.
	* @param profileid       Identificator of the profile.
	*/
	virtual GProfile* LoadProfile(unsigned int profileid);

	/**
	* Load the list of suggestions for the profiles for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identificator of the document.
	*/
	virtual void UpdateProfiles(unsigned int docid);

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
	* Save profiles in histoty
	* @param sub            Subprofile to save.
	* @param historicID     Identificator of the historic.
	*/
	virtual void SaveProfileInHistory(GProfile* sub, unsigned int historicID);

	/**
	* Save profiles in history
	* @param historicID      Identificator of the historic.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicID);

	/**
	* Add a suggestion for a given profile and a given test.
	* @param name            Name of the test.
	* @param profileid       Identificator of the profile.
	* @param docid           Identificator of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsProfile(const R::RString& name,unsigned int profileid,unsigned int docid,unsigned int rank);

	/**
	* Add an assessment for a given profile and document.
	* @param p               Identificator of the profile.
	* @param d               Identificator of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date,R::RDate computed);


	// @} Users/Profiles


	//-----------------------------------------------------
	/** @name Groups Methods
	*/
	// @{

	/**
	* Load the groups.
	*/
	virtual void LoadCommunities(void);

	/**
	* Method that load a group that is stored.
	* @param groupid         Identificator of the group.
	*/
	virtual GCommunity* LoadCommunity(unsigned int groupid);

	/**
	* Load an historic groups.
	* @param historicID      Identificator of the historic.
	* @return Pointer to a historic group.
	*/
	virtual GCommunitiesHistory* LoadHistoricCommunities(unsigned int historicID);

	/**
	* Load the historic groups.
	* @param mindate         Date of the first historic to load.
	* @param maxdate         Date of the last historic to load.
	*/
	virtual void LoadHistoricCommunitiesByDate(R::RString mindate, R::RString maxdate);

	/**
	* Load the list of suggestions for the groups for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsCommunities(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* Returns the number of historic groups stored in the database.
	*/
	virtual unsigned int GetHistorySize(void);

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	virtual void UpdateCommunities(unsigned int subid);

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
	* Save the groups in history.
	*/
	virtual void SaveCommunitiesHistory(void);

	/**
	* Add a suggestion for a given community and a given test.
	* @param name            Name of the test.
	* @param groupid         Identificator of the community.
	* @param docid           Identificator of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsCommunity(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank);

	// @} Groups

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GStorageMySQL(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
