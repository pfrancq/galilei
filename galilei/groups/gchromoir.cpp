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
	  Sims(0), MinSimLevel(0), AvgSim(0.0), AvgProf(0.0), OKFactor(0.0),
	  DiffFactor(1.0), SocialFactor(1.0)
{
	#ifdef RGADEBUG
		Global=0.0;
	#endif
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Init(GThreadDataIR* thData) throw(bad_alloc)
{
	unsigned int i;
	bool** ptr;

	// Parent Initialisation
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(thData);

	// Current
	Sims=Instance->Sims;
	MinSimLevel=Instance->MinSimLevel;
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;

	// Pairs
	NbRows=Objs->GetNb();
	Pairs=new bool*[NbRows];
	(*Pairs)=new bool[NbRows];
	for(i=NbRows,ptr=Pairs+1;--i;ptr++)
		(*ptr)=new bool[i-1];
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

	for(i=nb+1,MaxSim=0.0;--i;grp++)
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

	for(i=grp->NbSubObjects+1,ptr=GetObjs(grp->SubObjects),MaxSim=0.0;--i;ptr++)
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

	for(nb++,MaxSim=0.0;--nb;grp++)
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
bool GALILEI::GChromoIR::MergeGroups(GGroupIR* grp1,GGroupIR* grp2,unsigned int nbcrit)
{
	GObjIR** ptr;
	GObjIR** ptr2;
	unsigned int i;
	unsigned int NbCrit=0;
	RPromKernel* Kernel;
	RPromCriterion* CritSim;
	RPromCriterion* CritOKDocs;
	RPromCriterion* CritDiffDocs;
	RPromSol* OldSol;
	RPromSol* NewSol;

	// Init Prom
	if(nbcrit==3)
	{
		Kernel=new RPromKernel("GChromoIR",2,3);
		CritSim=Kernel->NewCriterion(Maximize,"Similarities",0.2,0.05,1.0);
		CritOKDocs=Kernel->NewCriterion(Maximize,"Common OK Documents",0.2,0.05,1.0);
		CritDiffDocs=Kernel->NewCriterion(Minimize,"Common Documents with different Judgment",0.2,0.05,1.0);
		OldSol=Kernel->NewSol();
		NewSol=Kernel->NewSol();
	}

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

	EvaluateSim(grp1,grp2);
	EvaluateOKFactor(grp1,grp2);
	EvaluateDiffFactor(grp1,grp2);

	// Look number of criteria ameliorated
	if(nbcrit==3)
	{
		Kernel->Assign(NewSol,CritSim,LocalAvgSim);
		Kernel->Assign(OldSol,CritSim,AvgSim);
		Kernel->Assign(NewSol,CritOKDocs,LocalOKFactor);
		Kernel->Assign(OldSol,CritOKDocs,OKFactor);
		Kernel->Assign(NewSol,CritDiffDocs,LocalDiffFactor);
		Kernel->Assign(OldSol,CritDiffDocs,DiffFactor);
		Kernel->ComputePrometheeII();
		if(Kernel->GetBestSol()->GetId())
			nbcrit=0;
		else
			nbcrit=4;
		delete Kernel;
	}
	else
	{
		if(LocalAvgSim>=AvgSim)
			NbCrit++;
		if(LocalOKFactor>=OKFactor)
			NbCrit++;
		if(LocalDiffFactor<=DiffFactor)
			NbCrit++;
	}
	if(NbCrit>=nbcrit)
	{
		// Release grp1 and grp2
		ReleaseGroup(grp1);
		ReleaseGroup(grp2);
		Set(grp1->Id,false);
		Set(grp2->Id,false);

		// Reserve a new group and insert the elements of thObjs1
		grp1=ReserveGroup();
		for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
			grp1->Insert(*ptr);
		Set(grp1->Id,true);
		return(true);
	}

	return(false);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::DivideGroup(GGroupIR* grp,unsigned int nbcrit)
{
	unsigned int i;
	GObjIR** ptr;
	GObjIR** o1;
	GObjIR** o2;
	double Avg;
	GSubProfile* sub;
	GSubProfile* obj;
	unsigned int NbCrit=0;

	// If group has not at least 2 objects do nothing
	if(grp->NbSubObjects<2) return(false);

	// Select randomly a subprofile of the group and compute the avg sim with
	// other subprofiles
	sub=GetObj(grp->SubObjects+Instance->RRand(grp->NbSubObjects))->GetSubProfile();
	Avg=0.0;
	for(i=grp->NbSubObjects+1,ptr=grp->GetObjects();--i;ptr++)
	{
		obj=(*ptr)->GetSubProfile();
		if(obj==sub) continue;
		Avg+=Sims->GetSim(sub,obj);
	}
	Avg/=grp->NbSubObjects-1;

	// Put in thObjs1 all subprofiles with a similarity > Avg
	// Put in thObjs2 all subprofiles with a similarity <= Avg
	NbObjs1=NbObjs2=0;
	o1=thObjs1;
	o2=thObjs2;
	for(i=grp->NbSubObjects+1,ptr=grp->GetObjects();--i;ptr++)
	{
		obj=(*ptr)->GetSubProfile();
		if(Sims->GetSim(sub,obj)>Avg)
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

	// If only one group do nothing
	if((!NbObjs1)||(!NbObjs2)) return(false);


	EvaluateSim(grp);
	EvaluateOKFactor(grp);
	EvaluateDiffFactor(grp);

	// Look number of criteria ameliorated
	if(LocalAvgSim>=AvgSim)
		NbCrit++;
	if(LocalOKFactor>=OKFactor)
		NbCrit++;
	if(LocalDiffFactor<=DiffFactor)
		NbCrit++;
	if(NbCrit>=nbcrit)
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
		return(true);
	}

	// Group can not be divided
	return(false);
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

	LocalAvgSim=0.0;

	// Compute Avg Similarity for grp1
	if(grp1)
	{
		for(ptr=thObjs1,i=NbObjs1,NbComp=0.0;--i;ptr++)
		{
			for(j=i+1,ptr2=ptr+1;--j;ptr2++)
			{
				LocalAvgSim+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
				NbComp+=1.0;
			}
		}
		LocalAvgSim/=NbComp;
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
		LocalAvgSim+=tmp/NbComp;
	}

	// Computed the average similairies of the configurations
	Cur.Set(Used);
	for(Cur.Start(),AvgSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeAvgSim();
		AvgSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalAvgSim+=tmp;
	}
	AvgSim/=((double)Used.NbPtr);
	if(grp1)
	{
		if(grp2)
			LocalAvgSim/=((double)Used.NbPtr-1);
		else
			LocalAvgSim/=((double)Used.NbPtr+1);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateJ(GGroupIR* grp1,GGroupIR* grp2)
{
	unsigned int i,j;
	GGroupIRCursor Cur;
	double max,localmax;
	GGroupIRCursor Cur2;
	GObjIR* rel1;
	GObjIR* rel2;
	double tmp;

	// Compute (Sum Intra) for the configurations.
	Cur.Set(Used);
	for(Cur.Start(),LocalAvgSim=AvgSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSumDist();
		AvgSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalAvgSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSumDist(thObjs1,NbObjs1,rel1);
		LocalAvgSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSumDist(thObjs2,NbObjs2,rel2);
		LocalAvgSim+=tmp;
	}

	// Compute Max intra for the configurations.
	Cur.Set(Used);
	Cur2.Set(Used);
	for(Cur.Start(),i=0,j=Cur.GetNb(),max=0.0,localmax=0.0;--j;Cur.Next(),i++)
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),Cur2()->Relevant->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp>max)
				max=tmp;
			if((grp1&&(Cur2()==grp1))||(grp2&&(Cur2()==grp2)))
				continue;
			if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2)))
				continue;
			if(tmp>localmax)
				localmax=tmp;
		}
		if(grp1)
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp>localmax)
				localmax=tmp;
		}
		if(grp1&&(!grp2))
		{
			tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
			tmp=tmp*tmp;
			if(tmp>localmax)
				localmax=tmp;
		}
	}
	if(grp1)
	{
		tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel1->GetSubProfile());
		tmp=tmp*tmp;
		if(tmp>localmax)
			localmax=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=1-Sims->GetSim(Cur()->Relevant->GetSubProfile(),rel2->GetSubProfile());
		tmp=tmp*tmp;
		if(tmp>localmax)
			localmax=tmp;
	}

	// Compute (Sum Intra)/(Max intra) for the configurations.
	if(AvgSim)
		AvgSim=max/AvgSim;
	if(LocalAvgSim)
		LocalAvgSim=localmax/LocalAvgSim;
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
	for(Cur.Start(),AvgSim=LocalAvgSim=0.0;!Cur.End();Cur.Next())
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
		AvgSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalAvgSim+=tmp;
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
		LocalAvgSim+=MinIntra/MaxInter;
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
		LocalAvgSim+=MinIntra/MaxInter;
	}
	if(Used.NbPtr)
		AvgSim/=static_cast<double>(Used.NbPtr);
	if(grp1)
	{
		if(grp2)
			LocalAvgSim/=((double)Used.NbPtr-1);
		else
			LocalAvgSim/=((double)Used.NbPtr+1);
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
	for(Cur.Start(),AvgSim=LocalAvgSim=0.0,localfirst=first=true;!Cur.End();Cur.Next())
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
			AvgSim=tmp;
			first=false;
		}
		else
			if(tmp<AvgSim)
				AvgSim=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		if(localfirst)
		{
			LocalAvgSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalAvgSim)
				LocalAvgSim=tmp;
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
			LocalAvgSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalAvgSim)
				LocalAvgSim=tmp;
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
			LocalAvgSim=tmp;
			localfirst=false;
		}
		else
			if(tmp<LocalAvgSim)
				LocalAvgSim=tmp;
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
	for(Cur.Start(),MaxInter=LocalMaxInter=AvgSim=LocalAvgSim=0.0,MinIntra=LocalMinIntra=1.0;!Cur.End();Cur.Next())
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
		AvgSim=MinIntra/MaxInter;
	if(LocalMaxInter)
		LocalAvgSim=LocalMinIntra/LocalMaxInter;
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
	for(Cur.Start(),LocalAvgSim=AvgSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSumDist();
		AvgSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalAvgSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSumDist(thObjs1,NbObjs1,rel1);
		LocalAvgSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSumDist(thObjs2,NbObjs2,rel2);
		LocalAvgSim+=tmp;
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
	if(AvgSim)
		AvgSim=Used.NbPtr*sb/AvgSim;
	if(LocalAvgSim)
	{
		if(grp1)
		{
			if(grp2)
				LocalAvgSim=((double)Used.NbPtr-1)*localsb/LocalAvgSim;
			else
				LocalAvgSim=((double)Used.NbPtr+1)*localsb/LocalAvgSim;
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
	for(Cur.Start(),LocalAvgSim=AvgSim=0.0;!Cur.End();Cur.Next())
	{
		tmp=Cur()->ComputeRelevantSum();
		AvgSim+=tmp;
		if((grp1&&(Cur()==grp1))||(grp2&&(Cur()==grp2))) continue;
		LocalAvgSim+=tmp;
	}
	if(grp1)
	{
		tmp=ComputeRelevantSum(thObjs1,NbObjs1,rel1);
		LocalAvgSim+=tmp;
	}
	if(grp1&&(!grp2))
	{
		tmp=ComputeRelevantSum(thObjs2,NbObjs2,rel2);
		LocalAvgSim+=tmp;
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
		AvgSim/=Used.NbPtr*sb;
	if(localsb)
	{
		if(grp1)
		{
			if(grp2)
				LocalAvgSim/=((double)Used.NbPtr-1)*localsb;
			else
				LocalAvgSim/=((double)Used.NbPtr+1)*localsb;
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
			AvgSim=LocalAvgSim=0.0;
	}
}



//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateOKFactor(GGroupIR* grp1,GGroupIR* grp2)
{
	GSubProfilesSameGroupIR* same;
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
	for(Instance->SameGroups.Start(),LocalOKFactor=OKFactor=0.0;!Instance->SameGroups.End();Instance->SameGroups.Next())
	{
		same=Instance->SameGroups();
		a1=ObjectsAss[same->Id1];
		a2=ObjectsAss[same->Id2];
		if(a1!=a2) continue;
		OKFactor+=1.0;
		if(!grp1) continue;
		if(grp2)
		{
			if((a1==id1||a1==id2)&&(a2==id1||a2==id2))
				LocalOKFactor+=1.0;
		}
		else
		{
			if(a1==grp1->GetId())
			{
				// Go through thObjs1 and count (j) number of objects of same
				for(ptr=thObjs1,i=NbObjs1+1,j=0;--i;ptr++)
				{
					if(same->IsIn(*ptr)) j++;
				}

				// If j<>1 -> In Same group
				if(j!=1)
					LocalOKFactor+=1.0;
			}
			else
				LocalOKFactor+=1.0;
		}
	}
	if(Instance->SameGroups.NbPtr)
	{
		LocalOKFactor/=Instance->SameGroups.NbPtr;
		OKFactor/=Instance->SameGroups.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::EvaluateDiffFactor(GGroupIR* grp1,GGroupIR* grp2)
{
	GSubProfilesSameGroupIR* diff;
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
	for(Instance->DiffGroups.Start(),LocalDiffFactor=DiffFactor=0.0;!Instance->DiffGroups.End();Instance->DiffGroups.Next())
	{
		diff=Instance->DiffGroups();
		a1=ObjectsAss[diff->Id1];
		a2=ObjectsAss[diff->Id2];
		if(a1!=a2) continue;
		DiffFactor+=1.0;
		if(!grp1) continue;
		if(grp2)
		{
			if((a1==id1||a1==id2)&&(a2==id1||a2==id2))
				LocalDiffFactor+=1.0;
		}
		else
		{
			if(a1==grp1->GetId())
			{
				// Go through thObjs1 and count (j) number of objects of same
				for(ptr=thObjs1,i=NbObjs1+1,j=0;--i;ptr++)
				{
					if(diff->IsIn(*ptr)) j++;
				}

				// If j<>1 -> In Same group
				if(j!=1)
					LocalDiffFactor+=1.0;
			}
			else
				LocalDiffFactor+=1.0;
		}
	}
	if(Instance->DiffGroups.NbPtr)
	{
		LocalDiffFactor/=Instance->SameGroups.NbPtr;
		DiffFactor/=Instance->SameGroups.NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(void)
{
	GGroupIRCursor Cur1;
	unsigned int i;

	// Some groups must exists
	if(!Used.NbPtr) return;

	// Compute Social factor.
	Cur1.Set(Used);
	for(Cur1.Start(),SocialFactor=0.0;!Cur1.End();Cur1.Next())
	{
		if(Cur1()->NbSubObjects!=1) continue;
		if(!Instance->NoSocialSubProfiles.IsIn<const unsigned int>(GetObj(Cur1()->SubObjects)->GetId()))
			SocialFactor+=1.0;
	}
	i=Objs->GetNb()-Instance->NoSocialSubProfiles.NbPtr;
	if(i)
		SocialFactor/=i;

	// Compute Average number of profiles.
	AvgProf=((double)Objs->GetNb())/((double)Used.NbPtr);

	EvaluateSim();
	EvaluateOKFactor();
	EvaluateDiffFactor();
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::LocalOptimisation(void)
{
	bool Cont;
	GObjIR* obj;
	GGroupIRCursor Cur1,Cur2;
	unsigned int i,j;
	bool** ptr;
	GGroupIR* grp=0;
	double tmp,max;

	// Init Pairs
	for(i=NbRows+1,ptr=Pairs;--i;ptr++)
		memset(*ptr,0x0,(i-1)*sizeof(bool));
	Cur1.Set(Used);
	Cur2.Set(Used);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		for(Cur2.GoTo(i+1);(!Cur2.End())&&(!Cont);Cur2.Next())
			Set(Cur1()->Id,Cur2()->Id,true);

	// Try to regroup groups until it is not possible anymore
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		Cur2.Set(Used);
		for(Cur1.Start(),i=0,j=Cur1.GetNb();(--j)&&(!Cont);Cur1.Next(),i++)
		{
			for(Cur2.GoTo(i+1);(!Cur2.End())&&(!Cont);Cur2.Next())
			{
				if(!Get(Cur1()->Id,Cur2()->Id)) continue;
				if(MergeGroups(Cur1(),Cur2(),3))
				{
					Cont=true;
				}
				else
					Set(Cur1()->Id,Cur2()->Id,false);
			}
		}
	}

	// Go through the groups where there are only one subprofile and find
	// another if it is social.
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		Cur2.Set(Used);
		for(Cur1.Start();(!Cur1.End())&&(!Cont);Cur1.Next())
		{
			if(Cur1()->NbSubObjects!=1) continue;
			obj=GetObj(Cur1()->SubObjects);
			if(!obj->GetSubProfile()->GetProfile()->IsSocial()) continue;
			for(Cur2.Start(),max=-1.0,grp=0;!Cur2.End();Cur2.Next())
			{
				if(Cur1()==Cur2()) continue;
				if(!Cur2()->CanInsert(obj)) continue;
				tmp=Cur2()->ComputeAvgSim(obj);
				if(tmp>max)
				{
					max=tmp;
					grp=Cur2();
				}
			}
			if(grp)
			{
				Cont=true;
				ReleaseGroup(Cur1());
				grp->Insert(obj);
			}
		}
	}
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
	ReOrganisation();
	return(b);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Modify(void)
{
	return(RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Mutation());
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Set(unsigned int id1,unsigned int id2,bool s)
{
	if(id1<id2)
		Pairs[id1][id2-id1-1]=s;
	else
		Pairs[id2][id1-id2-1]=s;
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Get(unsigned int id1,unsigned int id2)
{
	if(id1<id2)
		return(Pairs[id1][id2-id1-1]);
	else
		return(Pairs[id2][id1-id2-1]);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Set(unsigned int id1,bool s)
{
	unsigned int i;

	for(i=0;i<=id1;i++)
	{
		if(i<id1)
			Pairs[i][id1-i-1]=s;
		if(i==id1)
		{
			memset(Pairs[id1],0x0,(NbRows-i-2)*sizeof(bool));
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::ReOrganisation(void)
{
	bool Cont;
	GGroupIRCursor Cur1,Cur2;
	unsigned int i,j;
	bool** ptr;

//	cout<<"ReOrganisation Gen="<<Instance->Gen<<" ; Id="<<Id<<endl;
	Cur1.Set(Used);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
		Cur1()->Done=false;

	// Try to divided groups until it is not possible anymore
//	cout<<"\tDivision... ";
//	memset(*ptr,0x0,NbRows*sizeof(bool));
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		for(Cur1.Start();(!Cur1.End())&&(!Cont);Cur1.Next())
		{
			if(Cur1()->Done) continue;
			if(DivideGroup(Cur1(),2))
				Cont=true;
			else
				Cur1()->Done=true;
		}
	}
//	cout<<"Done"<<endl;

	// Init Pairs
	for(i=NbRows+1,ptr=Pairs;--i;ptr++)
		memset(*ptr,0x0,(i-1)*sizeof(bool));
	Cur1.Set(Used);
	Cur2.Set(Used);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		for(Cur2.GoTo(i+1);(!Cur2.End())&&(!Cont);Cur2.Next())
			Set(Cur1()->Id,Cur2()->Id,true);

	// Try to regroup groups until it is not possible anymore
//	cout<<"\tRegroups... ";
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		Cur2.Set(Used);
		for(Cur1.Start(),i=0,j=Cur1.GetNb();(--j)&&(!Cont);Cur1.Next(),i++)
		{
			for(Cur2.GoTo(i+1);(!Cur2.End())&&(!Cont);Cur2.Next())
			{
				if(!Get(Cur1()->Id,Cur2()->Id)) continue;
				if(MergeGroups(Cur1(),Cur2(),2))
					Cont=true;
				else
					Set(Cur1()->Id,Cur2()->Id,false);
			}
		}
	}
//	cout<<"Done"<<endl;

//	cout<<"Done"<<endl;
	ComputeOrd();
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
	AvgSim=chromo.AvgSim;
	AvgProf=chromo.AvgProf;
	OKFactor=chromo.OKFactor;
	DiffFactor=chromo.DiffFactor;
	SocialFactor=chromo.SocialFactor;
	Global=chromo.Global;
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GChromoIR::~GChromoIR(void)
{
	unsigned int i;
	bool** ptr;

	for(i=NbRows+1,ptr=Pairs;--i;ptr++)
		delete[] (*ptr);
	delete[] Pairs;
}
