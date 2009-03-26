/*

	GALILEI Research Project

	LangES.cpp

	Spanish Language - Implementation.

	Copyright 2001 by David Wartel.
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
#include <langes.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
using namespace stemming;



//-----------------------------------------------------------------------------
//
// class GLangES
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangES::GLangES(GFactoryLang* fac)
	: GLang(fac,"Spanish","es")
{
}


//-----------------------------------------------------------------------------
RString GLangES::GetStemming(const RString& _kwd)
{
	string word;
	 word=_kwd.ToString();
	(*this)(word);
	return RString(word);
}


//------------------------------------------------------------------------------
void GLangES::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangES::~GLangES(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Spanish",GLangES,"es")
