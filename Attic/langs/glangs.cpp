/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

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
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei

#include "glangs.h"

using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GLangs
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GLangs::GLangs(unsigned nb) throw(bad_alloc)
  : RContainer<GLang,unsigned,true,true>(nb,nb/2)
{
}


//---------------------------------------------------------------------------
GLang* GLangs::GetLang(const char *code)
{
  GLang **ptr;
  long i;

	if(!code) return(0);
  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    if(!strcmp(code,(*ptr)->Code)) return(*ptr);
  return(0);
}


//---------------------------------------------------------------------------
void GLangs::CreateDicsStops(GDicts *dicts,GDicts *stops) throw(bad_alloc,GException)
{
  GLang **ptr;
  long i;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    dicts->CreateDic((*ptr)->GetDic(),*ptr);
    stops->CreateDic((*ptr)->GetStop(),*ptr);
  }
}


