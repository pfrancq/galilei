/*

	GALILEI Research Project

	GWordOccurs.h

	Word Occurences - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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
#ifndef GWordOccurs_H
#define GWordOccurs_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWordOccurs class provides a representation for a word and all its
* occurences in the documents.
* @author Pascal Francq
* @short Word Occurences.
*/
class GWordOccurs
{
	/**
	* The word.
	*/
	R::RString Word;

	/**
	* Documents containing the documents.
	*/
	R::RContainer<GInfo,true,true> Docs;

public:

	/**
	* Constructor of a word.
	* @param word            Word.
	*/
	GWordOccurs(R::RString word) throw(std::bad_alloc);

	/**
	* Compare the names of two data (work like the 'strcmp' function of ANSI
	* C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param word            Data.
	* @return int
	*/
	int Compare(const GWordOccurs& word) const;

	/**
	* Compare the names of two data (work like the 'strcmp' function of ANSI
	* C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param word            Pointer to a data.
	* @return int
	*/
	int Compare(const GWordOccurs* word) const;

	/**
	* Compare the name of a data and a string (work like the 'strcmp' function
	* of ANSI C/C++).
	* @see R::RContainer and R::RHashContainer.
	* @param name            Name.
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
	* Compute the first hash index of the name of a data.
	* @see R::RHashContainer
	* @param d               Pointer to a data.
	*/
	static int HashIndex(const GWordOccurs* word)
		{return(R::RString::HashIndex(word->Word));}

	/**
	* Compute the first hash index of the name of a data.
	* @see R::RHashContainer
	* @param word            Data.
	*/
	static int HashIndex(const GWordOccurs& word)
		{return(R::RString::HashIndex(word.Word));}

	/**
	* Compute the first hash index of a string.
	* @see R::RHashContainer
	* @param word            String.
	*/
	static int HashIndex(const R::RString& word)
		{return(R::RString::HashIndex(word));}

	/**
	* Compute the first hash index of an array of characters.
	* @see R::RHashContainer
	* @param word           Pointer to the array.
	*/
	static int HashIndex(const R::RChar* word)
		{return(R::RString::HashIndex(word));}

	/**
	* Compute the second hash index of the name of a data.
	* @see R::RHashContainer
	* @param word            Data.
	*/
	static int HashIndex2(const GWordOccurs& word)
		{return(R::RString::HashIndex2(word.Word));}

	/**
	* Compute the second hash index of the name of a data.
	* @see R::RHashContainer
	* @param word            Pointer to a data.
	*/
	static int HashIndex2(const GWordOccurs* word)
		{return(R::RString::HashIndex2(word->Word));}

	/**
	* Compute the second hash index of an array of characters.
	* @see R::RHashContainer
	* @param word           Pointer to to the array.
	*/
	static int HashIndex2(const R::RChar* word)
		{return(R::RString::HashIndex2(word));}

	/**
	* Compute the second hash index of a string.
	* @see R::RHashContainer
	* @param word           String.
	*/
	static int HashIndex2(const R::RString& word)
		{return(R::RString::HashIndex2(word));}

	/**
	* Add a document to the list of those containing the word.
	* @param docid           Identificator of the document.
	*/
	void AddDoc(unsigned int docid);

	/**
	* Look which documents are in the word:
	* - If the container is empty, the method adds all the documents where the
	*   word appears.
	* - If the container is not empty, the method removes all the documents
	*   which are not containing the word.
	* @param docs            Container of documents.
	*/
	void FilterDocs(R::RContainer<GInfo,true,false>& docs) const;

	/**
	* Destructor.
	*/
	virtual ~GWordOccurs(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
