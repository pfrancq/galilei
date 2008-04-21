/*

	GALILEI Research Project

	GWeightInfos.cpp

	List of weighted information entities - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfos.h>
#include <gweightinfo.h>
#include <glang.h>
#include <gconcepttype.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
const double Factor=0.5;


//------------------------------------------------------------------------------
//
//  GWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(unsigned int max)
	: RContainer<GWeightInfo,true,true>(max,50), State(osNew)
{
}


//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(const GWeightInfos& w)
	: RContainer<GWeightInfo,true,true>(w), State(w.State)
{
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator=(const GWeightInfos::GWeightInfos& w)
{
	RContainer<GWeightInfo,true,true>::operator=(w);
	State=w.State;
	return(*this);
}


//------------------------------------------------------------------------------
void GWeightInfos::CopyInfos(const R::RContainer<GWeightInfo,false,true>* infos)
{
	RContainer<GWeightInfo,true,true>::operator=(*infos);
	State=osUpToDate;
}


//------------------------------------------------------------------------------
void GWeightInfos::Copy(const GWeightInfos& src)
{
	RContainer<GWeightInfo,true,true>::Copy(src);
	State=src.State;
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
GWeightInfo* GWeightInfos::GetInfo(const GWeightInfo* info)
{
	return(GetInsertPtr(info->GetConcept()));
}


//------------------------------------------------------------------------------
void GWeightInfos::Clear(void)
{
	RContainer<GWeightInfo,true,true>::Clear();
}


//------------------------------------------------------------------------------
void GWeightInfos::SetState(tObjState state)
{
	if((State==osNeedLoad)&&(state==osDelete))
	{
		// Force to load the description since the references must be decreased
		// in the destructor.
		const_cast<GWeightInfos*>(this)->State=osOnDemand;      // The object is on-demand of loading
		LoadInfos();           // Load it.
	}
	State=state;
}


//------------------------------------------------------------------------------
void GWeightInfos::LoadInfos(void) const
{
}


//------------------------------------------------------------------------------
RCursor<GWeightInfo> GWeightInfos::GetInfos() const
{
	if(State==osNeedLoad)
	{
		const_cast<GWeightInfos*>(this)->State=osOnDemand;      // The object is on-demand of loading
		LoadInfos();           // Load it.
	}
	return(RCursor<GWeightInfo>(*this));
}


//------------------------------------------------------------------------------
void GWeightInfos::InsertInfo(GWeightInfo* info)
{
	InsertPtr(info);
}


//------------------------------------------------------------------------------
void GWeightInfos::DeleteInfo(GWeightInfo* info)
{
	DeletePtr(*info);
}


//------------------------------------------------------------------------------
size_t GWeightInfos::GetNb(void) const
{
 	if(State==osNeedLoad)
	{
		const_cast<GWeightInfos*>(this)->State=osOnDemand;      // The object is on-demand of loading
		LoadInfos();           // Load it.
	}
	return(R::RContainer<GWeightInfo,true,true>::GetNb());
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxWeight(GConceptType* type) const
{
	// If no profile, maximal weight is null.
	if(!GetNb())
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Suppose first weight is the highest
	RCursor<GWeightInfo> ptr(*this);
	ptr.Start();	
	while((!ptr.End())&&(ptr()->GetType()!=type))
		ptr.Next();
	
	if(ptr.End())
		return(0.0);
	double max(ptr()->GetWeight());

	// Look if there is a greater one.
	for(ptr.Next();(!ptr.End())&&(ptr()->GetType()==type);ptr.Next())
	{
		if(ptr()->GetWeight()>max)
			max=ptr()->GetWeight();
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxAbsWeight(GConceptType* type) const
{
	// If no profile, maximal weight is null.
	if(!GetNb())
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Suppose first weight is the highest
	RCursor<GWeightInfo> ptr(*this);
	ptr.Start();
	while((!ptr.End())&&(ptr()->GetType()!=type))
		ptr.Next();

	if(ptr.End())
		return(0.0);
	double max(fabs(ptr()->GetWeight()));
	
	// Look if there is a greater one.
	for(ptr.Next();(!ptr.End())&&(ptr()->GetType()==type);ptr.Next())
	{
		if(fabs(ptr()->GetWeight())>max)
			max=fabs(ptr()->GetWeight());
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::Similarity(const GWeightInfos& w) const
{
	// if one list is empty -> the similarity is null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);
	
	double Sim;
	double norm1;
	double norm2;
	double GlobalSim(1.0);	
	GConceptType* type(0);
	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr()->GetConcept()->GetType();
			norm1=norm2=Sim=0.0;
		}
		
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				norm1=norm2=Sim=0.0;
			}			
			norm2+=ptr2()->GetWeight()*ptr2()->GetWeight();
			ptr2.Next();
		}
		if((!ptr2.End())&&(ptr2()->GetId()==ptr()->GetId()))
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				norm1=norm2=Sim=0.0;
			}						
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
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr2()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr2()->GetConcept()->GetType();
			norm1=norm2=Sim=0.0;
		}					
		norm2+=ptr2()->GetWeight()*ptr2()->GetWeight();
		ptr2.Next();
	}
	// If a type exist -> modify global sim
	if(type)
		GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
	return(GlobalSim);
}


//------------------------------------------------------------------------------
double GWeightInfos::SimilarityIFF(const GWeightInfos& w,tObjType ObjType) const
{
	// if one SubProfile is not defined -> the similarity must be null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);

	double max1;
	double max2;	
	double GlobalSim(1.0);	
	double Sim;
	double norm1;
	double norm2;
	double w1,w2,iff;
	double TotalRef;
	GConceptType* type(0);
		
	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr()->GetConcept()->GetType();
			TotalRef=type->GetRef(ObjType);
			norm1=norm2=Sim=0.0;
			max1=GetMaxAbsWeight(type);
			max2=w.GetMaxAbsWeight(type);
		}

		iff=TotalRef/static_cast<double>(type->GetRef(ptr()->GetId(),ObjType));
		w1=(ptr()->GetWeight()/max1)*log10(iff);
		while((!ptr2.End())&&((*ptr2())<(*ptr())))
		{
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef=type->GetRef(ObjType);
				norm1=norm2=Sim=0.0;
				max1=GetMaxAbsWeight(type);
				max2=w.GetMaxAbsWeight(type);				
			}			
			iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType));
			w2=(ptr2()->GetWeight()/max2)*log10(iff);
			norm2+=w2*w2;
			ptr2.Next();
		}
		if((!ptr2.End())&&((*ptr2())==(*ptr())))
		{
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef=type->GetRef(ObjType);
				norm1=norm2=Sim=0.0;
				max1=GetMaxAbsWeight(type);
				max2=w.GetMaxAbsWeight(type);				
			}			
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType));
				w2=(ptr2()->GetWeight()/max2)*log10(iff);
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
		if(ptr2()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr2()->GetConcept()->GetType();
			TotalRef=type->GetRef(ObjType);
			norm1=norm2=Sim=0.0;
			max1=GetMaxAbsWeight(type);
			max2=w.GetMaxAbsWeight(type);			
		}			
		iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType));
		w2=(ptr2()->GetWeight()/max2)*log10(iff);
		norm2+=w2*w2;
		ptr2.Next();
	}

	// If a type exist -> modify global sim
	if(type)
		GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
	return(GlobalSim);
}


//------------------------------------------------------------------------------
double GWeightInfos::SimilarityIFF2(const GWeightInfos& w,tObjType ObjType1,tObjType ObjType2) const
{
	// if one SubProfile is not defined -> the similarity must be null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);

	double GlobalSim(1.0);
	double Sim;
	double norm1;
	double norm2;
	double max1;
	double max2;
	double w1,w2,iff1,iff2;
	double TotalRef1;
	double TotalRef2;
	GConceptType* type(0);

	// Compute Similarity
	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr()->GetConcept()->GetType();
			TotalRef1=type->GetRef(ObjType1);
			TotalRef2=type->GetRef(ObjType2);
			norm1=norm2=Sim=0.0;
			max1=GetMaxAbsWeight(type);
			max2=w.GetMaxAbsWeight(type);
		}
				
		iff1=TotalRef1/static_cast<double>(type->GetRef(ptr()->GetId(),ObjType1));
		iff2=TotalRef2/static_cast<double>(type->GetRef(ptr()->GetId(),ObjType2));
		
		w1=(ptr()->GetWeight()/max1)*log10(iff1)*log10(iff2);
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef1=type->GetRef(ObjType1);
				TotalRef2=type->GetRef(ObjType2);
				norm1=norm2=Sim=0.0;
				max1=GetMaxAbsWeight(type);
				max2=w.GetMaxAbsWeight(type);
			}
			iff1=TotalRef1/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType1));
			iff2=TotalRef2/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType2));
			w2=(ptr2()->GetWeight()/max2)*log10(iff1)*log10(iff2);
			norm2+=w2*w2;
			ptr2.Next();
		}
		if((!ptr2.End())&&(ptr2()->GetId()==ptr()->GetId()))
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef1=type->GetRef(ObjType1);
				TotalRef2=type->GetRef(ObjType2);
				norm1=norm2=Sim=0.0;
				max1=GetMaxAbsWeight(type);
				max2=w.GetMaxAbsWeight(type);
			}								
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				iff1=TotalRef1/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType1));
				iff2=TotalRef2/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType2));
				w2=(ptr2()->GetWeight()/max2)*log10(iff1)*log10(iff2);
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
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr2()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr2()->GetConcept()->GetType();
			TotalRef1=type->GetRef(ObjType1);
			TotalRef2=type->GetRef(ObjType2);
			norm1=norm2=Sim=0.0;
			max1=GetMaxAbsWeight(type);
			max2=w.GetMaxAbsWeight(type);
		}							
		iff1=TotalRef1/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType1));
		iff2=TotalRef2/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjType2));
		w2=(ptr2()->GetWeight()/max2)*log10(iff1)*log10(iff2);
		norm2+=w2*w2;
		ptr2.Next();
	}
	// If a type exist -> modify global sim
	if(type)
		GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
	return(GlobalSim);
}


//------------------------------------------------------------------------------
bool GWeightInfos::SimilarityBool(const GWeightInfos& w,unsigned int nb) const
{
	unsigned int same=0;

	// if one list is empty -> the boolean similarity is false
	if((!GetNb())||(!w.GetNb()))
		return(false);

	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);

	// If 0, verify all the entities
	if(nb==0)
		nb=w.GetNb();

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
void GWeightInfos::Extract(GWeightInfos& dest,const GLang* lang,bool universal)
{
	// Empty destination
	dest.Clear();

	RCursor<GWeightInfo> Entities(*this);
	for(Entities.Start();!Entities.End();Entities.Next())
	{
		GLang* curLang=Entities()->GetType()->GetLang();
		
		if((curLang==lang)||(universal&&(!curLang)))
			dest.InsertPtr(new GWeightInfo(*Entities()));
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::AddRefs(tObjType ObjType) const
{
	if(!GetNb()) return;
	GConceptType* type(0);
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// Yes -> A new object uses this concept type.
			type=ptr()->GetConcept()->GetType();
			type->IncRef(ObjType);
		}

		// AddRef for the concept
		type->IncRef(ptr()->GetConcept()->GetId(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::DelRefs(tObjType ObjType) const
{
	if(!GetNb()) return;
	GConceptType* type(0);
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// Yes -> A new object does not use this concept type anymore.
			type=ptr()->GetConcept()->GetType();
			type->DecRef(ObjType);
		}

		// AddRef for the concept
		type->DecRef(ptr()->GetConcept()->GetId(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeIFF(tObjType ObjType)
{
	if(!GetNb()) return;
	double max,iff,ref;
	GConceptType* type(0);
	
	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(type!=ptr()->GetConcept()->GetType())
		{
			type=ptr()->GetConcept()->GetType();
			max=GetMaxAbsWeight(type);
			ref=static_cast<double>(type->GetRef(ObjType));
		}
		iff=ref/static_cast<double>(type->GetRef(ptr()->GetId(),ObjType));
		ptr()->SetWeight((ptr()->GetWeight()/max)*log(iff));
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeQuery(tObjType,GLang*)
{
	throw GException("GWeightInfos::RecomputeQuery not implemented");
/*	GWeightInfo* ptr;
	unsigned int i;
	double max=GetMaxAbsWeight();
	double TotalRef;
	double idffactor,nbref;
	double freq;
	const GConcept** words;

	if(!lang)
		throw GException("No Language defined");
	if(!GetNb()) return;

	TotalRef=lang->GetRef(ObjType);

	#pragma warn Handle correctly multiple concepts
	for(i=lang->GetDict()->GetConceptMaxId()+1,words=lang->GetConcepts(1);--i;words++)
	{
		if(!(*words)) continue;
		nbref=(*words)->GetRef(ObjType);
		if(!nbref) continue;
		ptr=GetPtr(**words);
		if(!ptr)
			InsertPtr(ptr=new GWeightInfo((*words)->GetId(),(*words)->GetType()));
		freq=Factor+((Factor*ptr->GetWeight())/max);
		idffactor=log(TotalRef/nbref);
		ptr->SetWeight(freq*idffactor);
	}*/
}


//------------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}
