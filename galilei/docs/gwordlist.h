



//---------------------------------------------------------------------------
#ifndef GWordListH
#define GWordListH


//---------------------------------------------------------------------------
// include files for Rainbow


#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>


#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// class GWordList

class GWordList : public RContainer<HWordRef,unsigned,true,true>
{
public:
  unsigned Id;

  GWordList(void) throw(bad_alloc);
};


}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
