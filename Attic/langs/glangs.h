


//---------------------------------------------------------------------------
#ifndef GLangsH
#define GLangsH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <glangs/glang.h>
#include <glangs/gdicts.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------
class GDicts;
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
