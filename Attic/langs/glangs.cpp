/*

	GALILEI Research Project

	GLangs.h

	List of the languages - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>


//-----------------------------------------------------------------------------
// include file for Galilei
#include "glangs.h"
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GLangs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangs::GLangs(unsigned nb) throw(bad_alloc)
  : RContainer<GLang,unsigned,true,true>(nb,nb/2)
{
}


//-----------------------------------------------------------------------------
GLang* GALILEI::GLangs::GetLang(const char* code)
{
	RReturnValIfFail(code,0);
	return(GetPtr<const char*>(code));
}


//-----------------------------------------------------------------------------
GALILEI::GLangs::~GLangs(void)
{
}
