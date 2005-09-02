/*

	GALILEI Research Project

	GStorageMySQL.h

	Storage Manager using a MySQL Database - Header.

	Copyright 2001-2005 by the Université libre de Bruxelles.

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

public:

	/**
	* Constructor.
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the user.
	* @param db             Name of the database.
	* @param coding         Coding used by the MySQL database.
	* @param all            Load all data (true) or only the modified one (false).
	* @param filter         Date used to filter the entry to load.
	*/
	GStorageMySQL(R::RString host,R::RString user,R::RString pwd,R::RString db,R::RString coding,bool all,const R::RDate& filter) throw(std::bad_alloc,GException);

protected:

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	virtual unsigned int GetCount(R::RString tbl) throw(R::RMySQLError);

	/**
	* Count the maximal value for a field of a table.
	* @param tbl            Table to analyse.
	* @param fld            Field to analyse.
	* @returns Maximal value.
	*/
	unsigned int GetMax(R::RString tbl,R::RString fld) throw(R::RMySQLError);

	/**
	* Transform a MySQL date into a C string, in particular when the
	* parameter is null, it chooses '1970-01-01'.
	* @param date           MySQL date.
	* @return string.
	*/
	R::RString GetMySQLToDate(R::RString date);

public:

	/**
	* Get the database.
	*/
	R::RDb* GetDb(void) {return(Db.get());}

	/**
	* Compute the number of objects of a given type which are saved.
	* @param type            Type of the objects.
	* @return Number of objects.
	*/
	virtual unsigned int GetNbSaved(tObjType type) throw(GException);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict) throw(GException);

	/**
	* Assign an identifier to a new document.
	* @param doc            Document.
	*/
	virtual void AssignId(GDoc* doc);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identificator of the document.
	* @param lang            Language of the document.
	*/
	virtual void UpdateProfiles(unsigned int docid,GLang* lang);

	/**
	* A subprofile was updated and the corresponding groups must be updated.
	* @param subid           Identificator of the subprofile.
	*/
	virtual void UpdateGroups(unsigned int subid);

	/**
	* Save the references of a given object type and for a given information
	* entity.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param lang            Language of the object.
	* @param id              Identificator of the information entity.
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(tObjType type,GLang* lang,size_t id,size_t refs);

	/**
	* Save the references of a given object type.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param lang            Language of the object.
	* @param refs            Number of references.
	*/
	virtual void SaveRefs(tObjType type,GLang* lang,size_t refs);

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param lang            Language of the object.
	* @param type            Type of the object (otDoc,otSubProfile,otGroup).
	* @param id              Identificator of the object.
	*/
	virtual void LoadInfos(R::RContainer<GWeightInfo,false,true>& infos,GLang* lang,tObjType type,size_t id);

	/**
	* Loading a dictionary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param lang           Languague.
	* @param s              Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(std::bad_alloc,GException);

	/**
	* Load an indexer.
	* @param indexer        Pointer to the indexer.
	* @param langs          Pointer to the langs.
	*/
	virtual void LoadIndexer(GIndexer* &indexer,GLangManager* langs);

	/**
	* Load a specific word from a dictionary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual R::RString LoadWord(unsigned int id,const char* code) throw(std::bad_alloc,GException);

	/**
	* Load the identificator of a specific word from a dictionary.
	* @param word           Word.
	* @param code           Code of the languague.
	*/
	virtual unsigned int LoadWord(const R::RString word,const char* code) throw(std::bad_alloc,GException);

	/**
	* Save a data in the database.
	* @param data            Data.
	* @param lang            Languague.
	*/
	virtual void SaveData(GData* data,GLang* lang)  throw(std::bad_alloc,GException);

	/**
	* Method that load a document that is stored.
	* @param docid           Identificator of the document.
	*/
	virtual GDoc* LoadDoc(unsigned int docid);

	/**
	* Load the documents.
	*/
	virtual void LoadDocs(void) throw(std::bad_alloc,GException);

	/**
	* Method that load a user that is stored.
	* @param userid          Identificator of the user.
	*/
	virtual GUser* LoadUser(unsigned int userid);

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
	* Load the subprofiles (and profiles and users).
	*/
	virtual void LoadUsers(void) throw(std::bad_alloc,GException);

	/**
	* Method that load a group that is stored.
	* @param groupid         Identificator of the group.
	*/
	virtual GGroup* LoadGroup(unsigned int groupid);

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void) throw(std::bad_alloc,GException);

	/**
	* Load the Subjects.
	*/
	virtual void LoadSubjects(void) throw(std::bad_alloc,GException);

	/**
	* Save a document.
	* @param doc            Document to save.
	*/
	virtual void SaveDoc(GDoc* doc) throw(GException);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException);

	/**
	* Save Subprofiles in histoty
	* @param sub            Subprofile to save.
	* @param historicID     Identificator of the historic.
	*/
	virtual void SaveSubProfileInHistory(GSubProfile* sub, unsigned int historicID) throw(GException);

	/**
	* Save a profile.
	* @param prof       Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException);

	/**
	* Save the groups description.
	*/
	virtual void SaveGroups(void) throw(GException);

	/**
	* Save the groups in history.
	*/
	virtual void SaveGroupsHistory(void) throw(GException);

	/**
	* Save the Profiles in history.
	* @param historicID     Identificator of the historic.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicID) throw(GException);

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const R::RString& filename) throw(GException);

	/**
	* Load an historic groups.
	* @param historicID     Identificator of the historic.
	* @return Pointer to a historic group.
	*/
	GGroupsHistory* LoadAnHistoricGroups(unsigned int historicID) throw(std::bad_alloc,GException);

	/**
	* Load the historic groups.
	* @param mindate        Date of the first historic to load.
	* @param maxdate        Date of the last historic to load.
	*/
	void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate);

	/**
	* @return the number of historic groups stored in database.
	*/
	virtual unsigned int GetHistorySize(void) throw(GException);

	/**
	* Prepare a given number of suggestions for a test.
	* @param name           Name of the test.
	*/
	virtual void CreateSugs(const R::RString& name);

	/**
	* Add a suggestion for a given profile and a given test.
	* @param name           Name of the test.
	* @param profileid      Identificator of the profile.
	* @param docid          Identificator of the document to suggest.
	* @param rank           Ranking of the document.
	*/
	virtual void AddSugsProfile(const R::RString& name,unsigned int profileid,unsigned int docid,unsigned int rank);

	/**
	* Add a suggestion for a given community and a given test.
	* @param name           Name of the test.
	* @param groupid        Identificator of the community.
	* @param docid          Identificator of the document to suggest.
	* @param rank           Ranking of the document.
	*/
	virtual void AddSugsGroup(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank);

	/**
	* Load the list of suggestions for the profiles for a given test.
	* @param name           Name of the test.
	* @param res            Container that will hold the suggestions.
	*/
	virtual void GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* Load the list of suggestions for the groups for a given test.
	* @param name           Name of the test.
	* @param res            Container that will hold the suggestions.
	*/
	virtual void GetSugsGroups(const R::RString& name,R::RContainer<GSugs,true,false>& res);

	/**
	* Get the different tests that were made.
	* @param res            Container that will hold the names.
	*/
	virtual void GetSugsTests(R::RContainer<R::RString,true,true>& res);

	/**
	* Create a dummy table to store different kid of data.
	* @param name              name of the dummy object.
	*/
	virtual void CreateDummy(R::RString name) throw(GException);

	/**
	* Delete all the data of a given dummy table.
	* @param name              name of the dummy object.
	*/
	virtual void ClearDummy(R::RString name) throw(GException);

	/**
	* Add a dummy entry into a dummy table;
	* @param name              Name of the dummy table.
	* @param id                Identificator of the dymmy entry.
	* @param desc              Description of the dymmy entry.
	* @param parentid          Identificator of the parent.
	*/
	virtual void AddDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid) throw(GException);

	/**
	* Select a dummy entry from a dummy table;
	* @param name              Name of the dummy table.
	* @param id                Identificator of the dymmy entry.
	* @param desc              description of the dymmy entry.
	* @param parentid          Identificator of the parent.
	* @param filter            Filter to set the fields on which the select is done.
	*/
	virtual R::RQuery* SelectDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter) throw(GException);

	/**
	* Clear a dummy entry from a dummy table;
	* @param name              Name of the dummy table.
	* @param id                Identificator of the dymmy entry.
	* @param desc              description of the dymmy entry.
	* @param parentid          Identificator of the parent.
	* @param filter            Filter to set the fields on which the select is done.
	*/
	virtual void ClearDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GStorageMySQL(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
