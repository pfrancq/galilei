/*

	GALILEI Research Project

	GWord.h

	Word - Implementation.

	(C) 2001 by P. Francq.

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
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giword.h>
#include <galilei.h>
using namespace GALILEI;


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
	/**
	* Identificator of the word.
	*/
	unsigned Id;

	/**
	* The word.
	*/
	RString Word;

public:

  // Constructors
  GWord(void);
  GWord(const RString& word);
  GWord(unsigned id,const RString& word);
  GWord(const GWord& word);

  // Functions needed by GDict (-> RHashContainer)
  int Compare(const GWord& word) const;
  int Compare(const GWord* word) const;
  int Compare(const RString& word) const;
  int Compare(const GIWord* ref) const;
	char HashIndex(void) const
		{ return(Word.HashIndex());}

	/**
	* @return Identificator of the word.
	*/
	unsigned int GetId(void) {return(Id);}

	/**
	* @return A C string representing the word.
	*/
	const char* GetWord(void) {return(Word);}

	// friend classes
	friend class GDict;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
