

//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei

#include "glangs.h"

using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GLangs
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GLangs::GLangs(unsigned nb) throw(bad_alloc)
  : RContainer<GLang,unsigned,true,true>(nb,nb/2)
{
}


//---------------------------------------------------------------------------
GLang* GLangs::GetLang(const char *code)
{
  GLang **ptr;
  long i;

	if(!code) return(0);
  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    if(!strcmp(code,(*ptr)->Code)) return(*ptr);
  return(0);
}


//---------------------------------------------------------------------------
void GLangs::CreateDicsStops(GDicts *dicts,GDicts *stops) throw(bad_alloc,GException)
{
  GLang **ptr;
  long i;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    dicts->CreateDic((*ptr)->GetDic(),*ptr);
    stops->CreateDic((*ptr)->GetStop(),*ptr);
  }
}


