/*

	GALILEI Research Project

	GWord.h

	Word (or stem) - Header.

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
#ifndef GWordH
#define GWordH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gdata.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWord class provides a representation for a given word. In fact, it
* represents a given stem. It is data which name is the word (or stem).
* @author Pascal Francq
* @short Word.
*/
class GWord : public GData
{
public:

	/**
	* Default constructor.
	*/
	GWord(void) throw(std::bad_alloc);

	/**
	* Construct a word. Its idenfiticator is invalid (cNoRef).
	* @param word           Word.
	*/
	GWord(const R::RString& word) throw(std::bad_alloc);

	/**
	* Construct a word.
	* @param id             Identificator of the word.
	* @param word           Word.
	*/
	GWord(unsigned int id,const R::RString& word) throw(std::bad_alloc);

	/**
	* Copy constructor of a word.
	* @param word           Word.
	*/
	GWord(const GWord& word) throw(std::bad_alloc);

	/**
	* Copy constructor of a word.
	* @param word           Pointer to a word.
	*/
	GWord(const GWord* word) throw(std::bad_alloc);

	/**
	* The methodes creates a copy of the word.
	* @return Pointer to a GData.
	*/
	virtual GData* CreateCopy(void) const throw(std::bad_alloc);

	/**
	* Destructor.
	*/
	virtual ~GWord(void);
};


//------------------------------------------------------------------------------
/**
* The GWordCursor class provides a way to go trough a vector of words.
* @short Word Cursor
*/
CLASSCURSOR(GWordCursor,GWord);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
