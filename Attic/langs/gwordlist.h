/*

	GALILEI Research Project

	gwordlist.h

	Representation of a list of words - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Nicolas Kumps (nkumps@ulb.ac.be).

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



//---------------------------------------------------------------------------
#ifndef GWordListH
#define GWordListH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <langs/gword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------
/**
* The GWordList class provides a representation for a list of words.
* @author Nicolas Kumps
* @short List of words.
*/

class GWordList : public GWord
{


public:

	/**
	* Id of the two words of the list.
	*/
	R::RContainer<GWord,unsigned,false,true>* List;

public:

	/**
	* Constructor.
	*/
	GWordList(void);

	/**
	* Constructor.
	*/
	GWordList(unsigned int word);

	/**
	* Constructor.
	*/
	GWordList(unsigned int id,const R::RString& word);

	/**
	* Constructor.
	*/
	GWordList(GWordList* wordlist);

	/**
	* Insert a new word in the list.
	* @param word             word to insert to the list.
	*/
	void InsertWord(GWord* word);

	/**
	* Set the Type.
	* @param type             type of the information.
	*/
	void SetType(GWordType type);

	/**
	* Set the id.
	* @param id               id of the world list.
	*/
	void SetId(unsigned int id);

	/**
	* Destructor.
	*/
	virtual ~GWordList(void);

};
}
#endif

