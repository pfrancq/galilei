/*

	GALILEI Research Project

	GSessionMySQL.h

	GALILEI Session using a MySQL Database - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GSessionMySQLH
#define GSessionMySQLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmysql/rmysql.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsessions/gsession.h>



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
class GSessionMySQL : public RMySQL::RDb, public GSession
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
	GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db,GURLManager* mng) throw(bad_alloc,GException,RMySQL::RMySQLError);

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
	* Transform a RTimeDate::RDate to a C string with the MySQL format.
	* @param d              Date to transform.
	* @param tmp            C String containing the date.
	* @returns Pointer to the tmp C string.
	*/
	const char* GetDateToMySQL(const RTimeDate::RDate* d,char* tmp);

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
	* Save a list of words for a specific subprofile.
	* @param list           List to save.
	* @param id             SubProfile id.
	* @param name           Name of the list (ok,com,ko).
	* @param lang           Code of the language.
	*/
	void Save(GIWordList* list,unsigned int id,const char* name,const char* lang) throw(GException);

	/**
	* Load a list of words for a specific subprofile.
	* @param list           List to save.
	* @param id             SubProfile id.
	* @param name           Name of the list (ok,com,ko).
	* @param lang           Code of the language.
	*/
	void Load(GIWordList* list,unsigned int id,const char* name,const char* lang) throw(GException);

	/**
	*  Load the Users.
	*/
	virtual void LoadUsers(void) throw(bad_alloc,GException);

	/**
	*  Load the Users' Feedback.
	*/
	virtual void LoadFdbks(void) throw(bad_alloc,GException);

	/**
	* Load the groups.
	*/
	virtual void LoadGroups(void) throw(bad_alloc,GException);

public:

	/**
	* Create a new document.
	* @param url        URL of the document.
	* @param name       Name of the document.
	* @param mime       MIME Type of the document
	* @returns Pointer to a new created document.
	*/
	virtual GDoc* NewDoc(const char* url,const char* name,const char* mime) throw(GException);

	/**
	* Save a document.
	* @param doc            Document to save.
	*/
	virtual void Save(GDoc* doc) throw(GException);

	/**
	* Create a new profile.
	* @param usr        Pointer to the user of the profile.
	* @param desc       Description of the profile.
	* @returns Pointer to GProfile.
	*/
	virtual GProfile* NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException);

	/**
	* Save information about the groupement (Group and attachment date) of
	* a subprofile. For a complete save, call Save(const GProfile*).
	* @param sub        Subprofile to save.
	*/
	virtual void Save(const GSubProfile* sub) throw(GException);

	/**
	* Save a profile.
	* @param prof       Profile to save.
	*/
	virtual void Save(const GProfile* prof) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GSessionMySQL(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
