/*

	GALILEI Research Project

	GenericSims.cpp

	Generic Similarity Measure - Inline Implementation.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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


//-----------------------------------------------------------------------------
// Defines
#define NormalizeVector 0


//------------------------------------------------------------------------------
//
// class GSimType
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//GSimType::GSimType(GGenericSims* owner,GConceptType* type)
//	: Owner(owner), Type(type)
//{
//}



//------------------------------------------------------------------------------
//
// class GSimTypeCosinys
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//void GSimTypeCosinus::Init(void)
//{
//	Norm1=0.0;
//	Norm2=0.0;
//	Num=0.0;
//	Value=0.0;
//	LastNbComps=0;
//	#if NormalizeVector
//		Max1=-2;
//		Max2=-2;
//	#endif
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeCosinus::Add(GConceptRef* info1,GConceptRef* info2,GMeasure* mes)
//{
//	double w1(info1->GetWeight()*GetIF(info1->GetConcept(),mes));
//	Norm1+=w1*w1;
//	double w2(info2->GetWeight()*GetIF(info2->GetConcept(),mes));
//	Norm2+=w2*w2;
//	if((info1->GetWeight()>0)||(info2->GetWeight()>0))
//	{
//		Num+=w1*w2;
//		LastNbComps++;
//	}
//	#if NormalizeVector
//	if(info1->GetWeight()>Max1)
//		Max1=info1->GetWeight();
//	if(info2->GetWeight()>Max2)
//		Max2=info2->GetWeight();
//	#endif
//
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeCosinus::AddObj1(GConceptRef* info,GMeasure* mes)
//{
//	double w1(info->GetWeight()*GetIF(info->GetConcept(),mes));
//	Norm1+=w1*w1;
//	#if NormalizeVector
//		if(info->GetWeight()>Max1)
//			Max1=info->GetWeight();
//	#endif
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeCosinus::AddObj2(GConceptRef* info,GMeasure* mes)
//{
//	double w2(info->GetWeight()*GetIF(info->GetConcept(),mes));
//	Norm2+=w2*w2;
//	#if NormalizeVector
//		if(info->GetWeight()>Max2)
//			Max2=info->GetWeight();
//	#endif
//
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeCosinus::Done(GMeasure*)
//{
//	if(Num==0.0)
//		Value=0.0;
//	else
//	{
//		#if NormalizeVector
//			Num/=Max1*Max2;
//			Norm1/=Max1*Max1;
//			Norm2/=Max2*Max2;
//		#endif
//		Value=Num/(sqrt(Norm1)*sqrt(Norm2));
//	}
//	if(Owner->Transform)
//		Value=(Value+1.0)/2.0;
//}


//------------------------------------------------------------------------------
//
// class GSimTypeXMLIndex
//
//------------------------------------------------------------------------------

////------------------------------------------------------------------------------
//void GSimTypeXMLIndex::Init(void)
//{
//	Obj1.Clear();
//	Obj2.Clear();
//	Value=0;
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeXMLIndex::Add(GConceptRef* info1,GConceptRef* info2,GMeasure*)
//{
//	Obj1.InsertPtr(info1);
//	Obj2.InsertPtr(info2);
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeXMLIndex::AddObj1(GConceptRef* info,GMeasure*)
//{
//	Obj1.InsertPtr(info);
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeXMLIndex::AddObj2(GConceptRef* info,GMeasure*)
//{
//	Obj2.InsertPtr(info);
//}
//
//
////------------------------------------------------------------------------------
//void GSimTypeXMLIndex::Done(GMeasure* mes)
//{
//	double w1,w2;
//	double num(0.0);
//	double den(0.0);
//
//	RCursor<GConceptRef> Cur1(Obj1);
//	RCursor<GConceptRef> Cur2(Obj2);
//	for(Cur1.Start();!Cur1.End();Cur1.Next())
//	{
//		w1=Cur1()->GetWeight()*GetIF(Cur1()->GetConcept(),mes);
//
//		GXMLIndex* c1(dynamic_cast<GXMLIndex*>(Cur1()->GetConcept()));
//		for(Cur2.Start();!Cur2.End();Cur2.Next())
//		{
//			GXMLIndex* c2(dynamic_cast<GXMLIndex*>(Cur2()->GetConcept()));
//
//			// Compare only if both index are based on the same XML tag
//			if(c1->GetXMLTag()!=c2->GetXMLTag())
//				continue;
//
//			w2=Cur2()->GetWeight()*GetIF(Cur2()->GetConcept(),mes);
//			den+=fabs(w1*w2);
//			if((w1<0.0)&&(w2<0.0))
//				continue;
////			cout<<c1->GetSimilarity(c2)<<endl;
//			num+=c1->GetSimilarity(c2)*w1*w2;
//		}
//	}
//
//	// Return similarity
//	if(den!=0.0)
//		Value=num/den;
//
//	if(Owner->Transform)
//		Value=(Value+1.0)/2.0;
//}



//------------------------------------------------------------------------------
//
//  GGenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	GGenericSims<cObj1,cObj2>::GGenericSims(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols)
	: GMatrixMeasure(session,fac,lines,cols,lines==cols), Desc1(0), Desc2(0),/*Types(30), Valid(30),*/ FeatureEval(0)
{
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	RString GGenericSims<cObj1,cObj2>::GetFilesName(void) const
{
	return(RString(GMatrixMeasure::GetFilesName()+RString::Number(SimType)));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::ApplyConfig(void)
{
	GMatrixMeasure::ApplyConfig();
	RString type=FindParam<RParamValue>("SimType")->Get();
	Factor=FindParam<RParamValue>("Factor")->GetDouble();
	ContentCapacity=FindParam<RParamValue>("ContentCapacity")->GetDouble();
	StructCapacity=FindParam<RParamValue>("StructCapacity")->GetDouble();
	MetaCapacity=FindParam<RParamValue>("MetaCapacity")->GetDouble();
	ContentStructCapacity=FindParam<RParamValue>("ContentStructCapacity")->GetDouble();
	ContentMetaCapacity=FindParam<RParamValue>("ContentMetaCapacity")->GetDouble();
	MetaStructCapacity=FindParam<RParamValue>("MetaStructCapacity")->GetDouble();
	Transform=FindParam<RParamValue>("Transform")->GetBool();
	tSimType sim(Undefined);
	if(type=="Product")
		sim=Product;
	else if(type=="Text Only")
		sim=TextOnly;
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
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::Init(void)
{
	GMatrixMeasure::Init();

//	GSimType* ptr;
	TextSpace=Session->GetConceptCat("Text",false);
	MetaSpace=Session->GetConceptCat("MetaData",false);
	StructSpace=Session->GetConceptCat("Structure",false);
//	RCursor<GConceptType> Cur(Session->GetConceptTypes());
//	for(Cur.Start();!Cur.End();Cur.Next())
//	{
//		// Takes only the language, the structure and the metadata space
//		if(Cur()==MetaSpace)
//		{
//			Types.InsertPtrAt(ptr=new GSimTypeXMLIndex(this,Cur()),Cur()->GetId());
//			Valid.InsertPtr(ptr);
//		}
//		else if(Cur()==StructSpace)
//		{
//			Types.InsertPtrAt(ptr=new GSimTypeCosinus(this,Cur()),Cur()->GetId());
//			Valid.InsertPtr(ptr);
//		}
//		else if(Cur()->GetName().Mid(2)=="Terms")
//		{
//			Types.InsertPtrAt(ptr=new GSimTypeCosinus(this,Cur()),Cur()->GetId());
//			Valid.InsertPtr(ptr);
//		}
//	}
	CurLang=0;
//	CurType=0;
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::ComputeMetaSim(GVector* vec1,GVector* vec2)
{
	double Num(0.0);
	size_t NbCommon(0),NbDiff(0);

	RCursor<GConceptRef> ptr1(vec1->GetRefs());
	RCursor<GConceptRef> ptr2(vec2->GetRefs());
	for(ptr1.Start(),ptr2.Start();!ptr1.End();ptr1.Next())
	{
		NbDiff++;

		for(;(!ptr2.End())&&(ptr2()->GetId()<ptr1()->GetId());ptr2.Next())
			NbDiff++;

		// Same concept -> Add Num
		if((!ptr2.End())&&(ptr2()->GetId()==ptr1()->GetId()))
		{
			double w1(ptr1()->GetWeight()*GetIF(ptr1()->GetConcept()));
			double w2(ptr2()->GetWeight()*GetIF(ptr2()->GetConcept()));

			// If both weight are negative -> pass
		   if((w1<0.0)&&(w2<0.0))
				continue;

			// Compute the product
			NbCommon++;
			double d(w2*w1);
			if(fabs(d-1.0)<GetCutoffFrequency())
				d=1.0;
			if(fabs(1.0+d)<GetCutoffFrequency())
				d=-1.0;
			Num+=d;
		}
	}

	// Compute the rest of the NbDiff
	for(;!ptr2.End();ptr2.Next())
		NbDiff++;

	// Assign to the category
	if(Num!=0.0)
		SimSpaces[2]+=(NbCommon*Num)/static_cast<double>(NbDiff);
	NbSpaces[2]+=NbCommon;
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::ComputeCosineSim(GVector* vec1,GVector* vec2,GConceptCat* cat)
{
	double Num(0.0),Norm1(0.0),Norm2(0.0);
	size_t NbCommon(0);

	RCursor<GConceptRef> ptr1(vec1->GetRefs());
	RCursor<GConceptRef> ptr2(vec2->GetRefs());
	for(ptr1.Start(),ptr2.Start();!ptr1.End();ptr1.Next())
	{
		double w1(ptr1()->GetWeight()*GetIF(ptr1()->GetConcept()));
		Norm1+=w1*w1;
		for(;(!ptr2.End())&&(ptr2()->GetId()<ptr1()->GetId());)
		{
			double w2(ptr2()->GetWeight()*GetIF(ptr2()->GetConcept()));
			Norm2+=w2*w2;
			ptr2.Next();
		}

		if((!ptr2.End())&&(ptr2()->GetId()==ptr1()->GetId()))
		{
			double w2(ptr2()->GetWeight()*GetIF(ptr2()->GetConcept()));
			Norm2+=w2*w2;

			// If both weight are negative -> pass
		   if((w1<0.0)&&(w2<0.0))
				continue;

			// Compute the product
			double d(w2*w1);
			if(fabs(d-1.0)<GetCutoffFrequency())
				d=1.0;
			if(fabs(1.0+d)<GetCutoffFrequency())
				d=-1.0;
			Num+=d;
			NbCommon++;
		}
	}

	// Rest of Norm2
	for(;!ptr2.End();ptr2.Next())
	{
		double w2(ptr2()->GetWeight()*GetIF(ptr2()->GetConcept()));
		Norm2+=w2*w2;
	}

	// Assign to the right category
	if(cat==TextSpace)
	{
		if(NbCommon)
			SimSpaces[0]+=(NbCommon*Num)/(Norm1*Norm2);
		NbSpaces[0]+=NbCommon;
	}
	else if(cat==StructSpace)
	{
		if(NbCommon)
			SimSpaces[1]+=(NbCommon*Num)/(Norm1*Norm2);
		NbSpaces[1]+=NbCommon;
	}
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	bool GGenericSims<cObj1,cObj2>::ComputeSimSpaces(void)
{
	FeatureEval=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation");

	// Init all the measures
	SimSpaces[0]=SimSpaces[1]=SimSpaces[2]=0.0;
	NbSpaces[0]=NbSpaces[1]=NbSpaces[2]=0;

	// Go trough the vectors and compare those having the same similarity
	RCursor<GVector> Vec1(Desc1->GetVectors());
	RCursor<GVector> Vec2(Desc2->GetVectors());
	for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next())
	{
		for(;(!Vec2.End())&&(Vec2()->GetConceptId()<Vec1()->GetConceptId());)
			Vec2.Next();

		if((!Vec2.End())&&(Vec2()->GetConceptId()==Vec1()->GetConceptId()))
		{
			GConceptCat* Cat(Vec1()->GetConcept()->GetType()->GetCategory());
			if(Cat==MetaSpace)
				ComputeMetaSim(Vec1(),Vec2());
			else if((Cat==TextSpace)||(Cat==StructSpace))
				ComputeCosineSim(Vec1(),Vec2(),Cat);
		}
	}
	SimSpaces[0]/=static_cast<double>(NbSpaces[0]);
	SimSpaces[1]/=static_cast<double>(NbSpaces[1]);
	SimSpaces[2]/=static_cast<double>(NbSpaces[2]);


	// if one vector is not defined -> the similarity must be null
//	if((!vec1->GetVector().GetNb())||(!vec2->GetVector().GetNb()))
//		return(false);

//	size_t TotalLangsComp(0);
//	RCursor<GSimType> Sims(Valid);
//
//	// Init all the measures
//	for(Sims.Start();!Sims.End();Sims.Next())
//		Sims()->Init();
//
//	SimSpaces[0]=SimSpaces[1]=SimSpaces[2]=0.0;
//
//	// Go trough all the information
//	RCursor<GConceptRef> Info1(vec1->GetVector().GetInfos());
//	RCursor<GConceptRef> Info2(vec2->GetVector().GetInfos());
//	for(Info1.Start(),Info2.Start();!Info1.End();Info1.Next())
//	{
//		for(;(!Info2.End())&&(Info2()->GetId()<Info1()->GetId());Info2.Next())
//			Types[Info2()->GetConcept()->GetType()->GetId()]->AddObj2(Info2(),FeatureEval);
//
//		if((!Info2.End())&&(Info2()->GetId()==Info1()->GetId()))
//		{
//			Types[Info2()->GetConcept()->GetType()->GetId()]->Add(Info1(),Info2(),FeatureEval);
//			Info2.Next();
//		}
//		else
//			Types[Info1()->GetConcept()->GetType()->GetId()]->AddObj1(Info1(),FeatureEval);
//	}
//	for(;!Info2.End();Info2.Next())
//		Types[Info2()->GetConcept()->GetType()->GetId()]->AddObj2(Info2(),FeatureEval);
//
//	// Done all the measures
//	for(Sims.Start();!Sims.End();Sims.Next())
//		Sims()->Done(FeatureEval);
//
//	// Dispatch the measures
//	for(Sims.Start();!Sims.End();Sims.Next())
//	{
//		// OK Compute it
//		double d(Sims()->Value);
//		//RAssert(fabs(d)<=1.0)
//		if(fabs(d-1.0)<GetCutoffFrequency())
//			d=1.0;
//		if(fabs(1.0+d)<GetCutoffFrequency())
//			d=-1.0;
//		if(Sims()->Type->GetLang())
//		{
//			// Language -> Make the weighted sum for all languages
//			SimSpaces[0]+=d*static_cast<double>(Sims()->LastNbComps);
//			TotalLangsComp+=Sims()->LastNbComps;
//			Ok=true;
//		}
//		else
//		{
//			if(Sims()->Type==StructSpace)
//			{
//				SimSpaces[1]=d;
//				Ok=true;
//			}
//			if(Sims()->Type==MetaSpace)
//			{
//				SimSpaces[2]=d;
//				Ok=true;
//			}
//		}
//	}
//
//	if(TotalLangsComp)
//		SimSpaces[0]/=static_cast<double>(TotalLangsComp);
//	cout<<"\t(0,"<<SimSpaces[0]<<")\t(1,"<<SimSpaces[1]<<")\t(2,"<<SimSpaces[2]<<")";


	return((SimSpaces[0]!=0.0)||(SimSpaces[1]!=0.0)||(SimSpaces[2]!=0.0));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityChoquet(void)
{
	if(!ComputeSimSpaces())
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
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilaritySum(void)
{
	if(!ComputeSimSpaces())
		return(0.0);
	return((SimSpaces[0]*ContentCapacity+SimSpaces[1]*StructCapacity+SimSpaces[2]*MetaCapacity)/(ContentCapacity+StructCapacity+MetaCapacity));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityProduct(void)
{
	if(!ComputeSimSpaces())
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
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityLang(void)
{
	if(!ComputeSimSpaces())
		return(0.0);
   	double d(SimSpaces[0]);
	if(fabs(d)<GetCutoffFrequency())
		return(0.0);
	return(d);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::Compute(GObject* obj1,GObject* obj2)
{
	if(obj1==obj2)
		return(1.0);
	Desc1=dynamic_cast<cObj1*>(obj1);
	Desc2=dynamic_cast<cObj2*>(obj2);

	if((!Desc1->IsDefined())||(!Desc2->IsDefined()))
		return(0.0);
	double sim;
	switch(SimType)
	{
		case Undefined:
			ThrowGException("No similarity model defined.");
			break;
		case Product:
			sim=SimilarityProduct();
			break;
		case TextOnly:
			sim=SimilarityLang();
			break;
		case Sum:
			sim=SimilaritySum();
			break;
		case Choquet:
			sim=SimilarityChoquet();
			break;
	}
	//std::cout<<"Sim("<<obj1->GetId()<<","<<obj2->GetId()<<")="<<sim<<std::endl;
	return(sim);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	size_t GGenericSims<cObj1,cObj2>::GetId(void* obj,bool)
{
	return(static_cast<GObject*>(obj)->GetId());
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::GetIF(GConcept* concept)
{
	double IF(concept->GetIF(GetLinesType()));
	if(GetLinesType()!=GetColsType())
		IF+=concept->GetIF(GetColsType());
	return(IF);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::CreateConfig(void)
{
	GMatrixMeasure::CreateConfig();
	InsertParam(new RParamValue("SimType","Text Only"));
	InsertParam(new RParamValue("Factor",0.01));
	InsertParam(new RParamValue("ContentCapacity",0.01));
	InsertParam(new RParamValue("StructCapacity",0.01));
	InsertParam(new RParamValue("MetaCapacity",0.01));
	InsertParam(new RParamValue("ContentStructCapacity",0.01));
	InsertParam(new RParamValue("ContentMetaCapacity",0.01));
	InsertParam(new RParamValue("MetaStructCapacity",0.01));
	InsertParam(new RParamValue("Transform",false));
}
