/*

	GALILEI Research Project

	GChromoIR.cpp

	Chromosome for an IR Problem - Implementation

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
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/gobjir.h>
#include <profiles/gsubprofile.h>
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
    Sims(0), MinSimLevel(0), AvgSim(0.0), AvgProf(0.0), OKFactor(0.0), DiffFactor(1.0)
{
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Init(GThreadDataIR* thData) throw(bad_alloc)
{
	// Parent Initialisation
	RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Init(thData);
	Sims=Instance->Sims;
	MinSimLevel=Instance->MinSimLevel;
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::MergeGroups(GGroupIR* grp1,GGroupIR* grp2)
{
	GObjIR** ptr;
	GObjIR** ptr2;
	unsigned int i,j;
	double LocalAvgSim,NbComp,LocalOKFactor;
	int NbCrit=0;
	GSubProfilesSameGroupIR* sub;
	unsigned int id1=grp1->Id;
	unsigned int id2=grp2->Id;
	unsigned int a1,a2;
	unsigned int NbObjs1;

	// Put in thObjs1 the merge of the two groups
	for(ptr2=grp1->GetObjects(),ptr=thObjs1,i=grp1->GetNbObjs()+1,NbObjs1=0;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);
	for(ptr2=grp2->GetObjects(),i=grp2->GetNbObjs()+1;--i;ptr2++,ptr++,NbObjs1++)
		(*ptr)=(*ptr2);

	// Compute Avg Similarity for the new created group
	for(ptr=thObjs1,i=NbObjs1,NbComp=LocalAvgSim=0.0;--i;ptr++)
	{
		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
		{
			LocalAvgSim+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
			NbComp+=1.0;
		}
	}
	LocalAvgSim/=NbComp;
	if(LocalAvgSim>(grp1->ComputeAvgSim()+grp2->ComputeAvgSim())/2)
	{
		NbCrit++;
	}

	// Number of subprofiles having common OK documents and being in the same group.
	if(Instance->SameGroups.NbPtr)
	{
		LocalOKFactor=OKFactor=0.0;
		for(Instance->SameGroups.Start();!Instance->SameGroups.End();Instance->SameGroups.Next())
		{
			sub=Instance->SameGroups();
			a1=ObjectsAss[sub->Id1];
			a2=ObjectsAss[sub->Id2];
			if((a1==a2)||((a1==id1||a1==id2)&&(a2==id1||a2==id2)))
				LocalOKFactor+=1.0;
			if(a1==a2)
				OKFactor+=1.0;
		}
		LocalOKFactor/=Instance->SameGroups.NbPtr;
		OKFactor/=Instance->SameGroups.NbPtr;
	}
	else
	{
		OKFactor=LocalOKFactor=1.0;
	}
	if(LocalOKFactor>OKFactor)
	{
		NbCrit++;
	}

	// Look number of criteria ameliorated
	if(NbCrit>Instance->RRand(2))
	{
		// Release grp1 and grp2
		ReleaseGroup(grp1);
		ReleaseGroup(grp2);

		// Reserve a new group and insert the elements of thObjs1
		grp1=ReserveGroup();
		for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
			grp1->Insert(*ptr);
		return(true);
	}

	return(false);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::DivideGroup(GGroupIR* grp)
{
	unsigned int i,j;
	GObjIR** ptr;
	GObjIR** ptr2;
	GObjIR** o1;
	GObjIR** o2;
	double Avg;
	GSubProfile* sub;
	GSubProfile* obj;
	unsigned int NbObjs1;
	unsigned int NbObjs2;
	int NbCrit=0;
	double LocalAvgSim1,LocalAvgSim2,NbComp,LocalDiffFactor;
	unsigned int a1,a2;
	GSubProfilesSameGroupIR* same;

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

	// Compute Avg Similarity for the two new created group
	for(ptr=thObjs1,i=NbObjs1,LocalAvgSim1=NbComp=0.0;--i;ptr++)
	{
		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
		{
			LocalAvgSim1+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
			NbComp+=1.0;
		}
	}
	LocalAvgSim1/=NbComp;
	for(ptr=thObjs2,i=NbObjs2,LocalAvgSim2=NbComp=0.0;--i;ptr++)
	{
		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
		{
			LocalAvgSim2+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
			NbComp+=1.0;
		}
	}
	LocalAvgSim2/=NbComp;
	if((LocalAvgSim1+LocalAvgSim2)/2>grp->ComputeAvgSim())
	{
		NbCrit++;
	}

	// Number of subprofiles having common documents with different judgment and being in the same group.
	DiffFactor=LocalDiffFactor=0.0;
	for(Instance->DiffGroups.Start();!Instance->DiffGroups.End();Instance->DiffGroups.Next())
	{
		same=Instance->DiffGroups();
		a1=ObjectsAss[same->Id1];
		a2=ObjectsAss[same->Id2];
		if(a1==a2)
		{
			DiffFactor+=1.0;
			if(a1==grp->GetId())
			{
				// Go through thObjs1 and count (j) number of objects of same
				for(ptr=thObjs1,i=NbObjs1+1,j=0;--i;ptr++)
				{
					if(same->IsIn(*ptr)) j++;
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
		LocalDiffFactor/=Instance->DiffGroups.NbPtr;
		DiffFactor/=Instance->DiffGroups.NbPtr;
	}
	if(LocalDiffFactor<DiffFactor)
		NbCrit++;

	// Look number of criteria ameliorated
	if(NbCrit>Instance->RRand(2))
	{
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
	bool b;

	// Look if already a solution in the session
	if(Instance->CurrentGroups)
		 ConstructChromo(Instance->CurrentGroups);

	// Call classic heuristic for non-assigned objects
	b=RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::RandomConstruct();

	// If not first element, do a mutation
	if(Instance->CurrentGroups&&Id)
		b=RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Mutation();

	return(b);
}


//-----------------------------------------------------------------------------
void GALILEI::GChromoIR::Evaluate(void)
{
	GSubProfilesSameGroupIR* ptr;

	// Some groups must exists
	if(!Used.NbPtr)
	{
		OKFactor=AvgSim=AvgProf=0.0;
		DiffFactor=1.0;
		return;
	}

	// Compute Average Similarity and the average number of profiles.
	AvgSim=AvgProf=0.0;
	for(Used.Start();!Used.End();Used.Next())
	{
		AvgSim+=Used()->ComputeAvgSim();
		AvgProf+=Used()->NbSubObjects;
	}
	AvgSim/=((double)Used.NbPtr);
	AvgProf/=((double)Used.NbPtr);

	// Number of subprofiles having common OK documents and being in the same group.
	if(Instance->SameGroups.NbPtr)
	{
		OKFactor=0.0;
		for(Instance->SameGroups.Start();!Instance->SameGroups.End();Instance->SameGroups.Next())
		{
			ptr=Instance->SameGroups();
			if(ObjectsAss[ptr->Id1]==ObjectsAss[ptr->Id2])
				OKFactor+=1.0;
		}
		OKFactor/=Instance->SameGroups.NbPtr;
	}
	else
	{
		OKFactor=1.0;
	}

	// Number of subprofiles having common documents with different judgment and being in the same group.
	DiffFactor=0.0;
	for(Instance->DiffGroups.Start();!Instance->DiffGroups.End();Instance->DiffGroups.Next())
	{
		ptr=Instance->DiffGroups();
		if(ObjectsAss[ptr->Id1]==ObjectsAss[ptr->Id2])
			DiffFactor+=1.0;
	}
	if(Instance->DiffGroups.NbPtr)
	{
		DiffFactor/=Instance->DiffGroups.NbPtr;
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::Crossover(GChromoIR* parent1,GChromoIR* parent2)
{
	bool b;

	b=RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>::Crossover(parent1,parent2);
	ReOrganisation();
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
void GALILEI::GChromoIR::ReOrganisation(void)
{
	bool Cont;
	GGroupIRCursor Cur1,Cur2;
	unsigned int i,j;

	// Try to divided groups until it is not possible anymore
	for(Cont=true;Cont;)
	{
		Cont=false;
		Cur1.Set(Used);
		for(Cur1.Start();(!Cur1.End())&&(!Cont);Cur1.Next())
		{
			if(DivideGroup(Cur1()))
				Cont=true;
		}
	}

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
				if(MergeGroups(Cur1(),Cur2()))
					Cont=true;
			}
		}
	}

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
	return(*this);
}


//-----------------------------------------------------------------------------
GALILEI::GChromoIR::~GChromoIR(void)
{
}
