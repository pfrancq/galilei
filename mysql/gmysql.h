/*

	GALILEI Research Project

	GMySQL.h

	Storage Manager using a MySQL Database - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef GStorageMySQLH
#define GStorageMySQLH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <memory.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rdbmysql.h>
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
	* MySQL Database.
	*/
	RDbMySQL* Db;

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
	RCString Encoding;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GStorageMySQL(GSession* session,GPlugInFactory* fac);

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
	inline R::RString Num(int i) {return("'"+R::RString::Number(i)+"'");}

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
	* Initialize the access to the storage.
	*/
	virtual void Init(void);

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
	 * Save the subject, including the information on the documents (and which
	 * ones are used) and the profiles assigned to them.
	 * @param subject        Subject to save.
	 */
	virtual void SaveSubject(GSubject* subject);

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename        Name of the file.
	*/
	virtual void ExecuteData(const R::RString& filename);

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
	* Loading the all concepts.
	*/
	virtual void LoadConcepts(void);

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
	*/
	virtual R::RString LoadConcept(size_t id);

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
	 * Save the index information of a given object type for a given concept.
	 * @param concept         Concept.
	 * @param what            Type of the object.
	 * @param indexdocs       Identifier of the block containing the index.
	 */
	virtual void SaveIndex(const GConcept* concept,tObjType what,size_t indexdocs);

	/**
	* Save the references of a given object type for a given concept type.
	* @param type            Type of the concept.
	* @param what            Type of the object (otDoc,otSubProfile,otGroup).
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(GConceptType* type,tObjType what,size_t refs);

	/**
	* Load the predicates from the database.
	*/
	virtual void LoadPredicates(void);

	/**
	* Assign an identifier to a new predicate.
	* @param predicate       Predicate.
	*/
	virtual void AssignId(GPredicate* predicate);

	/**
	* Load the statements from the database.
	*/
	virtual void LoadStatements(void);

	/**
	* Assign an identifier to a new statement.
	* @param statement       Statement.
	*/
	virtual void AssignId(GStatement* statement);

	/**
	* Method that load the classes from where they are stored. This method is
	* called once by the session.
	*/
	void LoadClasses(void);

	/**
	* Assign an identifier to a new class.
	* @param class           Class.
	*/
	virtual void AssignId(GClass* theclass);

	/**
	 * Save a class.
	 * @param theclass        Class.
	 */
	virtual void SaveClass(GClass* theclass);

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
	* Method that load a document that is stored.
	* @param docid           Identifier of the document
	*/
	virtual GDoc* LoadDoc(size_t docid);

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
	* Add an assessment for a given profile and document.
	* @param p               Identifier of the profile.
	* @param d               Identifier of the document.
	* @param fdbk            Feedback type.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(size_t p,size_t d,tFdbkType fdbk,R::RDate date,R::RDate computed);


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
	* Save a community.
	* @param grp            Group.
	*/
	virtual void SaveCommunity(GCommunity* grp);

	/**
	 * Save the relevant documents associated with a given community.
	 * @param docs           List of relevant.
	 */
	virtual void SaveDocs(const GCommunityDocs& docs);

	/**
	 * Load the relevant documents associated with a given community.
	 * @param docs           List of relevant.
	 */
	virtual void LoadDocs(GCommunityDocs& docs);

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
	* Save a topic.
	* @param grp            Group.
	*/
	virtual void SaveTopic(GTopic* grp);

	// @} Topics


	//-----------------------------------------------------
	/** @name Suggestions Methods
	*/
	// @{

	/**
	* Save a list of suggestions.
	* @param sugs            Suggestions. The container is not cleared by the
	*                        method.
	* @param max             Maximum number of suggestions to save.
	*/
	virtual void SaveSugs(const GSugs& sugs,size_t max);

	/**
	* Load a list of suggestions.
	* @param sugs            Suggestions.
	*/
	virtual void LoadSugs(GSugs& sugs);

	// @} Suggestions


	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destruct.
	*/
	virtual ~GStorageMySQL(void);
};


//------------------------------------------------------------------------------
#endif
