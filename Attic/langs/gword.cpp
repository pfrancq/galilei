/*

	GALILEI Research Project

	gword.cpp

	Basic Information - Implementation.

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


//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei
#include <glangs/gword.h>
using namespace GALILEI;




//---------------------------------------------------------------------------
//
// class GWord
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWord::GWord(void)
  : Id(cNoRef),Word()
{
}


//---------------------------------------------------------------------------
GWord::GWord(const RString& word)
  : Id(cNoRef),Word(word)
{
}


//---------------------------------------------------------------------------
GWord::GWord(unsigned id,const RString& word)
  : Id(id),Word(word)
{
}


//---------------------------------------------------------------------------
GWord::GWord(const GWord& word)
  : Id(word.Id),Word(word.Word)
{
}


//---------------------------------------------------------------------------
int GWord::Compare(const GWord& word)
{
  return(Word.Compare(word.Word));
}


//---------------------------------------------------------------------------
int GWord::Compare(const GWord* word)
{
  return(Word.Compare(word->Word));
}


//---------------------------------------------------------------------------
int GWord::Compare(const RString& word)
{
  return(Word.Compare(word));
}


//---------------------------------------------------------------------------
int GWord::Compare(const GIWord* ref)
{
  return(Id-ref->Id);
}



