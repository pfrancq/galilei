

//---------------------------------------------------------------------------
#ifndef docsG
#define docsG


//---------------------------------------------------------------------------
// include files for Rainbow
#ifdef __BORLANDC__
  #pragma warn -ccc
  #pragma warn -rch
#endif
#include <rstd/rcontainer.h>
#ifdef __BORLANDC__
  #pragma warn .ccc
  #pragma warn .rch
#endif
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include "Galilei.h"



#include "words.h"



//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GWordOccur
class GWordOccur : public GWordRef
{
public:
  GDoc *Doc;
  unsigned Occur;
public:
  GWordOccur(void);
  GWordOccur(unsigned id);
  int Compare(const GWordOccur& ref);
  int Compare(GWordOccur *ref);
  int Compare(unsigned id);
};



}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
