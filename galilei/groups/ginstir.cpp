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
// include files for R Project
//#include <rpromethee/rpromkernel.h>
#include <rpromethee/rpromsol.h>
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


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
GALILEI::GInstIR::GInstIR(double m,unsigned int max,unsigned int popsize,RGA::RObjs<GObjIR>* objs,GProfilesSim* s,HeuristicType h,RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(popsize,objs,h,debug),
	  RPromKernel("GALILEI",PopSize+1,2),
	  Sims(s), MinSimLevel(m), MaxGen(max),CritSim(0),CritNb(0), Sols(0)
{
	RPromSol** ptr;
	unsigned int i;

	CritSim=NewCriterion(Maximize,"Similarities",0.2,0.05,1.0);
	CritNb=NewCriterion(Maximize,"Profiles",0.2,0.05,1.0);
	Sols=new RPromSol*[PopSize+1];
	if(Sols)
	{
		for(i=PopSize+2,ptr=Sols;--i;ptr++)
			(*ptr)=NewSol();
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GInstIR::StopCondition(void)
{
	return(Gen==MaxGen);
}


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::PostEvaluate(void) throw(eGA)
{
	unsigned int i;
	GChromoIR **C;
	RPromSol** Res;
	RPromSol** ptr;
	double r;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("PostEvaluate","GInstIR");
	#endif
	ptr=Sols;
	Assign((*ptr),CritSim,BestChromosome->AvgSim);
	Assign((*ptr),CritNb,BestChromosome->AvgProf);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		Assign((*ptr),CritSim,(*C)->AvgSim);
		Assign((*ptr),CritNb,(*C)->AvgProf);
	}
	ComputePrometheeII();
	Res=GetSols();
	ptr=Res;

	// Look if the best chromosome ever is still the best -> If not, change
	// the fitness of the best solution.
	if((*ptr)->GetId())
		(*Chromosomes[(*ptr)->GetId()-1]->Fitness)=Gen+1.1;
	ptr++;

	//  The second best has the fitness of 1
	if((*ptr)->GetId()==0)
		(*BestChromosome->Fitness)=1.0;
	else
		(*Chromosomes[(*ptr)->GetId()-1]->Fitness)=1.0;

	// Look for the rest
	for(i=PopSize,ptr++;--i;ptr++)
	{
		r=((double)i)/((double)(PopSize));
		if((*ptr)->GetId()==0)
			(*BestChromosome->Fitness)=r;
		else
			(*Chromosomes[(*ptr)->GetId()-1]->Fitness)=r;
	}

	// Delete the resulting array
	delete[] Res;

	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("PostEvaluate","GInstIR");
	#endif
}


//-----------------------------------------------------------------------------
GALILEI::GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
