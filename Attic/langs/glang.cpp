/*

  GLang.cpp

  Words Handling - Implementation.

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  HyperPRISME base library

*/



//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for HyperPRISME
#include "GLang.h"

using namespace Galilei;



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



