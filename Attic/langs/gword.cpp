/*

	GALILEI Research Project

	GWord.cpp

	Word - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include file for Galilei
#include <glangs/gword.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GWord
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(void) throw(bad_alloc)
  : Id(cNoRef),Word()
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const RString& word) throw(bad_alloc)
  : Id(cNoRef),Word(word)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(unsigned id,const RString& word) throw(bad_alloc)
  : Id(id),Word(word)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const GWord& word) throw(bad_alloc)
  : Id(word.Id),Word(word.Word)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const GWord& word) const
{
	return(Word.Compare(word.Word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const GWord* word) const
{
	return(Word.Compare(word->Word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const RString& word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const char* word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const unsigned int id) const
{
	return(Id-id);
}
