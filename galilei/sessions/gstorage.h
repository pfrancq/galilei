/*

	GALILEI Research Project

	GStorage.h

	Generic Storage Manager - Header.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#ifndef GStorageH
#define GStorageH


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
	GStorage(const char* n) throw(bad_alloc,GException);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict)=0;

	/**
	* Loading a dictionary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param lang           Languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(bad_alloc,GException)=0;

	/**
	* Load a specific word from a dictionary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(unsigned int id,const char* code)=0;

	/**
	* Load a specific wordlist from a dictionary.
	* @param w              WordList to load.
	* @param code           Code of the languague.
	*/
	virtual void LoadWordList(GWordList* w,GLang* lang) throw(bad_alloc,GException)=0;

	/**
	* Save a specific wordlist in the database.
	* @param dic            Pointer to the dictionary.
	* @param w              WordList to save.
	*/
	virtual void SaveWordList(GDict* dic,GWordList* w) throw(bad_alloc,GException)=0;

	/**
	* Method that load the documents from where they are stored. This method
	* must be overloaded.
	*/
	virtual void LoadDocs(GSession* session) throw(bad_alloc,GException)=0;

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
	*/
	virtual void LoadUsers(GSession* session) throw(bad_alloc,GException)=0;

	/**
	* Load the Feedbacks.
	*/
	virtual void LoadFdbks(GSession* session) throw(bad_alloc,GException)=0;

	/**
	* Load the SubjectTree.
	*/
	virtual void LoadSubjectTree(GSession* session)=0;

	/**
	* Save the groups of the session
	* @param mixedgroups    groups to save
	* @param id             Identificator.
	* @param historic       if false,  groups will be saved in 'tempchromo', if true in 'historic'
	*/
	virtual void SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic=false)=0;

	/**
	* Save profiles in history
	* @param historicid     id of the historic.
	*/
	virtual void SaveHistoricProfiles(GSession* session,unsigned int historicid)=0;

	/**
	* Return the name of the current database.
	*/
	R::RString& GetName(void) const;

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const char* filename) throw(GException)=0;

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(GSession* session) throw(bad_alloc,GException)=0;

	/**
	* Save the groups of the session.
	*/
	virtual void SaveGroups(GSession* session)=0;

	/**
	* Load the ideal groupment.
	* @param idealgroup   The ideal container of group
	*/
	virtual void LoadIdealGroupment(GSession* session)=0;

	/**
	* Save the ideal groupment
	* @param idealgroup   The ideal container of group
	*/
	virtual void SaveIdealGroupment(GGroups* idealgroup)=0;

	/**
	* Save the feedbaks
	*/
	virtual void SaveFdbks(GSession* session)=0;

	/**
	* Save clusters of words in a document.
	* @param doc            Document to save.
	* @param n              Id of the first cluster of words.
	*/
	virtual void SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)=0;

	/*
	* Load an historic groups.
	*/
	virtual GGroupsHistory* LoadAnHistoricGroups(R::RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicaID)=0;

	/**
	* Returns the number of historic groups stored in the database.
	*/
	virtual unsigned int GetHistorySize(void)=0;

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	virtual unsigned int GetCount(const char* tbl)=0;

	/**
	* Create a dummy table to store different kid of
	* data.
	* @ param name              name of the dummy object.
	*/
	virtual void CreateDummy(const char* name)=0;

	/**
	* Add a dummy entry into a dummy table;
	* @param name               name of the dummy table.
	* @param id                id of the dymmy entry.
	* @param desc              description of the dymmy entry.
	*/
	virtual void AddDummyEntry(const char* name, unsigned int id, const char* desc, unsigned int parentid)=0;

	/**
	* Destructor.
	*/
	virtual ~GStorage(void) throw(GException);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
