/*

	GALILEI Research Project

	GInstIR.cpp

	Instance for an IR Problem - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rpromethee/rpromsol.h>
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rgroupingheuristic.h>
using namespace RGGA;
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ginstir.h>
#include <groups/gchromoir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <groups/girheuristic.h>
#include <groups/gcomparegrouping.h>
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilessamedocs.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GThreadDataIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GThreadDataIR::GThreadDataIR(GInstIR* owner)
	: RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(owner),
	  tmpObjs1(0),tmpObjs2(0)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GThreadDataIR::Init(void) throw(bad_alloc)
{
	RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init();
	tmpObjs1=new GObjIR*[Owner->Objs->GetNb()];
	tmpObjs2=new GObjIR*[Owner->Objs->GetNb()];
}


//-----------------------------------------------------------------------------
GALILEI::GThreadDataIR::~GThreadDataIR(void)
{
	if(tmpObjs1) delete[] tmpObjs1;
	if(tmpObjs2) delete[] tmpObjs2;
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,double m,unsigned int max,unsigned int popsize,GGroups* grps,RGA::RObjs<GObjIR>* objs,bool g,GProfilesSim* s,SimType st,RGA::RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(popsize,objs,FirstFit,debug),
	  RPromKernel("GALILEI",PopSize+1,5), Sims(s), SameFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  DiffFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  MinSimLevel(m), MaxGen(max), SimMeasure(st),
	  CritSim(0), CritInfo(0), CritSameFeedbacks(0), CritDiffFeedbacks(0), CritSocial(0),
	  Sols(0), GlobalSim(g),
	  CurrentGroups(grps), Session(ses), Lang(l), NoSocialSubProfiles(objs->NbPtr)
{
	RPromSol** ptr;
	RCursor<GObjIR,unsigned int> Cur1;
	RCursor<GObjIR,unsigned int> Cur2;
	unsigned int i,j;
	double nb,tmp;

	#ifdef RGADEBUG
		IdealGroups=0;
	#endif

	// Change Freq
	MaxBestPopAge=5;
	MaxBestAge=8;
	AgeNextMutation=MaxBestPopAge;
	AgeNextBestMutation=MaxBestAge;

	// Init subprofiles that are in the same group
	Cur1.Set(objs);
	Cur2.Set(objs);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		if(!Cur1()->GetSubProfile()->GetProfile()->IsSocial())
			NoSocialSubProfiles.InsertPtr(Cur1());
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=Cur1()->GetSubProfile()->GetCommonDocs(Cur2()->GetSubProfile());
			nb=Cur1()->GetSubProfile()->GetCommonOKDocs(Cur2()->GetSubProfile());
			if(nb)
				SameFeedbacks.InsertPtr(new GSubProfilesSameDocs(Cur1()->GetId(),Cur2()->GetId(),nb/tmp));
			nb=Cur1()->GetSubProfile()->GetCommonDiffDocs(Cur2()->GetSubProfile());
			if(nb)
				DiffFeedbacks.InsertPtr(new GSubProfilesSameDocs(Cur1()->GetId(),Cur2()->GetId(),nb/tmp));
		}
	}

	// Init Criterion and Solutions of the PROMETHEE part
	CritSim=NewCriterion(Maximize,"Similarity",0.2,0.05,1.0);
	CritInfo=NewCriterion(Maximize,"Information",0.2,0.05,1.0);
	CritSameFeedbacks=NewCriterion(Maximize,"Same Feedbacks",0.2,0.05,1.0);
	CritDiffFeedbacks=NewCriterion(Minimize,"Diff Feedbacks",0.2,0.05,1.0);
	CritSocial=NewCriterion(Minimize,"Social",0.2,0.05,1.0);
	Sols=new RPromSol*[PopSize+1];
	if(Sols)
	{
		for(i=PopSize+2,ptr=Sols;--i;ptr++)
			(*ptr)=NewSol();
	}
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* GALILEI::GInstIR::CreateHeuristic(void) throw(bad_alloc)
{
	return(new GIRHeuristic(Random,Objs));
}


//-----------------------------------------------------------------------------
bool GALILEI::GInstIR::StopCondition(void)
{
	return((Gen==MaxGen)/*||(AgeBest==30)*/);
}


#ifdef RGADEBUG
//-----------------------------------------------------------------------------
void GALILEI::GInstIR::WriteChromoInfo(GChromoIR* c)
{
	char Tmp[300];
	double Precision,Recall,Total;
//	RPromSol* s;

	if(!Debug) return;
	Precision=0.0;
	Recall=0.0;
	Total=0.0;
	if(IdealGroups)
	{
		c->CompareIdeal(Session,IdealGroups);
		Precision=c->Precision;
		Recall=c->Recall;
		Total=c->Global;
	}
//	if(c->Id==PopSize) s=Sols[0]; else s=Sols[c->Id+1];
//	sprintf(Tmp,"Id %2u (Fi=%f,Fi+=%f,Fi-=%f): Sim=%1.3f - Nb=%1.3f - OK=%1.3f - Diff=%1.3f - Social=%1.3f  ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
//	        c->Id,s->GetFi(),s->GetFiPlus(),s->GetFiMinus(),c->AvgSim,c->AvgProf,c->OKFactor,c->DiffFactor,c->SocialFactor,Recall,Precision,Total);
	sprintf(Tmp,"Id %2u: Sim=%1.3f - Info=%1.3f - SameFdbk=%1.3f - DiffFdbk=%1.3f - Social=%1.3f  ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
	        c->Id,c->CritSim,c->CritInfo,c->CritSameFeedbacks,c->CritDiffFeedbacks,c->CritSocial,Recall,Precision,Total);
//	file<<c->Id<<c->AvgSim<<c->AvgProf<<c->OKFactor<<c->DiffFactor<<c->SocialFactor<<Total<<endl;
	Debug->PrintInfo(Tmp);
}
#endif


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::PostEvaluate(void) throw(eGA)
{
	unsigned int i;
	GChromoIR** C;
 	GChromoIR* s;
	RPromSol** Res;
	RPromSol** ptr;
	double r;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("PostEvaluate","GInstIR");
	#endif
	ptr=Sols;
	Assign((*ptr),CritSim,BestChromosome->CritSim);
	Assign((*ptr),CritInfo,BestChromosome->CritInfo);
	Assign((*ptr),CritSameFeedbacks,BestChromosome->CritSameFeedbacks);
	Assign((*ptr),CritDiffFeedbacks,BestChromosome->CritDiffFeedbacks);
	Assign((*ptr),CritSocial,BestChromosome->CritSocial);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		Assign((*ptr),CritSim,(*C)->CritSim);
		Assign((*ptr),CritInfo,(*C)->CritInfo);
		Assign((*ptr),CritSameFeedbacks,(*C)->CritSameFeedbacks);
		Assign((*ptr),CritDiffFeedbacks,(*C)->CritDiffFeedbacks);
		Assign((*ptr),CritSocial,(*C)->CritSocial);
	}
	ComputePrometheeII();
	Res=GetSols();
	ptr=Res;

	// Look if the best chromosome ever is still the best -> If not, change
	// the fitness of the best solution.
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=Gen+1.1;
	}
	else
	{
		s=BestChromosome;
		if(s->Fitness->Value==0.0)
			(*s->Fitness)=1.1;
	}
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	#ifdef RGADEBUG
		WriteChromoInfo(s);
	#endif
	ptr++;

	//  The second best has the fitness of 1
	if((*ptr)->GetId())
		s=Chromosomes[(*ptr)->GetId()-1];
	else
		s=BestChromosome;
	(*s->Fitness)=1.0;
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	#ifdef RGADEBUG
		WriteChromoInfo(s);
	#endif

	// Look for the rest
	for(i=PopSize,ptr++;--i;ptr++)
	{
		r=((double)i)/((double)(PopSize));
		if((*ptr)->GetId())
			s=Chromosomes[(*ptr)->GetId()-1];
		else
			s=BestChromosome;
		s->FiPlus=(*ptr)->GetFiPlus();
		s->FiMinus=(*ptr)->GetFiMinus();
		s->Fi=(*ptr)->GetFi();
		(*s->Fitness)=r;
		#ifdef RGADEBUG
			WriteChromoInfo(s);
		#endif
	}

	// Delete the resulting array
	delete[] Res;

	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("PostEvaluate","GInstIR");
	#endif
}


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::SetCriterionParam(const char* crit,double p,double q,double w)
{
	if(!strcmp(crit,"Similarity"))
	{
		CritSim->SetP(p);
		CritSim->SetQ(q);
		CritSim->SetWeight(w);
	}
	else if(!strcmp(crit,"Information"))
	{
		CritInfo->SetP(p);
		CritInfo->SetQ(q);
		CritInfo->SetWeight(w);
	}
	else if(!strcmp(crit,"Same Feedbacks"))
	{
		CritSameFeedbacks->SetP(p);
		CritSameFeedbacks->SetQ(q);
		CritSameFeedbacks->SetWeight(w);
	}
	else if(!strcmp(crit,"Diff Feedbacks"))
	{
		CritDiffFeedbacks->SetP(p);
		CritDiffFeedbacks->SetQ(q);
		CritDiffFeedbacks->SetWeight(w);
	}
	else if(!strcmp(crit,"Social"))
	{
		CritSocial->SetP(p);
		CritSocial->SetQ(q);
		CritSocial->SetWeight(w);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::SetMaxGen(unsigned int max)
{
	MaxGen=max;
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetRatioDiff(GSubProfile* sub1,GSubProfile* sub2) const
{
	GSubProfilesSameDocs Test(sub1->GetId(),sub2->GetId(),0.0);
	GSubProfilesSameDocs* ptr;

	ptr=SameFeedbacks.GetPtr<const GSubProfilesSameDocs&>(Test);
	if(ptr)
		return(ptr->GetRatio());
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetRatioSame(GSubProfile* sub1,GSubProfile* sub2) const
{
	GSubProfilesSameDocs Test(sub1->GetId(),sub2->GetId(),0.0);
	GSubProfilesSameDocs* ptr;

	ptr=DiffFeedbacks.GetPtr<const GSubProfilesSameDocs&>(Test);
	if(ptr)
		return(ptr->GetRatio());
	return(0.0);
}


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::SetMinRatios(double same,double diff)
{
	MinCommonSame=same;
	MinCommonDiff=diff;
}


//-----------------------------------------------------------------------------
GALILEI::GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
