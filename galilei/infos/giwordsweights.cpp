/*

	GALILEI Research Project

	GIWordsWeights.cpp

	Weights of a list of words - Implementation.

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
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gword.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GIWordsWeights
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GIWordsWeights::GIWordsWeights(unsigned int nb) throw(bad_alloc)
	: RContainer<GIWordWeight,unsigned,true,true>(nb,50), NbWordsDocs(0.0)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordsWeights::ClassName(void) const
{
	return("GIWordsWeights");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordsWeights::InfoType(void) const
{
	return(infoWordCalcs);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::Clear(void)
{
	RContainer<GIWordWeight,unsigned,true,true>::Clear();
	NbWordsDocs=0.0;
}


//--------------------------------------------------------------------------
int GALILEI::GIWordsWeights::sortOrder(const void *a,const void *b)
{
  double af=(*((GIWordWeight**)(a)))->Weight;
  double bf=(*((GIWordWeight**)(b)))->Weight;

  if(af==bf) return(0);
  if(af>bf)
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
double GALILEI::GIWordsWeights::GetMaxWeight(void) const
{
	double max;
	GIWordWeight** ptr;
	unsigned int i;

	// If no profile, maximal weight is null.
	if(!NbPtr) return(0.0);

	// Suppose first weight is the highest
	ptr=Tab;
	max=(*ptr)->Weight;

	// Look if there is a greather one.
	for(i=NbPtr,ptr++;--i;ptr++)
	{
		if((*ptr)->Weight>max)
			max=(*ptr)->Weight;
	}
	return(max);
}


//-----------------------------------------------------------------------------
double GALILEI::GIWordsWeights::Similarity(const GIWordsWeights* w) const
{
	double Sim=0.0;
	GIWordWeight** ptr=Tab;
	GIWordWeight** ptr2=w->Tab;
	unsigned int i=NbPtr+1;
	unsigned int j=w->NbPtr;
	double norm1=0.0;
	double norm2=0.0;

	while(--i)
	{
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			norm2+=(*ptr2)->Weight*(*ptr2)->Weight;
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			norm2+=(*ptr2)->Weight*(*ptr2)->Weight;
			Sim+=(*ptr)->Weight*(*ptr2)->Weight;
			ptr2++;
		}
		norm1+=(*ptr)->Weight*(*ptr)->Weight;
		ptr++;
	}
	while(j)
	{
		j--;
		norm2+=(*ptr2)->Weight*(*ptr2)->Weight;
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GIWordsWeights::SimilarityIdf(const GIWordsWeights* w,tObjType ObjType,GLang* lang) const
{
	double Sim=0.0;
	GIWordWeight** ptr=Tab;
	GIWordWeight** ptr2=w->Tab;
	unsigned int i=NbPtr+1;
	unsigned int j=w->NbPtr;
	double norm1=0.0;
	double norm2=0.0;
	double max1=GetMaxWeight();
	double max2=w->GetMaxWeight();
	double TotalRef=lang->GetRef(ObjType);
	double w1,w2;

	while(--i)
	{
		w1=((*ptr)->Weight/max1)*log(TotalRef/lang->GetRef((*ptr)->GetId(),ObjType));
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			w2=((*ptr2)->Weight/max2)*log(TotalRef/lang->GetRef((*ptr2)->GetId(),ObjType));
			norm2+=w2*w2;
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			w2=((*ptr2)->Weight/max2)*log(TotalRef/lang->GetRef((*ptr2)->GetId(),ObjType));
			norm2+=w2*w2;
			Sim+=w1*w2;
			ptr2++;
		}
		norm1+=w1*w1;
		ptr++;
	}
	while(j)
	{
		j--;
		w2=((*ptr2)->Weight/max2)*log(TotalRef/lang->GetRef((*ptr2)->GetId(),ObjType));
		norm2+=w2*w2;
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::AddRefs(tObjType ObjType,GDict* dic) const
{
	GIWordWeight** ptr;
	unsigned int i;

	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		dic->IncRef((*ptr)->GetId(),ObjType);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::DelRefs(tObjType ObjType,GDict* dic) const
{
	GIWordWeight** ptr;
	unsigned int i;

	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		dic->DecRef((*ptr)->GetId(),ObjType);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::ModifyQuery(tObjType ObjType,GLang* lang)
{
	GIWordWeight** ptr;
	unsigned int i;
	double max=GetMaxWeight();
	double TotalRef=lang->GetRef(ObjType);
	double idffactor,nbref;
	double freq;

	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		nbref=lang->GetRef((*ptr)->GetId(),ObjType);
		if(!nbref) continue;
		freq=0.5+((0.5*(*ptr)->Weight)/max);
		idffactor=log(TotalRef/nbref);
		(*ptr)->SetWeight(freq*idffactor);
	}
}


//---------------------------------------------------------------------------
GALILEI::GIWordsWeights::~GIWordsWeights(void)
{
}
