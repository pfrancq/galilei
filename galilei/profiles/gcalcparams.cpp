/*

	GALILEI Research Project

	GCalcParams.cpp

	Generic Parameters for a Computing Method - Implementation

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
#include <profiles/gcalcparams.h>
#include <profiles/gprofilecalc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GCalcParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GCalcParams::GCalcParams(const char* n) throw(bad_alloc)
	: ComputingName(n)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GCalcParams::Compare(const GCalcParams* grp) const
{
	return(ComputingName.Compare(grp->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GCalcParams::Compare(const GCalcParams& grp) const
{
	return(ComputingName.Compare(grp.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GCalcParams::Compare(const GProfileCalc* grp) const
{
	return(ComputingName.Compare(grp->GetComputingName()));
}


//-----------------------------------------------------------------------------
int GALILEI::GCalcParams::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GCalcParams& GALILEI::GCalcParams::operator=(const GALILEI::GCalcParams& src)
{
	ComputingName=src.ComputingName;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GCalcParams::~GCalcParams(void)
{
}
