/*

	GALILEI Research Project

	GLangDE.cpp

	German Language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangde.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
using namespace stemming;



//-----------------------------------------------------------------------------
//
// class GLangDE
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangDE::GLangDE(GFactoryLang* fac) throw(bad_alloc)
	: GLang(fac,"German ","de")
{
}


//-----------------------------------------------------------------------------
RString GLangDE::GetStemming(const RString& _kwd) throw(GException)
{
	string word;
	 word=_kwd.ToString();
	(*this)(word);
	return RString(word);
}


//------------------------------------------------------------------------------
void GLangDE::CreateParams(GParams*)
{
}


//-----------------------------------------------------------------------------
GLangDE::~GLangDE(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("German",GLangDE,"de")
