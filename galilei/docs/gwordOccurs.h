

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
// class GWordOccurs
class GWordOccurs : public RContainer<GWordOccur,unsigned,true,true>
{
	GDoc *Owner;

public:
	GWordOccurs(GDoc *owner,unsigned nb) throw(bad_alloc);
	void Insert(unsigned id,GDoc *doc) throw(bad_alloc);
	GWordOccur* GetPtr(unsigned id,GDoc *doc) throw(bad_alloc);
};





}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
