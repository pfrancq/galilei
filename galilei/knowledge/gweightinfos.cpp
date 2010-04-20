/*

	GALILEI Research Project

	GWeightInfos.cpp

	List of weighted information entities - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
// include files for R
#include <rvector.h>


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

	// Find the biggest weight
	double max(-numeric_limits<double>().max());
	RCursor<GWeightInfo> Infos(*this);
	for(Infos.Start();!Infos.End();Infos.Next())
	{
		if(Infos()->GetType()!=type)
			continue;
		if(Infos()->GetWeight()>max)
			max=Infos()->GetWeight();
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::GetMaxAbsWeight(GConceptType* type) const
{
	// If no profile, maximal weight is null.
	if(!GetNb())
		throw GException("GWeightInfos is empty for GetMaxHeight");

	// Find the biggest absolute weight
	double max(0);
	RCursor<GWeightInfo> Infos(*this);
	for(Infos.Start();!Infos.End();Infos.Next())
	{
		if(Infos()->GetType()!=type)
			continue;
		if(Infos()->GetWeight()>fabs(max))
			max=fabs(Infos()->GetWeight());
	}
	return(max);
}


//------------------------------------------------------------------------------
double GWeightInfos::Inclusion(const GWeightInfos& w,tObjType ObjType) const
{
	// if one list is empty -> the inclusion is null
	if((!GetNb())||(!w.GetNb()))
		return(0.0);

	// Initialize
	RVector Max(GSession::Get()->GetNbConceptTypes()+1);
	RVector Num(GSession::Get()->GetNbConceptTypes()+1);
	RVector Den(GSession::Get()->GetNbConceptTypes()+1);
	Max.Init(GSession::Get()->GetNbConceptTypes()+1,-numeric_limits<double>().max());
	Num.Init(GSession::Get()->GetNbConceptTypes()+1,0.0);
	Den.Init(GSession::Get()->GetNbConceptTypes()+1,0.0);

	// Start the first list
	RCursor<GWeightInfo> ptr(*this);
	RCursor<GWeightInfo> ptr2(w);
	for(ptr.Start(),ptr2.Start();!ptr.End();ptr.Next())
	{
		size_t TypeId(ptr()->GetConcept()->GetType()->GetId());

		// Verify if it is not the highest weight
		if(fabs(ptr()->GetWeight())>Max[TypeId])
			Max[TypeId]=fabs(ptr()->GetWeight());

		// While the element of the second list is not the current one, skip it.
		while((!ptr2.End())&&(ptr2()->GetId()<ptr()->GetId()))
			ptr2.Next();
		double tmp(ptr()->GetWeight()*ptr()->GetConcept()->GetIF(ObjType));
		Den[TypeId]+=tmp;
		if((!ptr2.End())&&(ptr()->GetId()==ptr2()->GetId()))
			Num[TypeId]+=tmp;
	}

	double num(0.0), den(0.0);
	RNumCursor<double> Cur1(Max);
	RNumCursor<double> Cur2(Num);
	RNumCursor<double> Cur3(Den);
	for(Cur1.Start(),Cur2.Start(),Cur3.Start();!Cur1.End();Cur1.Next(),Cur2.Next(),Cur3.Next())
	{
		if(Cur3()==0.0)
			continue;
		den+=Cur3()/Cur1();
		num+=Cur2()/Cur1();
	}
	return(num/den);
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
	RVectorBool Types(GSession::Get()->GetNbConceptTypes()+1);
	Types.Init(GSession::Get()->GetNbConceptTypes()+1,true);

	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		GConceptType* type(ptr()->GetConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> A new object uses this concept type.
			type->IncRef(ObjType);
			Types[TypeId]=false;
		}

		// IncRef for the concept
		type->IncRef(ptr()->GetConcept(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::DelRefs(tObjType ObjType) const
{
	if(!GetNb()) return;
	RVectorBool Types(GSession::Get()->GetNbConceptTypes()+1);
	Types.Init(GSession::Get()->GetNbConceptTypes()+1,true);

	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		GConceptType* type(ptr()->GetConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> An old object uses this concept type.
			type->DecRef(ObjType);
			Types[TypeId]=false;
		}

		// DecRef for the concept
		type->DecRef(ptr()->GetConcept(),ObjType);
	}
}


//------------------------------------------------------------------------------
void GWeightInfos::RecomputeIFF(tObjType ObjType)
{
	if(!GetNb()) return;

	// Initialize
	double max(0.0),iff,ref(0.0);
	GConceptType* type(0);
	RVector Max(GSession::Get()->GetNbConceptTypes()+1);
	Max.Init(GSession::Get()->GetNbConceptTypes()+1,-numeric_limits<double>().max());

	RCursor<GWeightInfo> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(type!=ptr()->GetConcept()->GetType())
		{
			type=ptr()->GetConcept()->GetType();
			size_t Id(ptr()->GetConcept()->GetType()->GetId());
			if(Max[Id]==-numeric_limits<double>().max())
				Max[Id]=GetMaxAbsWeight(type);
			max=Max[Id];
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
void GWeightInfos::Intersection(const GWeightInfos& infos)
{
	RContainer<GWeightInfo,false,false> ToDel(20);
	RCursor<GWeightInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If in infos, add it -> else delete
		GWeightInfo* Info(infos.GetPtr(*Cur()));
		if(!Info)
			ToDel.InsertPtr(Cur());
		else
			Cur()->Weight+=Info->Weight;
	}

	// Delete the necessary information entities
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		DeleteInfo(Cur());
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator+=(const GWeightInfos& infos)
{
	RCursor<GWeightInfo> Cur(infos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GWeightInfo* Info(GetInfo(Cur()));
		Info->Weight+=Cur()->Weight;
	}
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator-=(const GWeightInfos& infos)
{
	RCursor<GWeightInfo> Cur(infos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GWeightInfo* Info(GetInfo(Cur()));
		Info->Weight-=Cur()->Weight;
	}
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator*=(const double nb)
{
	RCursor<GWeightInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Weight*=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfos& GWeightInfos::operator/=(const double nb)
{
	RCursor<GWeightInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Weight/=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfos::~GWeightInfos(void)
{
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfos GALILEI::operator+(const GWeightInfos &arg1,const GWeightInfos &arg2)
{
	GWeightInfos res(arg1);
	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GWeightInfos GALILEI::operator-(const GWeightInfos &arg1,const GWeightInfos &arg2)
{
	GWeightInfos res(arg1);
	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GWeightInfos GALILEI::operator*(const double arg1,const GWeightInfos &arg2)
{
	GWeightInfos res(arg2);
	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
GWeightInfos GALILEI::operator*(const GWeightInfos& arg1,const double arg2)
{
	GWeightInfos res(arg1);
	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GWeightInfos GALILEI::operator/(const GWeightInfos& arg1,const double arg2)
{
	GWeightInfos res(arg1);
	res/=arg2;
	return(res);
}
