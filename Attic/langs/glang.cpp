/*

	GALILEI Research Project

	GLang.cpp

	Basic Language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++1
#include <string.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glang.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GLang
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLang::GLang(const RString& lang,const char* code) throw(bad_alloc)
	: RLang(lang,code), Activ(true)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const GLang& lang) const
{
	return(strcmp(Code,lang.Code));
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const GLang* lang) const
{
	return(strcmp(Code,lang->Code));
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const char* code) const
{
	return(strcmp(Code,code));
}


//-----------------------------------------------------------------------------
GALILEI::GLang::~GLang(void)
{
}
