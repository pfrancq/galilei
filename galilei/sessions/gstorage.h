/*

	GALILEI Research Project

	GStorage.h

	Generic Storage Manager - Header.

	Copyright 2003-2004 by the Université libre de Bruxelles.

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
#include <rstd/rstring.h>
#include <rdb/rmysql.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


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
class GStorage
{
protected:

	/**
	* Name of the storage manager.
	*/
	R::RString Name;

public:

	/**
	* Constructor.
	* @param n              Name.
	*/
	GStorage(R::RString n) throw(std::bad_alloc,GException);

	/**
	* Compute the number of objects of a given type which are saved.
	* @param type            Type of the objects.
	* @return Number of objects.
	*/
	virtual unsigned int GetNbSaved(tObjType type) throw(GException)=0;

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict) throw(GException)=0;

	/**
	* Loading a dictionary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param lang           Languague.
	* @param s              Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(std::bad_alloc,GException)=0;

	/**
	* Load an indexer.
	* @param indexer        Pointer to the indexer.
	* @param langs          Pointer to the langs.
	*/
	virtual void LoadIndexer(GIndexer* &indexer,GLangManager* langs)=0;

	/**
	* Load a specific word from a dictionary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual R::RString LoadWord(unsigned int id,const char* code) throw(std::bad_alloc,GException)=0;

	/**
	* Load the identificator of a specific word from a dictionary.
	* @param word           Word.
	* @param code           Code of the languague.
	*/
	virtual unsigned int LoadWord(const R::RString word,const char* code) throw(std::bad_alloc,GException)=0;

	/**
	* Load a specific wordlist from a dictionary.
	* @param w              WordList to load.
	* @param lang           Languague.
	*/
	virtual void LoadWordList(GWordList* w,GLang* lang) throw(std::bad_alloc,GException)=0;

	/**
	* Save a specific wordlist in the database.
	* @param dic            Pointer to the dictionary.
	* @param w              WordList to save.
	*/
	virtual void SaveWordList(GDict* dic,GWordList* w) throw(GException)=0;

	/**
	* Method that load the documents from where they are stored. This method
	* must be overloaded.
	* @param session        Session.
	*/
	virtual void LoadDocs(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Method that load the documents having given information in their
	* description from where they are stored. This method must be overloaded.
	* @param session        Session.
	* @param list           List of information that must be contained in the documents.
	* @param code           Code of the languague.
	*/
	virtual void LoadDocs(GSession* session,GInfoList& list,GLang* lang) throw(std::bad_alloc,GException)=0;

	/**
	* Load the new documents.
	* @param session        Session.
	*/
	virtual void LoadNewDocs(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Save a document where it is stored. This method is called after an
	* analsyis of a document if the result has to be saved. This method must be
	* overloaded.
	* @param doc        Document to save.
	*/
	virtual void SaveDoc(GDoc* doc) throw(GException)=0;

	/**
	* Save a profile.
	* @param prof           Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException)=0;

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException)=0;

	/**
	* Load the Users.
	* @param session        Session.
	*/
	virtual void LoadUsers(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Load the Feedbacks.
	* @param session        Session.
	*/
	virtual void LoadFdbks(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Load the SubjectTree.
	* @param session        Session.
	*/
	virtual void LoadSubjectTree(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Save the groups of the session
	* @param mixedgroups    groups to save
	* @param id             Identificator.
	* @param historic       if false,  groups will be saved in 'tempchromo', if true in 'historic'
	*/
	virtual void SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic=false) throw(GException)=0;

	/**
	* Save profiles in history
	* @param session        Session.
	* @param historicID     id of the historic.
	*/
	virtual void SaveHistoricProfiles(GSession* session,unsigned int historicID) throw(GException)=0;

	/**
	* Return the name of the current database.
	*/
	R::RString GetName(void) const;

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const char* filename) throw(GException)=0;

	/**
	* Load the groups.
	* @param session        Session.
	*/
	virtual void LoadGroups(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Save the groups of the session.
	* @param session        Session.
	*/
	virtual void SaveGroups(GSession* session) throw(GException)=0;

	/**
	* Save the groups in history.
	* @param session        Session.
	*/
	virtual void SaveGroupsHistory(GSession* session) throw(GException)=0;

	/**
	* Load the ideal groupment.
	* @param session        Session.
	*/
	virtual void LoadIdealGroupment(GSession* session) throw(std::bad_alloc,GException)=0;

	/**
	* Save the ideal groupment
	* @param idealgroup     The ideal container of group
	*/
	virtual void SaveIdealGroupment(GGroups* idealgroup) throw(GException)=0;

	/**
	* Save the feedbaks
	* @param session        Session.
	*/
	virtual void SaveFdbks(GSession* session) throw(GException)=0;

	/**
	* Save the links.
	* @param session        Session.
	*/
	virtual void SaveLinks(GSession* session) throw(GException)=0;

	/**
	* Save clusters of words in a document.
	* @param doc            Document to save.
	* @param n              Id of the first cluster of words.
	*/
	virtual void SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)=0;

	/**
	* Load an historic groups.
	* @param session        Session.
	* @param historicID     Identificator of the historic.
	* @return Pointer to a historic group.
	*/
	virtual GGroupsHistory* LoadAnHistoricGroups(GSession* session,unsigned int historicID) throw(std::bad_alloc,GException)=0;

	/**
	* Load the historic groups.
	* @param session        Session.
	* @param mindate        Date of the first historic to load.
	* @param maxdate        Date of the last historic to load.
	*/
	virtual void LoadHistoricGroupsByDate(GSession* session,R::RString mindate, R::RString maxdate)=0;

	/**
	* Returns the number of historic groups stored in the database.
	*/
	virtual unsigned int GetHistorySize(void) throw(GException)=0;

	/**
	* Create a dummy table to store different kid of
	* data.
	* @param name           Name of the dummy object.
	*/
	virtual void CreateDummy(R::RString name) throw(GException)=0;

	/**
	* Delete all the data of a given dummy table.
	* @ param name          Name of the dummy object.
	*/
	virtual void ClearDummy(R::RString name) throw(GException)=0;

	/**
	* Add a dummy entry into a dummy table;
	* @param name           Name of the dummy table.
	* @param id             Identificator of the dymmy entry.
	* @param desc           Description of the dymmy entry.
	* @param parentid       Identificator of the parent.
	*/
	virtual void AddDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid) throw(GException)=0;

	/**
	* Select a dummy entry from a dummy table;
	* @param name           Name of the dummy table.
	* @param id             Identificator of the dymmy entry.
	* @param desc           Description of the dymmy entry.
	* @param parentid       Identificator of the parent.
	* @param filter         Filter to set the fields on which the select is done.
	*/
	virtual R::RQuery* SelectDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter) throw(GException)=0;

	/**
	* Clear a dummy entry from a dummy table;
	* @param name           Name of the dummy table.
	* @param id             Identificator of the dymmy entry.
	* @param desc           Description of the dymmy entry.
	* @param parentid       Identificator of the parent.
	* @param filter         Filter to set the fields on which the select is done.
	*/
	virtual void ClearDummyEntry(R::RString name,unsigned int id,R::RString desc,unsigned int parentid,unsigned int filter) throw(GException)=0;

	/**
	* Export the vectors/words matrix
	* @param session        Session.
	* @param rec            Slot to receive information.
	* @param type           Type of export ("Profiles", "Documents" or "Groups").
	* @param filename       Export file name.
	* @param lang           Language of the export.
	* @param label          Display words id and vectors id ?
	*/
	virtual void ExportMatrix(GSession* session,GSlot* rec,R::RString type,R::RString filename,GLang* lang,bool label)=0;

	/**
	* Destructor.
	*/
	virtual ~GStorage(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
