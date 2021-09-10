/*

	GALILEI Project

	GChromoh.cpp

	Chromosomes representing a tree of vectors - Implementation

	Copyright 1998-2014 by Pascal Francq (pfrancq@ulb.ac.be).
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
// include files for ANSI C/C++
#include <values.h>


//------------------------------------------------------------------------------
// include files for R
#include <robjh.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gchromoh.h>
#include <ginsth.h>
#include <gnodeinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class GChromoH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GChromoH::GChromoH(GInstH* inst,unsigned id)
	: RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,RObjH>(inst,id)
{
	(*Fitness)=MAXDOUBLE;
}


//------------------------------------------------------------------------------
void GChromoH::Init(GThreadDataH* thData)
{
	RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,RObjH>::Init(thData);
}


//------------------------------------------------------------------------------
void GChromoH::Evaluate(void)
{
	double val=0.0;

	RNodeCursor<GChromoH,GNodeInfos> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Evaluate(val,0.0);
	(*Fitness)=val/static_cast<double>(Objs.GetNb());
}


//------------------------------------------------------------------------------
GChromoH::~GChromoH(void)
{
}
