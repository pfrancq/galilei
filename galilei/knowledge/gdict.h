/*

	GALILEI Research Project

	GDict.h

	Dictionary - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GDictH
#define GDictH



//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDict class provides a representation for a dictionary of data, each data
* having is own identificator and name. These data may be stopwords.
* @author Pascal Francq
* @short Dictionary.
*/
class GDict : protected R::RDblHashContainer<GData,unsigned,27,27,true>
{
	class GDataTypes;

	/**
	* Array of data ordered by identificator.
	*/
	GData** Direct;

	/**
	* Highest identificator that the dictionary can handle.
	*/
	unsigned int MaxId;

	/**
	* Highest identificator of the data stored.
	*/
	unsigned int UsedId;

	/**
	* Language corresponding to the dictionary.
	*/
	GLang *Lang;

	/**
	* Name of the dictionary. This can be the name of a file or a table in a
	* database.
	*/
	R::RString Name;

	/**
	* Description of the dictionary.
	*/
	R::RString Desc;

	/**
	* State of the dictionary.
	*/
	bool Loaded;

	/**
	* Specify if the data of the dictionary is stopwords.
	*/
	bool Stop;

	/**
	* Number of references in documents.
	*/
	unsigned int NbRefDocs;

	/**
	* Number of references in subprofiles.
	*/
	unsigned int NbRefSubProfiles;

	/**
	* Number of references in groups.
	*/
	unsigned int NbRefGroups;

private :

	/**
	* Set of containers where the data are categorized by information type.
	*/
	R::RContainer<GDataTypes,unsigned int,true,true> DataTypes;

public:

	/**
	* Constructor of the dictionary.
	* @param name           Name of the dictionary.
	* @param desc           Description of the dictionary.
	* @param lang           Corresponding language.
	* @param m              Total maximal number of data to create at
	*                       initialisation.
	* @param ml             Maximal number of data to create at initialisation
	*                       for the different letters.
	* @param st             Are the data stopwords?
	*/
	GDict(const R::RString& name,const R::RString& desc,GLang* lang,unsigned m,unsigned ml,bool st) throw(std::bad_alloc);

	/**
	* Clear the dictionary.
	*/
	void Clear(void);

private:

	/**
	* Put a data in direct.
	* @param data           Pointer to the data to insert.
	*/
	void PutDirect(GData* data) throw(std::bad_alloc);

public:

	/**
	* Insert a data in the dictionary. In practice, it is a copy of the data
	* which is inserted.
	* @param data            Data to insert.
	* @return Identificator of the data inserted in the dictionary.
	*/
	unsigned int InsertData(const GData* data) throw(std::bad_alloc, GException);

	/**
	* Delete a given data from the dictionary.
	* @param data            Data to delete.
	*/
	void DeleteData(GData* data) throw(std::bad_alloc, GException);

	/**
	* Get the data with a specific identificator.
	* @param id             Identificator.
	* @return Pointer to a GData.
	*/
	GData* GetData(const unsigned int id) const throw(GException);

	/**
	* Get the highest identificator of the data stored by the dictionary.
	* @return unsigned int
	*/
	unsigned int GetDataMaxId(void) const {return(UsedId);}

	/**
	* Get the list of the words currently defined in the dictionary.
	* @returns const Pointer to array of GWord*.
	*/
	const GData** GetDatas(void) const {return(const_cast<const GData**>(Direct));}

	/**
	* Get a cursor on a unordered container of data of a particular information
	* type.
	* @param type            Information type.
	*/
	GDataCursor GetDataCursor(GInfoType type) throw(std::bad_alloc,GException);

	/**
	* Get the name of the dictionary.
	* @returns RString.
	*/
	R::RString GetName(void) const throw(std::bad_alloc);

	/**
	* Get the language holding the dictionary.
	* @return Pointer to GLang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Method giving the number of data contained in the dictionary.
	* @return unsigned int
	*/
	unsigned int GetNbDatas(void) const {return(GetNb());}

	/**
	* Method giving the number of data contained in the dictionary of a given
	* information type.
	* @param type            Information type.
	*/
	unsigned int GetNbDatas(GInfoType type) const throw(GException);

	/**
	* Look if a given data is in the dictionary.
	* @param name            Name fo the data to look for.
	* @return true if the data is in the dictionary.
	*/
	bool IsIn(const R::RString& name) const;

	/**
	* Test if the data of the dictionary are stopwords.
	* @returns true if the data are stopwords.
	*/
	bool IsStopList(void) const {return(Stop);}

	/**
	* Increase the number of objects of a given type that make a reference to a
	* data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	*/
	void IncRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Decrease the number of objects of a given type that make a reference to a
	* data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	*/
	void DecRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Get the number of objects of a given type that make a reference to a data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	* @return unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Increase the number of objects of a given type using the dictionary.
	* @param ObjType        Type of the object.
	*/
	void IncRef(tObjType ObjType) throw(GException);

	/**
	* Decrease the number of objects of a given type using the dictionary.
	* @param ObjType        Type of the object.
	*/
	void DecRef(tObjType ObjType) throw(GException);

	/**
	* Decrease the number of objects of a given type using the dictionary.
	* @param ObjType        Type of the object.
	*/
	unsigned int GetRef(tObjType ObjType);

	/**
	* Destructor of the dictionary.
	*/
	virtual ~GDict(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
