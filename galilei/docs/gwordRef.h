
#ifndef wordsG
#define wordsG


//---------------------------------------------------------------------------
// include files for Rainbow
#ifdef __BORLANDC__
  #pragma warn -ccc
  #pragma warn -rch
#endif
#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#ifdef __BORLANDC__
  #pragma warn .ccc
  #pragma warn .rch
#endif
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include "Galilei.h"



//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GWordRef
class GWordRef
{
public:
  unsigned Id;

  GWordRef(void);
  GWordRef(unsigned int id);
  int Compare(const GWordRef& ref);
  int Compare(GWordRef *ref);
};



}  //-------- End of namespace HyperPRISME-----------------------------------


//---------------------------------------------------------------------------
#endif
