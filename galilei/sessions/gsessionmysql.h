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
/**
* The GSessionMySQL class provides a representation for a GALILEI session using
* a MySQL Database as store medium.
* @author Pascal Francq
* @short MySQL Session.
*/
class GSessionMySQL : public RMySQL::RDb, public GSession
{
	/**
	* Temporary C String to hold a date.
	*/
	char sTmpDate[10];

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
	* @returns C String containing the date.
	*/
	const char* GetDateToMySQL(RTimeDate::RDate* d);

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
	*  Load the documents.
	*/
	virtual void LoadDocs(void) throw(bad_alloc,GException);

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

	/**
	* Load the groups' member.
	*/
	virtual void LoadGroupsMember(GGroup* grp) throw(bad_alloc,GException);

public:

	/**
	* Destructor.
	*/
	virtual ~GSessionMySQL(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
