
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei
#include "GDicts.h"
using namespace Galilei;



//---------------------------------------------------------------------------
//
// class GDicts
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDicts::GDicts(unsigned nb,GSession *session) throw(bad_alloc)
  : RContainer<GDict,unsigned,true,true>(nb,nb/2),Session(session)
{
}

//---------------------------------------------------------------------------
void GDicts::Load(bool dep,bool del) throw(bad_alloc,GException)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    if((dep&&(*ptr)->Lang->Activ)||(!dep))
      (*ptr)->Load();
    else
      if(del) (*ptr)->Clear();
  }
}


//---------------------------------------------------------------------------
void GDicts::Clear(void)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    (*ptr)->Clear();
}


