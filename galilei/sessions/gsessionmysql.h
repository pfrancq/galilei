/*

	GALILEI Research Project

	GSessionMySQL.h

	GALILEI Session using a MySQL Database - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GSessionMySQLH
#define GSessionMySQLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rdb/rmysql.h>
#include <rstd/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GIWordList;


//-----------------------------------------------------------------------------
/**
* The GSessionMySQL class provides a representation for a GALILEI session using
* a MySQL Database as store medium.
* @author Pascal Francq
* @short MySQL Session.
*/
class GSessionMySQL : public R::RDb, public GSession
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
	* @param pwd            Password of the uzer.
	* @param db             Name of the database.
	* @param mng            URL Manager.
	*/
	GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db,GURLManager* mng) throw(bad_alloc,GException,R::RMySQLError);

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	unsigned int GetCount(const char* tbl);

	/**
	* Count the maximal value for a field of a table.
	* @param tbl            Table to analyse.
	* @param fld            Field to analyse.
	* @returns Maximal value.
	*/
	unsigned int GetMax(const char* tbl,const char* fld);

protected:

	/**
	* Transform a R::RDate to a C string with the MySQL format.
	* @param d              Date to transform.
	* @param tmp            C String containing the date.
	* @returns Pointer to the tmp C string.
	*/
	const char* GetDateToMySQL(const R::RDate* d,char* tmp);

	/**
	* Parse a field and when for quotes and double them.
	* @param tmp            C String containing the date.
	* @returns Pointer to the tmp C string.
	*/
	const char* ValidSQLValue(const char* val,char* tmp);

	/**
	* Return the identifier of a new word of a dictionnary.
	* @param word           Word to find.
	* @param dict           Dictionnary.
	*/
	virtual unsigned int GetDicNextId(const char* word,const GDict* dict);

	/**
	* Loading a dictionnary/stoplist.
	* @param code           Code of the languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(const char* code,bool s) throw(bad_alloc,GException);

	/**
	* Load a specific word from a dictionnary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(const unsigned int id,const char* code);

	/**
	* Load the documents.
	*/
	virtual void LoadDocs(void) throw(bad_alloc,GException);

	/**
	*  Load the users.
	*/
	virtual void LoadUsers(void) throw(bad_alloc,GException);

	/**
	*  Load the users Feedback.
	*/
	virtual void LoadFdbks(void) throw(bad_alloc,GException);

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void) throw(bad_alloc,GException);

public:

	/**
	* Load the Subjectree.
	* @param subjects     The tree of subjects.
	*/
	virtual void LoadSubjectTree(void);

	/**
	* Load the ideal groupment.
	* @param idealgroup   The ideal container of group	
	*/
	virtual void LoadIdealGroupment();


	/**
	* Save the ideal groupment
	* @param idealgroup   The ideal container of group	
	*/
	virtual void SaveIdealGroupment(R::RContainer<GGroups,unsigned int,true,true>* idealgroup);

	/**
	* Create a new document.
	* @param url          URL of the document.
	* @param name         Name of the document.
	* @param mime         MIME Type of the document
	* @returns Pointer to a new created document.
	*/
	virtual GDoc* NewDoc(const char* url,const char* name,const char* mime) throw(GException);

	/**
	* Save a document.
	* @param doc            Document to save.
	*/
	virtual void SaveDoc(GDoc* doc) throw(GException);

	virtual void SaveWordsGroups(GDict* dict)throw(GException);

	virtual void LoadWordsGroups(GDict* dict) throw(GException);

	virtual void SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException);

	/**
	* Save a the users feedback.
	*/
	virtual void SaveFdbks(void) throw(GException);

	/**
	* Create a new profile.
	* @param usr        Pointer to the user of the profile.
	* @param desc       Description of the profile.
	* @returns Pointer to GProfile.
	*/
	virtual GProfile* NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException);

	/**
	* Create a new subprofile.
	* @param prof            Pointer to the profile.
	* @param lang            Language of the subprofile.
	* @returns Pointer to GSubProfile.
	*/
	virtual GSubProfile* NewSubProfile(GProfile* prof,GLang* lang) throw(bad_alloc,GException);

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
	* Create a new group.
	* @param lang       Language of the group to create.
	* @param grp        Group created.
	*/
	virtual void NewGroup(GLang* lang,GGroup* grp);

	/**
	* Save the groups description.
	* @param lang       Language of the group to create.
	* @param grp        Group created.
	*/
	virtual void SaveGroups(void);

	/**
	* Save the mixed groups of the session.
	* @param mixedgroups    groups to save.
	* @param id             Identificator.
	* @param historic       if false,  groups will be saved in 'tempchromo', if true in 'historic'
	*/
	virtual void SaveMixedGroups(R::RContainer<GGroups,unsigned int,true,true>* mixedgroups,unsigned int id, bool historic=false);

	/**
	* Save the Profiles in history.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicid);

	/**
	* Delete a group.
	* @param grp        Group to delete.
	*/
	virtual void DeleteGroup(GGroup* grp);

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const char* filename) throw(GException);

	/**
	* Clear all the chromosomes stored.
	*/
	virtual void ClearStoredChromos(void);

	/**
	* Save A Chromosome into the database.
	* @param chromo         The chromosome to save.
	* @param id             An id to find the chromosome after saving. 
	* @param objs           Objects.
	* @param lang           The lang of the subprofiles in the chromosome.
	*/
	virtual void SaveChromo(GChromoIR* chromo,unsigned int id,R::RObjs<GObjIR>* objs);

	/**
	* Load a instance of chromosome for statisical use only.
	* @param lang           The lang of the subprofiles in the chromosome.
	* @param objs           Objects.
	* @param p              Parameters.
	* @return pointer to a Ginstir
	*/
	virtual GInstIR* LoadInstIR(GLang* lang,R::RObjs<GObjIR>* objs,GIRParams* p);

	/**
	* Save The Documents Simylarities into the database.
	*/
	virtual void SaveDocSim(void);

	/**
	* prepear the generation of pov
	*/
	void PrepearPoV(GGroup * grp, GSession* s);

	/**
	* check if pov exist or not
	*/
	int Alreadyexist(int grpid);

	/**
	* load the historic groups.
	*/
	GGroupsHistory* LoadAnHistoricGroups(R::RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicaID);

	/**
	*returns the number of historic groups stored in database.
	*/
	virtual unsigned int GetHistorySize(void) ;

	/**
	*  save the Point of views (expert kwds for a profile)
	*/
	virtual unsigned int SavePointOfView(R::RContainer<GStandardInOutPut,unsigned,false,true>*spov);

	/**
	*  load the Point of views (expert kwds for a profile) returns 0 if profile is empty
	*/
	virtual GInOutputBase * LoadPointOfView(GSubProfile *profile);

	/**
	*  load the Point of views (expert kwds for a profile) returns 0 if profile is empty
	*/
	virtual GInOutputBase * GetDocList(int grp, int profile, int idw);

	/**
	*  check if a specific level does exist in the table
	*/
	virtual int LevelExist(int level, int profileid);

	/**
	* Destructor.
	*/
	virtual ~GSessionMySQL(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
