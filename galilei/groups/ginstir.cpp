/*                                                            	CritSim=Prom.NewCriterion(Maximize,"Similarities",0.05,0.2,1.0);
	CritNb=Prom.NewCriterion(Maximize,"Profiles",0.05,0.2,1.0);


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
#include <rpromethee/rpromkernel.h>
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
	  Sims(s), MinSimLevel(m), MaxGen(max)
{
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
	RPromCriterion* CritSim;
	RPromCriterion* CritNb;
	RPromKernel Prom("GALILEI",PopSize,2);
	RPromSol* s;
	RPromSol** Res;
	RPromSol** ptr;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("PostEvaluate","GInstIR");
	#endif
	CritSim=Prom.NewCriterion(Maximize,"Similarities",0.2,0.05,1.0);
	CritNb=Prom.NewCriterion(Maximize,"Profiles",0.2,0.05,1.0);
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		s=Prom.NewSol();
		Prom.Assign(s,CritSim,(*C)->AvgSim);
		Prom.Assign(s,CritNb,(*C)->AvgProf);
	}
	Prom.ComputePrometheeII();
	Res=Prom.GetSols();
	for(i=PopSize+1,ptr=Res;--i;ptr++)
	{
		(*Chromosomes[(*ptr)->GetId()]->Fitness)=(*ptr)->GetFi();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("PostEvaluate","GInstIR");
	#endif
}


//-----------------------------------------------------------------------------
GALILEI::GInstIR::~GInstIR(void)
{
}
