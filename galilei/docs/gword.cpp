


//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei
#include "gword.h"
using namespace Galilei;




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
int GWord::Compare(GWord* word)
{
  return(Word.Compare(word->Word));
}


//---------------------------------------------------------------------------
int GWord::Compare(const RString& word)
{
  return(Word.Compare(word));
}


//---------------------------------------------------------------------------
int GWord::Compare(GWordRef* ref)
{
  return(Id-ref->Id);
}



