/*

	GALILEI Research Project

	GWeightInfos.cpp

	List of weighted information entities - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include standard api files
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gweightinfos.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
#include <infos/gdict.h>
#include <infos/gword.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(unsigned int max) throw(std::bad_alloc)
	: RContainer<GWeightInfo,true,true>(max,50)
{
}


//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(const GWeightInfos& w) throw(std::bad_alloc)
	: RContainer<GWeightInfo,true,true>(w)
{
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator=(const GWeightInfos::GWeightInfos& w) throw(std::bad_alloc)
{
	RContainer<GWeightInfo,true,true>::operator=(w);
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator=(const RContainer<GWeightInfo,false,true>& w) throw(std::bad_alloc)
{
	RContainer<GWeightInfo,true,true>::operator=(w);
	return(*this);
}


//------------------------------------------------------------------------------
void GWeightInfos::Copy(const GWeightInfos& src) throw(std::bad_alloc)
{
	RContainer<GWeightInfo,true,true>::Copy(src);
}


//------------------------------------------------------------------------------
void GWeightInfos::Copy(const GWeightInfos* src) throw(std::bad_alloc)
{
	RContainer<GWeightInfo,true,true>::Copy(*src);
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
	RContainer<GWeightInfo,true,true>::Clear();
}


//------------------------------------------------------------------------------
RCursor<GWeightInfo> GWeightInfos::GetWeightInfoCursor(void)
{
	RCursor<GWeightInfo> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
void GWeightInfos::InsertInfo(GWeightInfo* info) throw(std::bad_alloc)
{
	InsertPtr(info);
}


//------------------------------------------------------------------------------
void GWeightInfos::DeleteInfo(GWeightInfo* info) throw(std::bad_alloc)
{
	DeletePtr(*info);
}


//------------------------------------------------------------------------------
GWeightInfo* GWeightInfos::GetInfo(unsigned int id) const throw(std::bad_alloc)
{
	return(GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxWeight(void) const throw(GException)
{
	double max;

	// If no profile, maximal weight is null.
	if(!GetNb())
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Suppose first weight is the highest
	RCursor<GWeightInfo> ptr(*this);
	ptr.Start();
	max=ptr()->GetWeight();

	// Look if there is a greather one.
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		if(ptr()->GetWeight()>max)
			max=ptr()->GetWeight();
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxAbsWeight(void) const throw(GException)
{
	double max;

	// If no profile, maximal weight is null.
	if(!GetNb())
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Suppose first weight is the highest
	RCursor<GWeightInfo> ptr(*this);
	ptr.Start();
	max=fabs(ptr()->GetWeight());

	// Look if there is a greather one.
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		if(fabs(ptr()->GetWeight())>max)
			max=fabs(ptr()->GetWeight());
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::Similarity(const GWeightInfos* w) const
{
	double Sim=0.0;
	double norm1=0.0;
	double norm2=0.0;
	RCursor<GWeightInfo> ptr=(*this);
	RCursor<GWeightInfo> ptr2=(*w);

	// if one list is empty -> the similarity is null
	if((!GetNb())||(!w->GetNb()))
		return(0.0);

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
		{
			norm2+=ptr2()->GetWeight()*ptr2()->GetWeight();
			ptr2.Next();
		}
		if((!ptr2.End())&&(ptr2()->GetId()==ptr()->GetId()))
		{
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				norm2+=ptr2()->GetWeight()*ptr2()->GetWeight();
				norm1+=ptr()->GetWeight()*ptr()->GetWeight();
				Sim+=ptr()->GetWeight()*ptr2()->GetWeight();
			}
			ptr2.Next();
		}
		else
			norm1+=ptr()->GetWeight()*ptr()->GetWeight();
		ptr.Next();
	}
	while(!ptr2.End())
	{
		norm2+=ptr2()->GetWeight()*ptr2()->GetWeight();
		ptr2.Next();
	}
	if(norm1==0.0 || norm2==0.0)
		return (0.0);
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//------------------------------------------------------------------------------
double GWeightInfos::SimilarityIFF(const GWeightInfos* w,tObjType ObjType,GLang* lang) const throw(GException)
{
	double Sim=0.0;
	double norm1=0.0;
	double norm2=0.0;
	double max1;
	double max2;
	double w1,w2,iff;
	double TotalRef;
	RCursor<GWeightInfo> ptr=(*this);
	RCursor<GWeightInfo> ptr2=(*w);

	if(!lang)
		throw GException("No Language defined");

	// if one SubProfile is not defined -> the similarity must be null
	if((!GetNb())||(!w->GetNb()))
		return(0.0);

	// Compute Similarity
	max1=GetMaxAbsWeight();
	max2=w->GetMaxAbsWeight();
	if(max1==0.0 || max2==0.0)
		return (0.0);

	TotalRef=lang->GetRef(ObjType);
	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		iff=TotalRef/static_cast<double>(lang->GetRef(ptr()->GetId(),ObjType));
		w1=(ptr()->GetWeight()/max1)*log(iff);
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
		{
			iff=TotalRef/static_cast<double>(lang->GetRef(ptr2()->GetId(),ObjType));
			w2=(ptr2()->GetWeight()/max2)*log(iff);
			norm2+=w2*w2;
			ptr2.Next();
		}
		if((!ptr2.End())&&(ptr2()->GetId()==ptr()->GetId()))
		{
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				iff=TotalRef/static_cast<double>(lang->GetRef(ptr2()->GetId(),ObjType));
				w2=(ptr2()->GetWeight()/max2)*log(iff);
				norm2+=w2*w2;
				norm1+=w1*w1;
				Sim+=w1*w2;
			}
			ptr2.Next();
		}
		else
			norm1+=w1*w1;
		ptr.Next();
	}
	while(!ptr2.End())
	{
		iff=TotalRef/static_cast<double>(lang->GetRef(ptr2()->GetId(),ObjType));
		w2=(ptr2()->GetWeight()/max2)*log(iff);
		norm2+=w2*w2;
		ptr2.Next();
	}
	if(norm1==0.0 || norm2==0.0)
		return (0.0);
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//------------------------------------------------------------------------------
bool GWeightInfos::SimilarityBool(const GWeightInfos* w,unsigned int nb) const
{
	unsigned int same=0;
	RCursor<GWeightInfo> ptr=(*this);
	RCursor<GWeightInfo> ptr2=(*w);

	// if one list is empty -> the boolean similarity is false
	if((!GetNb())||(!w->GetNb()))
		return(false);

	// If 0, verify all the entities
	if(nb==0)
		nb=w->GetNb();

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
			ptr2.Next();
		if((!ptr2.End())&&(ptr2()->GetId()==ptr()->GetId()))
		{
			same++;
			if(same>=nb)
				return(true);
			ptr2.Next();
		}
		ptr.Next();
	}
	return(false);
}


//------------------------------------------------------------------------------
void GWeightInfos::AddRefs(tObjType ObjType,GLang* lang) const throw(GException)
{
	if(!lang)
		throw GException("No Language defined");
	if(!GetNb()) return;
	lang->IncRef(ObjType);
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		lang->IncRef(ptr()->GetId(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::DelRefs(tObjType ObjType,GLang* lang) const throw(GException)
{
	if(!lang)
		throw GException("No Language defined");
	if(!GetNb()) return;
	lang->DecRef(ObjType);
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		lang->DecRef(ptr()->GetId(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeIFF(tObjType ObjType,GLang* lang) throw(GException)
{
	double max,iff;

	if(!lang)
		throw GException("No Language defined");
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start(),max=GetMaxAbsWeight();!ptr.End();ptr.Next())
	{
		iff=static_cast<double>(lang->GetRef(ObjType))/static_cast<double>(lang->GetRef(ptr()->GetId(),ObjType));
		ptr()->SetWeight((ptr()->GetWeight()/max)*log(iff));
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeQuery(tObjType ObjType,GLang* lang) throw(GException)
{
	GWeightInfo* ptr;
	unsigned int i;
	double max=GetMaxAbsWeight();
	double TotalRef;
	double idffactor,nbref;
	double freq;
	const GData** words;

	if(!lang)
		throw GException("No Language defined");
	for(i=lang->GetDict()->GetDataMaxId()+1,words=lang->GetDatas();--i;words++)
	{
		if((!(*words))||(!lang->GetRef((*words)->GetId(),ObjType))) continue;
		nbref=lang->GetRef((*words)->GetId(),ObjType);
		if(!nbref) continue;
		ptr=GetPtr<const unsigned int>((*words)->GetId());
		if(!ptr)
			InsertPtr(ptr=new GWeightInfo((*words)->GetId()));
		freq=0.5+((0.5*ptr->GetWeight())/max);
		TotalRef=lang->GetRef(ObjType);
		idffactor=log(TotalRef/nbref);
		ptr->SetWeight(freq*idffactor);
	}
}


//------------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}
