/*

	GALILEI Research Project

	GWord.h

	Word - Header.

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
#ifndef GWordH
#define GWordH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GWord class provides a representation for a word.
* @author Pascal Francq
* @short Word.
*/
class GWord
{
protected :

	/**
	* Identificator of the word.
	*/
	unsigned int Id;

	/**
	* The word.
	*/
	R::RString Word;

	GWordType Type;

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
	* Default constructor for a word. The word is not a valid one.
	*/
	GWord(void) throw(bad_alloc);

	/**
	* Construct a word but without to know which will be its identificator.
	* @param word           Word.
	*/
	GWord(const R::RString& word) throw(bad_alloc);

	/**
	* Construct a word.
	* @param id             Identificator of the word.
	* @param word           Word.
	*/
	GWord(unsigned id,const R::RString& word) throw(bad_alloc);

	/**
	* Copy constructor.
	* @param word           Word used as reference.
	*/
	GWord(const GWord& word) throw(bad_alloc);

	/**
	* Compare method used by R::RContainer and R::RHashContainer.
	*/
	int Compare(const GWord& word) const;

	/**
	* Compare method used by R::RContainer and R::RHashContainer.
	*/
	int Compare(const GWord* word) const;

	/**
	* Compare method used by R::RContainer and R::RHashContainer.
	*/
	int Compare(const R::RString& word) const;

	/**
	* Compare method used by R::RContainer and R::RHashContainer.
	*/
	int Compare(const char* word) const;

	/**
	* Compare method used by R::RContainer and R::RHashContainer.
	*/
	int Compare(const unsigned int id) const;


	void SetType(GWordType type);

	GWordType GetType() {return(Type);}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex(const GWord* w)
		{return(R::RString::HashIndex(w->Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex(const GWord& w)
		{return(R::RString::HashIndex(w.Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex(const char* word)
		{return(R::RString::HashIndex(word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex2(const GWord* w)
		{return(R::RString::HashIndex2(w->Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex2(const GWord& w)
		{return(R::RString::HashIndex2(w.Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the R::RHashContainer class.
	*/
	static char HashIndex2(const char* word)
		{return(R::RString::HashIndex2(word));}

	/**
	* @return Identificator of the word.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* @return A C string representing the word.
	*/
	const char* GetWord(void) const {return(Word);}

	/**
	* Increase the number of references on this word.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(tObjType ObjType);

	/**
	* Decrease the number of references on this word.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(tObjType ObjType);

	/**
	* Get the number of references on this word.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType);

	// friend classes
	friend class GDict;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
