/*

	GALILEI Research Project

	GGroupCalcRelevant.cpp

	Group Description is Relevant SubProfile Point Computing Method - Implementation.

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
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroupcalcrelevant.h>
#include <groups/ggroupvector.h>
#include <profiles/gsubprofilevector.h>
#include <sessions/gsession.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GCalcRelevantParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GCalcRelevantParams::GCalcRelevantParams(void)
	: GGroupCalcParams("Relevant SubProfile")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GCalcRelevantParams::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(GlobalSim) c='1'; else c='0';
	sprintf(tmp,"%u %c",MaxNonZero,c);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GCalcRelevantParams::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %c",&MaxNonZero,&c);
	if(c=='1') GlobalSim=true; else GlobalSim=false;
}


//-----------------------------------------------------------------------------
//
//  class GGroupCalcRelevant
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupCalcRelevant::GGroupCalcRelevant(GSession* session, GCalcRelevantParams* p) throw(bad_alloc)
	: GGroupCalc("Relevant SubProfile",session), Params(p), Order(0)
{
	//init parameters
	Params->GlobalSim=true;
	Params->MaxNonZero=60;
	Params->MaxOrderSize=5000;

	Order=new GIWordWeight*[Params->MaxOrderSize];
}


//---------------------------------------------------------------------------
double GALILEI::GGroupCalcRelevant::ComputeSumSim(GGroup* grp,GSubProfile* sub)
{
	unsigned int i;
	GSubProfile** ptr;
	double AvgSim;

	for(i=grp->NbPtr+1,ptr=grp->Tab,AvgSim=0.0;--i;ptr++)
	{
		if((*ptr)==sub) continue;
		if(Params->GlobalSim)
			AvgSim+=sub->GlobalSimilarity(*ptr);
		else
			AvgSim+=sub->Similarity(*ptr);
	}
	return(AvgSim);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcRelevant::Compute(GGroup* grp)
{
	unsigned int i;
	GSubProfile** ptr;
	GSubProfile* Relevant;
	double BestSumSim;
	double SumSim;
	GIWordsWeights* Desc=static_cast<GGroupVector*>(grp)->GetVector();
	GIWordsWeights* Ref;
	GIWordWeight** w;

	// Clear the Vector.
	(static_cast<GGroupVector*>(grp))->RemoveRefs();
	Desc->Clear();

	// If no subprofiles -> No relevant one.
	if(!grp->NbPtr) return;

	// Suppose the first subprofile is the most relevant.
	ptr=grp->Tab;
	Relevant=(*ptr);
	BestSumSim=ComputeSumSim(grp,Relevant);

	// Look if in the other subprofiles, there is a better one
	for(i=grp->NbPtr,ptr++;--i;ptr++)
	{
		SumSim=ComputeSumSim(grp,*ptr);
		if(SumSim>BestSumSim)
		{
			Relevant=(*ptr);
			BestSumSim=SumSim;
		}
	}

	// Copy the information of the relevant subprofile to the group.
	Ref=static_cast<GSubProfileVector*>(Relevant)->GetVector();
	if(Ref->IsEmpty()) return;
	if(Ref->NbPtr+1>Params->MaxOrderSize)
	{
		if(Order) delete[] Order;
		Params->MaxOrderSize=static_cast<unsigned int>((Ref->NbPtr+1)*1.1);
		Order=new GIWordWeight*[Params->MaxOrderSize];
	}
	memcpy(Order,Ref->Tab,Ref->NbPtr*sizeof(GIWordWeight*));
	qsort(static_cast<void*>(Order),Ref->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
	Order[Ref->NbPtr]=0;
	if(Params->MaxNonZero)
	{
		for(i=Params->MaxNonZero+1,w=Order;(--i)&&(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight(*w));
		}
	}
	else
	{
		for(w=Order;(*w);w++)
		{
			if((*w)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight(*w));
		}
	}

	// Update the references of the vector.
	(static_cast<GGroupVector*>(grp))->UpdateRefs();
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupCalcRelevant::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(Params->GlobalSim) c='1'; else c='0';
	sprintf(tmp,"%u %c",Params->MaxNonZero,c);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcRelevant::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %c",&Params->MaxNonZero,&c);
	if(c=='1') Params->GlobalSim=true; else Params->GlobalSim=false;
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalcRelevant::~GGroupCalcRelevant(void)
{
	if(Order) delete[] Order;
}
