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
// class GChromoIR::GroupsPair
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//class GALILEI::GChromoIR::GroupsPair
//{
//public:
//	GGroupIR* Grp1;
//	GGroupIR* Grp2;
//
//	GroupsPair(GGroupIR* g1,GGroupIR* g2) : Grp1(g1), Grp2(g2) {}
//	GroupsPair(GroupsPair* p) : Grp1(p->Grp1), Grp2(p->Grp2) {}
//	int Compare(const GroupsPair*) const {return(-1);}
//};



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
	NbRows=200;
	Pairs=new bool*[NbRows];
	(*Pairs)=new bool[NbRows];
	for(i=NbRows,ptr=Pairs+1;--i;ptr++)
		(*ptr)=new bool[i-1];
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeSumSim(GObjIR** grp,unsigned int nb,GObjIR* obj)
{
	unsigned int i;
	double AvgSim;

	AvgSim=0.0;
	for(i=nb+1;--i;grp++)
	{
		if((*grp)==obj) continue;
		AvgSim+=1-Sims->GetSim(obj->GetSubProfile(),(*grp)->GetSubProfile());
	}
	return(AvgSim);
}


//---------------------------------------------------------------------------
double GALILEI::GChromoIR::ComputeRelevant(GObjIR** grp,unsigned int nb,GSubProfile* &rel)
{
	unsigned int i;
	double BestSumSim;
	double SumSim;
	GObjIR** ptr;

	// Suppose the first element is the most relevant.
	rel=(*grp)->GetSubProfile();
	ptr=grp;
	BestSumSim=ComputeSumSim(grp,nb,*ptr);

	// Look if in the other objects, there is a better one
	for(i=nb;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,nb,*ptr);
		if(SumSim>BestSumSim)
		{
			rel=(*ptr)->GetSubProfile();
			BestSumSim=SumSim;
		}
	}

	// Return results
	AvgSim=BestSumSim;
	return(BestSumSim);
}


//-----------------------------------------------------------------------------
bool GALILEI::GChromoIR::MergeGroups(GGroupIR* grp1,GGroupIR* grp2)
{
	GObjIR** ptr;
	GObjIR** ptr2;
	unsigned int i,j;
	double LocalAvgSim,NbComp,LocalOKFactor,max,tmp,localmax;
	int NbCrit=0;
	GSubProfilesSameGroupIR* same;
	unsigned int id1=grp1->Id;
	unsigned int id2=grp2->Id;
	unsigned int a1,a2;
	unsigned NbObjs1;
	GGroupIRCursor Cur1,Cur2;
	GSubProfile* rel1;

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

	// Compute Avg Similarity for the new created group
//	for(ptr=thObjs1,i=NbObjs1,NbComp=LocalAvgSim=0.0;--i;ptr++)
//	{
//		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
//		{
//			LocalAvgSim+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
//			NbComp+=1.0;
//		}
//	}
//	LocalAvgSim/=NbComp;
//	if(LocalAvgSim>(grp1->ComputeAvgSim()+grp2->ComputeAvgSim())/2)
//		NbCrit++;

	// Compute (Sum Intra)/(Max intra).
	LocalAvgSim=AvgSim=0.0;
	for(Used.Start();!Used.End();Used.Next())
	{
		AvgSim+=Used()->ComputeRelevant();
		if((Used()==grp1)||(Used()==grp2))
			continue;
		LocalAvgSim+=Used()->ComputeRelevant();
	}
	LocalAvgSim+=ComputeRelevant(thObjs1,NbObjs1,rel1);
	Cur1.Set(Used);
	Cur2.Set(Used);
	for(Cur1.Start(),i=0,j=Cur1.GetNb(),max=0.0,localmax=0.0;--j;Cur1.Next(),i++)
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=Sims->GetSim(Cur1()->Relevant,Cur2()->Relevant);
			if(tmp>max)
				max=tmp;
			if((Cur2()==grp1)||(Cur2()==grp2))
				continue;
			if((Cur1()==grp1)||(Cur1()==grp2))
				continue;
			if(tmp>localmax)
				localmax=tmp;
		}
		tmp=Sims->GetSim(Cur1()->Relevant,rel1);
		if(tmp>localmax)
			localmax=tmp;
	}
	tmp=Sims->GetSim(Cur1()->Relevant,rel1);
	if(tmp>localmax)
		localmax=tmp;
	if(max)
		AvgSim/=max;
	if(localmax)
		LocalAvgSim/=localmax;
	if(LocalAvgSim>AvgSim)
		NbCrit++;

	// Number of subprofiles having common OK documents and being in the same group.
	if(Instance->SameGroups.NbPtr)
	{
		LocalOKFactor=OKFactor=0.0;
		for(Instance->SameGroups.Start();!Instance->SameGroups.End();Instance->SameGroups.Next())
		{
			same=Instance->SameGroups();
			a1=ObjectsAss[same->Id1];
			a2=ObjectsAss[same->Id2];
			if((a1==a2)||((a1==id1||a1==id2)&&(a2==id1||a2==id2)))
				LocalOKFactor+=1.0;
			if(a1==a2)
				OKFactor+=1.0;
		}
		LocalOKFactor/=Instance->SameGroups.NbPtr;
		OKFactor/=Instance->SameGroups.NbPtr;
		if(LocalOKFactor>OKFactor)
			NbCrit++;
	}
	else
	{
		OKFactor=LocalOKFactor=1.0;
		NbCrit++;
	}

	// Look number of criteria ameliorated
	if(NbCrit)
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
	int NbCrit=0;
	double LocalAvgSim,LocalAvgSim2,NbComp,LocalDiffFactor,tmp,max,localmax;
	unsigned int a1,a2;
	GSubProfilesSameGroupIR* same;
	unsigned NbObjs1,NbObjs2;
	GGroupIRCursor Cur1,Cur2;
	GSubProfile* rel1;
	GSubProfile* rel2;

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
//	for(ptr=thObjs1,i=NbObjs1,LocalAvgSim1=NbComp=0.0;--i;ptr++)
//	{
//		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
//		{
//			LocalAvgSim1+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
//			NbComp+=1.0;
//		}
//	}
//	LocalAvgSim1/=NbComp;
//	for(ptr=thObjs2,i=NbObjs2,LocalAvgSim2=NbComp=0.0;--i;ptr++)
//	{
//		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
//		{
//			LocalAvgSim2+=Sims->GetSim((*ptr)->GetSubProfile(),(*ptr2)->GetSubProfile());
//			NbComp+=1.0;
//		}
//	}
//	LocalAvgSim2/=NbComp;
//	if((LocalAvgSim1+LocalAvgSim2)/2>grp->ComputeAvgSim())
//		NbCrit++;

	// Compute (Sum Intra)/(Max intra).
	LocalAvgSim=AvgSim=0.0;
	for(Used.Start();!Used.End();Used.Next())
	{
		AvgSim+=Used()->ComputeRelevant();
		if(Used()==grp)
			continue;
		LocalAvgSim+=Used()->ComputeRelevant();
	}
	LocalAvgSim+=ComputeRelevant(thObjs1,NbObjs1,rel1);
	LocalAvgSim+=ComputeRelevant(thObjs2,NbObjs2,rel2);
	Cur1.Set(Used);
	Cur2.Set(Used);
	for(Cur1.Start(),i=0,j=Cur1.GetNb(),max=0.0,localmax=0.0;--j;Cur1.Next(),i++)
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=Sims->GetSim(Cur1()->Relevant,Cur2()->Relevant);
			if(tmp>max)
				max=tmp;
			if((Cur2()==grp)||(Cur1()==grp))
				continue;
			if(tmp>localmax)
				localmax=tmp;
		}
		tmp=Sims->GetSim(Cur1()->Relevant,rel1);
		if(tmp>localmax)
			localmax=tmp;
		tmp=Sims->GetSim(Cur1()->Relevant,rel2);
		if(tmp>localmax)
			localmax=tmp;
	}
	tmp=Sims->GetSim(Cur1()->Relevant,rel1);
	if(tmp>localmax)
		localmax=tmp;
	tmp=Sims->GetSim(Cur1()->Relevant,rel2);
	if(tmp>localmax)
		localmax=tmp;
	if(max)
		AvgSim/=max;
	if(localmax)
		LocalAvgSim/=localmax;
	if(LocalAvgSim>AvgSim)
		NbCrit++;

	// Number of subprofiles having common documents with different judgment and being in the same group.
	DiffFactor=LocalDiffFactor=0.0;
	if(Instance->DiffGroups.NbPtr)
	{
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
		LocalDiffFactor/=Instance->DiffGroups.NbPtr;
		DiffFactor/=Instance->DiffGroups.NbPtr;
		if(LocalDiffFactor<DiffFactor)
			NbCrit++;
	}
	else
		NbCrit++;

	// Look number of criteria ameliorated
	if(NbCrit==2)
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
void GALILEI::GChromoIR::Evaluate(void)
{
	GSubProfilesSameGroupIR* ptr;
	GGroupIRCursor Cur1,Cur2;
	double tmp;
	double max;
	unsigned int i,j;

	// Some groups must exists
	if(!Used.NbPtr) return;

	// Compute Average number of profiles.
	AvgProf=((double)Objs->GetNb())/((double)Used.NbPtr);

	// Compute Average Similarity.
//	AvgSim=0.0;
//	for(Used.Start();!Used.End();Used.Next())
//	{
//		AvgSim+=Used()->ComputeAvgSim();
//	}
//	AvgSim/=((double)Used.NbPtr);

	// Compute (Sum Intra)/(Max intra).
	AvgSim=0.0;
	for(Used.Start();!Used.End();Used.Next())
	{
		AvgSim+=Used()->ComputeRelevant();
	}
	Cur1.Set(Used);
	Cur2.Set(Used);
	for(Cur1.Start(),i=0,j=Cur1.GetNb(),max=0.0;--j;Cur1.Next(),i++)
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			tmp=Sims->GetSim(Cur1()->Relevant,Cur2()->Relevant);
			if(tmp>max)
				max=tmp;
		}
	if(max)
		AvgSim/=max;

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
	for(Cur1.Start();(!Cur1.End())&&(!Cont);Cur1.Next())
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
			if(DivideGroup(Cur1()))
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
	for(Cur1.Start(),i=0,j=Cur1.GetNb();(--j)&&(!Cont);Cur1.Next(),i++)
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
				if(MergeGroups(Cur1(),Cur2()))
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
