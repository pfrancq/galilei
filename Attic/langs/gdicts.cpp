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
#include <string.h>

//---------------------------------------------------------------------------
// include file for Galilei
#include "gdicts.h"
#include <glangs/gdict.h>

using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GDicts
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDicts::GDicts(unsigned nb,GSession *session) throw(bad_alloc)
  : RContainer<GDict,unsigned,true,true>(nb,nb/2),Session(session)
{
}

//---------------------------------------------------------------------------
void GDicts::Load(bool dep,bool del) throw(bad_alloc,GException)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    if((dep&&(*ptr)->Lang->Activ)||(!dep))
      (*ptr)->owner->LoadDics();
    else
      if(del) (*ptr)->Clear();
  }
}


//---------------------------------------------------------------------------
void GDicts::Clear(void)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    (*ptr)->Clear();
}


