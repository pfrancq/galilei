/*

	GALILEI Research Project

	GGroupCalcParams.cpp

	Generic Parameters for a Grouping Method - Implementation

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#include <groups/ggroupcalcparams.h>
#include <groups/ggroupcalc.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGroupCalcParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupCalcParams::GGroupCalcParams(const char* n) throw(bad_alloc)
	: ComputingName(n)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalcParams::Compare(const GGroupCalcParams* p) const
{
	return(ComputingName.Compare(p->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalcParams::Compare(const GGroupCalcParams& p) const
{
	return(ComputingName.Compare(p.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalcParams::Compare(const GGroupCalc* calc) const
{
	return(ComputingName.Compare(calc->GetComputingName()));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalcParams::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalcParams& GALILEI::GGroupCalcParams::operator=(const GALILEI::GGroupCalcParams& src)
{
	ComputingName=src.ComputingName;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalcParams::~GGroupCalcParams(void)
{
}
