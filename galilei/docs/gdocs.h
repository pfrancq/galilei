

//---------------------------------------------------------------------------
#ifndef docsG
#define docsG


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include "galilei.h"
#include "gsession.h"
#include "glangs.h"
#include "gprofile.h"



//---------------------------------------------------------------------------
namespace Galilei{
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// class GDocs
class GDocs : public RContainer<GDoc,unsigned,true,false>
{
public:
  bool AllDocs;             // All Docs must be Loaded?

  GSession *Session;        // Languages

  GDocs(unsigned int nb,bool alldocs,GSession *session) throw(bad_alloc);
  virtual void Load(GLangs*) throw(bad_alloc,GException)=0;
  virtual void Load(GLangs*,GProfile *) throw(bad_alloc,GException)=0;
  void Analyse(URLFunc *urlfunc,InfoFunc *infofunc) throw(GException);
	void Download(void);
	virtual ~GDocs(void) {}
};




}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
