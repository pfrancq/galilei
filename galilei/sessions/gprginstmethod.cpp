/*

	GALILEI Research Project

	GPrgInstMethod.cpp

	Method of a class - Implementation.

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
#include <sessions/gprginstmethod.h>
#include <sessions/gprgvar.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GPrgInst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgInstMethod::GPrgInstMethod(GPrgFunc* inst) throw(bad_alloc)
	: GPrgInst(), Method(inst), Params(10,5)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgInstMethod::Run(GSessionPrg* prg,GSlot* r) throw(GException)
{
	Method->Run(prg,r,&Params);
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgInstMethod::AddParam(GPrgVar* var) throw(bad_alloc)
{
	Params.InsertPtr(var);
}


//-----------------------------------------------------------------------------
GALILEI::GPrgInstMethod::~GPrgInstMethod(void)
{
}
