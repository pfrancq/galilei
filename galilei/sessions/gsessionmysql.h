/*

	R Project Library

	gsessionmsql.h

	Description - Header.

	(C) 2001 by Pascal Francq

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
	*
	*/
	unsigned DicNextId(const RString& word);

	
	/**
	* Loading the groupment from database.
	*/
	void LoadGroupsFromDB(void);

	/**
	*
	*/
	void FillDict(GDict* dict) ;
	
	/**
	*  loading docs ...
	*/
	void LoadDocs(void);


	/**
	*  downloading doc ...
	*/
	
	void DownloadDoc(const RString& URL) ;
	
};

}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif




