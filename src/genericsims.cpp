/*

	GALILEI Research Project

	GenericSims.cpp

	Similarities between documents and groups - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

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


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfos.h>
#include <gweightinfo.h>
#include <glang.h>
#include <gconcept.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gxmlindex.h>
#include <gsession.h>


//------------------------------------------------------------------------------
// include files
#include "genericsims.h"
using namespace std;


//-----------------------------------------------------------------------------
// Defines
#define NormalizeVector 0


//------------------------------------------------------------------------------
//
// class GSimType
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSimType::GSimType(GGenericSims* owner,GConceptType* type)
	: Owner(owner), Type(type)
{
}


//------------------------------------------------------------------------------
double GSimType::Compute(RCursor<GWeightInfo>& Obj1,RCursor<GWeightInfo>& Obj2)
{
	#if NormalizeVector
		double max1(-2);
		double max2(-2);
	#endif
	double norm1(0.0);
	double norm2(0.0);
	double w1,w2,iff1,iff2;
	double TotalRef(Owner->GetRef(Type));
	double num(0.0);

	while((!Obj1.End())&&(Obj1()->GetConcept()->GetType()==Type))
	{
		iff1=log10(TotalRef/static_cast<double>(Owner->GetRef(Obj1()->GetId(),Type)));
		w1=Obj1()->GetWeight()*iff1;
		#if NormalizeVector
			if(Obj1()->GetWeight()>max1)
				max1=Obj1()->GetWeight();
		#endif
		while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type)&&((*Obj2())<(*Obj1())))
		{
			iff2=log10(TotalRef/static_cast<double>(Owner->GetRef(Obj2()->GetId(),Type)));
			w2=Obj2()->GetWeight()*iff2;
			#if NormalizeVector
				if(Obj2()->GetWeight()>max2)
					max2=Obj2()->GetWeight();
			#endif
			norm2+=w2*w2;
			Obj2.Next();
		}
		if((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type)&&((*Obj2())==(*Obj1())))
		{
			// Obj2()==Obj1() -> iff2=iff1
			w2=Obj2()->GetWeight()*iff1;
			#if NormalizeVector
				if(Obj2()->GetWeight()>max2)
					max2=Obj2()->GetWeight();
			#endif
			norm2+=w2*w2;
			if((Obj1()->GetWeight()>0)||(Obj2()->GetWeight()>0))
				num+=w1*w2;
			Obj2.Next();
		}
		norm1+=w1*w1;
		Obj1.Next();
	}
	while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type))
	{
		iff2=log10(TotalRef/static_cast<double>(Owner->GetRef(Obj2()->GetId(),Type)));
		w2=Obj2()->GetWeight()*iff2;
		#if NormalizeVector
			if(w2>max2)
				max2=w2;
		#endif
		norm2+=w2*w2;
		Obj2.Next();
	}

	// Return similarity
	if(num==0.0)
		return(0.0);
	#if NormalizeVector
		num/=max1*max2;
		norm1/=max1*max1;
		norm2/=max2*max2;
	#endif
	return(num/(sqrt(norm1*norm2)));
}



//------------------------------------------------------------------------------
//
// class GSimTypeXMLIndex
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double GSimTypeXMLIndex::Compute(RCursor<GWeightInfo>& Obj1,RCursor<GWeightInfo>& Obj2)
{
	double w1,w2,iff1,iff2;
	double TotalRef(Owner->GetRef(Type));
	double num(0.0);
	double den(0.0);

	while((!Obj1.End())&&(Obj1()->GetConcept()->GetType()==Type))
	{
		iff1=log10(TotalRef/static_cast<double>(Owner->GetRef(Obj1()->GetId(),Type)));
		w1=Obj1()->GetWeight()*iff1;

		GXMLIndex* c1=dynamic_cast<GXMLIndex*>(Obj1()->GetConcept());
		RCursor<GWeightInfo> Cur(Obj2);
		for(;(!Cur.End())&&(Cur()->GetConcept()->GetType()==Type);Cur.Next())
		{
			GXMLIndex* c2=dynamic_cast<GXMLIndex*>(Cur()->GetConcept());

			// Compare only if both index are based on the same XML tag
			if(c1->GetXMLTag()!=c2->GetXMLTag())
				continue;

			iff2=log10(TotalRef/static_cast<double>(Owner->GetRef(Cur()->GetId(),Type)));
			w2=Cur()->GetWeight()*iff2;
			den+=fabs(w1*w2);
			if((w1<0.0)&&(w2<0.0))
				continue;
			num+=c1->GetSimilarity(c2)*w1*w2;
		}
		Obj1.Next();
	}
	while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type))
		Obj2.Next();

	// Return similarity
	if(den==0.0)
		return(0.0);
	return(num/den);
}



//------------------------------------------------------------------------------
//
//  GGenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGenericSims::GGenericSims(GFactoryMeasure* fac,tObjType lines,tObjType cols)
	: GMatrixMeasure(fac,lines,cols,lines==cols), Types(30)
{
}


//------------------------------------------------------------------------------
void GGenericSims::ApplyConfig(void)
{
	if(Session)
		return;
	GMatrixMeasure::ApplyConfig();
	SimType=0;
	RString type=Factory->Get("SimType");
	Factor=Factory->GetDouble("Factor");
	unsigned int sim(0);
	if(type=="Multi-space")
		sim=1;
	if(type=="Language")
		sim=2;
	if(!sim)
	{
		cerr<<"No valid similarity measure"<<endl;
		SimType=1;
	}
	else
		SimType=sim;
}


//------------------------------------------------------------------------------
void GGenericSims::Connect(GSession* session)
{
	GMatrixMeasure::Connect(session);
	GConceptType* IndexSpace=Session->GetInsertConceptType("XMLIndex","XML Index");
	RCursor<GConceptType> Cur(Session->GetConceptTypes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()==IndexSpace)
			Types.InsertPtr(new GSimTypeXMLIndex(this,Cur()));
		else
			Types.InsertPtr(new GSimType(this,Cur()));
	}
	CurLang=0;
	CurType=0;
}


//------------------------------------------------------------------------------
void GGenericSims::Disconnect(GSession* session)
{
	Types.Clear();
	GMatrixMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
double GGenericSims::SimilarityIFFMV(void)
{
	// if one vector is not defined -> the similarity must be null
	if((!vec1->GetNb())||(!vec2->GetNb()))
		return(0.0);

	double Sim(0.0);
	double NbComps(GetNbDiffElements());
	NbComps=Factor*NbComps*(NbComps-1);
	double CommonSpace(0.0); // Suppose that the two vectors have no common spaces
	RCursor<GWeightInfo> ptr(*vec1);
	RCursor<GWeightInfo> ptr2(*vec2);
	RCursor<GSimType> Cur(Types);
	for(ptr.Start(),ptr2.Start(),Cur.Start();(!Cur.End())&&(!ptr.End())&&(!ptr2.End());Cur.Next())
	{
		if((ptr()->GetConcept()->GetType()==Cur()->Type)&&(ptr2()->GetConcept()->GetType()==Cur()->Type))
		{
			// OK Compute it
			double d=Cur()->Compute(ptr,ptr2);
			if(d<GetNullValue())
				d=Factor;
			if(fabs(d-1.0)<GetNullValue())
				d=1.0;
			if(fabs(1.0+d)<GetNullValue())
				d=-1.0;
//			cout<<"  "<<d<<endl;
			RAssert(fabs(d)<=1.0)
			Sim+=log10(d);
//			Sim*=(d/NbComps);
			CommonSpace+=1.0;
		}
		else
		{
			// At least one of the vector has no concepts of type Cur()->Type
			// Skip it
			while((!ptr.End())&&(ptr()->GetConcept()->GetType()==Cur()->Type))
				ptr.Next();
			while((!ptr2.End())&&(ptr2()->GetConcept()->GetType()==Cur()->Type))
				ptr2.Next();
		}
	}
	if(CommonSpace>0.0)
		return(pow(10,Sim-CommonSpace*log10(NbComps)));
/*	if(CommonSpace>0.0)
		return(Sim);*/
	return(0.0);
}


//------------------------------------------------------------------------------
double GGenericSims::SimilarityIFFL(void)
{
	// If one vector is not defined -> the similarity must be null
	if((!vec1->GetNb())||(!vec2->GetNb()))
		return(0.0);

	//-------------------------------------------------------
	// Suppose the two vectors have only a language in common.

	// Parse vec1 until a language is found
	RCursor<GWeightInfo> ptr(*vec1);
	ptr.Start();
	while((!ptr.End())&&(ptr()->GetConcept()->GetType()->GetLang()))
		ptr.Next();
	if(ptr.End())
		return(0.0); // No language found -> Similarity is null
	GConceptType* Lang=(ptr()->GetConcept()->GetType());

	// Parse vec2 until first language is found
	RCursor<GWeightInfo> ptr2(*vec2);
	ptr2.Start();
	while((!ptr2.End())&&(ptr2()->GetConcept()->GetType()->GetLang()))
		ptr2.Next();
	if(ptr.End())
		return(0.0); // No language found -> Similarity is null

	// If both language are not the same -> Similarity is null
	if(Lang!=ptr2()->GetConcept()->GetType())
		return(0.0);

	// Compute it now.
	if(CurLang!=Lang)
	{
		CurLang=Lang;
		CurType=Types.GetPtr(CurLang);
	}
	double d=CurType->Compute(ptr,ptr2);
	if(fabs(d)<GetNullValue())
		return(0.0);
	return(d);
}


//------------------------------------------------------------------------------
double GGenericSims::Compute(void* obj1,void* obj2)
{
	if(obj1==obj2)
		return(1.0);
	vec1=static_cast<GWeightInfos*>(obj1);
	vec2=static_cast<GWeightInfos*>(obj2);
	switch(SimType)
	{
		case 1:
		{
/*			double res(SimilarityIFFMV());
			cout<<"Compute sim("<<static_cast<GProfile*>(obj1)->GetId()<<","<<static_cast<GProfile*>(obj2)->GetId()<<")="<<res<<endl;
			return(res);*/
			return(SimilarityIFFMV());
			break;
		}
		case 2:
/*			double res(SimilarityIFFL());
			cout<<"Compute sim("<<static_cast<GDoc*>(obj1)->GetId()<<","<<static_cast<GDoc*>(obj2)->GetId()<<")="<<res<<endl;
			return(res);*/
			return(SimilarityIFFL());
	}
	return(0.0);
}


//------------------------------------------------------------------------------
size_t GGenericSims::GetRef(GConceptType* type)
{
	size_t nb(type->GetRef(GetLinesType()));
	if(GetLinesType()!=GetColsType())
		nb+=type->GetRef(GetColsType());
	return(nb);
}

//------------------------------------------------------------------------------
size_t GGenericSims::GetRef(size_t id,GConceptType* type)
{
	size_t nb(type->GetRef(id,GetLinesType()));
	if(GetLinesType()!=GetColsType())
		nb+=type->GetRef(id,GetColsType());
	return(nb);
}


//------------------------------------------------------------------------------
void GGenericSims::CreateParams(RConfig* params)
{
	GMatrixMeasure::CreateParams(params);
	params->InsertParam(new RParamValue("SimType","Multi-space"));
	params->InsertParam(new RParamValue("Factor",0.01));
}
