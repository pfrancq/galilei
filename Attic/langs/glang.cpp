/*

	GALILEI Research Project

	GLang.cpp

	Basic Language - Implementation.

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
#include <glangs/glang.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GLang
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GLang::GLang(const RString& lang,const char* code) throw(bad_alloc)
	: RLang(lang,code), Activ(true)
{
}


//---------------------------------------------------------------------------
int GLang::Compare(const GLang& lang) const
{
  return(strcmp(Code,lang.Code));
}


//---------------------------------------------------------------------------
int GLang::Compare(const GLang* lang) const
{
  return(strcmp(Code,lang->Code));
}


//---------------------------------------------------------------------------
int GLang::Compare(const char* code) const
{
  return(strcmp(Code,code));
}


//---------------------------------------------------------------------------
GLang::~GLang(void)
{
}
