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
public:
	/**
	* Constructor.
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the uzer.
	* @param db             Name of the database.
	*/
	GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db) throw(bad_alloc,GException,RMySQL::RMySQLError);

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

	/**
	* Loading Users function.
	*/
	void LoadAllFromDB();

	/**
	* Loading Users function.
	*/
	void LoadUsersFromDB();
	
	/**
	* Loading Profiles function
	*@param usr             User owner of profiles.
	*/
	void LoadProfilesFromDB(GUser* usr);
	
	/**
	* Loading SubProfiles function
	*@param prof            Profile owner of the subprofiles.
	*/
	void LoadSubProfilesFromDB(GProfile* prof);
	
	/**
	* Loading Langs into the 'Langs' container
	*/
	void LoadLangsFromDB(void);

	virtual void LoadDics(void) throw(bad_alloc,GException);

	/**
	* Loading Dics into the 'Dics' container from Database.
	*@param name            name of the database table containing the kwds.
	*@param lang            languague of the dic.
	*/
	void LoadDicFromDB(const RString &name,GLang *lang);

	
    /**
	* Loading Dics into the 'Stops' container from Database.
	*@param name            name of the database table containing the kwds.
	*@param lang            languague of the dic.
	*/
	void LoadStopsFromDB(const RString &name,GLang *lang);

    /**
	* Loads all the documents for all  profiles.
	*/
	void LoadProfilesDocs();

	/**
	* Loads all the documents for a given  profiles.
	* @param profile        given profile.
	*/
	void LoadProfileDocs(GProfile* profile);

	/**
	*
	*/
	unsigned DicNextId(const RString& word);

	
	/**
	* Loading the groupment from database.
	*/
	void LoadGroupsFromDB(void);

	/**
	* Add a judgement by a profile for a document.
	*/
	void AddDocJudgement(GDoc* doc, GProfile* profile, char fdbk) {}

	/**
	*
	*/
	void FillDict(GDict* dict) ;
	
	/**
	*  loading docs ...
	*/
	void LoadDocs(void);

public:

	/**
	* Destructor.
	*/
	~GSessionMySQL(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
