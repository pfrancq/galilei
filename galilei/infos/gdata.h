/*

	GALILEI Research Project

	GData.h

	Generic data - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GDataH
#define GDataH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GData class provides a representation for a generic data. This may be a
* word or a group of words for example. A date has several characteristics :
* - An identificator which is used by the corresponding GInfo objects ;
* - A name ;
* - A type ;
* - Statistics on the number of documents, subprofiles or groups contained int.
* @author Pascal Francq
* @short Generic Data.
*/
class GData
{
protected :

	/**
	* Identificator of the word.
	*/
	unsigned int Id;

	/**
	* Name of the data.
	*/
	R::RString Name;

	/**
	* Type of the data.
	*/
	tInfoType Type;

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

public:

	/**
	* Construct a data with a known identificator.
	* @param id             Identificator of the data.
	* @param name           Name.
	* @param type           Type of the data.
	* @param refdocs         Number of documents referenced.
	* @param refsubprofiles  Number of subprofiles referenced.
	* @param refgroups       Number of groups referenced.
	*/
	GData(unsigned int id,const R::RString& name,tInfoType type,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups);

	/**
	* Copy constructor of a data.
	* @param d               Data.
	*/
	GData(const GData& d);

	/**
	* Compare the names of two data (work like the 'strcmp' function of ANSI
	* C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param d               Data.
	* @return int
	*/
	int Compare(const GData& d) const;

	/**
	* Compare the names of two data (work like the 'strcmp' function of ANSI
	* C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param d              Pointer to a data.
	* @return int
	*/
	int Compare(const GData* d) const;

	/**
	* Compare the name of a data and a string (work like the 'strcmp' function
	* of ANSI C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param name           String.
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Compare the name of a data and a C string (work like the 'strcmp' function
	* of ANSI C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param name            C String.
	* @return int
	*/
	int Compare(const char* name) const;

	/**
	* Compare the identificator of the data with another one.
	* @see R::RContainer and R::RHashContainer.
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Assignment operator for data.
	* @param d               Data.
	*/
	GData& operator=(const GData& d);

	/**
	* Compute the first hash index of the name of a data.
	* @see R::RHashContainer
	* @param d               Data.
	*/
	static int HashIndex(const GData& d)
		{return(R::RString::HashIndex(d.Name));}

	/**
	* Compute the first hash index of the name of a data.
	* @see R::RHashContainer
	* @param d               Data.
	*/
	static int HashIndex(const GData* d)
		{return(R::RString::HashIndex(d->Name));}

	/**
	* Compute the first hash index of a string.
	* @see R::RHashContainer
	* @param name           String.
	*/
	static int HashIndex(const R::RString& name)
		{return(R::RString::HashIndex(name));}

	/**
	* Compute the first hash index of an array of characters.
	* @see R::RHashContainer
	* @param name           Pointer to the array.
	*/
	static int HashIndex(const R::RChar* name)
		{return(R::RString::HashIndex(name));}

	/**
	* Compute the second hash index of the name of a data.
	* @see R::RHashContainer
	* @param d               Data.
	*/
	static int HashIndex2(const GData& d)
		{return(R::RString::HashIndex2(d.Name));}

	/**
	* Compute the second hash index of the name of a data.
	* @see R::RHashContainer
	* @param d               Data.
	*/
	static int HashIndex2(const GData* d)
		{return(R::RString::HashIndex2(d->Name));}

	/**
	* Compute the second hash index of an array of characters.
	* @see R::RHashContainer
	* @param name           Pointer to to the array.
	*/
	static int HashIndex2(const R::RChar* name)
		{return(R::RString::HashIndex2(name));}

	/**
	* Compute the second hash index of a string.
	* @see R::RHashContainer
	* @param name           String.
	*/
	static int HashIndex2(const R::RString& name)
		{return(R::RString::HashIndex2(name));}

	/**
	* Get the type of the data.
	* @return tInfoType.
	*/
	tInfoType GetType(void) const {return(Type);}

	/**
	* Set the Identificator of the word.
	* @param id              Identificator of the word.
	*/
	void SetId(unsigned int id);

	/**
	* @return Identificator of the word.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* @return A string representing the word.
	*/
	R::RString GetName(void) const;

	/**
	* Increase the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	*/
	unsigned int IncRef(tObjType ObjType);

	/**
	* Decrease the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	*/
	unsigned int DecRef(tObjType ObjType);

	/**
	* Get the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType) const;

	/**
	* Clear the information of the data link to a specific object.
	* @param ObjType        Type of the object.
	*/
	virtual void Clear(tObjType ObjType);

	/**
	* Clear all the information of the data.
	*/
	virtual void Clear(void);

	/**
	* This method creates a copy of himself. This method must be overaloaded by
	* the descending classes. It is used by the dictionary to insert a data the
	* first time it find its.
	* @return Pointer to a GData.
	*/
	virtual GData* CreateCopy(void) const=0;

	/**
	* Test if the data is considered as empty. By default, every data defined by
	* a non-empty name, is also considered as not empty.
	* @return bool
	*/
	virtual bool IsEmpty(void) const;

	/**
	* Destructor.
	*/
	virtual ~GData(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
