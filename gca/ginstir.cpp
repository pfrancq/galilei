/*

	GALILEI Research Project

	GInstIR.cpp

	Instance for an IR Problem - Implementation

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
// include files for R Project
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <rgroupingheuristic.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginstir.h>
#include <gchromoir.h>
#include <ggroupir.h>
#include <gobjir.h>
#include <girheuristic.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <gpluginmanagers.h>
#include <ggroup.h>
#include <gsession.h>
#include <gsubjects.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// GThreadDataIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GThreadDataIR::GThreadDataIR(GInstIR* owner)
	: RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(owner),
	  ToDel(owner->Objs->GetNb()<4?4:owner->Objs->GetNb()/4), tmpObjs1(0),tmpObjs2(0), Tests(0),
	  Prom(Owner->Params), Sols(0), NbSols((Owner->Params->NbDivChromo*2)+2)
{
	RPromSol** s;
	unsigned int i;

	Tests=new GChromoIR*[NbSols];
	Sols=new RPromSol*[NbSols];
	for(i=NbSols+1,s=Sols;--i;s++)
		(*s)=Prom.NewSol();
}


//-----------------------------------------------------------------------------
void GThreadDataIR::Init(void) throw(bad_alloc)
{
	GGroupDataIR data;
	unsigned int i;

	RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init();
	tmpObjs1=new GObjIR*[Owner->Objs->GetNb()];
	tmpObjs2=new GObjIR*[Owner->Objs->GetNb()];
	for(i=0;i<NbSols;i++)
	{
		Tests[i]=new GChromoIR(Owner,Owner->PopSize+1+i);
		Tests[i]->Init(this);
		(static_cast<RGroups<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>*>(Tests[i]))->Init(&data);
	}
}


//-----------------------------------------------------------------------------
GThreadDataIR::~GThreadDataIR(void)
{
	GChromoIR** C;
	unsigned int i;

	if(Tests)
	{
		for(i=NbSols+1,C=Tests;--i;C++)
			delete (*C);
		delete[] Tests;
	}
	delete[] Sols;
	delete[] tmpObjs1;
	delete[] tmpObjs2;
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GInstIR::GInstIR(GSession* ses,GLang* l,RObjs<GObjIR>* objs,GIRParams* p,RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(p->PopSize,objs,FirstFit,"GCA",debug),
	  GIRProm(p), Params(p), Sols(0), Session(ses), Lang(l), NoSocialSubProfiles(objs->GetNb()),
	  Ratios(objs->GetNb()), ProfilesSims(GPluginManagers::GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities"))
	, ProfilesAgree(GPluginManagers::GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Agreements"))
	, ProfilesDisagree(GPluginManagers::GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Disagreements"))
#if BESTSOLSVERIFICATION
	  , BestSols(p->MaxGen,p->MaxGen/2)
#endif
{
	RPromSol** ptr;
	unsigned int i;

	// Change Freq
	MaxBestPopAge=5;
	MaxBestAge=8;
	AgeNextMutation=MaxBestPopAge;
	AgeNextBestMutation=MaxBestAge;

	// Init Solutions of the PROMETHEE part
	Sols=new RPromSol*[PopSize+1];
	if(Sols)
	{
		for(i=PopSize+2,ptr=Sols;--i;ptr++)
		{
			(*ptr)=NewSol();
		}
	}
}


//-----------------------------------------------------------------------------
void GInstIR::Init(GGroupDataIR* gdata) throw(std::bad_alloc)
{
	// Init the GGA
	RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(gdata);

	// Init the Ratios
	GIRMaxRatios* ptr;
	double ratio;
	R::RCursor<GObjIR> Cur1(*Objs);
	R::RCursor<GObjIR> Cur2(*Objs);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		Ratios.InsertPtrAt(ptr=new GIRMaxRatios(Cur1()->GetId(),Objs->GetNb()),Cur1()->GetId());

		// Add all the object with a greater agreement ratio than the minimum
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(Cur1()==Cur2()) continue;
			ratio=GetAgreementRatio(Cur1()->GetSubProfile(),Cur2()->GetSubProfile());
			if(ratio>=Params->MinAgreement)
				ptr->InsertPtr(new GIRMaxRatio(Cur2()->GetId(),ratio));
		}

		// ReOrder to have the best ratio first
		ptr->ReOrder(GIRMaxRatio::sortOrder);
	}
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* GInstIR::CreateHeuristic(void) throw(bad_alloc)
{
	return(new GIRHeuristic(Random,Objs,Ratios,Debug));
}


//-----------------------------------------------------------------------------
GObjIR* GInstIR::GetObj(const GSubProfile* sub) const
{
	R::RCursor<GObjIR> Cur;

	Cur=(*Objs);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetSubProfile()==sub)
			return(Cur());
	return(0);
}


//-----------------------------------------------------------------------------
bool GInstIR::StopCondition(void)
{
	return(Gen==Params->MaxGen);
}


//-----------------------------------------------------------------------------
void GInstIR::WriteChromoInfo(GChromoIR* c)
{
	char Tmp[300];
	char Tmp2[300];

	if(!Debug) return;
	sprintf(Tmp,"Id %2u (Fi=%f,Fi+=%f,Fi-=%f): ",c->Id,c->Fi,c->FiPlus,c->FiMinus);
	sprintf(Tmp2,"J=%1.3f - Agr.=%1.3f - Disagr.=%1.3f",c->CritSimJ,c->CritAgreement,c->CritDisagreement);
	strcat(Tmp,Tmp2);
	Debug->PrintInfo(Tmp);
}


//-----------------------------------------------------------------------------
void GInstIR::PostEvaluate(void) throw(eGA)
{
	unsigned int i;
	GChromoIR** C;
	GChromoIR* s;
	#if BESTSOLSVERIFICATION
		GChromoIR* b;
		GGroupDataIR GrpData;
	#endif
	RPromSol** Res;
	RPromSol** ptr;
	double r;

	if(Debug)
		Debug->BeginFunc("PostEvaluate","GInstIR");
	ptr=Sols;
	Assign(*ptr,BestChromosome);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		Assign(*ptr,*C);
	}
	ComputePrometheeII();
	Res=GetSols();
	ptr=Res;

	// Look if the best chromosome ever is still the best (and have some groups)
	// -> If not, change the fitness of the best solution.
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=Gen+1.1;
		#if BESTSOLSVERIFICATION
			BestSols.InsertPtr(b=new GChromoIR(this,BestSols.NbPtr));
			b->Init(thDatas[0]);
			(static_cast<RGroups<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>*>(b))->Init(&GrpData);
			(*b)=(*s);
		#endif
	}
	else
	{
		s=BestChromosome;
		// Verify that BestChromosome has at least one group
		// -> If not, exchange the first two solutions
		if(!BestChromosome->Used.GetNb())
		{
			RPromSol* old=(*ptr);
			(*ptr)=(*(ptr+1));
			(*(ptr+1))=old;
			s=Chromosomes[(*ptr)->GetId()-1];
		}

		if(s->Fitness->Value==0.0)
			(*s->Fitness)=1.1;
	}
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	if(Debug)
		WriteChromoInfo(s);
	ptr++;

	//  The second best has the fitness of 1
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=1.0;
	}
	else
		s=BestChromosome;
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	if(Debug)
		WriteChromoInfo(s);

	// Look for the rest
	for(i=PopSize,ptr++;--i;ptr++)
	{
		r=((double)i)/((double)(PopSize));
		if((*ptr)->GetId())
		{
			s=Chromosomes[(*ptr)->GetId()-1];
			(*s->Fitness)=r;
		}
		else
			s=BestChromosome;
		s->FiPlus=(*ptr)->GetFiPlus();
		s->FiMinus=(*ptr)->GetFiMinus();
		s->Fi=(*ptr)->GetFi();
		if(Debug)
			WriteChromoInfo(s);
	}

	// Delete the resulting array
	delete[] Res;

	if(Debug)
		Debug->EndFunc("PostEvaluate","GInstIR");
}


//-----------------------------------------------------------------------------
double GInstIR::GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	return(ProfilesDisagree->GetMeasure(sub1->GetId(),sub2->GetId()));
}


//-----------------------------------------------------------------------------
double GInstIR::GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	return(ProfilesAgree->GetMeasure(sub1->GetId(),sub2->GetId()));
}


//-----------------------------------------------------------------------------
double GInstIR::GetSim(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	return(ProfilesSims->GetMeasure(sub1->GetId(),sub2->GetId()));
}


//-----------------------------------------------------------------------------
double GInstIR::GetSim(const GObjIR* obj1,const GObjIR* obj2) const
{
	return(GetSim(obj1->GetSubProfile(),obj2->GetSubProfile()));
}


//-----------------------------------------------------------------------------
void GInstIR::PostRun(void)
{
#if BESTSOLSVERIFICATION
	RPromSol* s;

	// Init Criterion and Solutions of the PROMETHEE part
	GIRProm::ClearSols();
	for(BestSols.Start();!BestSols.End();BestSols.Next())
	{
		s=NewSol();
		Assign(s,BestSols());
	}
	ComputePrometheeII();
	(*BestChromosome)=(*BestSols.Tab[GetBestSol()->GetId()]);
	emitBestSig();
#endif
}


//-----------------------------------------------------------------------------
GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
