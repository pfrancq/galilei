
#ifndef   GDictsH
#define   GDictsH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gsessions/gsession.h>
#include <glangs/glang.h>
#include <glangs/gdict.h>


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------

class GSession;

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
