/*

	GALILEI Research Project

	GStorageMySQL.h

	Storage Manager using a MySQL Database - Header.

	Copyright 2001-2007 by the Université libre de Bruxelles.

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
	inline R::RString Num(unsigned int i) {return("'"+RString::Number(i)+"'");}
	
	/**
	 * Return a SQL string for a given language.
	 */
	inline R::RString Lang(GLang* lang) {return("'"+RString(lang->GetCode())+"'");}
	 	
public:

	//-----------------------------------------------------
	/** @name General Methods
	*/
	// @{

	/**
	* Get a string representing the world stored. This string must be a valid
	* directory name since it is used everywhere.
	*/
	virtual RString GetWorld(void) const;

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
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param lang            Language of the object.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param id              Identificator of the object.
	*/
	virtual void LoadInfos(R::RContainer<GWeightInfo,false,true>& infos,GLang* lang,tObjType type,size_t id);

	/**
	* Load the Subjects.
	*/
	virtual void LoadSubjects(void);

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
	* Loading information on a langauge.
	* @param lang           Languague.
	*/
	virtual void LoadLang(GLang* lang);

	/**
	* Load the concept types from the database.
	*/
	virtual void LoadConceptTypes(void);

	/**
	* Load the relation types from the database.
	*/
	virtual void LoadRelationTypes(void);

	/**
	* Load the relation from the database.
	*/
	virtual void LoadRelations(void);

	/**
	* Loading a dictionary/stoplist.
	* @param lang            Language of the dictonnary.
	* @param type            Type of concepts.
	* @return Pointer to the dictionary
	*/
	virtual GDict* LoadDic(GLang* lang,unsigned int type);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data            Data.
	* @param dict            Dictionary.
	*/
	virtual void AssignId(GConcept* data,const GDict* dict);

	/**
	* Delete a data from the dictionnary.
	* @param data            Data.
	*/
	virtual void DeleteConcept(GConcept* data);

	/**
	* Load a specific concept from a dictionary.
	* @param id              Idenfificator of the concept.
	* @param code            Code of the languague.
	* @param type            Type of the concept.
	*/
	virtual R::RString LoadConcept(unsigned int id,const char* code,unsigned int type);

	/**
	* Load the identificator of a specific concept from a dictionary.
	* @param word            Name of the concept.
	* @param code            Code of the languague.
	* @param type            Type of the concept.
	*/
	virtual unsigned int LoadConcept(const R::RString word,const char* code,unsigned int type);

	/**
	* Save a concept in the database.
	* @param concept         Concept.
	*/
	virtual void SaveConcept(GConcept* concept);

	/**
	* Save the references of a given object type and for a given information
	* entity.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param lang            Language of the object.
	* @param type            Type of the concepts.
	* @param id              Identificator of the information entity.
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(tObjType type,GLang* lang,unsigned int type,size_t id,size_t refs);

	/**
	* Save the references of a given object type.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param lang            Language of the object.
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(tObjType type,GLang* lang,size_t refs);

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
	/** @name Users/Profiles/Subprofiles Methods
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
	* Method that load a subprofile that is stored.
	* @param subprofileid    Identificator of the subprofile.
	*/
	virtual GSubProfile* LoadSubProfile(unsigned int subprofileid);

	/**
	* Load the list of suggestions for the profiles for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identificator of the document.
	* @param lang            Language of the document.
	*/
	virtual void UpdateProfiles(unsigned int docid,GLang* lang);

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
	* Save Subprofiles in histoty
	* @param sub            Subprofile to save.
	* @param historicID     Identificator of the historic.
	*/
	virtual void SaveSubProfileInHistory(GSubProfile* sub, unsigned int historicID);

	/**
	* Save profiles in history
	* @param historicID      Identificator of the historic.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicID);

	/**
	* Assign an identifier to a new subprofile.
	* @param sub             Subprofile.
	*/
	virtual void AssignId(GSubProfile* sub);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub             Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub);

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
	* @param lang            Language of the document.
	* @param assess          Feedback.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	*/
	virtual void AddFdbk(unsigned int p,unsigned int d,GLang* lang,tDocAssessment assess,R::RDate date,R::RDate computed);


	// @} Users/Profiles/Subprofiles


	//-----------------------------------------------------
	/** @name Groups Methods
	*/
	// @{

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void);

	/**
	* Method that load a group that is stored.
	* @param groupid         Identificator of the group.
	*/
	virtual GGroup* LoadGroup(GLang* lang,unsigned int groupid);

	/**
	* Load an historic groups.
	* @param historicID      Identificator of the historic.
	* @return Pointer to a historic group.
	*/
	virtual GGroupsHistory* LoadAnHistoricGroups(unsigned int historicID);

	/**
	* Load the historic groups.
	* @param mindate         Date of the first historic to load.
	* @param maxdate         Date of the last historic to load.
	*/
	virtual void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate);

	/**
	* Load the list of suggestions for the groups for a given test.
	* @param name            Name of the test.
	* @param res             Container that will hold the suggestions.
	*/
	virtual void GetSugsGroups(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* Returns the number of historic groups stored in the database.
	*/
	virtual unsigned int GetHistorySize(void);

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	virtual void UpdateGroups(unsigned int subid);

	/**
	* Assign an identifier to a new group.
	* @param grp            Group.
	*/
	virtual void AssignId(GGroup* grp);

	/**
	* Save the groups of the session.
	* @param lang            Define the language of the groups to save.
	*/
	virtual void SaveGroups(GLang* lang);

	/**
	* Save the groups in history.
	*/
	virtual void SaveGroupsHistory(void);

	/**
	* Add a suggestion for a given community and a given test.
	* @param name            Name of the test.
	* @param groupid         Identificator of the community.
	* @param docid           Identificator of the document to suggest.
	* @param rank            Ranking of the document.
	*/
	virtual void AddSugsGroup(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank);

	// @} Groups

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GStorageMySQL(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
