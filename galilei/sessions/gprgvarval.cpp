/*

	GALILEI Research Project

	GPrgVarVal.cpp

	Representation of a Prg Variable - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#include <sessions/gprgvarval.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GPrgVarVal
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgVarVal::GPrgVarVal(const char* name,const char* value) throw(bad_alloc)
	: GPrgVar(name), Value(value)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgVarVal::Assign(const void* data) throw(GException)
{
	Value=static_cast<const char*>(data);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GPrgVarVal::GetValue(GSessionPrg*) throw(GException)
{
	return(Value);
}


//-----------------------------------------------------------------------------
GALILEI::GPrgVarVal::~GPrgVarVal(void)
{
}
