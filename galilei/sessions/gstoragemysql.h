/*

	GALILEI Research Project

	GStorageMySQL.h

	Storage Manager using a MySQL Database - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for R Project
#include <rdb/rmysql.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gstorage.h>
#include <sessions/galilei.h>


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
class GStorageMySQL : private R::RDb, public GStorage
{
	/**
	* String representing the null value.
	*/
	static const char SQLNULL[5];

public:

	/**
	* Constructor.
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the user.
	* @param db             Name of the database.
	*/
	GStorageMySQL(const char* host,const char* user,const char* pwd,const char* db) throw(std::bad_alloc,GException,R::RMySQLError);

protected:

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	virtual unsigned int GetCount(const char* tbl) throw(R::RMySQLError);

	/**
	* Count the maximal value for a field of a table.
	* @param tbl            Table to analyse.
	* @param fld            Field to analyse.
	* @returns Maximal value.
	*/
	unsigned int GetMax(const char* tbl,const char* fld) throw(R::RMySQLError);

	/**
	* Transform a RTimeDate::RDate to a C string with the MySQL format.
	* @param d              Date to transform.
	* @param tmp            C String containing the date.
	* @returns Pointer to the tmp C string.
	*/
	const char* GetDateToMySQL(const R::RDate& d,char* tmp) throw(R::RMySQLError);

	/**
	* Parse a field and when for quotes and double them.
	* @param tmp            C String containing the date.
	* @returns Pointer to the tmp C string.
	*/
	const char* ValidSQLValue(const char* val,char* tmp) throw(R::RMySQLError);

public:

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
	* Loading a dictionary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param code           Languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(std::bad_alloc,GException);

	/**
	* Load a specific word from a dictionary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(unsigned int id,const char* code) throw(std::bad_alloc,GException);

	/**
	* Load a specific wordlist from a dictionary.
	* @param w              WordList to load.
	* @param code           Code of the languague.
	*/
	virtual void LoadWordList(GWordList* w,GLang* lang) throw(std::bad_alloc,GException);

	/**
	* Save a specific wordlist in the database.
	* @param dic            Pointer to the dictionary.
	* @param w              WordList to save.
	*/
	virtual void SaveWordList(GDict* dic,GWordList* w) throw(GException);

	/**
	* Load the documents.
	* @param wg             Enable the use of wordlists.
	* @param w              Enable the use of words.
	*/
	virtual void LoadDocs(GSession* session) throw(std::bad_alloc,GException);

	/**
	*  Load the users.
	* @param session         Session.
	*/
	virtual void LoadUsers(GSession* session) throw(std::bad_alloc,GException);

	/**
	*  Load the users Feedback.
	*/
	virtual void LoadFdbks(GSession* session) throw(std::bad_alloc,GException);

	/**
	* Load the groups.
	* @param wg             Enable the use of wordlists.
	* @param w              Enable the use of words.
	*/
	virtual void LoadGroups(GSession* session) throw(std::bad_alloc,GException);

	/**
	* Load the Subjectree.
	* @param subjects     The tree of subjects.
	*/
	virtual void LoadSubjectTree(GSession* session) throw(std::bad_alloc,GException);

	/**
	* Load the ideal groupment.
	* @param idealgroup   The ideal container of group
	*/
	virtual void LoadIdealGroupment(GSession* session) throw(std::bad_alloc,GException);

	/**
	* Save the ideal groupment
	* @param idealgroup   The ideal container of group
	*/
	virtual void SaveIdealGroupment(GGroups* idealgroup) throw(GException);

	/**
	* Save a document.
	* @param doc            Document to save.
	*/
	virtual void SaveDoc(GDoc* doc) throw(GException);

	/**
	* Save clusters of words in a document.
	* @param doc            Document to save.
	* @param n              Id of the first cluster of words.
	*/
	virtual void SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException);

	/**
	* Save a the users feedback.
	*/
	virtual void SaveFdbks(GSession* session) throw(GException);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException);

	/**
	* Save Subprofiles in histoty
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfileInHistory(GSubProfile* sub, unsigned int historicid) throw(GException);

	/**
	* Save a profile.
	* @param prof       Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException);


	/**
	* Save the groups description.
	* @param session        opened session.
	*/
	virtual void SaveGroups(GSession* session) throw(GException);

	/**
	* Save the groups in history.
	* @param session        opened session.
	*/
	virtual void SaveGroupsHistory(GSession* session) throw(GException);

	/**
	* Save the mixed groups of the session.
	* @param mixedgroups    groups to save.
	* @param id             Identificator.
	* @param historic       if false,  groups will be saved in 'tempchromo', if true in 'historic'
	*/
	virtual void SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic=false) throw(GException);

	/**
	* Save the Profiles in history.
	*/
	virtual void SaveHistoricProfiles(GSession* session,unsigned int historicid) throw(GException);

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const char* filename) throw(GException);

	/**
	* load the historic groups.
	*/
	GGroupsHistory* LoadAnHistoricGroups(R::RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicaID) throw(std::bad_alloc,GException);

	/**
	*returns the number of historic groups stored in database.
	*/
	virtual unsigned int GetHistorySize(void) throw(GException);

	/**
	* Create a dummy table to store different kid of
	* data.
	* @ param name              name of the dummy object.
	*/
	virtual void CreateDummy(const char* name) throw(GException);

	/**
	* Add a dummy entry into a dummy table;
	* @param name               name of the dummy table.
	* @param id                id of the dymmy entry.
	* @param desc              description of the dymmy entry.
	*/
	virtual void AddDummyEntry(const char* name, unsigned int id, const char* desc, unsigned int parentid) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GStorageMySQL(void) throw(GException);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
