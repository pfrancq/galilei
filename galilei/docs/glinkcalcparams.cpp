/*
	GALILEI Research Project

	GLinkCalcParams.cpp

	Generic Parameters for a Link Method - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
#include <docs/glinkcalcparams.h>
#include <docs/glinkcalc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// GLinkCalcParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcParams::GLinkCalcParams(const char* n) throw(bad_alloc)
	: ComputingName(n)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalcParams::Compare(const GLinkCalcParams* p) const
{
	return(ComputingName.Compare(p->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalcParams::Compare(const GLinkCalcParams& p) const
{
	return(ComputingName.Compare(p.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalcParams::Compare(const GLinkCalc* calc) const
{
	return(ComputingName.Compare(calc->GetComputingName()));
}


//-----------------------------------------------------------------------------
int GALILEI::GLinkCalcParams::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcParams& GALILEI::GLinkCalcParams::operator=(const GALILEI::GLinkCalcParams& src)
{
	ComputingName=src.ComputingName;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcParams::~GLinkCalcParams(void)
{
}
