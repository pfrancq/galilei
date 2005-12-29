/*

	GALILEI Research Project

	GLangFI.cpp

	Finnish Language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include files for ANSI C/C++
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangfi.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
using namespace stemming;



//-----------------------------------------------------------------------------
//
// class GLangFI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangFI::GLangFI(GFactoryLang* fac) 
	: GLang(fac,"Finnish","fi")
{
}


//-----------------------------------------------------------------------------
RString GLangFI::GetStemming(const RString& _kwd) 
{
	char* end;
	unsigned int len;
	string word;
	 word=_kwd.ToString();
	(*this)(word);
	return RString(word);
}


//------------------------------------------------------------------------------
void GLangFI::CreateParams(GParams*)
{
}


//-----------------------------------------------------------------------------
GLangFI::~GLangFI(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Finnish",GLangFI,"fi")
