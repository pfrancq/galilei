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
// include files for GALILEI
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gwordlist.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GIWordsWeights
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordsWeights::GIWordsWeights(unsigned int nb) throw(bad_alloc)
	: RContainer<GIWordWeight,unsigned,true,true>(nb,50), NbWordsDocs(0.0)
{
	Type=infoWordCalcs;
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordsWeights::ClassName(void) const
{
	return("GIWordsWeights");
}


//-----------------------------------------------------------------------------
const GInfoType GALILEI::GIWordsWeights::InfoType(void) const
{
	return(Type);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordsWeights& GALILEI::GIWordsWeights::operator=(const GALILEI::GIWordsWeights::GIWordsWeights& src) throw(bad_alloc)
{
	RContainer<GIWordWeight,unsigned,true,true>::operator=(src);
	NbWordsDocs=src.NbWordsDocs;
	return(*this);
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::Clear(void)
{
	RContainer<GIWordWeight,unsigned,true,true>::Clear();
	NbWordsDocs=0.0;
}


//-----------------------------------------------------------------------------
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

	// if one SubProfile is not defined -> the similarity must be null
	if((!NbPtr)||(!w->NbPtr))     
		return(0.0);
	
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
			if(((*ptr)->Weight>0)||((*ptr2)->Weight>0))
			{
				norm2+=(*ptr2)->Weight*(*ptr2)->Weight;
				norm1+=(*ptr)->Weight*(*ptr)->Weight;
				Sim+=(*ptr)->Weight*(*ptr2)->Weight;
			}
			ptr2++;
		}
		else
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
	double TotalRefW=lang->GetRef(ObjType,tWord);
	double TotalRefWL=lang->GetRef(ObjType,tWordList);
	double w1,w2;

	// if one SubProfile is not defined -> the similarity must be null
	if((!NbPtr)||(!w->NbPtr))
		return(0.0);

	// Compute Similarity
	while(--i)
	{
		if((*ptr)->InfoType()==infoWordList)
			w1=((*ptr)->Weight/max1)*log(TotalRefWL/lang->GetRef((*ptr)->GetId(),ObjType));
		else
			w1=((*ptr)->Weight/max1)*log(TotalRefW/lang->GetRef((*ptr)->GetId(),ObjType));
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			if((*ptr2)->InfoType()==infoWordList)
				w2=((*ptr2)->Weight/max2)*log(TotalRefWL/lang->GetRef((*ptr2)->GetId(),ObjType));
			else
				w2=((*ptr2)->Weight/max2)*log(TotalRefW/lang->GetRef((*ptr2)->GetId(),ObjType));
			norm2+=w2*w2;
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			if(((*ptr)->Weight>0)||((*ptr2)->Weight>0))
			{
				if((*ptr2)->InfoType()==infoWordList)
					w2=((*ptr2)->Weight/max2)*log(TotalRefWL/lang->GetRef((*ptr2)->GetId(),ObjType));
				else
					w2=((*ptr2)->Weight/max2)*log(TotalRefW/lang->GetRef((*ptr2)->GetId(),ObjType));
				norm2+=w2*w2;
				norm1+=w1*w1;
				Sim+=w1*w2;
			}
			ptr2++;
		}
		else
			norm1+=w1*w1;
		ptr++;
	}
	while(j)
	{
		j--;
		if((*ptr2)->InfoType()==infoWordList)
			w2=((*ptr2)->Weight/max2)*log(TotalRefWL/lang->GetRef((*ptr2)->GetId(),ObjType));
		else
			w2=((*ptr2)->Weight/max2)*log(TotalRefW/lang->GetRef((*ptr2)->GetId(),ObjType));
		norm2+=w2*w2;
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::AddRefs(tObjType ObjType,GDict* dic) const
{
	GIWordWeight** ptr;
	unsigned int i;

	dic->IncRef(ObjType,tWord);
	dic->IncRef(ObjType,tWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			dic->IncRef((*ptr)->GetId(),ObjType,tWordList);
		else dic->IncRef((*ptr)->GetId(),ObjType,tWord);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::DelRefs(tObjType ObjType,GDict* dic) const
{
	GIWordWeight** ptr;
	unsigned int i;

	dic->DecRef(ObjType,tWord);
	dic->DecRef(ObjType,tWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			dic->DecRef((*ptr)->GetId(),ObjType,tWordList);
		else
			dic->DecRef((*ptr)->GetId(),ObjType,tWord);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::Transform(tObjType ObjType,GLang* lang)
{
	GIWordWeight** ptr;
	unsigned int i;
	double max;
	double TotalRef;

	for(i=NbPtr+1,ptr=Tab,max=GetMaxWeight();--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,tWordList);
		else
			TotalRef=lang->GetRef(ObjType,tWord);
		(*ptr)->Weight=((*ptr)->Weight/max)*log(TotalRef/lang->GetRef((*ptr)->GetId(),ObjType));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::ModifyQueryGroups(tObjType ObjType,GLang* lang)
{
	GIWordWeight* ptr;
	unsigned int i;
	double max=GetMaxWeight();
	double TotalRef;
	double idffactor,nbref;
	double freq;
	GWord** words;

	if(!lang->GetDict()) return;
	for(i=lang->GetDict()->GetMaxId()+1,words=lang->GetDict()->GetWords();--i;words++)
	{
		if((!(*words))||(!lang->GetRef((*words)->GetId(),otGroup))) continue;
		nbref=lang->GetRef((*words)->GetId(),ObjType);
		if(!nbref) continue;
		ptr=GetPtr<const unsigned int>((*words)->GetId());
		if(!ptr)
			InsertPtr(ptr=new GIWordWeight((*words)->GetId()));
		freq=0.5+((0.5*ptr->Weight)/max);
		idffactor=log(TotalRef/nbref);
		if(ptr->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,tWordList);
		else
			TotalRef=lang->GetRef(ObjType,tWord);
		ptr->SetWeight(freq*idffactor);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIWordsWeights::ModifyQuery(tObjType ObjType,GLang* lang)
{
	GIWordWeight* ptr;
	unsigned int i;
	double max=GetMaxWeight();
	double TotalRef;
	double idffactor,nbref;
	double freq;
	GWord** words;

	if(!lang->GetDict()) return;
	for(i=lang->GetDict()->GetMaxId()+1,words=lang->GetDict()->GetWords();--i;words++)
	{
		if(!(*words)) continue;
		nbref=lang->GetRef((*words)->GetId(),ObjType);
		if(!nbref) continue;
		ptr=GetPtr<const unsigned int>((*words)->GetId());
		if(!ptr)
			InsertPtr(ptr=new GIWordWeight((*words)->GetId()));
		freq=0.5+((0.5*ptr->Weight)/max);
		if(ptr->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,tWordList);
		else
			TotalRef=lang->GetRef(ObjType,tWord);
		idffactor=log(TotalRef/nbref);
		ptr->SetWeight(freq*idffactor);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GIWordsWeights::~GIWordsWeights(void)
{
}
