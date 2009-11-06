/*

	GALILEI Research Project

	GenericSims.cpp

	Generic Similarity Measure - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Université Libre de Bruxelles (ULB).

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
#include <gweightinfosobj.h>
#include <gweightinfo.h>
#include <glang.h>
#include <gconcept.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gtopic.h>
#include <gcommunity.h>
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
	double w1,w2;
	double num(0.0);

	LastNbComps=0;
	while((!Obj1.End())&&(Obj1()->GetConcept()->GetType()==Type))
	{
		w1=Obj1()->GetWeight()*Owner->GetIF(Obj1()->GetConcept());
		#if NormalizeVector
			if(Obj1()->GetWeight()>max1)
				max1=Obj1()->GetWeight();
		#endif
		while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type)&&(Obj2()->GetConcept()->GetId()<Obj1()->GetConcept()->GetId()))
		{
			w2=Obj2()->GetWeight()*Owner->GetIF(Obj2()->GetConcept());
			#if NormalizeVector
				if(Obj2()->GetWeight()>max2)
					max2=Obj2()->GetWeight();
			#endif
			norm2+=w2*w2;
			Obj2.Next();
		}
		if((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type)&&(Obj2()->GetConcept()->GetId()==Obj1()->GetConcept()->GetId()))
		{
			// Obj2()==Obj1() -> iff2=iff1
			w2=Obj2()->GetWeight()*Owner->GetIF(Obj2()->GetConcept());
			#if NormalizeVector
				if(Obj2()->GetWeight()>max2)
					max2=Obj2()->GetWeight();
			#endif
			norm2+=w2*w2;
			if((Obj1()->GetWeight()>0)||(Obj2()->GetWeight()>0))
			{
				num+=w1*w2;
				LastNbComps++;
			}
			Obj2.Next();
		}
		norm1+=w1*w1;
		Obj1.Next();
	}
	while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type))
	{
		w2=Obj2()->GetWeight()*Owner->GetIF(Obj2()->GetConcept());
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

	double sim(num/(sqrt(norm1*norm2)));
	if(Owner->Transform)
		sim=(sim+1.0)/2.0;
	return(sim);
}



//------------------------------------------------------------------------------
//
// class GSimTypeXMLIndex
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double GSimTypeXMLIndex::Compute(RCursor<GWeightInfo>& Obj1,RCursor<GWeightInfo>& Obj2)
{
	double w1,w2;
	double num(0.0);
	double den(0.0);

	while((!Obj1.End())&&(Obj1()->GetConcept()->GetType()==Type))
	{
		w1=Obj1()->GetWeight()*Owner->GetIF(Obj1()->GetConcept());

		GXMLIndex* c1=dynamic_cast<GXMLIndex*>(Obj1()->GetConcept());
		RCursor<GWeightInfo> Cur(Obj2);
		for(;(!Cur.End())&&(Cur()->GetConcept()->GetType()==Type);Cur.Next())
		{
			GXMLIndex* c2=dynamic_cast<GXMLIndex*>(Cur()->GetConcept());

			// Compare only if both index are based on the same XML tag
			if(c1->GetXMLTag()!=c2->GetXMLTag())
				continue;

			w2=Cur()->GetWeight()*Owner->GetIF(Obj2()->GetConcept());
			den+=fabs(w1*w2);
			if((w1<0.0)&&(w2<0.0))
				continue;
//			cout<<c1->GetSimilarity(c2)<<endl;
			num+=c1->GetSimilarity(c2)*w1*w2;
		}
		Obj1.Next();
	}
	while((!Obj2.End())&&(Obj2()->GetConcept()->GetType()==Type))
		Obj2.Next();

	// Return similarity
	if(den==0.0)
		return(0.0);

	double sim(num/den);

	if(Owner->Transform)
		sim=(sim+1.0)/2.0;

	return(sim);
}



//------------------------------------------------------------------------------
//
//  GGenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGenericSims::GGenericSims(GPluginFactory* fac,tObjType lines,tObjType cols)
	: GMatrixMeasure(fac,lines,cols,lines==cols), vec1(0), vec2(0), Types(30)
{
}


//------------------------------------------------------------------------------
RString GGenericSims::GetFilesName(void) const
{
	return(RString(GMatrixMeasure::GetFilesName()+RString::Number(SimType)));
}


//------------------------------------------------------------------------------
void GGenericSims::ApplyConfig(void)
{
	GMatrixMeasure::ApplyConfig();
	RString type=Factory->Get("SimType");
	Factor=Factory->GetDouble("Factor");
	ContentCapacity=Factory->GetDouble("ContentCapacity");
	StructCapacity=Factory->GetDouble("StructCapacity");
	MetaCapacity=Factory->GetDouble("MetaCapacity");
	ContentStructCapacity=Factory->GetDouble("ContentStructCapacity");
	ContentMetaCapacity=Factory->GetDouble("ContentMetaCapacity");
	MetaStructCapacity=Factory->GetDouble("MetaStructCapacity");
	Transform=Factory->GetBool("Transform");
	tSimType sim(Undefined);
	if(type=="Product")
		sim=Product;
	else if(type=="Language only")
		sim=LanguageOnly;
	else if(type=="Sum")
		sim=Sum;
	else if(type=="Integral of Choquet")
		sim=Choquet;
	if(sim==Undefined)
	{
		cerr<<"No valid similarity measure"<<endl;
		SimType=Choquet;
	}
	else
		SimType=sim;
}


//------------------------------------------------------------------------------
void GGenericSims::Connect(GSession* session)
{
	GMatrixMeasure::Connect(session);
	MetaSpace=Session->GetConceptType("XMLIndex",false);
	StructSpace=Session->GetConceptType("XMLStruct",false);
	RCursor<GConceptType> Cur(Session->GetConceptTypes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Takes only the language, the structure and the metadata space
		if(Cur()==MetaSpace)
			Types.InsertPtr(new GSimTypeXMLIndex(this,Cur()));
		else if(Cur()==StructSpace)
			Types.InsertPtr(new GSimType(this,Cur()));
		else if(Cur()->GetName().Mid(2)=="Terms")
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
bool GGenericSims::ComputeSimSpace(void)
{
	// if one vector is not defined -> the similarity must be null
	if((!vec1->GetVector().GetNb())||(!vec2->GetVector().GetNb()))
		return(false);
	RCursor<GWeightInfo> ptr(vec1->GetVector().GetInfos());
	RCursor<GWeightInfo> ptr2(vec2->GetVector().GetInfos());
	RCursor<GSimType> Cur(Types);
	double Ok(false);
	size_t TotalLangsComp(0);

	SimSpaces[0]=SimSpaces[1]=SimSpaces[2]=0.0;

	for(ptr.Start(),ptr2.Start(),Cur.Start();(!Cur.End())&&(!ptr.End())&&(!ptr2.End());Cur.Next())
	{
		if((ptr()->GetConcept()->GetType()==Cur()->Type)&&(ptr2()->GetConcept()->GetType()==Cur()->Type))
		{
			// OK Compute it
			double d=Cur()->Compute(ptr,ptr2);
			//RAssert(fabs(d)<=1.0)
			if(fabs(d-1.0)<GetCutoffFrequency())
				d=1.0;
			if(fabs(1.0+d)<GetCutoffFrequency())
				d=-1.0;
			if(Cur()->Type->GetLang())
			{
				// Language -> Make the weighted sum for all languages
				SimSpaces[0]+=d*static_cast<double>(Cur()->LastNbComps);
				TotalLangsComp+=Cur()->LastNbComps;
				Ok=true;
			}
			else
			{
				if(Cur()->Type==StructSpace)
				{
					SimSpaces[1]=d;
					Ok=true;
				}
				if(Cur()->Type==MetaSpace)
				{
					SimSpaces[2]=d;
					Ok=true;
				}
			}
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
	if(TotalLangsComp)
		SimSpaces[0]/=static_cast<double>(TotalLangsComp);
//	cout<<"\t(0,"<<SimSpaces[0]<<")\t(1,"<<SimSpaces[1]<<")\t(2,"<<SimSpaces[2]<<")";
	return(Ok);
}


//------------------------------------------------------------------------------
double GGenericSims::SimilarityChoquet(void)
{
	if(!ComputeSimSpace())
		return(0.0);

	// Put them in descending order
	struct tSimSpace
	{
		double Sim;
		int What;
		static int Compare(const void *a, const void *b)
		{
		  double temp = static_cast<const tSimSpace*>(a)->Sim - static_cast<const tSimSpace*>(b)->Sim;
		  if(temp>0)
		    return(1);
		  else if(temp<0)
		    return(-1);
		  else
		    return(0);
		}

	};
	tSimSpace Tab[3];
	Tab[0].Sim=SimSpaces[0];
	Tab[0].What=0;
	Tab[1].Sim=SimSpaces[1];
	Tab[1].What=1;
	Tab[2].Sim=SimSpaces[2];
	Tab[2].What=2;
	qsort(Tab,static_cast<size_t>(3),sizeof(tSimSpace),tSimSpace::Compare);

	// First element (capacity=1)
	double Choquet(Tab[0].Sim);

	// Second element: Find the right capacity (in Cap)
	double Cap;
	if((Tab[1].What==0&&Tab[2].What==1)||(Tab[1].What==1&&Tab[2].What==0))
		Cap=ContentStructCapacity;
	else if((Tab[1].What==0&&Tab[2].What==2)||(Tab[1].What==2&&Tab[2].What==0))
		Cap=ContentMetaCapacity;
	else if((Tab[1].What==1&&Tab[2].What==2)||(Tab[1].What==2&&Tab[2].What==1))
		Cap=MetaStructCapacity;
	else
		Cap=0;
	Choquet+=(Tab[1].Sim-Tab[0].Sim)*Cap;

	// Third element : Find the right capacity
	if(Tab[2].What==0)
		Cap=ContentCapacity;
	else if(Tab[2].What==1)
		Cap=StructCapacity;
	else if(Tab[2].What==2)
		Cap=MetaCapacity;
	return(Choquet+(Tab[2].Sim-Tab[1].Sim)*Cap);
}


//------------------------------------------------------------------------------
double GGenericSims::SimilaritySum(void)
{
	if(!ComputeSimSpace())
		return(0.0);
	return((SimSpaces[0]*ContentCapacity+SimSpaces[1]*StructCapacity+SimSpaces[2]*MetaCapacity)/(ContentCapacity+StructCapacity+MetaCapacity));
}


//------------------------------------------------------------------------------
double GGenericSims::SimilarityProduct(void)
{
	if(!ComputeSimSpace())
		return(0.0);
	double* ptr=SimSpaces;
	double Sim(0.0);
	double NbComps(static_cast<double>(GetNbDiffElements()));
	NbComps=Factor*NbComps*(NbComps-1);

	// Makes the product of the similarity (a null similarity is replaced by Factor).
	for(int i=0;i<3;i++,ptr++)
		Sim+=log10(*ptr+Factor);
	return(pow(10,Sim-3.0*log10(NbComps)));
}


//------------------------------------------------------------------------------
double GGenericSims::SimilarityLang(void)
{
	// If one vector is not defined -> the similarity must be null
	if((!vec1->GetVector().GetNb())||(!vec2->GetVector().GetNb()))
		return(0.0);

	//-------------------------------------------------------
	// Suppose the two vectors have only a language in common.

	// Parse vec1 until a language is found
	RCursor<GWeightInfo> ptr(vec1->GetVector().GetInfos());
	ptr.Start();
	while((!ptr.End())&&(!ptr()->GetConcept()->GetType()->GetLang()))
		ptr.Next();
	if(ptr.End())
		return(0.0); // No language found -> Similarity is null
	GConceptType* Lang=(ptr()->GetConcept()->GetType());

	// Parse vec2 until first language is found
	RCursor<GWeightInfo> ptr2(vec2->GetVector().GetInfos());
	ptr2.Start();
	while((!ptr2.End())&&(!ptr2()->GetConcept()->GetType()->GetLang()))
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
	if(fabs(d)<GetCutoffFrequency())
		return(0.0);
	return(d);
}


//------------------------------------------------------------------------------
double GGenericSims::Compute(void* obj1,void* obj2)
{
	if(obj1==obj2)
		return(1.0);
	vec1=static_cast<GWeightInfosObj*>(obj1);
	vec2=static_cast<GWeightInfosObj*>(obj2);
	if((!vec1->GetVector().IsDefined())||(!vec2->GetVector().IsDefined()))
		return(0.0);
	double sim(0.0);
	switch(SimType)
	{
		case Undefined:
			throw GException("No similarity model defined.");
			break;
		case Product:
			sim=SimilarityProduct();
			break;
		case LanguageOnly:
			sim=SimilarityLang();
			break;
		case Sum:
			sim=SimilaritySum();
			break;
		case Choquet:
			sim=SimilarityChoquet();
			break;
	}
//	cout<<"\t->\t"<<sim<<endl;
	return(sim);
}


//------------------------------------------------------------------------------
/*size_t GGenericSims::GetRef(GConceptType* type)
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
*/

//------------------------------------------------------------------------------
double GGenericSims::GetIF(GConcept* concept)
{
	double IF(concept->GetIF(GetLinesType()));
	if(GetLinesType()!=GetColsType())
		IF+=concept->GetIF(GetColsType());
	return(IF);
}


//------------------------------------------------------------------------------
void GGenericSims::CreateParams(RConfig* params)
{
	GMatrixMeasure::CreateParams(params);
	params->InsertParam(new RParamValue("SimType","Integral of Choquet"));
	params->InsertParam(new RParamValue("Factor",0.01));
	params->InsertParam(new RParamValue("ContentCapacity",0.01));
	params->InsertParam(new RParamValue("StructCapacity",0.01));
	params->InsertParam(new RParamValue("MetaCapacity",0.01));
	params->InsertParam(new RParamValue("ContentStructCapacity",0.01));
	params->InsertParam(new RParamValue("ContentMetaCapacity",0.01));
	params->InsertParam(new RParamValue("MetaStructCapacity",0.01));
	params->InsertParam(new RParamValue("Transform",false));
}
