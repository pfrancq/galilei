
//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for HyperPRISME
#include "glang.h"

using namespace GALILEIi;



//---------------------------------------------------------------------------
//
// class GLang
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GLang::GLang(const RString& lang,const char* code) throw(bad_alloc)
  : Lang(lang),Activ(true)
{
  memcpy(Code,code,2*sizeof(char));
  Code[2]=0;
}


//---------------------------------------------------------------------------
int GLang::Compare(const GLang& lang)
{
  return(Lang.Compare(lang.Lang));
}


//---------------------------------------------------------------------------
int GLang::Compare(const GLang *lang)
{
  return(Lang.Compare(lang->Lang));
}



