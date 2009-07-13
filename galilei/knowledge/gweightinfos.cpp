/*

	GALILEI Research Project

	GWeightInfos.cpp

	List of weighted information entities - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <glang.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <gindexer.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
const double Factor=0.5;


//------------------------------------------------------------------------------
//
//  GWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(size_t max)
	: RContainer<GWeightInfo,true,true>(max,50)
{
}


//------------------------------------------------------------------------------
GWeightInfos::GWeightInfos(const GWeightInfos& w)
	: RContainer<GWeightInfo,true,true>(w)
{
}


//------------------------------------------------------------------------------
int GWeightInfos::SortOrder(const void* a,const void* b)
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
void GWeightInfos::CopyInfos(const R::RContainer<GWeightInfo,false,true>& infos)
{
	RContainer<GWeightInfo,true,true>::operator=(infos);
}


//------------------------------------------------------------------------------
GWeightInfo* GWeightInfos::GetInfo(const GWeightInfo* info)
{
	return(GetInsertPtr(info->GetConcept()));
}


//------------------------------------------------------------------------------
GWeightInfo* GWeightInfos::GetInfo(GConcept* concept)
{
	return(GetInsertPtr(concept));
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
void GWeightInfos::DeleteInfo(GConcept* concept)
{
	DeletePtr(*concept);
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
double GWeightInfos::Inclusion(const GWeightInfos& w,tObjType ObjType) const
{
	// if one list is empty -> the inclusion is null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);

	double num(0.0), den(0.0),tmp;
	double max(-1.0);
	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);
	GConceptType* type(0);

	// Start the first list
	for(ptr.Start(),ptr2.Start();!ptr.End();ptr.Next())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// Yes -> A new total number of references.
			type=ptr()->GetConcept()->GetType();
			max=GetMaxAbsWeight(type);
		}

		// While the element of the second list is not the current one, skip it.
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
			ptr2.Next();
		tmp=(ptr()->GetWeight()*ptr()->GetConcept()->GetIF(ObjType))/max;
		den+=tmp;
		if((!ptr2.End())&&(ptr()->GetId()==ptr2()->GetId()))
			num+=tmp;
	}

	return(num/den);
}


//------------------------------------------------------------------------------
double GWeightInfos::Similarity(const GWeightInfos& w) const
{
	// if one list is empty -> the similarity is null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);

	double Sim(0.0);
	double norm1(0.0);
	double norm2(0.0);
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
bool GWeightInfos::SimilarityBool(const GWeightInfos& w,size_t nb) const
{
	size_t same=0;

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
		type->IncRef(ptr()->GetConcept(),ObjType);
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
		type->DecRef(ptr()->GetConcept(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeIFF(tObjType ObjType)
{
	if(!GetNb()) return;
	double max(0.0),iff,ref(0.0);
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
		iff=ref/static_cast<double>(ptr()->GetConcept()->GetRef(ObjType));
		ptr()->SetWeight((ptr()->GetWeight()/max)*log(iff));
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::Print(R::RString msg)
{
	cout<<msg<<endl<<"\t(Type,Id)"<<endl;
	RCursor<GWeightInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		cout<<"\t("<<(int)Cur()->GetType()->GetId()<<","<<Cur()->GetId()<<")"<<endl;
	cout<<"OK : "<<msg<<endl;
}


//------------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}
