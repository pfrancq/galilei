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
//#include <rio/rtextfile.h>
//using namespace RIO;
//RTextFile file("/home/pfrancq/prom.txt",RTextFile::Create);
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
// GSubProfilesSameGroupIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool GSubProfilesSameGroupIR::IsIn(const GObjIR* obj) const
{
	return((obj->GetId()==Id1)||(obj->GetId()==Id2));
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,double m,unsigned int max,unsigned int popsize,GGroups* grps,RGA::RObjs<GObjIR>* objs,bool g,GProfilesSim* s,SimType st,RGA::RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(popsize,objs,FirstFit,debug),
	  RPromKernel("GALILEI",PopSize+1,5), Sims(s), SameGroups(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  DiffGroups(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  MinSimLevel(m), MaxGen(max), SimMeasure(st), CritSim(0), CritNb(0), CritOKDocs(0), Sols(0), GlobalSim(g),
	  CurrentGroups(grps), Session(ses), Lang(l), NoSocialSubProfiles(objs->NbPtr)
{
	RPromSol** ptr;
	RCursor<GObjIR,unsigned int> Cur1;
	RCursor<GObjIR,unsigned int> Cur2;
	unsigned int i,j,nb;

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
			nb=Cur1()->GetSubProfile()->GetCommonOKDocs(Cur2()->GetSubProfile());
//			if(nb)
//			{
//				double tmp=(2*nb*100)/((double)(Cur1()->GetSubProfile()->GetNbJudgedDocs()+Cur2()->GetSubProfile()->GetNbJudgedDocs()));
//				cout<<"% Common OK: "<<tmp<<endl;
//			}
			if(nb)
				SameGroups.InsertPtr(new GSubProfilesSameGroupIR(Cur1()->GetId(),Cur2()->GetId(),nb));
			nb=Cur1()->GetSubProfile()->GetCommonDiffDocs(Cur2()->GetSubProfile());
			if(nb)
				DiffGroups.InsertPtr(new GSubProfilesSameGroupIR(Cur1()->GetId(),Cur2()->GetId(),nb));
		}
	}

	// Init Criterion and Solutions of the PROMETHEE part
	CritSim=NewCriterion(Maximize,"Similarities",0.2,0.05,1.0);
	CritNb=NewCriterion(Maximize,"Profiles",0.2,0.05,1.0);
	CritOKDocs=NewCriterion(Maximize,"Common OK Documents",0.2,0.05,1.0);
	CritDiffDocs=NewCriterion(Minimize,"Common Documents with different Judgment",0.2,0.05,1.0);
	CritSocial=NewCriterion(Minimize,"Social Factor",0.2,0.05,1.0);
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
	sprintf(Tmp,"Id %2u: Sim=%1.3f - Nb=%1.3f - OK=%1.3f - Diff=%1.3f - Social=%1.3f  ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
	        c->Id,c->AvgSim,c->AvgProf,c->OKFactor,c->DiffFactor,c->SocialFactor,Recall,Precision,Total);
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
	Assign((*ptr),CritSim,BestChromosome->AvgSim);
	Assign((*ptr),CritNb,BestChromosome->AvgProf);
	Assign((*ptr),CritOKDocs,BestChromosome->OKFactor);
	Assign((*ptr),CritDiffDocs,BestChromosome->DiffFactor);
	Assign((*ptr),CritSocial,BestChromosome->SocialFactor);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		Assign((*ptr),CritSim,(*C)->AvgSim);
		Assign((*ptr),CritNb,(*C)->AvgProf);
		Assign((*ptr),CritOKDocs,(*C)->OKFactor);
		Assign((*ptr),CritDiffDocs,(*C)->DiffFactor);
		Assign((*ptr),CritSocial,(*C)->SocialFactor);
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
	}
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
	else if(!strcmp(crit,"Nb Profiles"))
	{
		CritNb->SetP(p);
		CritNb->SetQ(q);
		CritNb->SetWeight(w);
	}
	else if(!strcmp(crit,"OK Factor"))
	{
		CritOKDocs->SetP(p);
		CritOKDocs->SetQ(q);
		CritOKDocs->SetWeight(w);
	}
	else if(!strcmp(crit,"Diff Factor"))
	{
		CritDiffDocs->SetP(p);
		CritDiffDocs->SetQ(q);
		CritDiffDocs->SetWeight(w);
	}
	else if(!strcmp(crit,"Social Factor"))
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
GALILEI::GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
