/*

	GALILEI Research Project

	GWordList.h

	List of words - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Nicolas Kumps (nkumps@ulb.ac.be).

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
#ifndef GWordListH
#define GWordListH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gdata.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWordList class provides a representation for a list of words (or stems).
* Moreover, these lists are also considered to be a data that may be contained
* in documents (they are used as information entities).
* @author Pascal Francq and Nicolas Kumps
* @short List of words.
*/

class GWordList : public GData, protected R::RContainer<GWord,false,true>
{
public:

	/**
	* Constructor of a list of words.
	* @param id              Identificator of the list.
	* @param name            Name of the list.
	*/
	GWordList(unsigned int id,const R::RString& name) throw(std::bad_alloc);

	/**
	* Copy constructor fot a list of words.
	* @param list            List.
	*/
	GWordList(const GWordList& list) throw(std::bad_alloc);

	/**
	* Insert a new word in the list.
	* @param word             Pointer to the word to insert.
	*/
	void InsertWord(const GWord* word) throw(std::bad_alloc);

	/**
	* Create a cursor for the list of words.
	* @return GWordCursor.
	*/
	R::RCursor<GWord> GetWordCursor(void) throw(std::bad_alloc);

	/**
	* This method creates a copy of a list.
	* @return Pointer to a GData.
	*/
	virtual GData* CreateCopy(void) const throw(std::bad_alloc);

	/**
	* Test if the list is empty.
	* @return bool
	*/
	virtual bool IsEmpty(void) const {return(GetNb()==0);}

	/**
	* Destructor of a list of word.
	*/
	virtual ~GWordList(void);

};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
