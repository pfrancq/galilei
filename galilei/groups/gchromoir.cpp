/*

	GALILEI Research Project

	GChromoIR.cpp

	Chromosome for an IR Problem - Implementation

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
// include files for R Project
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
using namespace GALILEI;
using namespace RGGA;
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class GChromoIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GChromoIR::GChromoIR(GInstIR* inst,unsigned int id) throw(bad_alloc)
  : RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(inst,id),
    Sims(0), MinSimLevel(0), AvgSim(0.0), AvgProf(0.0), OKFactor(0.0), DiffFactor(1.0)
{
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Init(GThreadDataIR* thData) throw(bad_alloc)
{
	// Parent Initialisation
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(thData);
	Sims=Instance->Sims;
	MinSimLevel=Instance->MinSimLevel;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(void)
{
	GSubProfilesSameGroupIR* ptr;

	// Compute Average Similarity inside the groups
	AvgSim=AvgProf=0.0;
	if(!Used.NbPtr)
		return;
	for(Used.Start();!Used.End();Used.Next())
	{
		AvgSim+=Used()->ComputeAvgSim();
		AvgProf+=Used()->NbSubObjects;
	}
	AvgSim/=((double)Used.NbPtr);
	AvgProf/=((double)Used.NbPtr);

	// Number of subprofiles having common OK documents and being in the same group.
	OKFactor=0.0;
	for(Instance->SameGroups.Start();!Instance->SameGroups.End();Instance->SameGroups.Next())
	{
		ptr=Instance->SameGroups();
		if(ObjectsAss[ptr->Id1]==ObjectsAss[ptr->Id2])
			OKFactor+=1.0;
	}
	if(Instance->SameGroups.NbPtr)
	{
		OKFactor/=Instance->SameGroups.NbPtr;
	}
	else
	{
		OKFactor=1.0;
	}

	// Number of subprofiles having common documents with different judgment and being in the same group.
	DiffFactor=0.0;
	for(Instance->DiffGroups.Start();!Instance->DiffGroups.End();Instance->DiffGroups.Next())
	{
		ptr=Instance->DiffGroups();
		if(ObjectsAss[ptr->Id1]==ObjectsAss[ptr->Id2])
			DiffFactor+=1.0;
	}
	if(Instance->DiffGroups.NbPtr)
	{
		DiffFactor/=Instance->DiffGroups.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::LocalOptimisation(void)
{
	for(Used.Start();!Used.End();Used.Next())
		Used()->DoOptimisation();
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::SameGroup(unsigned int obj1,unsigned int obj2) const
{
	return(ObjectsAss[obj1]==ObjectsAss[obj2]);
}


//-----------------------------------------------------------------------------
GChromoIR& GALILEI::GChromoIR::operator=(const GChromoIR& chromo)
{
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::operator=(chromo);
	AvgSim=chromo.AvgSim;
	AvgProf=chromo.AvgProf;
	OKFactor=chromo.OKFactor;
	DiffFactor=chromo.DiffFactor;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GChromoIR::~GChromoIR(void)
{
}
