/*

	GALILEI Research Project

	GLangFR.cpp

	French Language - Implementation.

	(C) 2001 by P. Francq.

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



//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <string.h>


//---------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glangfr.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GLangFR
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GLangFR::GLangFR(void) throw(bad_alloc)
	: GLang("French","fr")
{
}

//-----------------------------------------------------------------------------
RString& GLangFR::GetStemming(const RString& _kwd)
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


//---------------------------------------------------------------------------
GLangFR::~GLangFR(void)
{
}
