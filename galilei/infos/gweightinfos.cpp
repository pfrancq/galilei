/*

	GALILEI Research Project

	GWeightInfos.cpp

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
#include <infos/gweightinfos.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
#include <infos/gdict.h>
#include <infos/gword.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GWeightInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GWeightInfos::GWeightInfos(unsigned int nb) throw(bad_alloc)
	: RContainer<GWeightInfo,unsigned,true,true>(nb,50), NbWordsDocs(0.0)
{
}


//-----------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator=(const GWeightInfos::GWeightInfos& src) throw(bad_alloc)
{
	RContainer<GWeightInfo,unsigned,true,true>::operator=(src);
	NbWordsDocs=src.NbWordsDocs;
	return(*this);
}


//-----------------------------------------------------------------------------
int GWeightInfos::sortOrder(const void *a,const void *b)
{
	double af=(*((GWeightInfo**)(a)))->Weight;
	double bf=(*((GWeightInfo**)(b)))->Weight;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
void GWeightInfos::Clear(void)
{
	RContainer<GWeightInfo,unsigned,true,true>::Clear();
	NbWordsDocs=0.0;
}


//-----------------------------------------------------------------------------
double GWeightInfos::GetMaxWeight(void) const
{
	double max;
	GWeightInfo** ptr;
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
double GWeightInfos::Similarity(const GWeightInfos* w) const
{
	double Sim=0.0;
	GWeightInfo** ptr=Tab;
	GWeightInfo** ptr2=w->Tab;
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
double GWeightInfos::SimilarityIdf(const GWeightInfos* w,tObjType ObjType,GLang* lang) const
{
	double Sim=0.0;
	GWeightInfo** ptr=Tab;
	GWeightInfo** ptr2=w->Tab;
	unsigned int i=NbPtr+1;
	unsigned int j=w->NbPtr;
	double norm1=0.0;
	double norm2=0.0;
	double max1=GetMaxWeight();
	double max2=w->GetMaxWeight();
	double TotalRefW=lang->GetRef(ObjType,infoWord);
	double TotalRefWL=lang->GetRef(ObjType,infoWordList);
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
void GWeightInfos::AddRefs(tObjType ObjType,GDict* dic) const
{
	GWeightInfo** ptr;
	unsigned int i;

	dic->IncRef(ObjType,infoWord);
	dic->IncRef(ObjType,infoWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			dic->IncRef((*ptr)->GetId(),ObjType,infoWordList);
		else dic->IncRef((*ptr)->GetId(),ObjType,infoWord);
	}
}


//-----------------------------------------------------------------------------
void GWeightInfos::DelRefs(tObjType ObjType,GDict* dic) const
{
	GWeightInfo** ptr;
	unsigned int i;

	dic->DecRef(ObjType,infoWord);
	dic->DecRef(ObjType,infoWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			dic->DecRef((*ptr)->GetId(),ObjType,infoWordList);
		else
			dic->DecRef((*ptr)->GetId(),ObjType,infoWord);
	}
}


//-----------------------------------------------------------------------------
void GWeightInfos::Transform(tObjType ObjType,GLang* lang)
{
	GWeightInfo** ptr;
	unsigned int i;
	double max;
	double TotalRef;

	for(i=NbPtr+1,ptr=Tab,max=GetMaxWeight();--i;ptr++)
	{
		if((*ptr)->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,infoWordList);
		else
			TotalRef=lang->GetRef(ObjType,infoWord);
		(*ptr)->Weight=((*ptr)->Weight/max)*log(TotalRef/lang->GetRef((*ptr)->GetId(),ObjType));
	}
}


//-----------------------------------------------------------------------------
void GWeightInfos::ModifyQueryGroups(tObjType ObjType,GLang* lang)
{
	GWeightInfo* ptr;
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
			InsertPtr(ptr=new GWeightInfo((*words)->GetId()));
		freq=0.5+((0.5*ptr->Weight)/max);
		idffactor=log(TotalRef/nbref);
		if(ptr->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,infoWordList);
		else
			TotalRef=lang->GetRef(ObjType,infoWord);
		ptr->SetWeight(freq*idffactor);
	}
}


//-----------------------------------------------------------------------------
void GWeightInfos::ModifyQuery(tObjType ObjType,GLang* lang)
{
	GWeightInfo* ptr;
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
			InsertPtr(ptr=new GWeightInfo((*words)->GetId()));
		freq=0.5+((0.5*ptr->Weight)/max);
		if(ptr->InfoType()==infoWordList)
			TotalRef=lang->GetRef(ObjType,infoWordList);
		else
			TotalRef=lang->GetRef(ObjType,infoWord);
		idffactor=log(TotalRef/nbref);
		ptr->SetWeight(freq*idffactor);
	}
}


//-----------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}
