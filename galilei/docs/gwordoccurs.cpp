/*

	GALILEI Research Project

	gwordoccurs.cpp

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
// include files for ANSI C/C++
#include <ctype.h>
#include <string.h>
#include <fstream.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------------------------------------------------------
// include files for HyperPRISME
#include "gwordoccurs.h"


using namespace GALILEI





//---------------------------------------------------------------------------
//
// GWordOccurs
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordOccurs::GWordOccurs(GDoc *owner,unsigned nb) throw(bad_alloc)
	: RContainer<GWordOccur,unsigned,true,true>(nb+nb/2,nb/2), Owner(owner)
{
}


//---------------------------------------------------------------------------
void GWordOccurs::Insert(unsigned id,GDoc *doc) throw(bad_alloc)
{
  GWordOccur *ptr;

  ptr=new GWordOccur(id);
  ptr->Doc=doc;
  InsertPtr(ptr);
}


//---------------------------------------------------------------------------
GWordOccur* GWordOccurs::GetPtr(unsigned id,GDoc *doc) throw(bad_alloc)
{
  GWordOccur *ptr;

  ptr=GetInsertPtr<unsigned>(id);
  if(!ptr->Doc) ptr->Doc=doc;
  return(ptr);
}



