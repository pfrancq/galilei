/*

	GALILEI Research Project

	GChromoIR.cpp

	Chromosome for an IR Problem - Implementation

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromkernel.h>
#include <rpromethee/rpromsol.h>
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/ggroups.h>
#include <groups/ggroupvector.h>
#include <groups/gobjir.h>
#include <groups/gcomparegrouping.h>
#include <sessions/gsession.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofilessamedocs.h>
#include <docs/gdocsim.h>
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
	  CritSim(0.0), CritSimAvgSim(0.0), CritSimJ(0.0), CritSimAvgRatio(0.0),
	  CritSimMinRatio(0.0), CritSimRatio(0.0), CritSimWOverB(0.0), CritSimSimWB(0.0),
	  CritInfo(0.0), CritEntropy(0.0), CritLikelihood(0.0), CritSameFeedbacks(0.0),
	  CritDiffFeedbacks(1.0), CritSocial(1.0), Protos(Used.MaxPtr),
	  thProm(0), thSols(0), Docs(100,50)
{
	#if GALILEITEST
		Recall=Precision=Global=0.0;
	#endif
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Init(GThreadDataIR* thData) throw(bad_alloc)
{
	// Parent Initialisation
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(thData);

	// Current
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;
	thTests=thData->Tests;
	thProm=&thData->Prom;
	thSols=thData->Sols;
}


//-----------------------------------------------------------------------------
int GALILEI::GChromoIR::Compare(const GChromoIR* c) const
{
	return(Id-c->Id);
}


#if GALILEITEST
//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::CompareIdeal(GSession* s,RStd::RContainer<GGroups,unsigned int,true,true>* ideal
)
{
	GGroupIR* gr;
	GGroup* g;
	GObjIR** ptr;
	unsigned int i;
	GGroups* Cur;

	// Make the current chromosome the current groupement
	s->ClearGroups(Instance->Lang);
	Cur=s->GetGroups(Instance->Lang);
	for(Used.Start();!Used.End();Used.Next())
	{
		gr=Used();
		g=new GGroupVector(cNoRef,Instance->Lang);
		s->NewGroup(Instance->Lang,g);
		Cur->InsertPtr(g);
		for(i=gr->NbSubObjects+1,ptr=GetObjs(gr->SubObjects);--i;ptr++)
			g->InsertSubProfile((*ptr)->GetSubProfile());
	}

	// Make the comparison with the ideal group
	GCompareGrouping Comp(s,ideal);
	Comp.Compare(0);
	Precision=Comp.GetPrecision();
	Recall=Comp.GetRecall();
	Global=Comp.GetTotal();
}
#endif


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeSumSim(GObjIR* obj)
{
	double SumSim;
	GObjIRCursor Cur;

	Cur=(*Objs);
	for(Cur.Start(),SumSim=0.0;!Cur.End();Cur.Next())
	{
		if(Cur()==obj) continue;
		SumSim+=Instance->GetSim(obj->GetSubProfile(),Cur()->GetSubProfile());
	}
	return(SumSim);
}


//-----------------------------------------------------------------------------
GObjIR* GALILEI::GChromoIR::ComputeGlobalRelevant(void)
{
	double BestSumSim;
	double SumSim;
	GObjIRCursor Cur;
	GObjIR*  rel;

	Cur=(*Objs);
	Cur.Start();

	// Suppose the first element is the most relevant.
	rel=Cur();
	BestSumSim=ComputeSumSim(Cur());

	// Look if in the other objects, there is a better one
	for(Cur.Next();!Cur.End();Cur.Next())
	{
		SumSim=ComputeSumSim(Cur());
		if(SumSim>BestSumSim)
		{
			rel=Cur();
			BestSumSim=SumSim;
		}
	}
	return(rel);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::ConstructChromo(GGroups* grps) throw(RGA::eGA)
{
	GGroupCursor Grp;
	GSubProfileCursor SubProfile;
	GGroupIR* grp;
	GObjIR** objs;
	unsigned int i;

	Grp=grps->GetGroupCursor();
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		// Reserve a GA group
		grp=ReserveGroup();

		// Go through the current profiles and store them in thObjs1
		SubProfile=Grp()->GetSubProfileCursor();
		for(SubProfile.Start(),objs=thObjs1,NbObjs1=0;!SubProfile.End();SubProfile.Next(),objs++,NbObjs1++)
			(*objs)=Instance->GetObj(SubProfile());

		// Mix randomly thObjs1
		Instance->RandOrder<GObjIR*>(thObjs1,NbObjs1);

		// Put the objects in the group if possible
		for(objs=thObjs1,i=NbObjs1+1;--i;objs++)
		{
			if(grp->CanInsert(*objs))
				grp->Insert(*objs);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::RandomConstruct(void) throw(RGA::eGA)
{
	// Look if already a solution in the session
	if(Instance->CurrentGroups)
		 ConstructChromo(Instance->CurrentGroups);

	// Call classic heuristic for non-assigned objects
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::RandomConstruct();

	// If first element, do nothing.
	if((!Instance->CurrentGroups)||(!Id))
		return;

	// Do a mutation
	Mutation();
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateAvgSim(void)
{
	GGroupIRCursor Cur;
	unsigned int i;
	GObjIR** ptr;
	double Sim;

	// Computed the average similairies of the configurations
	Cur.Set(Used);
	for(Cur.Start(),Sim=0.0;!Cur.End();Cur.Next())
	{
		if(Cur()->NbSubObjects>1)
		{
			for(i=Cur()->NbSubObjects+1,ptr=GetObjs(Cur()->SubObjects);--i;ptr++)
				Sim+=Cur()->ComputeAvgSim(*ptr);
		}
		else
			Sim+=1.0;
	}
	Sim/=static_cast<double>(Objs->GetNb());
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateJ(void)
{
	unsigned int i,j;
	GGroupIRCursor Cur;
	double min;
	GGroupIRCursor Cur2;
	double tmp;
	double Sim;

	// Compute (Sum Intra) for the configurations.
	Cur.Set(Used);
	for(Cur.Start(),Sim=0.0;!Cur.End();Cur.Next())
		Sim+=Cur()->ComputeRelevantSumDist();

	// Compute Min inter for the configurations.
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),i=0,j=Cur.GetNb(),min=1.0;--j;Cur.Next(),i++)
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=1-Instance->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp<min)
				min=tmp;
		}
	}

	// Compute (Min inter*NbGroups)/(Sum Intra) for the configurations.
	if(Sim)
		Sim=min/Sim*Used.NbPtr;
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateAvgRatio(void)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter,tmp,Sim;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();

	// Average min(intra)/max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),Sim=0.0;!Cur.End();Cur.Next())
	{
		MinIntra=Cur()->ComputeMinSim(Cur()->Relevant);
		for(Cur2.Start(),MaxInter=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Instance->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		Sim+=MinIntra/MaxInter;
	}
	Sim/=static_cast<double>(Used.NbPtr);
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateMinRatio(void)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter,tmp,Sim;
	bool first;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();

	// Min min(intra)/max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),Sim=0.0,first=true;!Cur.End();Cur.Next())
	{
		MinIntra=Cur()->ComputeMinSim(Cur()->Relevant);
		for(Cur2.Start(),MaxInter=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Instance->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=MinIntra/MaxInter;
		if(first)
		{
			Sim=tmp;
			first=false;
		}
		else
			if(tmp<Sim)
				Sim=tmp;
	}
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateRatio(void)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter;
	double tmpMin,tmpMax,tmp,Sim;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();

	// Determine overall min(intra) and overall max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),MaxInter=-1.0,Sim=0.0,MinIntra=1.0;!Cur.End();Cur.Next())
	{
		tmpMin=Cur()->ComputeMinSim(Cur()->Relevant);
		if(tmpMin<MinIntra)
			MinIntra=tmpMin;
		for(Cur2.Start(),tmpMax=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Instance->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		if(tmpMax>MaxInter)
			MaxInter=tmpMax;
	}
	if(MaxInter)
		Sim=MinIntra/MaxInter;
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateWOverB(void)
{
	GGroupIRCursor Cur;
	double sb,localsb;
	GObjIR* rel;
	double tmp,Sim;

	// Compute Sw
	Cur.Set(Used);
	for(Cur.Start(),Sim=0.0;!Cur.End();Cur.Next())
		Sim+=Cur()->ComputeRelevantSumDist();

	// Compute Sb
	rel=ComputeGlobalRelevant();
	Cur.Set(Used);
	for(Cur.Start(),sb=localsb=0.0;!Cur.End();Cur.Next())
	{
		if(rel==Cur()->Relevant) continue;
		tmp=1-Instance->GetSim(rel->GetSubProfile(),Cur()->Relevant->GetSubProfile());
		sb+=tmp*tmp;
	}

	// Compute Sb/Sw for the configurations.
	if(Sim)
		Sim=Used.NbPtr*sb/Sim;
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GChromoIR::EvaluateSimWB(void)
{
	GGroupIRCursor Cur;
	double sb,Sim;
	GObjIR* rel;

	// Compute SimW
	Cur.Set(Used);
	for(Cur.Start(),Sim=0.0;!Cur.End();Cur.Next())
		Sim+=Cur()->ComputeRelevantSum();

	// Compute SimB
	rel=ComputeGlobalRelevant();
	Cur.Set(Used);
	for(Cur.Start(),sb=0.0;!Cur.End();Cur.Next())
	{
		if(rel==Cur()->Relevant) continue;
		sb+=Instance->GetSim(rel->GetSubProfile(),Cur()->Relevant->GetSubProfile());
	}

	// Compute SimW/SimB for the configurations.
	if(sb)
		Sim/=Used.NbPtr*sb;
	else
		Sim=0.0;
	return(Sim);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSim(SimType s)
{
	switch(s)
	{
		case stNone:
			CritSim=1.0;
			break;

		case stAvgSim:
			CritSimAvgSim=EvaluateAvgSim();
			break;

		case stJ:
			CritSimJ=EvaluateJ();
			break;

		case stAvgRatio:
			CritSimAvgRatio=EvaluateAvgRatio();
			break;

		case stMinRatio:
			CritSimMinRatio=EvaluateMinRatio();
			break;

		case stRatio:
			CritSimRatio=EvaluateRatio();
			break;

		case stWOverB:
			CritSimWOverB=EvaluateWOverB();
			break;

		case stSimWB:
			CritSimSimWB=EvaluateSimWB();
			break;

		default:
			CritSim=0.0;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSim(void)
{
	GSimMeasure* s;
	SimCritType t=Instance->Params->SimMeasures;

	// Compute the necessary Measures
	for(Instance->Params->Measures.Start();!Instance->Params->Measures.End();Instance->Params->Measures.Next())
	{
		s=Instance->Params->Measures();
		if(((t==sctCorl)&&(s->Weight!=0.0))||((t==sctCrits)&&(s->Use)))
			EvaluateSim(s->Type);
	}

	// If Correlation, computes the correlation
	if(t!=sctCorl) return;
	for(Instance->Params->Measures.Start(),CritSim=0.0;!Instance->Params->Measures.End();Instance->Params->Measures.Next())
	{
		s=Instance->Params->Measures();
		switch(s->Type)
		{
			case stNone:
				CritSim+=1.0*s->Weight;
				break;

			case stAvgSim:
				CritSim+=CritSimAvgSim*s->Weight;
				break;

			case stJ:
				CritSim+=CritSimJ*s->Weight;
				break;

			case stAvgRatio:
				CritSim+=CritSimAvgRatio*s->Weight;
				break;

			case stMinRatio:
				CritSim+=CritSimMinRatio*s->Weight;
				break;

			case stRatio:
				CritSim+=CritSimRatio*s->Weight;
				break;

			case stWOverB:
				CritSim+=CritSimWOverB*s->Weight;
				break;

			case stSimWB:
				CritSim+=CritSimSimWB*s->Weight;
				break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateInfo(void)
{
	// Compute Average number of profiles.
	CritInfo=static_cast<double>(Used.NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateEntropy(void)
{
	GGroupIRCursor Cur;

	// Computed the entropy of each group
	Cur.Set(Used);
	for(Cur.Start(),CritEntropy=0.0;!Cur.End();Cur.Next())
	{
		CritEntropy+=Cur()->ComputeEntropy();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateLikelihood(void)
{
	GGroupIRCursor Cur;

	// Computed the entropy of each group
	Cur.Set(Used);
	for(Cur.Start(),CritLikelihood=0.0;!Cur.End();Cur.Next())
	{
		CritLikelihood+=Cur()->ComputeLikelihood();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSameFeedbacks(void)
{
	GSubProfilesSameDocs* same;
	unsigned int a1,a2;

	// Number of subprofiles having common OK documents and being in the same group.
	for(Instance->SameFeedbacks.Start(),CritSameFeedbacks=0.0;!Instance->SameFeedbacks.End();Instance->SameFeedbacks.Next())
	{
		same=Instance->SameFeedbacks();
		a1=ObjectsAss[same->GetId1()];
		a2=ObjectsAss[same->GetId2()];
		if(a1!=a2) continue;
		CritSameFeedbacks+=same->GetRatio();
	}
	if(Instance->SameFeedbacks.NbPtr)
	{
		CritSameFeedbacks/=Instance->SameFeedbacks.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateDiffFeedbacks(void)
{
	GSubProfilesSameDocs* diff;
	unsigned int a1,a2;

	// Number of subprofiles having common OK documents and being in the same group.
	for(Instance->DiffFeedbacks.Start(),CritDiffFeedbacks=0.0;!Instance->DiffFeedbacks.End();Instance->DiffFeedbacks.Next())
	{
		diff=Instance->DiffFeedbacks();
		a1=ObjectsAss[diff->GetId1()];
		a2=ObjectsAss[diff->GetId2()];
		if(a1!=a2) continue;
		CritDiffFeedbacks+=diff->GetRatio();
	}
	if(Instance->DiffFeedbacks.NbPtr)
	{
		CritDiffFeedbacks/=Instance->SameFeedbacks.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSocial(void)
{
	GGroupIRCursor Cur;
	unsigned int i;
	bool IsIn;

	// Look for the normal groups
	Cur.Set(Used);
	for(Cur.Start(),CritSocial=0.0;!Cur.End();Cur.Next())
	{
		if(Cur()->NbSubObjects!=1) continue;
		IsIn=Instance->NoSocialSubProfiles.IsIn<const GObjIR*>(GetObj(Cur()->SubObjects));
		if(IsIn) continue;
		CritSocial+=1.0;
	}

	i=Objs->GetNb()-Instance->NoSocialSubProfiles.NbPtr;
	if(i)
	{
		CritSocial/=i;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(void) throw(RGA::eGA)
{
	EvaluateSim();
	if(Instance->Params->ParamsInfo.Weight)
		EvaluateInfo();
	if(Instance->Params->ParamsEntropy.Weight)
		EvaluateEntropy();
	if(Instance->Params->ParamsLikelihood.Weight)
		EvaluateLikelihood();
	if(Instance->Params->ParamsSameFeedbacks.Weight)
		EvaluateSameFeedbacks();
	if(Instance->Params->ParamsDiffFeedbacks.Weight)
		EvaluateDiffFeedbacks();
	if(Instance->Params->ParamsSocial.Weight)
		EvaluateSocial();
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::ReAllocate(void) throw(RGA::eGA)
{
	GGroupIRCursor Grp;
	GObjIR** ptr;
	unsigned int i,nb;
	double sim,maxsim,ratio;
	GSubProfile* sub;
	GGroupIR* grp;
	GObjIR** Cur;
	bool FindAgreement;             // Find group with Agreement constraint respected?
	bool CanIn;                     // Can the object goes in the current group
	bool Agreement;                 // Agreement constraint respected with current group?

	// Put the prototypes in Protos
	Protos.Clear();
	Grp.Set(Used);
	for(Grp.Start();!Grp.End();Grp.Next())
		Protos.InsertPtr(Grp()->Relevant);

	// Clear the chromosome
	Clear();

	// Insert the Prototypes in a group
	for(Protos.Start();!Protos.End();Protos.Next())
	{
		grp=ReserveGroup();
		grp->Insert(Protos());
		grp->Relevant=Protos();
	}

	// Mix randomly thObjs1
	Instance->RandOrder<GObjIR*>(thObjs1,Objs->GetNb());

	// Go through the ranbomly ordered subprofiles and put them in the group of the
	// most similar prototype.
	for(Cur=thObjs1,nb=Objs->GetNb()+1;--nb;Cur++)
	{
		// If the subprofile is a prototype -> already in a group
		if(GetGroup(*Cur)) continue;

		// Find the subprofile
		sub=(*Cur)->GetSubProfile();

		// Go through each groups
		Grp.Set(Used);
		for(Grp.Start(),FindAgreement=false,maxsim=-1.0,grp=0;!Grp.End();Grp.Next())
		{
			// Compute Maximum agreement ratio of the current profile with the profiles of
			// the group.
			ratio=Grp()->GetMaxRatioSame(*Cur);

			// If all the hard constraints are not respected -> skip the group.
			// Rem: Agreement constraint is the more importante one.
			CanIn=Grp()->CanInsert(*Cur);
			Agreement=(ratio>=Instance->Params->MinCommonOK);
			if((!Agreement)&&(!CanIn)) continue;

			// Compute similarity with the relevant profile of the group.
			sim=Grp()->ComputeRelSim(*Cur);

			// Test Agreement constraint.
			if(Agreement)
			{
				// If agreement constraint respected
				if(!FindAgreement)
				{
					FindAgreement=true;
					maxsim=-1.0;
				}

				// Take the group with the highest sim.
				if(sim>maxsim)
				{
					maxsim=sim;
					grp=Grp();
				}
			}
			else
			{
				if((sim>maxsim)&&(!FindAgreement)&&(CanIn))
				{
					maxsim=sim;
					grp=Grp();
				}
			}
		}

		// If no group find -> Create a new group and make the current subprofile the
		// prototype of it.
		if(!grp)
		{
			grp=ReserveGroup();
			grp->Relevant=(*Cur);
		}

		// Insert the subprofile in the current group.
		grp->Insert(*Cur);
	}
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GChromoIR::CalcNewProtosNb(void) throw(RGA::eGA)
{
	unsigned int count;
	GGroupIRCursor Grp;

	// Computed the prototypes for each groups and count the number in Protos
	Grp.Set(Used);
	for(Grp.Start(),count=0;!Grp.End();Grp.Next())
	{
		Grp()->ComputeRelevant();
		if(!Protos.IsIn<const GObjIR*>(Grp()->Relevant))
			count++;
	}
	return(count);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::DoKMeans(void) throw(RGA::eGA)
{
	unsigned int itermax;
	int error;
	unsigned int IterNumber;
	GGroupIRCursor Grp;
	GObjIR** obj;

	#ifdef RGADEBUG
		if(Instance->Debug)
			Instance->Debug->BeginFunc("DoKMeans","GChomoIR");
	#endif


	// Copy the subprofiles into thObjs1
	for(Objs->Start(),obj=thObjs1;!Objs->End();Objs->Next(),obj++)
		(*obj)=(*Objs)();

	// Computed the prototypes for each groups
	Grp.Set(Used);
	for(Grp.Start();!Grp.End();Grp.Next())
		Grp()->ComputeRelevant();

	// Max Iterations
	for(itermax=0,error=1,IterNumber=Instance->Params->MaxKMeans;(itermax<IterNumber)&&(error!=0);itermax++)
	{
		ReAllocate();
		error=CalcNewProtosNb();
	}

	#ifdef RGADEBUG
		if(Instance->Debug)
		{
			char tmp[60];
			sprintf(tmp,"Number of k-Means runs: %u",itermax);
			Instance->Debug->PrintInfo(tmp);
			Instance->Debug->EndFunc("DoKMeans","GChomoIR");
		}
	#endif
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::DivideWorstSubProfiles(void) throw(RGA::eGA)
{
	GGroupIRCursor Grp;
	unsigned int i,j;
	GObjIR** ptr;
	GObjIR** ptr2;
	GObjIR** ptr3;
	double sim,minsim;
	GSubProfile* sub;
	GSubProfile* sub1;
	GSubProfile* sub2;
	GGroupIR* grp;
	GObjIR* worst1;
	GObjIR* worst2;

	// Find the group containing the two most dissimilar subprofiles
	Grp.Set(Used);
	for(Grp.Start(),minsim=1.0,grp=0;!Grp.End();Grp.Next())
	{
		if(Grp()->GetNbObjs()<3) continue;
		for(i=Grp()->GetNbObjs(),ptr=Grp()->GetObjects();--i;ptr++)
		{
			sub=(*ptr)->GetSubProfile();
			for(j=i+1,ptr2=ptr+1;--j;ptr2++)
			{
				sim=Instance->GetSim(sub,(*ptr2)->GetSubProfile());
				if(sim<minsim)
				{
					minsim=sim;
					grp=Grp();
					worst1=(*ptr);
					worst2=(*ptr2);
				}
			}
		}
	}

	// Put in worst1 in thObj1 and worst2 in thObjs2
	if(!grp) return;
	NbObjs1=NbObjs2=1;
	ptr2=thObjs1;
	ptr3=thObjs2;
	(*(ptr2++))=worst1;
	(*(ptr3++))=worst2;

	// Copy in thObjs1 the subprofiles most similar to worst1 and in thObjs2
	// the subprofiles most similar to worst2
	sub1=worst1->GetSubProfile();
	sub2=worst2->GetSubProfile();
	for(i=grp->NbSubObjects+1,ptr=grp->GetObjects();--i;ptr++)
	{
		if(((*ptr)==worst1)||((*ptr)==worst2)) continue;
		sub=(*ptr)->GetSubProfile();
		sim=Instance->GetSim(sub1,sub);
		minsim=Instance->GetSim(sub2,sub);
		if(sim>minsim)
		{
			NbObjs1++;
			(*(ptr2++))=(*ptr);
		}
		else
		{
			NbObjs2++;
			(*(ptr3++))=(*ptr);
		}
	}

	// Release grp
	ReleaseGroup(grp);

	// Reserve a new group and insert the elements of thObjs1
	grp=ReserveGroup();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
		grp->Insert(*ptr);

	// Reserve a new group and insert the elements of thObjs2
	grp=ReserveGroup();
	for(ptr=thObjs2,i=NbObjs2+1;--i;ptr++)
		grp->Insert(*ptr);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::MergeBestSubProfiles(void) throw(RGA::eGA)
{
	GObjIRCursor Cur,Cur2;
	unsigned int i,j;
	GGroupIR* grp1;
	GGroupIR* grp2;
	GGroupIR* bestgrp1;
	GGroupIR* bestgrp2;
	double sim,maxsim;
	GSubProfile* sub;
	GObjIR** ptr;
	GObjIR** ptr2;

	// Find the two groups containing the most similar objects.
	Cur=(*Objs);
	Cur2=(*Objs);
	for(Cur.Start(),i=Cur.GetNb(),j=0,maxsim=-1.0,bestgrp1=bestgrp2=0;--i;Cur.Next(),j++)
	{
		grp1=GetGroup(Cur());
		sub=Cur()->GetSubProfile();
		for(Cur2.GoTo(j+1);!Cur2.End();Cur2.Next())
		{
			grp2=GetGroup(Cur2());
			if(grp1==grp2) continue;
			sim=Instance->GetSim(sub,Cur2()->GetSubProfile());
			if(sim>maxsim)
			{
				maxsim=sim;
				bestgrp1=grp1;
				bestgrp2=grp2;
			}
		}
	}
	if((!bestgrp1)||(!bestgrp2)) return;

	// Put the objects of bestgrp1 and bestgrp2 in thObjs1
	for(ptr2=bestgrp1->GetObjects(),i=bestgrp1->GetNbObjs()+1,ptr=thObjs1,NbObjs1=0;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);
	for(ptr2=bestgrp2->GetObjects(),i=bestgrp2->GetNbObjs()+1;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);

	// Release the groups
	ReleaseGroup(bestgrp1);
	ReleaseGroup(bestgrp2);

	// Reserve a new group and insert the elements of thObjs1
	grp1=ReserveGroup();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
		grp1->Insert(*ptr);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::TreatSocialSubProfiles(bool rel)
{
	GObjIR* obj;
	GGroupIRCursor Cur1,Cur2;
	unsigned int i;
	GGroupIR* grp=0;
	double tmp,max;

	// Look if social subprofiles are alone
	for(i=0;i<Used.NbPtr;i++)
	{
		Cur1.Set(Used);
		Cur2.Set(Used);
		for(Cur1.GoTo(i);!Cur1.End();Cur1.Next(),i++)
		{
			if(Cur1()->GetNbObjs()!=1) continue;
			obj=Cur1()->GetObjPos(0);
			if(!obj->GetSubProfile()->GetProfile()->IsSocial()) continue;
			for(Cur2.Start(),max=-1.0,grp=0;!Cur2.End();Cur2.Next())
			{
				if(Cur1()==Cur2()) continue;
//				if(!Cur2()->CanInsert(obj)) continue;
				if(rel)
					tmp=Instance->GetSim(obj->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
				else
					tmp=Cur2()->ComputeAvgSim(obj);
				if(tmp>max)
				{
					max=tmp;
					grp=Cur2();
				}
			}
			if(grp)
			{
				ReleaseGroup(Cur1());
				grp->Insert(obj);
				i--;
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Optimisation(void) throw(RGA::eGA)
{
	unsigned int i;
	RPromSol** s;

	#ifdef RGADEBUG
		if(Instance->Debug)
			Instance->Debug->BeginFunc("Optimisation","GChomoIR");
	#endif

	// Copy the current chromosome in thTests
	Evaluate();
	(*thTests[0])=(*this);
	thTests[0]->DivideWorstSubProfiles();
	(*thTests[1])=(*thTests[0]);
	thTests[1]->DivideWorstSubProfiles();
	(*thTests[2])=(*this);
	(*thTests[3])=(*this);
	thTests[3]->MergeBestSubProfiles();
	(*thTests[4])=(*thTests[3]);
	thTests[4]->MergeBestSubProfiles();

	// Do K-Means on the temporaries chromosomes.
	for(i=0;i<5;i++)
	{
		thTests[i]->DoKMeans();
		thTests[i]->TreatSocialSubProfiles(true);
		thTests[i]->Evaluate();
	}

	// Use PROMETHEE to determine the best solution.
	s=thSols;
	thProm->Assign(*s,this);
	for(i=0,s++;i<5;i++,s++)
	{
		thProm->Assign(*s,thTests[i]);
	}
	thProm->ComputePrometheeII();
	if(thProm->GetBestSol()->GetId())
		(*this)=(*thTests[thProm->GetBestSol()->GetId()-1]);

	#ifdef RGADEBUG
		if(Instance->Debug)
			for(s=thProm->GetSols(),i=7;--i;s++)
			{
				if((*s)->GetId())
					Instance->WriteChromoInfo(thTests[(*s)->GetId()-1]);
				else
					Instance->WriteChromoInfo(this);
			}
	#endif

	#ifdef RGADEBUG
		if(Instance->Debug)
			Instance->Debug->EndFunc("Optimisation","GChomoIR");
	#endif
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Modify(void) throw(RGA::eGA)
{
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Mutation();
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
	CritSim=chromo.CritSim;
	CritSimAvgSim=chromo.CritSimAvgSim;
	CritSimJ=chromo.CritSimJ;
	CritSimAvgRatio=chromo.CritSimAvgRatio;
	CritSimMinRatio=chromo.CritSimMinRatio;
	CritSimRatio=chromo.CritSimRatio;
	CritSimWOverB=chromo.CritSimWOverB;
	CritSimSimWB=chromo.CritSimSimWB;
	CritInfo=chromo.CritInfo;
	CritEntropy=chromo.CritEntropy;
	CritLikelihood=chromo.CritLikelihood;
	CritSameFeedbacks=chromo.CritSameFeedbacks;
	CritDiffFeedbacks=chromo.CritDiffFeedbacks;
	CritSocial=chromo.CritSocial;
	Fi=chromo.Fi;
	FiPlus=chromo.FiPlus;
	FiMinus=chromo.FiMinus;
	#if GALILEITEST
		Global=chromo.Global;
		Precision=chromo.Precision;
		Recall=chromo.Recall;
	#endif
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GChromoIR::~GChromoIR(void)
{
}
