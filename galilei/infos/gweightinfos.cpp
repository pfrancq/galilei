/*

	GALILEI Research Project

	GWeightInfos.cpp

	List of weighted information entities - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#include <infos/gword.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(unsigned int max) throw(bad_alloc)
	: RContainer<GWeightInfo,unsigned,true,true>(max,50)
{
}


//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(const GWeightInfos& w) throw(bad_alloc)
	: RContainer<GWeightInfo,unsigned,true,true>(w)
{
}


//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(const GWeightInfos* w) throw(bad_alloc)
	: RContainer<GWeightInfo,unsigned,true,true>(w)
{
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator=(const GWeightInfos::GWeightInfos& w) throw(bad_alloc)
{
	RContainer<GWeightInfo,unsigned,true,true>::operator=(w);
	return(*this);
}


//------------------------------------------------------------------------------
int GWeightInfos::sortOrder(const void* a,const void* b)
{
	double af=(*((GWeightInfo**)(a)))->GetWeight();
	double bf=(*((GWeightInfo**)(b)))->GetWeight();

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
void GWeightInfos::Clear(void)
{
	RContainer<GWeightInfo,unsigned,true,true>::Clear();
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxWeight(void) const throw(GException)
{
	double max;
	GWeightInfo** ptr;
	unsigned int i;

	// If no profile, maximal weight is null.
	if(!NbPtr)
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Suppose first weight is the highest
	ptr=Tab;
	max=(*ptr)->GetWeight();

	// Look if there is a greather one.
	for(i=NbPtr,ptr++;--i;ptr++)
	{
		if((*ptr)->GetWeight()>max)
			max=(*ptr)->GetWeight();
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::Similarity(const GWeightInfos* w) const
{
	double Sim=0.0;
	GWeightInfo** ptr=Tab;
	GWeightInfo** ptr2=w->Tab;
	unsigned int i=NbPtr+1;
	unsigned int j=w->NbPtr;
	double norm1=0.0;
	double norm2=0.0;

	// if one list is empty -> the similarity is null
	if((!NbPtr)||(!w->NbPtr))
		return(0.0);

	while(--i)
	{
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			if(((*ptr)->GetWeight()>0)||((*ptr2)->GetWeight()>0))
			{
				norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
				norm1+=(*ptr)->GetWeight()*(*ptr)->GetWeight();
				Sim+=(*ptr)->GetWeight()*(*ptr2)->GetWeight();
			}
			ptr2++;
		}
		else
			norm1+=(*ptr)->GetWeight()*(*ptr)->GetWeight();
		ptr++;
	}
	while(j)
	{
		j--;
		norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//------------------------------------------------------------------------------
double GWeightInfos::SimilarityIFF(const GWeightInfos* w,tObjType ObjType,GLang* lang) const throw(GException)
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
	double w1,w2,frac;

	if(!lang)
		throw GException("No Language defined");

	// if one SubProfile is not defined -> the similarity must be null
	if((!NbPtr)||(!w->NbPtr))
		return(0.0);

	// Compute Similarity
	while(--i)
	{
		frac=static_cast<double>(lang->GetRef(ObjType,(*ptr)->InfoType()))/static_cast<double>(lang->GetRef((*ptr)->GetId(),ObjType));
		w1=((*ptr)->GetWeight()/max1)*log(frac);
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			frac=static_cast<double>(lang->GetRef(ObjType,(*ptr2)->InfoType()))/static_cast<double>(lang->GetRef((*ptr2)->GetId(),ObjType));
			w2=((*ptr2)->GetWeight()/max2)*log(frac);
			norm2+=w2*w2;
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			if(((*ptr)->GetWeight()>0)||((*ptr2)->GetWeight()>0))
			{
				frac=static_cast<double>(lang->GetRef(ObjType,(*ptr2)->InfoType()))/static_cast<double>(lang->GetRef((*ptr2)->GetId(),ObjType));
				w2=((*ptr2)->GetWeight()/max2)*log(frac);
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
		frac=static_cast<double>(lang->GetRef(ObjType,(*ptr2)->InfoType()))/static_cast<double>(lang->GetRef((*ptr2)->GetId(),ObjType));
		w2=((*ptr2)->GetWeight()/max2)*log(frac);
		norm2+=w2*w2;
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//------------------------------------------------------------------------------
void GWeightInfos::AddRefs(tObjType ObjType,GLang* lang) const throw(GException)
{
	GWeightInfo** ptr;
	unsigned int i;

	if(!lang)
		throw GException("No Language defined");
	lang->IncRef(ObjType,infoWord);
	lang->IncRef(ObjType,infoWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		lang->IncRef((*ptr)->GetId(),ObjType,(*ptr)->InfoType());
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::DelRefs(tObjType ObjType,GLang* lang) const throw(GException)
{
	GWeightInfo** ptr;
	unsigned int i;

	if(!lang)
		throw GException("No Language defined");
	lang->DecRef(ObjType,infoWord);
	lang->DecRef(ObjType,infoWordList);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		lang->DecRef((*ptr)->GetId(),ObjType,(*ptr)->InfoType());
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeIFF(tObjType ObjType,GLang* lang) throw(GException)
{
	GWeightInfo** ptr;
	unsigned int i;
	double max,frac;

	if(!lang)
		throw GException("No Language defined");
	for(i=NbPtr+1,ptr=Tab,max=GetMaxWeight();--i;ptr++)
	{
		frac=static_cast<double>(lang->GetRef(ObjType,(*ptr)->InfoType()))/static_cast<double>(lang->GetRef((*ptr)->GetId(),ObjType));
		(*ptr)->SetWeight(((*ptr)->GetWeight()/max)*log(frac));
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeQuery(tObjType ObjType,GLang* lang) throw(GException)
{
	GWeightInfo* ptr;
	unsigned int i;
	double max=GetMaxWeight();
	double TotalRef;
	double idffactor,nbref;
	double freq;
	GData** words;

	if(!lang)
		throw GException("No Language defined");
	for(i=lang->GetMaxId()+1,words=lang->GetDatas();--i;words++)
	{
		if((!(*words))||(!lang->GetRef((*words)->GetId(),ObjType))) continue;
		nbref=lang->GetRef((*words)->GetId(),ObjType);
		if(!nbref) continue;
		ptr=GetPtr<const unsigned int>((*words)->GetId());
		if(!ptr)
			InsertPtr(ptr=new GWeightInfo((*words)->GetId()));
		freq=0.5+((0.5*ptr->GetWeight())/max);
		TotalRef=lang->GetRef(ObjType,ptr->InfoType());
		idffactor=log(TotalRef/nbref);
		ptr->SetWeight(freq*idffactor);
	}
}


//------------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}
