/*

	R Project Library

	RNodeGA.hh

	GA Node - Header.

	(C) 2001 by P. Francq.

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
// include files for GALILEI
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <groups/ggroupir.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/gobjir.h>
using namespace GALILEI;
using namespace RGGA;
using namespace RGA;


//-----------------------------------------------------------------------------
//
// class SubProfileLocal
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class SubProfileLocal
{
public:
	GObjIR* Prof;
	double AvgSim;
};


//-----------------------------------------------------------------------------
int sort_function(const void* a,const void* b)
{
	double af=(static_cast<const SubProfileLocal*>(a))->AvgSim;
	double bf=(static_cast<const SubProfileLocal*>(b))->AvgSim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//-----------------------------------------------------------------------------
//
// class GGroupIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupIR::GGroupIR(GGroupIR* grp)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>(grp), AvgSim(0.0),
	  Relevant(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupIR::GGroupIR(GChromoIR* owner,const unsigned int id,const GGroupDataIR* data)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>(owner,id,data), AvgSim(0.0),
	Relevant(0)
{
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::Verify(void)
{
	if(!RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>::Verify())
		return(false);
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::Clear(void)
{
	RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>::Clear();
	AvgSim=0.0;
	Relevant=0;
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::CanInsert(const GObjIR* obj)
{
	unsigned int i;
	GObjIR** ptr;
	GSubProfile* sub;

	sub=obj->GetSubProfile();
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
		if(Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile())<Owner->MinSimLevel)
			return(false);
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostInsert(const GObjIR* /*obj*/)
{
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostDelete(const GObjIR* /*obj*/)
{
}


//---------------------------------------------------------------------------
double  GALILEI::GGroupIR::ComputeAvgSim(void)
{
	unsigned int i,j;
	GObjIR** ptr;
	GObjIR** ptr2;
	double NbComp;
	GSubProfile* sub;

	if(!NbSubObjects)
	{
		AvgSim=0.0;
		return(AvgSim);
	}
	if(NbSubObjects==1)
	{
		AvgSim=1.0;
		return(AvgSim);
	}
	AvgSim=0.0;
	NbComp=0.0;
	for(i=NbSubObjects,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
	{
		sub=(*ptr)->GetSubProfile();
		for(j=i+1,ptr2=ptr+1;--j;ptr2++)
		{
			AvgSim+=Owner->Sims->GetSim(sub,(*ptr2)->GetSubProfile());
			NbComp+=1.0;
		}
	}
	AvgSim/=NbComp;
	return(AvgSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeSumSim(GObjIR* obj)
{
	unsigned int i;
	GObjIR** ptr;
	double AvgSim;
	GSubProfile* sub;

	sub=obj->GetSubProfile();
	AvgSim=0.0;
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
	{
		if((*ptr)==obj) continue;
		AvgSim+=1-Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile());
	}
	return(AvgSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeRelevant(void)
{
	unsigned int i;
	GObjIR** ptr;
	double BestSumSim;
	double SumSim;

	// If no objects -> No relevant one.
	if(!NbSubObjects)
	{
		Relevant=0;
		AvgSim=0.0;
		return(0.0);
	}

	// Suppose the first element is the most relevant.
	ptr=Owner->GetObjs(SubObjects);
	Relevant=(*ptr)->GetSubProfile();
	BestSumSim=ComputeSumSim(*ptr);

	// Look if in the other objects, there is a better one
	for(i=NbSubObjects;--i;ptr++)
	{
		SumSim=ComputeSumSim(*ptr);
		if(SumSim>BestSumSim)
		{
			Relevant=(*ptr)->GetSubProfile();
			BestSumSim=SumSim;
		}
	}

	// Return results
	AvgSim=BestSumSim;
	return(BestSumSim);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::DoOptimisation(void)
{
	unsigned int i,Pos;
	SubProfileLocal* tab;
	SubProfileLocal* t;
	GObjIR** ptr;
	GObjIR*/*GSubProfile*/ sub;
	GObjIR*/*GSubProfile*/ obj;
	double Avg,tmp;
	GGroupIR* grp;

	if(NbSubObjects<2) return;

	// Init: Construct an arry and select randomly a subprofile
	tab=new SubProfileLocal[NbSubObjects-1];
	sub=Owner->GetObj(SubObjects+Owner->Instance->RRand(NbSubObjects))/*->GetSubProfile()*/;
//	cout<<sub->GetProfile()->GetName()<<"\t";

	// Go the other profiles, compute the average similarity to the selected
	// one. Classify them in descending order.
	Avg=0.0;
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),t=tab;--i;ptr++)
	{
		obj=(*ptr)/*->GetSubProfile()*/;
		if(obj==sub) continue;
		t->Prof=obj;
		t->AvgSim=Owner->Sims->GetSim(sub->GetSubProfile(),obj->GetSubProfile());
		Avg+=t->AvgSim;
		t++;
	}
	qsort(static_cast<void*>(tab),NbSubObjects-1,sizeof(SubProfileLocal),sort_function);
	Avg/=NbSubObjects-1;

	// Go through the ordered subprofiles to find where eventually to cut
	Pos=i=0;
	t=tab;
	Avg=t->AvgSim;
//	cout<<t->Prof->GetProfile()->GetName()<<"="<<t->AvgSim<<"\t";
	for(t++,i++;i<NbSubObjects-1;t++,i++)
	{
		if(!Pos)
		{
			tmp=(Avg-t->AvgSim)/Avg;
			if(tmp>0.2)
			{
//				cout<<"\t|\t";
				Pos=i;
				break;
			}
			Avg=t->AvgSim;
		}
//		cout<<t->Prof->GetProfile()->GetName()<<"="<<t->AvgSim<<"\t";
	}
//	cout<<endl;


	// If cut found do it.
	if(Pos)
	{
		#ifdef RGADEBUG
			if(Owner->Instance->Debug)
			{
				Owner->Instance->Debug->BeginFunc("DoOptimisation","GGroupIR");
				Owner->Instance->Debug->PrintInfo("Divided a group");
				Owner->Instance->Debug->EndFunc("DoOptimisation","GGroupIR");
			}
		#endif

		grp=Owner->ReserveGroup();
		for(;i<NbSubObjects-1;t++,i++)
		{
			Delete(t->Prof);
			grp->Insert(t->Prof);
		}
	}

	// Delete the created array.
	delete[] tab;
}


//---------------------------------------------------------------------------
//void GALILEI::GGroupIR::DoOptimisation(void)
//{
//	unsigned int Nb;
//	unsigned int i;
//	GObjIR** ptr;
//	double Avg;
//	GSubProfile* sub;
//	GSubProfile* obj;
//	GObjIR** tab;
//	GGroupIR* grp;
//
//	if(NbSubObjects<2) return;
//	tab=new GObjIR*[NbSubObjects];
//	sub=Owner->GetObj(SubObjects+Owner->Instance->RRand(NbSubObjects))->GetSubProfile();
//	Avg=0.0;
//	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
//	{
//		obj=(*ptr)->GetSubProfile();
//		if(obj==sub) continue;
//		Avg+=Owner->Sims->GetSim(sub,obj);
//	}
//	Avg/=NbSubObjects-1;
//	Nb=0;
//	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
//	{
//		obj=(*ptr)->GetSubProfile();
//		if(obj==sub) continue;
//		if(Owner->Sims->GetSim(sub,obj)>Avg)
//		{
//			tab[Nb++]=(*ptr);
//		}
//	}
//	if(Nb)
//	{
//		grp=Owner->ReserveGroup();
//		for(i=Nb+1,ptr=tab;--i;ptr++)
//		{
//			Delete(*ptr);
//			grp->Insert(*ptr);
//		}
//	}
//	delete[] tab;
//}


//---------------------------------------------------------------------------
GGroupIR& GALILEI::GGroupIR::operator=(const GGroupIR& grp)
{
	RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>::operator=(grp);
	AvgSim=grp.AvgSim;
	return(*this);
}


//---------------------------------------------------------------------------
GALILEI::GGroupIR::~GGroupIR(void)
{
}
