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
#include <rgga/rgroupingheuristic.h>
using namespace R;


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
	  tmpObjs1(0),tmpObjs2(0), Tests(0), Prom(Owner->Params), Sols(0)
{
	RPromSol** s;
	unsigned int i;

	Tests=new GChromoIR*[5];
	Sols=new RPromSol*[6];
	for(i=7,s=Sols;--i;s++)
		(*s)=Prom.NewSol();
}


//-----------------------------------------------------------------------------
void GALILEI::GThreadDataIR::Init(void) throw(bad_alloc)
{
	GGroupDataIR data;

	RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init();
	tmpObjs1=new GObjIR*[Owner->Objs->GetNb()];
	tmpObjs2=new GObjIR*[Owner->Objs->GetNb()];
	for(int i=0;i<5;i++)
	{
		Tests[i]=new GChromoIR(Owner,Owner->PopSize+1+i);
		Tests[i]->Init(this);
		(static_cast<RGroups<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>*>(Tests[i]))->Init(&data);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GThreadDataIR::~GThreadDataIR(void)
{
	GChromoIR** C;
	unsigned int i;

	if(Tests)
	{
		for(i=5+1,C=Tests;--i;C++)
			if(*C) delete (*C);
		delete[] Tests;
	}
	if(Sols)
		delete[] Sols;
	if(tmpObjs1) delete[] tmpObjs1;
	if(tmpObjs2) delete[] tmpObjs2;
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,GGroups* grps,RObjs<GObjIR>* objs,GIRParams* p,RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(p->PopSize,objs,FirstFit,debug),
	  GIRProm(p), SameFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  DiffFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1), Params(p), 
	  Sols(0),CurrentGroups(grps), Session(ses), Lang(l), NoSocialSubProfiles(objs->NbPtr)
#if BESTSOLSVERIFICATION
	  , BestSols(p->MaxGen,p->MaxGen/2)
#endif
{
	RPromSol** ptr;
	RCursor<GObjIR,unsigned int> Cur1;
	RCursor<GObjIR,unsigned int> Cur2;
	GSubProfilesSameDocs* sdocs;
	unsigned int i,j;
	double nb,tmp;

	#if GALILEITEST
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
			if((nb>0.0)&&(tmp>10.0))
			{
				SameFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetSubProfile()->GetId(),Cur2()->GetSubProfile()->GetId(),nb/tmp));
			}
			nb=Cur1()->GetSubProfile()->GetCommonDiffDocs(Cur2()->GetSubProfile());
			if((nb>0.0)&&(tmp>10.0))
			{
				DiffFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetSubProfile()->GetId(),Cur2()->GetSubProfile()->GetId(),nb/tmp));
			}
		}
	}

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
GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,GGroups* grps,RObjs<GObjIR>* objs,GIRParams* p,unsigned int pop,RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(pop,objs,FirstFit,debug),
	  GIRProm(p), SameFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  DiffFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1), Params(p),
	  Sols(0),CurrentGroups(grps), Session(ses), Lang(l), NoSocialSubProfiles(objs->NbPtr)
#if BESTSOLSVERIFICATION
	  , BestSols(p->MaxGen,p->MaxGen/2)
#endif
{
	RPromSol** ptr;
	RCursor<GObjIR,unsigned int> Cur1;
	RCursor<GObjIR,unsigned int> Cur2;
	GSubProfilesSameDocs* sdocs;
	unsigned int i,j;
	double nb,tmp;

	#if GALILEITEST
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
			if((nb>0.0)&&(tmp>5.0))
			{
				SameFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetSubProfile()->GetId(),Cur2()->GetSubProfile()->GetId(),nb/tmp));
			}
			nb=Cur1()->GetSubProfile()->GetCommonDiffDocs(Cur2()->GetSubProfile());
			if((nb>0.0)&&(tmp>5.0))
			{
				DiffFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetSubProfile()->GetId(),Cur2()->GetSubProfile()->GetId(),nb/tmp));
			}
		}
	}

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
RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* GALILEI::GInstIR::CreateHeuristic(void) throw(bad_alloc)
{
	return(new GIRHeuristic(Random,Objs,Debug));
}


//-----------------------------------------------------------------------------
GObjIR* GALILEI::GInstIR::GetObj(const GSubProfile* sub) const
{
	GObjIRCursor Cur;

	Cur=(*Objs);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetSubProfile()==sub)
			return(Cur());
	return(0);
}


//-----------------------------------------------------------------------------
bool GALILEI::GInstIR::StopCondition(void)
{
	return(Gen==Params->MaxGen);
}


#if GALILEITEST
//-----------------------------------------------------------------------------
void GALILEI::GInstIR::WriteChromoInfo(GChromoIR* c)
{
	char Tmp[300];
	char Tmp2[300];
	double Precision,Recall,Total;

	if(!Debug) return;
	Precision=Recall=Total=0;
	if(IdealGroups)
	{
		c->CompareIdeal(Session,IdealGroups);
		Precision=c->Precision;
		Recall=c->Recall;
		Total=c->Global;
	}
	if(Params->SimMeasures==sctCorl)
	{
		sprintf(Tmp,"Id %2u (Fi=%f,Fi+=%f,Fi-=%f): Sim=%1.3f - Info=%1.3f - SameFdbk=%1.3f - DiffFdbk=%1.3f - Social=%1.3f ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
	        c->Id,c->Fi,c->FiPlus,c->FiMinus,c->CritSim,c->CritInfo,c->CritSameFeedbacks,c->CritDiffFeedbacks,c->CritSocial,Recall,Precision,Total);
	}
	else
	{
		sprintf(Tmp,"Id %2u (Fi=%f,Fi+=%f,Fi-=%f): ",c->Id,c->Fi,c->FiPlus,c->FiMinus);
		for(Params->Measures.Start();!Params->Measures.End();Params->Measures.Next())
		{
			GSimMeasure* s=Params->Measures();
			if(s->Use)
			{
				sprintf(Tmp2,"%s=",s->Name());
				strcat(Tmp,Tmp2);
				switch(s->Type)
				{
					case stAvgSim:
						sprintf(Tmp2,"%1.3f - ",c->CritSimAvgSim);
						break;
					case stJ:
						sprintf(Tmp2,"%1.3f - ",c->CritSimJ);
						break;
					case stAvgRatio:
						sprintf(Tmp2,"%1.3f - ",c->CritSimAvgRatio);
						break;
					case stMinRatio:
						sprintf(Tmp2,"%1.3f - ",c->CritSimMinRatio);
						break;
					case stRatio:
						sprintf(Tmp2,"%1.3f - ",c->CritSimRatio);
						break;
					case stWOverB:
						sprintf(Tmp2,"%1.3f - ",c->CritSimWOverB);
						break;
					case stSimWB:
						sprintf(Tmp2,"%1.3f - ",c->CritSimSimWB);
						break;
					case stNone:
					default:
						strcpy(Tmp2,"");
						break;
				}
				strcat(Tmp,Tmp2);
			}
		}
		sprintf(Tmp2,"Info=%1.3f - SameFdbk=%1.3f - DiffFdbk=%1.3f - Social=%1.3f ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
		        c->CritInfo,c->CritSameFeedbacks,c->CritDiffFeedbacks,c->CritSocial,Recall,Precision,Total);
		strcat(Tmp,Tmp2);
	}
	Debug->PrintInfo(Tmp);
}
#endif


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::PostEvaluate(void) throw(eGA)
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

	#ifdef RGADEBUG
		if(Debug)
			Debug->BeginFunc("PostEvaluate","GInstIR");
	#endif
	ptr=Sols;
	Assign(*ptr,BestChromosome);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		Assign(*ptr,*C);
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
		if(s->Fitness->Value==0.0)
			(*s->Fitness)=1.1;
	}
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	#ifdef RGADEBUG
		if(Debug)
			WriteChromoInfo(s);
	#endif
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
	#ifdef RGADEBUG
		if(Debug)
			WriteChromoInfo(s);
	#endif

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
		#ifdef RGADEBUG
			if(Debug)
				WriteChromoInfo(s);
		#endif
	}

	// Delete the resulting array
	delete[] Res;

	#ifdef RGADEBUG
		if(Debug)
			Debug->EndFunc("PostEvaluate","GInstIR");
	#endif
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetRatioDiff(GSubProfile* sub1,GSubProfile* sub2) const
{
	GSubProfilesSameDocs Test(sub1->GetId(),sub2->GetId(),0.0);
	GSubProfilesSameDocs* ptr;

	ptr=DiffFeedbacks.GetPtr<const GSubProfilesSameDocs&>(Test);
	if(ptr)
		return(ptr->GetRatio());
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetRatioSame(GSubProfile* sub1,GSubProfile* sub2) const
{
	GSubProfilesSameDocs Test(sub1->GetId(),sub2->GetId(),0.0);
	GSubProfilesSameDocs* ptr;

	ptr=SameFeedbacks.GetPtr<const GSubProfilesSameDocs&>(Test);
	if(ptr)
		return(ptr->GetRatio());
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetSim(GSubProfile* sub1,GSubProfile* sub2)
{
	return(Session->GetSimProf(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GALILEI::GInstIR::GetSim(GObjIR* obj1,GObjIR* obj2)
{
	return(Session->GetSimProf(obj1->GetSubProfile(),obj2->GetSubProfile()));
}


//-----------------------------------------------------------------------------
void GALILEI::GInstIR::PostRun(void)
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
GALILEI::GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
