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
#include <groups/ggroupir.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/gobjir.h>
using namespace GALILEI;
using namespace RGGA;
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class GGroupIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupIR::GGroupIR(GGroupIR* grp)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>(grp), AvgSim(0.0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupIR::GGroupIR(GChromoIR* owner,const unsigned int id,const GGroupDataIR* data)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>(owner,id,data), AvgSim(0.0)
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
void GALILEI::GGroupIR::DoOptimisation(void)
{
	unsigned int Nb;
	unsigned int i;
	GObjIR** ptr;
	double Avg;
	GSubProfile* sub;
	GSubProfile* obj;
	GObjIR** tab;
	GGroupIR* grp;

	if(NbSubObjects<=1) return;
	tab=new GObjIR*[NbSubObjects];
	sub=Owner->GetObj(SubObjects+Owner->Instance->RRand(NbSubObjects))->GetSubProfile();
	Avg=0.0;
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
	{
		obj=(*ptr)->GetSubProfile();
		if(obj==sub) continue;
		Avg+=Owner->Sims->GetSim(sub,obj);
	}
	Avg/=NbSubObjects-1;
	Nb=0;
	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
	{
		obj=(*ptr)->GetSubProfile();
		if(obj==sub) continue;
		if(Owner->Sims->GetSim(sub,obj)>Avg)
		{
			tab[Nb++]=(*ptr);
		}
	}
	if(Nb)
	{
		grp=Owner->ReserveGroup();
		for(i=Nb+1,ptr=tab;--i;ptr++)
		{
			Delete(*ptr);
			grp->Insert(*ptr);
		}
	}
	delete[] tab;
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
