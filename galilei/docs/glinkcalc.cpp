/*
	GALILEI Research Project

	GLinkCalc.cpp

	Generic Link Method - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

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
//include file for GALILEI
#include <docs/glinkcalc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GLinkCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalc::GLinkCalc(const char* name, GSession* ses) throw(bad_alloc)
	: ComputingName(name), Session(ses)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalc::Compare(const GLinkCalc& lnkCalc) const
{
	return(ComputingName.Compare(lnkCalc.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalc::Compare(const GLinkCalc* lnkCalc) const
{
	return(ComputingName.Compare(lnkCalc->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalc::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalc::~GLinkCalc(void)
{
}


