


//---------------------------------------------------------------------------
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
#include "GLang.h"
using namespace Galilei;


//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------




// class GLangs
class GLangs : public RContainer<GLang,unsigned,true,true>
{
public:
  GLangs(unsigned nb) throw(bad_alloc);
  GLang* GetLang(const char* code);   // Return a pointer to the language
  virtual void Load(void) throw(bad_alloc,GException)=0;  //  Load the Languages
  void CreateDicsStops(GDicts *dicts,GDicts *stops) throw(bad_alloc,GException);
	virtual ~GLangs(void) {}
};

}
#endif
