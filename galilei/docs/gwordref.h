
#ifndef GWordRefH
#define GWordRefH


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



}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
