/*

	GALILEI Research Project

	GSessionMySQL.h

	GALILEI Session using a MySQL Database - Header.

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
#ifndef GSessionMySQLH
#define GSessionMySQLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmysql/rmysql.h>
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>



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
	virtual void SaveDoc(GDoc* doc) throw(GException);

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
	virtual void SaveSubProfile(GSubProfile* sub) throw(GException);

	/**
	* Save a profile.
	* @param prof       Profile to save.
	*/
	virtual void SaveProfile(GProfile* prof) throw(GException);

	/**
	* Create a new group.
	* @param lang       Language of the group to create.
	*/
	virtual GGroup* NewGroup(GLang* lang);

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
	* Export the words,document matrix into a file.
	* @param url            The url of the matrix file.
	*/
	virtual void ExportDM(const char* url);

	/**
	* IMPORT the words,document matrix from a file.
	* @param url            The url of the matrix file.
	*/
	virtual void ImportDC(const char* url);

	/**
	* Export the prolife,word matrix into a file.
	* @param url            The url of the matrix file.
	*/
	virtual void ExportP(const char* url);

	/**
	* Import the groupment matrix from a file.
	* @param url            The url of the matrix file.
	*/
	virtual void ImportG(const char* url);

	/**
	* Destructor.
	*/
	virtual ~GSessionMySQL(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
