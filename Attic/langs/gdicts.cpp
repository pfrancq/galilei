/*

	GALILEI Research Project

	GDicts.cpp

	List of dictionnaries - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <langs/gdicts.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GDicts
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GDicts::GDicts(unsigned nb) throw(bad_alloc)
  : RContainer<GDict,unsigned,true,true>(nb,nb/2)
{
}


//---------------------------------------------------------------------------
void GALILEI::GDicts::Clear(void)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    (*ptr)->Clear();
}


//---------------------------------------------------------------------------
GALILEI::GDicts::~GDicts(void)
{
}
