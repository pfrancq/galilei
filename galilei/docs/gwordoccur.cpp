/*

	GALILEI Research Project

	gwordoccur.cpp
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
#include "gwordoccur.h"


using namespace GALILEI;


//---------------------------------------------------------------------------
//
// class GWordOccur
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordOccur::GWordOccur(void)
  : GWordRef(),Doc(0),Occur(0)
{
}


//---------------------------------------------------------------------------
GWordOccur::GWordOccur(unsigned id)
  : GWordRef(id),Doc(0),Occur(0)
{
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(const GWordOccur& ref)
{
  return(Id-ref.Id);
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(const GWordOccur *ref)
{
  return(Id-ref->Id);
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(const unsigned id)
{
  return(Id-id);
}



