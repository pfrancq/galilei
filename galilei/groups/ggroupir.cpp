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
double GALILEI::GGroupIR::ComputeAvgSim(void)
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
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),AvgSim=0.0;--i;ptr++)
	{
		if((*ptr)==obj) continue;
		AvgSim+=Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile());
	}
	return(AvgSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeMaxSim(GObjIR* obj)
{
	unsigned int i;
	GObjIR** ptr;
	double MaxSim,tmp;
	GSubProfile* sub;

	sub=obj->GetSubProfile();
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),MaxSim=0.0;--i;ptr++)
	{
		if((*ptr)==obj) continue;
		tmp=Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile());
		if(tmp>MaxSim)
			MaxSim=tmp;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeMinSim(GObjIR* obj)
{
	unsigned int i;
	GObjIR** ptr;
	double MinSim,tmp;
	GSubProfile* sub;

	sub=obj->GetSubProfile();
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),MinSim=1.0;--i;ptr++)
	{
		if((*ptr)==obj) continue;
		tmp=Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile());
		if(tmp<MinSim)
			MinSim=tmp;
	}
	return(MinSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeAvgSim(GObjIR* obj)
{
	unsigned int i;
	GObjIR** ptr;
	double AvgSim,NbComp;
	GSubProfile* sub;

	sub=obj->GetSubProfile();
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),NbComp=AvgSim=0.0;--i;ptr++)
	{
		if((*ptr)==obj) continue;
		AvgSim+=Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile());
		NbComp+=1.0;
	}
	return(AvgSim/NbComp);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeMaxSim(GGroupIR* grp)
{
	unsigned int i;
	GObjIR** ptr;
	double MaxSim,tmp;

	for(i=grp->NbSubObjects+1,ptr=Owner->GetObjs(grp->SubObjects),MaxSim=0.0;--i;ptr++)
	{
		tmp=ComputeMaxSim(*ptr);
		if(tmp>MaxSim)
			MaxSim=tmp;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeMaxSim(GObjIR** grp,unsigned int nb)
{
	double MaxSim,tmp;

	for(nb++,MaxSim=0.0;--nb;grp++)
	{
		tmp=ComputeMaxSim(*grp);
		if(tmp>MaxSim)
			MaxSim=tmp;
	}
	return(MaxSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeMinSim(void)
{
	unsigned int i;
	GObjIR** ptr;
	double MinSim,tmp;

	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects),MinSim=1.0;--i;ptr++)
	{
		tmp=ComputeMinSim(*ptr);
		if(tmp<MinSim)
			MinSim=tmp;
	}
	return(MinSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeRelevantSum(void)
{
	unsigned int i;
	GObjIR** ptr;
	double BestSumSim;
	double SumSim;

	// If no objects -> No relevant one.
	if(!NbSubObjects)
	{
		Relevant=0;
		return(0.0);
	}

	// Suppose the first element is the most relevant.
	ptr=Owner->GetObjs(SubObjects);
	Relevant=(*ptr)->GetSubProfile();
	BestSumSim=ComputeSumSim(*ptr);

	// Look if in the other objects, there is a better one
	for(i=NbSubObjects,ptr++;--i;ptr++)
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
double GALILEI::GGroupIR::ComputeRelevantMax(void)
{
	unsigned int i;
	GObjIR** ptr;
	double BestSumSim;
 	double MaxSumSim;
	double SumSim;

	// If no objects -> No relevant one.
	if(!NbSubObjects)
	{
		Relevant=0;
		return(0.0);
	}

	// Suppose the first element is the most relevant.
	ptr=Owner->GetObjs(SubObjects);
	Relevant=(*ptr)->GetSubProfile();
	BestSumSim=ComputeSumSim(*ptr);
	MaxSumSim=ComputeMaxSim(*ptr);

	// Look if in the other objects, there is a better one
	for(i=NbSubObjects,ptr++;--i;ptr++)
	{
		SumSim=ComputeSumSim(*ptr);
		if(SumSim>BestSumSim)
		{
			Relevant=(*ptr)->GetSubProfile();
			BestSumSim=SumSim;
			MaxSumSim=ComputeMaxSim(*ptr);
		}
	}

	// Return results
	return(MaxSumSim);
}


//---------------------------------------------------------------------------
double GALILEI::GGroupIR::ComputeNonRelevant(void)
{
	unsigned int i;
	GObjIR** ptr;
	double BestSumSim;
	double SumSim;

	// If no objects -> No relevant one.
	if(!NbSubObjects) return(0.0);

	// Suppose the first element is the most non-relevant.
	ptr=Owner->GetObjs(SubObjects);
	BestSumSim=ComputeSumSim(*ptr);

	// Look if in the other objects, there is a better one
	for(i=NbSubObjects,ptr++;--i;ptr++)
	{
		SumSim=ComputeSumSim(*ptr);
		if(SumSim<BestSumSim)
			BestSumSim=SumSim;
	}

	// Return results
	return(BestSumSim);
}


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
