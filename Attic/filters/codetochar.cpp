/*

	R Project Library

	codetochar.cpp

	Main Window - Implementation.

	(C) 2001 by Lamoral Julien

	Version $Revision$

	Last Modify: $Date$

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
//
// class CodeToChar
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
# include <string.h>

//-----------------------------------------------------------------------------
// include files for GALILEI

# include "codetochar.h"

//-----------------------------------------------------------------------------
using namespace GALILEI;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CodeToChar::CodeToChar(char* Coden,char Returnn)
{
	Code=Coden;
	Return=Returnn;
}

//-----------------------------------------------------------------------------
int CodeToChar::Compare(const char* Coder)
{
	return (strcmp (Code,Coder));
}


//---------------------------------------------------------------------------
int CodeToChar::Compare(const CodeToChar& word)
{
	return(strcmp(Code,word.Code));
}


//---------------------------------------------------------------------------
int CodeToChar::Compare(const CodeToChar* word)
{
	return(strcmp(Code,word->Code));

}


//-----------------------------------------------------------------------------
CodeToChar::~CodeToChar(){
}
