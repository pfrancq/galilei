

//---------------------------------------------------------------------------
#ifndef wordsG
#define wordsG


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for HyperPRISME

#include "Galilei.h"

using namespace Galilei;


//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------

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
  int Compare(GWord* word);
  int Compare(const RString& word);
  int Compare(GWordRef* ref);
  char HashIndex(void) const { return(Word.HashIndex()); }
};


}

#endif
