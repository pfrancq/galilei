/*

	GALILEI Research Project

	GPrgVarConst.cpp

	Const Variable - Implementation.

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
#include <sessions/gprgvarconst.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GPrgVarConst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgVarConst::GPrgVarConst(GPrgVar* owner,const char* value) throw(bad_alloc)
	: GPrgVar(value,owner)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgVarConst::Assign(const void*) throw(GException)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GPrgVarConst::GetValue(GSessionPrg*) throw(GException)
{
	return(Name());
}


//-----------------------------------------------------------------------------
GALILEI::GPrgVarConst::~GPrgVarConst(void)
{
}
