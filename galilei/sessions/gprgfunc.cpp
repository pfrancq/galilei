/*

	GALILEI Research Project

	GPrgFunc.cpp

	Function of a Program - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gprgfunc.h>
#include <sessions/gprgvar.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GPrgFunc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgFunc::GPrgFunc(const char* name) throw(bad_alloc)
	: Name(name)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgFunc::Compare(const GPrgFunc* t) const
{
	return(Name.Compare(t->Name));
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgFunc::Compare(const char* t) const
{
	return(Name.Compare(t));
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgFunc::Run(GSessionPrg*,GSlot*,RContainer<GPrgVar,unsigned int,true,false>*) throw(GException)
{
}


//-----------------------------------------------------------------------------
GALILEI::GPrgFunc::~GPrgFunc(void)
{
}
