

//---------------------------------------------------------------------------
#ifndef GWordH
#define GWordH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for HyperPRISME

#include <galilei.h>
#include <gdocs/gwordref.h>
using namespace GALILEI;


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------

//class GWordRef;


//---------------------------------------------------------------------------
// class GWord
class GWord
{
public:
  unsigned Id;       // Id of the word
  RString Word;           // The word

  // Constructors
  GWord(void);
  GWord(const RString& word);
  GWord(unsigned id,const RString& word);
  GWord(const GWord& word);

  // Functions needed by GDict (-> RHashContainer)
  int Compare(const GWord& word);
  int Compare(const GWord* word);
  int Compare(const RString& word);
  int Compare(const GWordRef* ref);
  char HashIndex(void) const { return(Word.HashIndex());
}// vois si c pas GWORD
};


}

#endif
