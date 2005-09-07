/*

	GALILEI Research Project

	GIR.cpp

	GA for the Information Retrieval Problem - Implementation.

	Copyright 2002 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// includes files for GALILEI
#include <gir.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GIRParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIRParams::GIRParams(void)
{
}



//-----------------------------------------------------------------------------
//
// GIRMaxRatio
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
int GIRMaxRatio::sortOrder(const void* a,const void* b)
{
	double af=(*((GIRMaxRatio**)(a)))->Ratio;
	double bf=(*((GIRMaxRatio**)(b)))->Ratio;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//-----------------------------------------------------------------------------
//
// GIRMaxRatios
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIRMaxRatios::GIRMaxRatios(unsigned int objid,unsigned int max)
	: RContainer<GIRMaxRatio,true,false>(max>4?max/4:4), ObjId(objid)
{
}
