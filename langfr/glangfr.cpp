/*

	GALILEI Research Project

	GLangFR.cpp

	French Language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glangfr.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GLangFR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangFR::GLangFR(void) throw(bad_alloc)
	: GLang("French","fr")
{
}

//-------------------------------------------------------------------------------
RString& GALILEI::GLangFR::GetStemming(const RString& _kwd)
{
	RString *res=RString::GetString();
//	char kwd[50];
//	char* end;
//	unsigned int len;

	// Transform _kwd in lowercase and store it in kwd.
	(*res)=_kwd;
	res->StrLwr();
//	strcpy(kwd,(*res)());
//	len=_kwd.GetLen()-1;
//	end=&kwd[len];
//
//	// Do the different steps of the Porter algorithm.
//	ApplyRules(kwd,end,Rules1a);
//	if(ApplyRules(kwd,end,Rules1b))
//		ApplyRules(kwd,end,Rules1bb);
//	ApplyRules(kwd,end,Rules1c);
//	ApplyRules(kwd,end,Rules2);
//	ApplyRules(kwd,end,Rules3);
//	ApplyRules(kwd,end,Rules4);
//	ApplyRules(kwd,end,Rules5a);
//	ApplyRules(kwd,end,Rules5b);
//
//	// Put the result in res and return it.
//	(*res)=kwd;
	return((*res));
}


//-----------------------------------------------------------------------------
GALILEI::GLangFR::~GLangFR(void)
{
}
