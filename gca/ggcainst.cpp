/*

	Genetic Community Algorithm

	GGCAInst.h

	Instance - Implementation

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofile.h>
#include <ggalileiapp.h>
#include <ggroup.h>
#include <gsession.h>
#include <gsubjects.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggcainst.h>
#include <ggcachromo.h>
#include <ggcagroup.h>
#include <ggcaobj.h>
#include <ggcaheuristic.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GGCAThreadData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAThreadData::GGCAThreadData(GGCAInst* owner)
	: RThreadDataG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>(owner),
	  ToDel(owner->Objs.GetNb()<4?4:owner->Objs.GetNb()/4), tmpObjs1(0),tmpObjs2(0), Tests(0),
	  Prom(Owner->Params), Sols(0), NbSols((Owner->Params->NbDivChromo*2)+2)
{
	RPromSol** s;
	size_t i;

	Tests=new GGCAChromo*[NbSols];
	Sols=new RPromSol*[NbSols];
	for(i=NbSols+1,s=Sols;--i;s++)
		(*s)=Prom.NewSol();
}


//-----------------------------------------------------------------------------
void GGCAThreadData::Init(void)
{
	size_t i;

	RThreadDataG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>::Init();
	tmpObjs1=new GGCAObj*[Owner->Objs.GetNb()];
	tmpObjs2=new GGCAObj*[Owner->Objs.GetNb()];
	for(i=0;i<NbSols;i++)
	{
		Tests[i]=new GGCAChromo(Owner,Owner->GetPopSize()+1+i);
		Tests[i]->Init(this);
		(static_cast<RGroups<GGCAGroup,GGCAObj,GGCAChromo>*>(Tests[i]))->Init();
	}
}


//-----------------------------------------------------------------------------
GGCAThreadData::~GGCAThreadData(void)
{
	GGCAChromo** C;
	size_t i;

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
// GGCAInst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAInst::GGCAInst(GSession* ses,RCursor<GGCAObj> objs,GGCAParams* p,RDebug *debug,tObjType type)
	: RInstG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>(p->PopSize,objs,FirstFit,"GCA",debug),
	GGCAProm(p), Params(p), Sols(0), Session(ses), NoSocialProfiles(objs.GetNb()),
	Ratios(objs.GetNb()), Sims(0),Agree(0), Disagree(0), Type(type)
#if BESTSOLSVERIFICATION
	  , BestSols(p->MaxGen,p->MaxGen/2)
#endif
{
	RPromSol** ptr;
	size_t i;

	// Init measures
	switch(Type)
	{
		case otDoc :
			Sims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities");
			Agree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Agreements");
			Disagree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Disagreements");
			break;
		case otProfile :
			Sims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities");
			Agree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Agreements");
			Disagree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Disagreements");
			break;
		default:
			throw GException("GGCAInst::GGCAInst : Type "+GetObjType(Type)+" not supported");
	}

	// Change Freq
	SetMutationParams(5,8,1);

	// Init Solutions of the PROMETHEE part
	Sols=new RPromSol*[GetPopSize()+1];
	if(Sols)
	{
		for(i=GetPopSize()+2,ptr=Sols;--i;ptr++)
		{
			(*ptr)=NewSol();
		}
	}
}


//-----------------------------------------------------------------------------
void GGCAInst::Init(void)
{
	// Init the GGA
	RInstG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>::Init();

	// Init the Ratios
	GGCAMaxRatios* ptr;
	double ratio;
	RCursor<GGCAObj> Cur1(Objs);
	RCursor<GGCAObj> Cur2(Objs);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		Ratios.InsertPtrAt(ptr=new GGCAMaxRatios(Cur1()->GetId(),Objs.GetNb()),Cur1()->GetId());

		// Add all the object with a greater agreement ratio than the minimum
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(Cur1()==Cur2()) continue;
			ratio=GetAgreementRatio(Cur1()->GetElementId(),Cur2()->GetElementId());
			if(ratio>=Params->MinAgreement)
				ptr->InsertPtr(new GGCAMaxRatio(Cur2()->GetId(),ratio));
		}

		// ReOrder to have the best ratio first
		ptr->ReOrder(GGCAMaxRatio::sortOrder);
	}
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<GGCAGroup,GGCAObj,GGCAChromo>* GGCAInst::CreateHeuristic(void)
{
	return(new GGCAHeuristic(Random,Objs,Ratios,Debug));
}


//-----------------------------------------------------------------------------
GGCAObj* GGCAInst::GetObj(size_t id) const
{
	R::RCursor<GGCAObj> Cur(Objs);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetElementId()==id)
			return(Cur());
	return(0);
}


//-----------------------------------------------------------------------------
bool GGCAInst::StopCondition(void)
{
	return(GetGen()==Params->MaxGen);
}


//-----------------------------------------------------------------------------
void GGCAInst::WriteChromoInfo(GGCAChromo* c)
{
	char Tmp[300];
	char Tmp2[300];

	if(!Debug) return;
	sprintf(Tmp,"Id %2u (Fi=%1.5f,Fi+=%1.5f,Fi-=%1.5f): ",c->Id,c->Fi,c->FiPlus,c->FiMinus);
	sprintf(Tmp2,"J=%1.5f - Agr.=%1.5f - Disagr.=%1.5f",c->CritSimJ,c->CritAgreement,c->CritDisagreement);
	strcat(Tmp,Tmp2);
	Debug->PrintInfo(Tmp);
}


//-----------------------------------------------------------------------------
void GGCAInst::PostEvaluate(void)
{
	size_t i;
	GGCAChromo** C;
	GGCAChromo* s;
	#if BESTSOLSVERIFICATION
		GGCAChromo* b;
		GGCAGroupData GrpData;
	#endif
	RPromSol** Res;
	RPromSol** ptr;
	double r;

	if(Debug)
		Debug->BeginFunc("PostEvaluate","GGCAInst");
	ptr=Sols;
	Assign(*ptr,BestChromosome);
	for(i=GetPopSize()+1,C=Chromosomes,ptr++;--i;C++,ptr++)
		Assign(*ptr,*C);
	ComputePrometheeII();
	Res=GetSols();
	ptr=Res;

	// Look if the best chromosome ever is still the best (and have some groups)
	// -> If not, change the fitness of the best solution.
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=GetGen()+1.1;
		#if BESTSOLSVERIFICATION
			BestSols.InsertPtr(b=new GGCAChromo(this,BestSols.NbPtr));
			b->Init(thDatas[0]);
			(static_cast<RGroups<GGCAGroup,GGCAObj,GGCAGroupData,GGCAChromo>*>(b))->Init(&GrpData);
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
	for(i=GetPopSize(),ptr++;--i;ptr++)
	{
		r=((double)i)/((double)(GetPopSize()));
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
		Debug->EndFunc("PostEvaluate","GGCAInst");
}


//-----------------------------------------------------------------------------
double GGCAInst::GetDisagreementRatio(size_t element1,size_t element2) const
{
	double d;
	Disagree->Measure(0,element1,element2,&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GGCAInst::GetAgreementRatio(size_t element1,size_t element2) const
{
	double d;
	Agree->Measure(0,element1,element2,&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GGCAInst::GetSim(size_t element1,size_t element2) const
{
	double d;
	Sims->Measure(0,element1,element2,&d);
	return(d);
}



//-----------------------------------------------------------------------------
void GGCAInst::PostRun(void)
{
#if BESTSOLSVERIFICATION
	RPromSol* s;

	// Init Criterion and Solutions of the PROMETHEE part
	GGCAProm::ClearSols();
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
void GGCAInst::HandlerNotFound(const RNotification& /*notification*/)
{
//	std::cout<<" GCA '"<<notification.GetName()<<"' not treated (Gen="<<Gen<<")."<<std::endl;
}


//-----------------------------------------------------------------------------
GGCAInst::~GGCAInst(void)
{
	if(Sols)
		delete[] Sols;
}
