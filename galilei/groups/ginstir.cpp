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
	  tmpObjs1(0),tmpObjs2(0), Tests(0)
{
	Tests=new GChromoIR*[5];
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
		(static_cast<RGGA::RGroups<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>*>(Tests[i]))->Init(&data);
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
	if(tmpObjs1) delete[] tmpObjs1;
	if(tmpObjs2) delete[] tmpObjs2;
}



//-----------------------------------------------------------------------------
//
// GInstIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,double m,unsigned int max,unsigned int popsize,GGroups* grps,RGA::RObjs<GObjIR>* objs,bool g,GProfilesSim* s,SimType st,GIRParams* p,RGA::RDebug *debug) throw(bad_alloc)
GALILEI::GInstIR::GInstIR(GSession* ses,GLang* l,GGroups* grps,RGA::RObjs<GObjIR>* objs,GProfilesSim* s,GIRParams* p,RGA::RDebug *debug) throw(bad_alloc)
	: RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>(p->PopSize,objs,FirstFit,debug),
	  RPromKernel("GALILEI",PopSize+1,5), Sims(s), SameFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1),
	  DiffFeedbacks(objs->NbPtr/8+1,objs->NbPtr/16+1), Params(p), 
	  CritSim(0), CritSimAvgSim(0), CritSimJ(0), CritSimAvgRatio(0),CritSimMinRatio(0),CritSimRatio(0),
	  CritSimWOverB(0), CritSimSimWB(0), CritInfo(0), CritSameFeedbacks(0), CritDiffFeedbacks(0), CritSocial(0),
	  Sols(0),CurrentGroups(grps), Session(ses), Lang(l), NoSocialSubProfiles(objs->NbPtr), BestSols(p->MaxGen,p->MaxGen/2)
{
	RPromSol** ptr;
	RCursor<GObjIR,unsigned int> Cur1;
	RCursor<GObjIR,unsigned int> Cur2;
	GSubProfilesSameDocs* sdocs;
	unsigned int i,j;
	double nb,tmp;
//	RIO::RTextFile same("/home/pfrancq/same.txt",RIO::RTextFile::Create);
//	RIO::RTextFile diff("/home/pfrancq/diff.txt",RIO::RTextFile::Create);
//	same.SetSeparator(" ");
//	diff.SetSeparator(" ");

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
			{
				SameFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetId(),Cur2()->GetId(),nb/tmp));
//				same<<"Same:";
//				same<<"\tRatio="<<sdocs->GetRatio();
//				same<<"\tProfile"<<Cur1()->GetId()<<"'"<<Cur1()->GetSubProfile()->GetProfile()->GetName()<<"'";
//				same<<"\tProfile"<<Cur2()->GetId()<<"'"<<Cur2()->GetSubProfile()->GetProfile()->GetName()<<"'"<<endl;
			}
			nb=Cur1()->GetSubProfile()->GetCommonDiffDocs(Cur2()->GetSubProfile());
			if(nb)
			{
				DiffFeedbacks.InsertPtr(sdocs=new GSubProfilesSameDocs(Cur1()->GetId(),Cur2()->GetId(),nb/tmp));
//				diff<<"Diff:";
//				diff<<"\tRatio="<<sdocs->GetRatio();
//				diff<<"\tProfile"<<Cur1()->GetId()<<"'"<<Cur1()->GetSubProfile()->GetProfile()->GetName()<<"'";
//				diff<<"\tProfile"<<Cur2()->GetId()<<"'"<<Cur2()->GetSubProfile()->GetProfile()->GetName()<<"'"<<endl;
			}
		}
	}

	// Init Criterion and Solutions of the PROMETHEE part
	if(Params->SimMeasures==sctCorl)
		CritSim=NewCriterion(Maximize,"Similarity",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
	else
	{
		if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
			CritSimAvgSim=NewCriterion(Maximize,"Similarity AvgSim",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("J")->Use)
			CritSimJ=NewCriterion(Maximize,"Similarity J",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
			CritSimAvgRatio=NewCriterion(Maximize,"Similarity AvgRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
			CritSimMinRatio=NewCriterion(Maximize,"Similarity MinRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
			CritSimRatio=NewCriterion(Maximize,"Similarity Ratio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
			CritSimWOverB=NewCriterion(Maximize,"Similarity WoverB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
			CritSimSimWB=NewCriterion(Maximize,"Similarity SimWB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
	}
	CritInfo=NewCriterion(Maximize,"Information",Params->ParamsInfo.P,Params->ParamsInfo.Q,Params->ParamsInfo.Weight);
	CritSameFeedbacks=NewCriterion(Maximize,"Same Feedbacks",Params->ParamsSameFeedbacks.P,Params->ParamsSameFeedbacks.Q,Params->ParamsSameFeedbacks.Weight);
	CritDiffFeedbacks=NewCriterion(Minimize,"Diff Feedbacks",Params->ParamsDiffFeedbacks.P,Params->ParamsDiffFeedbacks.Q,Params->ParamsDiffFeedbacks.Weight);
	CritSocial=NewCriterion(Minimize,"Social",Params->ParamsSocial.P,Params->ParamsSocial.Q,Params->ParamsSocial.Weight);
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
	return((Gen==Params->MaxGen)/*||(AgeBest==30)*/);
}


#ifdef RGADEBUG
//-----------------------------------------------------------------------------
void GALILEI::GInstIR::WriteChromoInfo(GChromoIR* c)
{
	char Tmp[300];
	char Tmp2[300];
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
	if(Params->SimMeasures==sctCorl)
	{
		sprintf(Tmp,"Id %2u (Fi=%f,Fi+=%f,Fi-=%f): Sim=%1.3f - Info=%1.3f - SameFdbk=%1.3f - DiffFdbk=%1.3f - Social=%1.3f  ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
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
				}
				strcat(Tmp,Tmp2);
			}
		}
		sprintf(Tmp2,"Info=%1.3f - SameFdbk=%1.3f - DiffFdbk=%1.3f - Social=%1.3f  ***  Recall=%1.3f - Precision=%1.3f - Global=%1.3f",
		        c->CritInfo,c->CritSameFeedbacks,c->CritDiffFeedbacks,c->CritSocial,Recall,Precision,Total);
		strcat(Tmp,Tmp2);
	}
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
	GChromoIR* b;
	RPromSol** Res;
	RPromSol** ptr;
	double r;
	GGroupDataIR GrpData;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("PostEvaluate","GInstIR");
	#endif
	ptr=Sols;
	if(Params->SimMeasures==sctCorl)
		Assign((*ptr),CritSim,BestChromosome->CritSim);
	else
	{
		if(CritSimAvgSim)
			Assign((*ptr),CritSimAvgSim,BestChromosome->CritSimAvgSim);
		if(CritSimJ)
			Assign((*ptr),CritSimJ,BestChromosome->CritSimJ);
		if(CritSimAvgRatio)
			Assign((*ptr),CritSimAvgRatio,BestChromosome->CritSimAvgRatio);
		if(CritSimMinRatio)
			Assign((*ptr),CritSimMinRatio,BestChromosome->CritSimMinRatio);
		if(CritSimRatio)
			Assign((*ptr),CritSimRatio,BestChromosome->CritSimRatio);
		if(CritSimWOverB)
			Assign((*ptr),CritSimWOverB,BestChromosome->CritSimWOverB);
		if(CritSimSimWB)
			Assign((*ptr),CritSimSimWB,BestChromosome->CritSimSimWB);
	}
	Assign((*ptr),CritInfo,BestChromosome->CritInfo);
	Assign((*ptr),CritSameFeedbacks,BestChromosome->CritSameFeedbacks);
	Assign((*ptr),CritDiffFeedbacks,BestChromosome->CritDiffFeedbacks);
	Assign((*ptr),CritSocial,BestChromosome->CritSocial);
	for(i=PopSize+1,C=Chromosomes,ptr++;--i;C++,ptr++)
	{
		if(Params->SimMeasures==sctCorl)
			Assign((*ptr),CritSim,(*C)->CritSim);
		else
		{
			if(CritSimAvgSim)
				Assign((*ptr),CritSimAvgSim,(*C)->CritSimAvgSim);
			if(CritSimJ)
				Assign((*ptr),CritSimJ,(*C)->CritSimJ);
			if(CritSimAvgRatio)
				Assign((*ptr),CritSimAvgRatio,(*C)->CritSimAvgRatio);
			if(CritSimMinRatio)
				Assign((*ptr),CritSimMinRatio,(*C)->CritSimMinRatio);
			if(CritSimRatio)
				Assign((*ptr),CritSimRatio,(*C)->CritSimRatio);
			if(CritSimWOverB)
				Assign((*ptr),CritSimWOverB,(*C)->CritSimWOverB);
			if(CritSimSimWB)
				Assign((*ptr),CritSimSimWB,(*C)->CritSimSimWB);
		}
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
		if(Gen)
		{
			BestSols.InsertPtr(b=new GChromoIR(this,BestSols.NbPtr));
			b->Init(thDatas[0]);
			(static_cast<RGroups<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>*>(b))->Init(&GrpData);
			(*b)=(*s);
		}
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
void GALILEI::GInstIR::PostRun(void)
{
//	RPromKernel Prom("GALILEI",BestSols.NbPtr,5);
//	RPromSol* s;
//
//	// Init Criterion and Solutions of the PROMETHEE part
//	if(Params->SimMeasures==sctCorl)
//		CritSim=Prom.NewCriterion(Maximize,"Similarity",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//	else
//	{
//		if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
//			CritSimAvgSim=Prom.NewCriterion(Maximize,"Similarity AvgSim",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("J")->Use)
//			CritSimJ=Prom.NewCriterion(Maximize,"Similarity J",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
//			CritSimAvgRatio=Prom.NewCriterion(Maximize,"Similarity AvgRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
//			CritSimMinRatio=Prom.NewCriterion(Maximize,"Similarity MinRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
//			CritSimRatio=Prom.NewCriterion(Maximize,"Similarity Ratio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
//			CritSimWOverB=Prom.NewCriterion(Maximize,"Similarity WoverB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//		if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
//			CritSimSimWB=Prom.NewCriterion(Maximize,"Similarity SimWB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
//	}
//	CritInfo=Prom.NewCriterion(Maximize,"Information",Params->ParamsInfo.P,Params->ParamsInfo.Q,Params->ParamsInfo.Weight);
//	CritSameFeedbacks=Prom.NewCriterion(Maximize,"Same Feedbacks",Params->ParamsSameFeedbacks.P,Params->ParamsSameFeedbacks.Q,Params->ParamsSameFeedbacks.Weight);
//	CritDiffFeedbacks=Prom.NewCriterion(Minimize,"Diff Feedbacks",Params->ParamsDiffFeedbacks.P,Params->ParamsDiffFeedbacks.Q,Params->ParamsDiffFeedbacks.Weight);
//	CritSocial=Prom.NewCriterion(Minimize,"Social",Params->ParamsSocial.P,Params->ParamsSocial.Q,Params->ParamsSocial.Weight);
//	for(BestSols.Start();!BestSols.End();BestSols.Next())
//	{
//		s=Prom.NewSol();
//		if(Params->SimMeasures==sctCorl)
//			Prom.Assign(s,CritSim,BestSols()->CritSim);
//		else
//		{
//			if(CritSimAvgSim)
//				Prom.Assign(s,CritSimAvgSim,BestSols()->CritSimAvgSim);
//			if(CritSimJ)
//				Prom.Assign(s,CritSimJ,BestSols()->CritSimJ);
//			if(CritSimAvgRatio)
//				Prom.Assign(s,CritSimAvgRatio,BestSols()->CritSimAvgRatio);
//			if(CritSimMinRatio)
//				Prom.Assign(s,CritSimMinRatio,BestSols()->CritSimMinRatio);
//			if(CritSimRatio)
//				Prom.Assign(s,CritSimRatio,BestSols()->CritSimRatio);
//			if(CritSimWOverB)
//				Prom.Assign(s,CritSimWOverB,BestSols()->CritSimWOverB);
//			if(CritSimSimWB)
//				Prom.Assign(s,CritSimSimWB,BestSols()->CritSimSimWB);
//		}
//		Prom.Assign(s,CritInfo,BestSols()->CritInfo);
//		Prom.Assign(s,CritSameFeedbacks,BestSols()->CritSameFeedbacks);
//		Prom.Assign(s,CritDiffFeedbacks,BestSols()->CritDiffFeedbacks);
//		Prom.Assign(s,CritSocial,BestSols()->CritSocial);
//	}
//	Prom.ComputePrometheeII();
//	(*BestChromosome)=(*BestSols.Tab[Prom.GetBestSol()->GetId()]);
//	emitBestSig();
}


//-----------------------------------------------------------------------------
GALILEI::GInstIR::~GInstIR(void)
{
	if(Sols)
		delete[] Sols;
}
