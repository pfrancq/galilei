/*

	GALILEI Research Project

	GPrgVar.cpp

	Variable of a Program - Implementation.

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
// include files for GALILEI
#include <sessions/gprgvar.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GPrgVar
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgVar::GPrgVar(const char* name) throw(bad_alloc)
	: Name(name)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgVar::Compare(const GPrgVar* v) const
{
	return(Name.Compare(v->Name));
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgVar::Compare(const char* v) const
{
	return(Name.Compare(v));
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgVar::Assign(const void*) throw(GException)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GPrgVar::GetValue(GSessionPrg*) throw(GException)
{
	return(0);
}


//-----------------------------------------------------------------------------
GALILEI::GPrgVar::~GPrgVar(void)
{
}
