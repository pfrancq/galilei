/*

  gwordlist.h

  Description - Header.

  (c) 2002 by N. Kumps (nkumps@ulb.ac.be)

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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


//-----------------------------------------------------------------------------
/**
* The GWordList class provides a representation for a list of words.
*/
class GWordList : public GWord
{


public:

	/**
	* Id of the two words of the list.
	*/
	RStd::RContainer<GWord,unsigned,false,true>* List;

public:

	/**
	* Constructor.
	*/
	GWordList(void);

	GWordList(unsigned int word);

	/**
	* Constructor.
	*/
	GWordList(unsigned int id,const RStd::RString& word);

	/**
	* Constructor.
	*/
	GWordList(GWordList* wordlist);

	/**
	* Insert a new word in the list.
	*/
	void InsertWord(GWord* word);

	/**
	* Set the Type
	*/
	void SetType(GWordType type);

	/**
	* Set the id.
	*/
	void SetId(unsigned int id);

	/**
	* Destructor.
	*/
	virtual ~GWordList(void);

};
}
#endif

