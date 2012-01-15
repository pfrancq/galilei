/*

	GALILEI Project

	GInsth.hh

	Class representing an instance of a HGA for GALILEI - Implementation

	Copyright 1998-2012 by Pascal Francq (pfrancq@ulb.ac.be).
	Copyright 1998-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for R library
#include <rfirstnodeheuristic.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ginsth.h>
#include <gchromoh.h>
#include <gnodeinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// GThreadDataH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GThreadDataH::GThreadDataH(GInstH *owner)
	: RThreadDataH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,RObjH>(owner)
{
}


//------------------------------------------------------------------------------
GThreadDataH::~GThreadDataH(void)
{
}



//------------------------------------------------------------------------------
//
// GInstH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GInstH::GInstH(unsigned int max,unsigned int popsize,RCursor<RObjH> objs,const RString& h,RDebug* debug)
	: RInstH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,RObjH>(popsize,objs,h,"GALILEI GHA",debug),
	  MaxGen(max)
{
}


//------------------------------------------------------------------------------
void GInstH::Init(void)
{
	RInstH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,RObjH>::Init();
}


//------------------------------------------------------------------------------
bool GInstH::StopCondition(void)
{
	return(GetGen()==MaxGen);
}


//------------------------------------------------------------------------------
void GInstH::PostRun(void)
{
	BestChromosome->CleanUp();
}


//------------------------------------------------------------------------------
GInstH::~GInstH(void)
{
}
