
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
// include files for HyperPRISME
#include "Galilei.h"



//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// class GDicts
class GDicts : public RContainer<GDict,unsigned,true,true>
{
  GSession *Session;
public:
  GDicts(unsigned nb,GSession *session) throw(bad_alloc);
  void Load(bool dep,bool del) throw(bad_alloc,GException);
  virtual void CreateDic(const RString &name,GLang *lang) throw(bad_alloc,GException)=0;
  void Clear(void);
	virtual ~GDicts(void) {}
};




}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
