/*

	R Project Library

	gsessionmsql.h

	Description - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/


//-----------------------------------------------------------------------------
#ifndef GSESSIONMSQL_H
#define GSESSIONMSQL_H



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdio.h>      // Defining size_t
#include <stdlib.h>
#include <string.h>     // String functions


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmysql/rmysql.h>
using namespace RMySQL;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsessions/gsession.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


/**
  *@author Pascal Francq
  */

class GSessionMySQL : public GSession
{
public:
	
	
	GSessionMySQL(RDb* _db);
	~GSessionMySQL(void);

	RDb* db; 

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



};

}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif




