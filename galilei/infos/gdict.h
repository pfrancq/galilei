/*

	GALILEI Research Project

	GDic.h

	Dictionary - Header.

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
#ifndef GDictH
#define GDictH


//-----------------------------------------------------------------------------
// include files for R Project
//#include <rstd/rdblhashcontainer.h>
//#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDict class provides a representation for a dictionnary.
* @author Pascal Francq
* @short Dictionary.
*/
class GDict : protected R::RDblHashContainer<GData,unsigned,27,27,true>
{
	class GDataTypes;

	/**
	* Session responsible for the dictionary.
	*/
	GSession* Session;

	/**
	* Direct access to the words using an identificator.
	*/
	GData** Direct;

	//unsigned NbGroupsList;

	/**
	* Maximal value of the identificator handled by Direct.
	*/
	unsigned MaxId;

	/**
	* Highest value of the identificator.
	*/
	unsigned UsedId;

	/**
	* Pointer to the language corresponding to the dictionary.
	*/
	GLang *Lang;

	/**
	* Name of the dictionnary. This can be the name of a file or a table in a
	* database.
	*/
	R::RString Name;

	/**
	* Description of the dictionnary.
	*/
	R::RString Desc;

	/**
	* State of the dictionnary.
	*/
	bool Loaded;

	/**
	* Is this dictionnary a stop list or not.
	*/
	bool Stop;

	/**
	* Number of references in documents.
	*/
	unsigned int NbRefDocs[2];

	/**
	* Number of references in subprofiles.
	*/
	unsigned int NbRefSubProfiles[2];

	/**
	* Number of references in groups.
	*/
	unsigned int NbRefGroups[2];

private :

	/**
	* Containers of double hash tables where the datas are categorized by
	* information type.
	*/
	R::RContainer<GDataTypes,unsigned int,true,true> DataTypes;

public:

	/**
	* Constructor of the dictionnary.
	* @param s              Session.
	* @param name           Name of the dictionnary.
	* @param desc           Description of the dictionnary.
	* @param lang           Corresponding language.
	* @param m              Total maximal number of words to create at
	*                       initialisation.
	* @param ml             Maximal number of words to create at initialisation
	*                       for the different letters.
	* @param st             Is it a Stoplist?.
	*/
	GDict(GSession* s,const R::RString& name,const R::RString& desc,GLang* lang,unsigned m,unsigned ml,bool st) throw(bad_alloc);

	/**
	* Clear the dictionnary.
	*/
	void Clear(void);

private:

	/**
	* Put a word in direct.
	* @param word           Pointer to the word to insert.
	*/
	void PutDirect(GData* word) throw(bad_alloc);

public:

	/**
	* Insert a data in the dictionary. In practice, it is a copy of the data
	* which is inserted.
	* @param data            Data to insert.
	* @return Identificator of the data inserted in the dictionary.
	*/
	unsigned int InsertData(const GData* data) throw(bad_alloc, GException);

	/**
	* Delete a given data from the dictionnary.
	* @param data            Data to delete.
	*/
	void DeleteData(GData* data) throw(bad_alloc, GException);

	/**
	* Get the maximal number of word actually supposed.
	*/
	unsigned int GetMaxId(void) const {return(MaxId);}

	/**
	* Get the list of the words currently defined in the dictionnary.
	* @returns const Pointer to array of GWord*.
	*/
	GData** GetDatas(void) const {return(Direct);}

	/**
	* Get a cursor on a unordered container of data of a particular information
	* type.
	* @param type            Information type.
	*/
	GDataCursor& GetDataCursor(GInfoType type) throw(bad_alloc,GException);

	/**
	* Get thye name of the dictionnary.
	* @returns Pointer a C string.
	*/
	const char* GetName(void) const {return(Name);}

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the element with a specific identificator.
	* @param id             Identificator.
	* @return Pointer to a GWord.
	*/
	GData* GetData(const unsigned int id) const;

	/**
	* Method giving the number of datas contained in the dictionary.
	*/
	unsigned int GetNbDatas(void) const {return(GetNb());}

	/**
	* Method giving the number of datas contained in the dictionary if a given
	* information type.
	* @param type            Information type.
	*/
	unsigned int GetNbDatas(GInfoType type) const throw(GException);

	/**
	* Look if a certain data is in the dictionary.
	*/
	bool IsIn(const R::RString& name) const;

	/**
	* Get the number of group of information entities.
	* @returns Number of groups.
	*/
	//unsigned int GetNbGroupsList(void) const;

	/**
	* Test if the dictionnary is a stop list.
	* @returns true if it is a stop list.
	*/
	bool IsStopList(void) const;

	/**
	* Compare function used in the R::RContainer.
	*/
	int Compare(const GDict* dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GDict& dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Increase the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(unsigned int id,tObjType ObjType,GInfoType WordType);

	/**
	* Decrease the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(unsigned int id,tObjType ObjType,GInfoType WordType);

	/**
	* Get the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType);

	/**
	* Increase the number of objects using the dictionnary.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(tObjType ObjType,GInfoType WordType);

	/**
	* Decrease the number of objects using the dictionnary.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(tObjType ObjType,GInfoType WordType);

	/**
	* Get the total number of references.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType,GInfoType WordType);

	/**
	* Destructor of the dictionnary.
	*/
	virtual ~GDict(void);
};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
