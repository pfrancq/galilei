/*

	GALILEI Research Project

	CodeToChar.cpp

	Correspondence between Code and characters - Implementation.

	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by Nicolas Kumps.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <codetochar.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class CodeToChar
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CodeToChar::CodeToChar(const char* code,RChar c)
	: Char(c), Code(code)
{
}


//-----------------------------------------------------------------------------
int CodeToChar::Compare(const RString& code) const
{
	return(Code.Compare(code));
}


//---------------------------------------------------------------------------
int CodeToChar::Compare(const CodeToChar& code) const
{
	return(Code.Compare(code.Code));
}


//---------------------------------------------------------------------------
int CodeToChar::Compare(const CodeToChar* code) const
{
	return(Code.Compare(code->Code));

}


//-----------------------------------------------------------------------------
CodeToChar::~CodeToChar(void)
{
}
