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
	  Sims(0), MinSimLevel(0), CritSim(0.0), CritInfo(0.0), CritSameFeedbacks(0.0),
	  CritDiffFeedbacks(1.0), CritSocial(1.0)
{
	#ifdef RGADEBUG
		Global=0.0;
	#endif
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Init(GThreadDataIR* thData) throw(bad_alloc)
{
	// Parent Initialisation
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(thData);

	// Current
	Sims=Instance->Sims;
	MinSimLevel=Instance->MinSimLevel;
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;
}


#ifdef RGADEBUG
//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::CompareIdeal(GSession* s,RStd::RContainer<GGroups,unsigned int,true,true>* ideal)
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
bool GALILEI::GChromoIR::IsInGroup(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	for(nb++;--nb;grp++)
		if((*grp)==obj)
			return(true);
	return(false);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeSumSim(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	unsigned int i;
	double SumSim;

	for(i=nb+1,SumSim=0.0;--i;grp++)
	{
		if((*grp)==obj) continue;
		SumSim+=Sims->GetSim(obj->GetSubProfile(),(*grp)->GetSubProfile());
	}
	return(SumSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeSumSim(GObjIR* obj)
{
	double SumSim;
	GObjIRCursor Cur;

	Cur=(*Objs);
	for(Cur.Start(),SumSim=0.0;!Cur.End();Cur.Next())
	{
		if(Cur()==obj) continue;
		SumSim+=Sims->GetSim(obj->GetSubProfile(),Cur()->GetSubProfile());
	}
	return(SumSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeSumDist(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	unsigned int i;
	double Sum,tmp;

	for(i=nb+1,Sum=0.0;--i;grp++)
	{
		if((*grp)==obj) continue;
		tmp=1-Sims->GetSim(obj->GetSubProfile(),(*grp)->GetSubProfile());
		Sum+=tmp*tmp;
	}
	return(Sum);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeMaxSim(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	unsigned int i;
	double MaxSim,tmp;

	for(i=nb+1,MaxSim=-1.0;--i;grp++)
	{
		if((*grp)==obj) continue;
		tmp=Sims->GetSim(obj->GetSubProfile(),(*grp)->GetSubProfile());
		if(tmp>MaxSim)
			tmp=MaxSim;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeMaxSim(GObjIR** rgrp,unsigned int rnb,GGroupIR* grp)
{
	unsigned int i;
	GObjIR** ptr;
	double MaxSim,tmp;

	for(i=grp->NbSubObjects+1,ptr=GetObjs(grp->SubObjects),MaxSim=-1.0;--i;ptr++)
	{
		tmp=ComputeMaxSim(rgrp,rnb,*ptr);
		if(tmp>MaxSim)
			MaxSim=tmp;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeMaxSim(GObjIR** rgrp,unsigned int rnb,GObjIR** grp,unsigned int nb)
{
	double MaxSim,tmp;

	for(nb++,MaxSim=-1.0;--nb;grp++)
	{
		tmp=ComputeMaxSim(rgrp,rnb,*grp);
		if(tmp>MaxSim)
			MaxSim=tmp;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeMinSim(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	unsigned int i;
	double MinSim,tmp;

	for(i=nb+1,MinSim=1.0;--i;grp++)
	{
		if((*grp)==obj) continue;
		tmp=Sims->GetSim(obj->GetSubProfile(),(*grp)->GetSubProfile());
		if(tmp<MinSim)
			tmp=MinSim;
	}
	return(MinSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeMinSim(GObjIR** grp,unsigned int nb)
{
	unsigned int i;
	double MinSim,tmp;
	GObjIR** ptr;

	for(i=nb+1,ptr=grp,MinSim=1.0;--i;ptr++)
	{
		tmp=ComputeMinSim(grp,nb,*ptr);
		if(tmp<MinSim)
			tmp=MinSim;
	}
	return(MinSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeRelevantSum(GObjIR** grp,unsigned int nb,GObjIR* &rel)
{
	unsigned int i;
	double BestSumSim;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp);
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr);
			BestSumSim=SumSim;
		}
	}

	// Return results
	return(BestSumSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeRelevantSumDist(GObjIR** grp,unsigned int nb,GObjIR* &rel)
{
	unsigned int i;
 	double BestSumSim;
	double BestSum;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp);
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);
	BestSum=ComputeSumDist(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr);
			BestSum=ComputeSumDist(grp,nb,*ptr);
			BestSumSim=SumSim;
		}
	}

	// Return results
	return(BestSum);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeRelevantMin(GObjIR** grp,unsigned int nb,GObjIR* &rel)
{
	unsigned int i;
	double BestSumSim;
 	double MinSumSim;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp);
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);
	MinSumSim=ComputeMinSim(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr);
			BestSumSim=SumSim;
			MinSumSim=ComputeMinSim(grp,nb,*ptr);
		}
	}

	// Return results
	return(MinSumSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeRelevantMax(GObjIR** grp,unsigned int nb,GObjIR* &rel)
{
	unsigned int i;
	double BestSumSim;
 	double MaxSumSim;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp);
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);
	MaxSumSim=ComputeMaxSim(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr);
			BestSumSim=SumSim;
			MaxSumSim=ComputeMaxSim(grp,nb,*ptr);
		}
	}

	// Return results
	return(MaxSumSim);
}


//---------------------------------------------------------------------------
void GALILEI::GChromoIR::ComputeRelevant(GObjIR** grp,unsigned int nb,GObjIR* &rel)
{
	unsigned int i;
	double BestSumSim;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp);
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr);
			BestSumSim=SumSim;
		}
	}
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
bool GALILEI::GChromoIR::EvaluateOldNew(unsigned int nbcrit)
{
	unsigned int NbCrit;
	RPromKernel* Kernel;
	RPromCriterion* PromCritSim;
	RPromCriterion* PromCritSameFeedbacks;
	RPromCriterion* PromCritDiffFeedbacks;
	RPromCriterion* PromCritSocial;
	RPromCriterion* PromCritInfo;
	RPromSol* OldSol;
	RPromSol* NewSol;
	bool Comp;

	if(!nbcrit)
	{
		// Use PROMETHEE.
		Kernel=new RPromKernel("GChromoIR",2,5);
		PromCritSim=Kernel->NewCriterion(Maximize,"Similarity",Instance->CritSim->GetParams());
		PromCritSameFeedbacks=Kernel->NewCriterion(Maximize,"Same Feedbacks",Instance->CritSameFeedbacks->GetParams());
		PromCritDiffFeedbacks=Kernel->NewCriterion(Minimize,"Diff Feedbacks",Instance->CritDiffFeedbacks->GetParams());
		PromCritSocial=Kernel->NewCriterion(Minimize,"Social",Instance->CritSocial->GetParams());
		PromCritInfo=Kernel->NewCriterion(Maximize,"Information",Instance->CritInfo->GetParams());
		OldSol=Kernel->NewSol();
		NewSol=Kernel->NewSol();
		Kernel->Assign(NewSol,PromCritInfo,LocalCritInfo);
		Kernel->Assign(OldSol,PromCritInfo,CritInfo);
		Kernel->Assign(NewSol,PromCritSim,LocalCritSim);
		Kernel->Assign(OldSol,PromCritSim,CritSim);
		Kernel->Assign(NewSol,PromCritSameFeedbacks,LocalCritSameFeedbacks);
		Kernel->Assign(OldSol,PromCritSameFeedbacks,CritSameFeedbacks);
		Kernel->Assign(NewSol,PromCritDiffFeedbacks,LocalCritDiffFeedbacks);
		Kernel->Assign(OldSol,PromCritDiffFeedbacks,CritDiffFeedbacks);
		Kernel->Assign(NewSol,PromCritSocial,LocalCritSocial);
		Kernel->Assign(OldSol,PromCritSocial,CritSocial);
		Kernel->ComputePrometheeII();
		Comp=Kernel->GetBestSol()->GetId();
		delete Kernel;
	}
	else
	{
		NbCrit=0;
		if(LocalCritSim>=CritSim)
			NbCrit++;
		if(LocalCritSameFeedbacks>=CritSameFeedbacks)
			NbCrit++;
		if(LocalCritDiffFeedbacks<=CritDiffFeedbacks)
			NbCrit++;
		if(LocalCritInfo>=CritInfo)
			NbCrit++;
		if(LocalCritSocial<=CritSocial)
			NbCrit++;
		Comp=(NbCrit>=nbcrit);
	}
	return(Comp);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::MergeGroups(GGroupIR* grp1,GGroupIR* grp2,unsigned int nbcrit)
{
	GObjIR** ptr;
	GObjIR** ptr2;
	unsigned int i;
	bool Merge;

	//  Verify that the groups may be grouped together -> each subprofile of
	// grp1 can be inserted in grp2.
	for(ptr2=grp1->GetObjects(),i=grp1->GetNbObjs()+1;--i;ptr2++)
		if(!grp2->CanInsert(*ptr2))
			return(false);

	// Put in thObjs1 the merge of the two groups
	for(ptr2=grp1->GetObjects(),ptr=thObjs1,i=grp1->GetNbObjs()+1,NbObjs1=0;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);
	for(ptr2=grp2->GetObjects(),i=grp2->GetNbObjs()+1;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);

	// Evaluate and look number of criteria ameliorated.
	Evaluate(grp1,grp2);
	Merge=EvaluateOldNew(nbcrit);

	// Do the merge if necessary.
	if(Merge)
	{
		// Release grp1 and grp2
		ReleaseGroup(grp1);
		ReleaseGroup(grp2);
//		Set(grp1->Id,false);
//		Set(grp2->Id,false);

		// Reserve a new group and insert the elements of thObjs1
		grp1=ReserveGroup();
		for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
			grp1->Insert(*ptr);
//		Set(grp1->Id,true);
	}

	return(Merge);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::DivideGroup(GGroupIR* grp,unsigned int nbcrit)
{
	unsigned int i,j;
	GObjIR** ptr;
	GObjIR** ptr2;
	GObjIR** o1;
	GObjIR** o2;
	double Avg,NbComp;
	GSubProfile* sub1;
	GSubProfile* sub2;
	GSubProfile* obj;
	bool Divide;
	GObjIR* ref1;
	GObjIR* ref2;

	// If group has not at least 2 objects do nothing
	if(grp->NbSubObjects<2) return(false);

	// "Clear" the two groups.
	NbObjs1=NbObjs2=0;
	o1=thObjs1;
	o2=thObjs2;

	if(nbcrit)
	{
		// Select randomly a subprofile of the group
		ref1=GetObj(grp->SubObjects+Instance->RRand(grp->NbSubObjects));
		sub1=ref1->GetSubProfile();
		NbObjs1++;
		(*(o1++))=ref1;

		// Compute the avg sim of sub1 with other subprofiles
		for(i=grp->NbSubObjects+1,ptr=grp->GetObjects(),Avg=0.0;--i;ptr++)
		{
			obj=(*ptr)->GetSubProfile();
			if(obj==sub1) continue;
			Avg+=Sims->GetSim(sub1,obj);
		}

		// Put in thObjs1 all subprofiles with a similarity > Avg
		// Put in thObjs2 all subprofiles with a similarity <= Avg
		for(i=grp->NbSubObjects+1,ptr=grp->GetObjects();--i;ptr++)
		{
			if((*ptr)==ref1) continue;
			obj=(*ptr)->GetSubProfile();
			if(Sims->GetSim(sub1,obj)>Avg)
			{
				NbObjs1++;
				(*(o1++))=(*ptr);
			}
			else
			{
				NbObjs2++;
				(*(o2++))=(*ptr);
			}
		}
	}
	else
	{
		// Select the two subprofiles of the group which are the most similar
		for(i=grp->NbSubObjects,ptr=grp->GetObjects(),Avg=0.0,ref1=ref2=0;--i;ptr++)
		{
			for(j=i+1,ptr2=ptr+1;--j;ptr2++)
			{
				NbComp=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
				if(NbComp>Avg)
				{
					Avg=NbComp;
					ref1=(*ptr);
					sub1=ref1->GetSubProfile();
					ref2=(*ptr2);
					sub2=ref2->GetSubProfile();
				}
			}
		}
		if((!ref1)||(!ref2)) return(false);

		// Compute the avg sim of sub1 and sub2 with other subprofiles
		for(i=grp->NbSubObjects+1,ptr=grp->GetObjects(),Avg=0.0,NbComp=0.0;--i;ptr++)
		{
			obj=(*ptr)->GetSubProfile();
			if((obj==sub1)||(obj==sub2)) continue;
			Avg+=Sims->GetSim(sub1,obj);
			Avg+=Sims->GetSim(sub2,obj);
			NbComp+=2.0;
		}
		Avg/=NbComp;

		// Put in thObjs1 all subprofiles with a similarity > Avg
		// Put in thObjs2 all subprofiles with a similarity <= Avg
		for(i=grp->NbSubObjects+1,ptr=grp->GetObjects();--i;ptr++)
		{
			if(((*ptr)==ref1)||((*ptr)==ref2)) continue;
			obj=(*ptr)->GetSubProfile();
			if((Sims->GetSim(sub1,obj)+Sims->GetSim(sub2,obj))/2.0>Avg)
			{
				NbObjs1++;
				(*(o1++))=(*ptr);
			}
			else
			{
				NbObjs2++;
				(*(o2++))=(*ptr);
			}
		}
	}

	// If only one group do nothing
	if((!NbObjs1)||(!NbObjs2)) return(false);

	// Evaluate and look number of criteria ameliorated.
	Evaluate(grp,0);
	Divide=EvaluateOldNew(nbcrit);

	// Do the division if necessary.
	if(Divide)
	{
		// Release grp
		ReleaseGroup(grp);

		// Reserve a new group and insert the elements of thObjs1
		grp=ReserveGroup();
		grp->Done=false;
		for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
			grp->Insert(*ptr);

		// Reserve a new group and insert the elements of thObjs2
		grp=ReserveGroup();
		grp->Done=false;
		for(ptr=thObjs2,i=NbObjs2+1;--i;ptr++)
			grp->Insert(*ptr);
	}

	// Group can not be divided
	return(Divide);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::ConstructChromo(GGroups* grps)
{
	GGroupCursor Grp;
	GSubProfileCursor SubProfile;
	GGroupIR* grp;

	// Go through the groups
	Grp=grps->GetGroupCursor();
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		grp=ReserveGroup();
		SubProfile=Grp()->GetSubProfileCursor();
		for(SubProfile.Start();!SubProfile.End();SubProfile.Next())
		{
			for(Objs->Start();!Objs->End();Objs->Next())
				if((*Objs)()->GetSubProfile()==SubProfile())
				{
					grp->Insert((*Objs)());
					break;
				}
		}
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::RandomConstruct(void)
{
	// Look if already a solution in the session
	if(Instance->CurrentGroups)
		 ConstructChromo(Instance->CurrentGroups);

	// Call classic heuristic for non-assigned objects
	if(!RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::RandomConstruct())
		return(false);

	// If first element, do nothing.
	if((!Instance->CurrentGroups)||(!Id))
		return(true);

	// Do a mutation
	return(Mutation());
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateAvgSim(GGroupIR* grp1,GGroupIR* grp2)
{
	unsigned int i,j;
	GGroupIRCursor Cur;
	double NbComp;
	GObjIR** ptr2;
	GObjIR** ptr;
	double tmp;

	LocalCritSim=0.0;

	// Compute Avg Similarity for grp1
	if(grp1)
	{
		for(ptr=thObjs1,i=NbObjs1,NbComp=0.0;--i;ptr++)
		{
			for(j=i+1,ptr2=ptr+1;--j;ptr2++)
			{
				LocalCritSim+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
				NbComp+=1.0;
			}
		}
		LocalCritSim/=NbComp;
	}

	// Compute Avg Similarity for grp2
	if(grp1&&(!grp2))
	{
		for(ptr=thObjs2,i=NbObjs2,tmp=NbComp=0.0;--i;ptr++)
		{
			for(j=i+1,ptr2=ptr+1;--j;ptr2++)
			{
				tmp+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
				NbComp+=1.0;
			}
		}
		LocalCritSim+=tmp/NbComp;
	}

	// Computed the average similairies of the configurations
	Cur.Set(Used);
	for(Cur.Start(),CritSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeAvgSim();
		CritSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSim+=tmp;
	}
	CritSim/=((double)Used.NbPtr);
	if(grp1)
	{
		if(grp2)
			LocalCritSim/=((double)Used.NbPtr-1);
		else
			LocalCritSim/=((double)Used.NbPtr+1);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateJ(GGroupIR* grp1,GGroupIR* grp2)
{
	unsigned int i,j;
	GGroupIRCursor Cur;
	double min,localmin;
	GGroupIRCursor Cur2;
	GObjIR* rel1;
	GObjIR* rel2;
	double tmp;

	// Compute (Sum Intra) for the configurations.
	Cur.Set(Used);
	for(Cur.Start(),LocalCritSim=CritSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSumDist();
		CritSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSumDist(thObjs1,NbObjs1,rel1);
		LocalCritSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSumDist(thObjs2,NbObjs2,rel2);
		LocalCritSim+=tmp;
	}

	// Compute Min inter for the configurations.
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),i=0,j=Cur.GetNb(),min=localmin=1.0;--j;Cur.Next(),i++)
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp<min)
				min=tmp;
			if((grp1&&(Cur2()==grp1))||(grp2&&(Cur2()==grp2)))
				continue;
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2)))
				continue;
			if(tmp<localmin)
				localmin=tmp;
		}
		if(grp1)
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp<localmin)
				localmin=tmp;
		}
		if(grp1&&(!grp2))
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp<localmin)
				localmin=tmp;
		}
	}
	if(grp1&&(!grp2))
	{
		tmp=1-Sims->GetSim(rel1->GetSubProfile(),rel2->GetSubProfile());
		tmp=tmp*tmp;
		if(tmp<localmin)
			localmin=tmp;
	}

	// Compute (Min inter*NbGroups)/(Sum Intra) for the configurations.
	if(CritSim)
		CritSim=min/CritSim*Used.NbPtr;
	if(grp1)
	{
		if(grp2)
			LocalCritSim=localmin/LocalCritSim*((double)Used.NbPtr-1);
		else
			LocalCritSim=localmin/LocalCritSim*((double)Used.NbPtr+1);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateAvgRatio(GGroupIR* grp1,GGroupIR* grp2)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter,tmp;
	GObjIR* rel1;
	GObjIR* rel2;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();
	if(grp1)
	{
		ComputeRelevant(thObjs1,NbObjs1,rel1);
		if(!grp2)
			ComputeRelevant(thObjs2,NbObjs2,rel2);
	}

	// Average min(intra)/max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),CritSim=LocalCritSim=0.0;!Cur.End();Cur.Next())
	{
		MinIntra=Cur()->ComputeMinSim(Cur()->Relevant);
		for(Cur2.Start(),MaxInter=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		if(grp1)
		{
			if(!grp2)
			{
				tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
				if(tmp>MaxInter)
					MaxInter=tmp;
			}
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=MinIntra/MaxInter;
		CritSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSim+=tmp;
	}
	if(grp1)
	{
		MinIntra=ComputeMinSim(thObjs1,NbObjs1,rel1);
		for(Cur.Start(),MaxInter=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		if(!grp2)
		{
			tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		LocalCritSim+=MinIntra/MaxInter;
	}
	if(grp1&&(!grp2))
	{
		MinIntra=ComputeMinSim(thObjs2,NbObjs2,rel2);
		for(Cur.Start(),MaxInter=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
		if(tmp>MaxInter)
			MaxInter=tmp;
		LocalCritSim+=MinIntra/MaxInter;
	}
	if(Used.NbPtr)
		CritSim/=static_cast<double>(Used.NbPtr);
	if(grp1)
	{
		if(grp2)
			LocalCritSim/=((double)Used.NbPtr-1);
		else
			LocalCritSim/=((double)Used.NbPtr+1);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateMinRatio(GGroupIR* grp1,GGroupIR* grp2)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter,tmp;
	GObjIR* rel1;
	GObjIR* rel2;
	bool first,localfirst;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();
	if(grp1)
	{
		ComputeRelevant(thObjs1,NbObjs1,rel1);
		if(!grp2)
			ComputeRelevant(thObjs2,NbObjs2,rel2);
	}

	// Min min(intra)/max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),CritSim=LocalCritSim=0.0,localfirst=first=true;!Cur.End();Cur.Next())
	{
		MinIntra=Cur()->ComputeMinSim(Cur()->Relevant);
		for(Cur2.Start(),MaxInter=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		if(grp1)
		{
			if(!grp2)
			{
				tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
				if(tmp>MaxInter)
					MaxInter=tmp;
			}
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=MinIntra/MaxInter;
		if(first)
		{
			CritSim=tmp;
			first=false;
		}
		else
			if(tmp<CritSim)
				CritSim=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		if(localfirst)
		{
			LocalCritSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalCritSim)
				LocalCritSim=tmp;
	}
	if(grp1)
	{
		MinIntra=ComputeMinSim(thObjs1,NbObjs1,rel1);
		for(Cur.Start(),MaxInter=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		if(!grp2)
		{
			tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=MinIntra/MaxInter;
		if(localfirst)
		{
			LocalCritSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalCritSim)
				LocalCritSim=tmp;
	}
	if(grp1&&(!grp2))
	{
		MinIntra=ComputeMinSim(thObjs2,NbObjs2,rel2);
		for(Cur.Start(),MaxInter=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
			if(tmp>MaxInter)
				MaxInter=tmp;
		}
		tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
		if(tmp>MaxInter)
			MaxInter=tmp;
		tmp=MinIntra/MaxInter;
		if(localfirst)
		{
			LocalCritSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalCritSim)
				LocalCritSim=tmp;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateRatio(GGroupIR* grp1,GGroupIR* grp2)
{
	GGroupIRCursor Cur,Cur2;
	double MinIntra,MaxInter;
	double LocalMinIntra,LocalMaxInter;
	double tmpMin,tmpMax,tmp;
	GObjIR* rel1;
	GObjIR* rel2;

	// Compute Relevant Profiles.
	Cur.Set(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ComputeRelevant();
	if(grp1)
	{
		ComputeRelevant(thObjs1,NbObjs1,rel1);
		if(!grp2)
			ComputeRelevant(thObjs2,NbObjs2,rel2);
	}

	// Determine overall min(intra) and overall max(inter)
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),MaxInter=LocalMaxInter=CritSim=LocalCritSim=0.0,MinIntra=LocalMinIntra=1.0;!Cur.End();Cur.Next())
	{
		tmpMin=Cur()->ComputeMinSim(Cur()->Relevant);
		if(tmpMin<MinIntra)
			MinIntra=tmpMin;
		for(Cur2.Start(),tmpMax=0.0;!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		if(grp1)
		{
			if(!grp2)
			{
				tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
				if(tmp>tmpMax)
					tmpMax=tmp;
			}
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		if(tmpMax>MaxInter)
			MaxInter=tmpMax;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		if(tmpMin<LocalMinIntra)
			LocalMinIntra=tmpMin;
		if(tmpMax>LocalMaxInter)
			LocalMaxInter=tmpMax;
	}
	if(grp1)
	{
		tmpMin=ComputeMinSim(thObjs1,NbObjs1,rel1);
		if(tmpMin<LocalMinIntra)
			LocalMinIntra=tmpMin;
		for(Cur.Start(),tmpMax=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		if(!grp2)
		{
			tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		if(tmpMax>LocalMaxInter)
			LocalMaxInter=tmpMax;
	}
	if(grp1&&(!grp2))
	{
		tmpMin=ComputeMinSim(thObjs2,NbObjs2,rel2);
		if(tmpMin<LocalMinIntra)
			LocalMinIntra=tmpMin;
		for(Cur.Start(),tmpMax=0.0;!Cur.End();Cur.Next())
		{
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
			tmp=Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
			if(tmp>tmpMax)
				tmpMax=tmp;
		}
		tmp=Sims->GetSim(rel2->GetSubProfile(),rel1->GetSubProfile());
		if(tmp>tmpMax)
			tmpMax=tmp;
		if(tmpMax>LocalMaxInter)
			LocalMaxInter=tmpMax;
	}
	if(MaxInter)
		CritSim=MinIntra/MaxInter;
	if(LocalMaxInter)
		LocalCritSim=LocalMinIntra/LocalMaxInter;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateWOverB(GGroupIR* grp1,GGroupIR* grp2)
{
	GGroupIRCursor Cur;
	double sb,localsb;
	GObjIR* rel1;
	GObjIR* rel2;
	GObjIR* rel;
	double tmp;

	// Compute Sb over Sw. Normally, the measure is Sw/Sb but is to minimized.

	// Compute Sw
	Cur.Set(Used);
	for(Cur.Start(),LocalCritSim=CritSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSumDist();
		CritSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSumDist(thObjs1,NbObjs1,rel1);
		LocalCritSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSumDist(thObjs2,NbObjs2,rel2);
		LocalCritSim+=tmp;
	}

	// Compute Sb
	rel=ComputeGlobalRelevant();
	Cur.Set(Used);
	for(Cur.Start(),sb=localsb=0.0;!Cur.End();Cur.Next())
	{
		if(rel==Cur()->Relevant) continue;
		tmp=1-Sims->GetSim(rel->GetSubProfile(),Cur()->Relevant->GetSubProfile());
		tmp=tmp*tmp;
		sb+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2)))
			continue;
		localsb+=tmp;
	}
	if(grp1)
	{
		tmp=1-Sims->GetSim(rel->GetSubProfile(),rel1->GetSubProfile());
		localsb+=tmp*tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=1-Sims->GetSim(rel->GetSubProfile(),rel2->GetSubProfile());
		localsb+=tmp*tmp;
	}

	// Compute Sb/Sw for the configurations.
	if(CritSim)
		CritSim=Used.NbPtr*sb/CritSim;
	if(LocalCritSim)
	{
		if(grp1)
		{
			if(grp2)
				LocalCritSim=((double)Used.NbPtr-1)*localsb/LocalCritSim;
			else
				LocalCritSim=((double)Used.NbPtr+1)*localsb/LocalCritSim;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSimWB(GGroupIR* grp1,GGroupIR* grp2)
{
	GGroupIRCursor Cur;
	double sb,localsb;
	GObjIR* rel1;
	GObjIR* rel2;
	GObjIR* rel;
	double tmp;

	// Compute SimW/SimB.

	// Compute SimW
	Cur.Set(Used);
	for(Cur.Start(),LocalCritSim=CritSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSum();
		CritSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSum(thObjs1,NbObjs1,rel1);
		LocalCritSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSum(thObjs2,NbObjs2,rel2);
		LocalCritSim+=tmp;
	}

	// Compute SimB
	rel=ComputeGlobalRelevant();
	Cur.Set(Used);
	for(Cur.Start(),sb=localsb=0.0;!Cur.End();Cur.Next())
	{
		if(rel==Cur()->Relevant) continue;
		tmp=Sims->GetSim(rel->GetSubProfile(),Cur()->Relevant->GetSubProfile());
		sb+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2)))
			continue;
		localsb+=tmp;
	}
	if(grp1)
		localsb+=Sims->GetSim(rel->GetSubProfile(),rel1->GetSubProfile());
	if(grp1&&(!grp2))
		localsb+=Sims->GetSim(rel->GetSubProfile(),rel2->GetSubProfile());

	// Compute SimW/SimB for the configurations.
	if(sb)
		CritSim/=Used.NbPtr*sb;
	if(localsb)
	{
		if(grp1)
		{
			if(grp2)
				LocalCritSim/=((double)Used.NbPtr-1)*localsb;
			else
				LocalCritSim/=((double)Used.NbPtr+1)*localsb;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSim(GGroupIR* grp1,GGroupIR* grp2)
{
	switch(Instance->SimMeasure)
	{
		case stAvgSim:
			EvaluateAvgSim(grp1,grp2);
			break;

		case stJ:
			EvaluateJ(grp1,grp2);
			break;

		case stAvgRatio:
			EvaluateAvgRatio(grp1,grp2);
			break;

		case stMinRatio:
			EvaluateMinRatio(grp1,grp2);
			break;

		case stRatio:
			EvaluateRatio(grp1,grp2);
			break;

		case stWOverB:
			EvaluateWOverB(grp1,grp2);
			break;

		case stSimWB:
			EvaluateSimWB(grp1,grp2);
			break;

		default:
			CritSim=LocalCritSim=0.0;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateInfo(GGroupIR* grp1,GGroupIR* grp2)
{
	double NbProf;
	double NbGroups;

	// Compute Average number of profiles.
	NbProf=static_cast<double>(Objs->GetNb());
	NbGroups=static_cast<double>(Used.NbPtr);
	CritInfo=NbProf/NbGroups;
	if(grp1)
	{
		if(grp2)
			LocalCritInfo=NbProf/(NbGroups-1);
		else
			LocalCritInfo=NbProf/(NbGroups+1);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSameFeedbacks(GGroupIR* grp1,GGroupIR* grp2)
{
	GSubProfilesSameDocs* same;
	unsigned int i,j;
	GObjIR** ptr;
	unsigned int a1,a2;
	unsigned int id1;
	unsigned int id2;

	if(grp1)
		id1=grp1->Id;
	if(grp2)
		id2=grp2->Id;
	// Number of subprofiles having common OK documents and being in the same group.
	for(Instance->SameFeedbacks.Start(),LocalCritSameFeedbacks=CritSameFeedbacks=0.0;!Instance->SameFeedbacks.End();Instance->SameFeedbacks.Next())
	{
		same=Instance->SameFeedbacks();
		a1=ObjectsAss[same->GetId1()];
		a2=ObjectsAss[same->GetId2()];
		if(a1!=a2) continue;
		CritSameFeedbacks+=1.0;
		if(!grp1) continue;
		if(grp2)
		{
			if((a1==id1||a1==id2)&&(a2==id1||a2==id2))
				LocalCritSameFeedbacks+=same->GetRatio();
		}
		else
		{
			if(a1==grp1->GetId())
			{
				// Go through thObjs1 and count (j) number of objects of same
				for(ptr=thObjs1,i=NbObjs1+1,j=0;--i;ptr++)
				{
					if(same->IsIn((*ptr)->GetId())) j++;
				}

				// If j<>1 -> In Same group
				if(j!=1)
					LocalCritSameFeedbacks+=same->GetRatio();
			}
			else
				LocalCritSameFeedbacks+=same->GetRatio();
		}
	}
	if(Instance->SameFeedbacks.NbPtr)
	{
		LocalCritSameFeedbacks/=Instance->SameFeedbacks.NbPtr;
		CritSameFeedbacks/=Instance->SameFeedbacks.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateDiffFeedbacks(GGroupIR* grp1,GGroupIR* grp2)
{
	GSubProfilesSameDocs* diff;
	unsigned int i,j;
	GObjIR** ptr;
	unsigned int a1,a2;
	unsigned int id1;
	unsigned int id2;

	if(grp1)
		id1=grp1->Id;
	if(grp2)
		id2=grp2->Id;
	// Number of subprofiles having common OK documents and being in the same group.
	for(Instance->DiffFeedbacks.Start(),LocalCritDiffFeedbacks=CritDiffFeedbacks=0.0;!Instance->DiffFeedbacks.End();Instance->DiffFeedbacks.Next())
	{
		diff=Instance->DiffFeedbacks();
		a1=ObjectsAss[diff->GetId1()];
		a2=ObjectsAss[diff->GetId2()];
		if(a1!=a2) continue;
		CritDiffFeedbacks+=1.0;
		if(!grp1) continue;
		if(grp2)
		{
			if((a1==id1||a1==id2)&&(a2==id1||a2==id2))
				LocalCritDiffFeedbacks+=diff->GetRatio();
		}
		else
		{
			if(a1==grp1->GetId())
			{
				// Go through thObjs1 and count (j) number of objects of same
				for(ptr=thObjs1,i=NbObjs1+1,j=0;--i;ptr++)
				{
					if(diff->IsIn((*ptr)->GetId())) j++;
				}

				// If j<>1 -> In Same group
				if(j!=1)
					LocalCritDiffFeedbacks+=diff->GetRatio();
			}
			else
				LocalCritDiffFeedbacks+=diff->GetRatio();
		}
	}
	if(Instance->DiffFeedbacks.NbPtr)
	{
		LocalCritDiffFeedbacks/=Instance->SameFeedbacks.NbPtr;
		CritDiffFeedbacks/=Instance->SameFeedbacks.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateSocial(GGroupIR* grp1,GGroupIR* grp2)
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
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalCritSocial+=1.0;
	}

	// Look for thObjs1
	if(grp1)
	{
		if((NbObjs1==1)&&(!Instance->NoSocialSubProfiles.IsIn<const GObjIR*>(*thObjs1)))
			LocalCritSocial+=1.0;
	}

	// Look for thObjs2
	if(grp1&&(!grp2))
	{
		if((NbObjs2==1)&&(!Instance->NoSocialSubProfiles.IsIn<const GObjIR*>(*thObjs2)))
			LocalCritSocial+=1.0;
	}

	i=Objs->GetNb()-Instance->NoSocialSubProfiles.NbPtr;
	if(i)
	{
		CritSocial/=i;
		LocalCritSocial/=i;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(GGroupIR* grp1,GGroupIR* grp2)
{
	EvaluateSim(grp1,grp2);
	EvaluateInfo(grp1,grp2);
	EvaluateSameFeedbacks(grp1,grp2);
	EvaluateDiffFeedbacks(grp1,grp2);
	EvaluateSocial(grp1,grp2);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(void)
{
	Evaluate(0,0);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::TryDivisions(unsigned int nbcrit)
{
	bool Cont,Done;
	GGroupIRCursor Cur;

	Done=false;
//	Cur.Set(Used);
//	for(Cur.Start();!Cur.End();Cur.Next())
//		Cur()->Done=false;

	// Try to divided groups until it is not possible anymore
	cout<<"\tDivision... "<<endl;
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur.Set(Used);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(DivideGroup(Cur(),nbcrit))
			{
//				cout<<"\t\tDivide "<<endl;
				Cont=true;
				Done=true;
				break;
			}
		}
	}
//	cout<<"\tDone"<<endl;
	return(Done);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::TryMerges(unsigned int nbcrit)
{
	bool Cont,Done;
	GGroupIRCursor Cur1,Cur2;
	unsigned int i,j;

	// Init Pairs
	Done=false;

	// Try to regroup groups until it is not possible anymore
//	cout<<"\tRegroup... "<<endl;
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		Cur2.Set(Used);
		for(Cur1.Start(),i=0,j=Cur1.GetNb();(--j)&&(!Cont);Cur1.Next(),i++)
		{
			for(Cur2.GoTo(i+1);(!Cur2.End())&&(!Cont);Cur2.Next())
			{
				if(MergeGroups(Cur1(),Cur2(),nbcrit))
				{
//					cout<<"\t\tMerge "<<i<<" and "<<j<<endl;
					Cont=true;
					Done=true;
					break;
				}
			}
		}
	}
//	cout<<"\tDone"<<endl;
	return(Done);
}
 

//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Optimisation(void)
{
//	cout<<"Optimise Chromosome "<<Id<<" - Gen="<<Instance->Gen<<":"<<endl;
//	for(bool Cont=true;Cont;)
//	{
//		Cont=false;
//		if(TryDivisions())
//			Cont=true;
//		if(TryMerges())
//			Cont=true;
//	}
//	cout<<"\tDone"<<endl;
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Crossover(GChromoIR* parent1,GChromoIR* parent2)
{
	bool b;

	b=RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Crossover(parent1,parent2);
//	ReOrganisation();
	return(b);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Mutation(void)
{
	bool b;

	b=RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Mutation();
//	ReOrganisation();
	return(b);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Modify(void)
{
	return(RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Mutation());
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::ReOrganisation(void)
{
//	for(bool Cont=true;Cont;)
//	{
//		Cont=false;
//		if(TryDivisions(2))
//			Cont=true;
//		if(TryMerges(2))
//			Cont=true;
//	}
//
//	ComputeOrd();
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
	CritInfo=chromo.CritInfo;
	CritSameFeedbacks=chromo.CritSameFeedbacks;
	CritDiffFeedbacks=chromo.CritDiffFeedbacks;
	CritSocial=chromo.CritSocial;
	Global=chromo.Global;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GChromoIR::~GChromoIR(void)
{
}
