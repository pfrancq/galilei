/*

	GALILEI Research Project

	CodeToChar.cpp

	Correspondance between Code and characters in HTML - Implementation.

	(C) 2001 by Pascal Francq and Lamoral Julien

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/codetochar.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class CodeToChar
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::CodeToChar::CodeToChar(const char* code,char c) throw(bad_alloc)
	: Char(c), Code(code)
{
}


//-----------------------------------------------------------------------------
int GALILEI::CodeToChar::Compare(const char* code) const
{
	return(Code.Compare(code));
}


//---------------------------------------------------------------------------
int GALILEI::CodeToChar::Compare(const CodeToChar& code) const
{
	return(Code.Compare(code.Code));
}


//---------------------------------------------------------------------------
int GALILEI::CodeToChar::Compare(const CodeToChar* code) const
{
	return(Code.Compare(code->Code));

}


//-----------------------------------------------------------------------------
GALILEI::CodeToChar::~CodeToChar(void)
{
}
