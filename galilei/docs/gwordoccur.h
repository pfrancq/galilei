

//---------------------------------------------------------------------------
#ifndef GWordOccurH
#define GWordOccurH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include "Galilei.h"
#include <gdocs/gdoc.h>




//---------------------------------------------------------------------------
namespace GALILEI{
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
