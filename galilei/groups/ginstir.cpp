/*

	GALILEI Research Project

	GInstIR.h

	Instance for an IR Problem - Implementation

	(C) 2002 by P. Francq.

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
#include <groups/ginstir.h>
#include <groups/gchromoir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <profiles/gprofilessim.h>
using namespace GALILEI;
using namespace RGGA;
using namespace RGA;



//-----------------------------------------------------------------------------
//
// GThreadDataIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GThreadDataIR::GThreadDataIR(GInstIR* owner)
	: RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(owner)
{
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GInstIR::GInstIR(unsigned int max,unsigned int popsize,RGA::RObjs<GObjIR>* objs,GProfilesSim* s,HeuristicType h,RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(popsize,objs,h,debug),
	  Sims(s), MaxGen(max)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GInstIR::StopCondition(void)
{
	return(Gen==MaxGen);
}


//-----------------------------------------------------------------------------
GALILEI::GInstIR::~GInstIR(void)
{
}
