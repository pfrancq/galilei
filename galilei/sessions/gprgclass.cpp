/*

	GALILEI Research Project

	GPrgClass.cpp

	Generic Class - Implementation.

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
#include <sessions/gprgclass.h>
#include <sessions/gprgvar.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GPrgClass
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgClass::GPrgClass(const char* name) throw(bad_alloc)
	: Name(name), Vars(20,10), Methods(30,15)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgClass::Compare(const GPrgClass* c) const
{
	return(Name.Compare(c->Name));
}


//-----------------------------------------------------------------------------
int GALILEI::GPrgClass::Compare(const char* c) const
{
	return(Name.Compare(c));
}


//-----------------------------------------------------------------------------
GPrgFunc* GALILEI::GPrgClass::GetMethod(const char* method) const
{
	return(Methods.GetPtr<const char*>(method));
}


//-----------------------------------------------------------------------------
GALILEI::GPrgClass::~GPrgClass(void)
{
}
