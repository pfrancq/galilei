/*

	GALILEI Project Library

	GIRHeuristic.cpp

	Heuristic for an IR Problem - Implemenation

	(C) 2002 by By P. Francq.

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
#include <groups/girheuristic.h>
#include <groups/gchromoir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
using namespace GALILEI;
using namespace RGGA;



//-----------------------------------------------------------------------------
//
// class GIRHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIRHeuristic::GIRHeuristic(RRandom* r,RStd::RCursor<GObjIR,unsigned int>* objs)
	: RGGA::RFirstFitHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>(r,objs)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GIRHeuristic::Init(GChromoIR* groups)
{
	RGGA::RFirstFitHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>::Init(groups);
}


//-----------------------------------------------------------------------------
GGroupIR* GALILEI::GIRHeuristic::FindGroup(void) throw(RGGA::RGroupingHeuristicException)
{
	GGroupIR* grp=0;
	double max=-0.1;
	double tmp;

	for(Groups->Used.Start();!Groups->Used.End();Groups->Used.Next())
	{
		if(!Groups->Used()->CanInsert(CurObj)) continue;
		tmp=Groups->Used()->ComputeAvgSim(CurObj);
		if(tmp>max)
		{
			max=tmp;
			grp=Groups->Used();
		}
//		return(Groups->Used());
	}
	if(!grp)
		grp=Groups->ReserveGroup();
	return(grp);
}


//-----------------------------------------------------------------------------
GALILEI::GIRHeuristic::~GIRHeuristic(void)
{
}
