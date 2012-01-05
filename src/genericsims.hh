/*

	GALILEI Research Project

	GenericSims.cpp

	Generic Tensor Space Model Similarity Measure - Inline Implementation.

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



//------------------------------------------------------------------------------
//
//  GGenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	GGenericSims<cObj1,cObj2>::GGenericSims(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols)
	: GMatrixMeasure(session,fac,lines,cols,lines==cols), Desc1(0), Desc2(0)
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
	ProductFactor=FindParam<RParamValue>("Product Factor")->GetDouble();
	Cats[Textual].Capacity=FindParam<RParamValue>("Textual Capacity")->GetDouble();
	Cats[Semantic].Capacity=FindParam<RParamValue>("Semantic Capacity")->GetDouble();
	Cats[Metadata].Capacity=FindParam<RParamValue>("Metadata Capacity")->GetDouble();
	TextualSemanticCapacity=FindParam<RParamValue>("Textual/Semantic Capacity")->GetDouble();
	TextualMetadataCapacity=FindParam<RParamValue>("Textual/Metadata Capacity")->GetDouble();
	SemanticMetadataCapacity=FindParam<RParamValue>("Semantic/Metadata Capacity")->GetDouble();
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
	Cats[Textual].Cat=Session->GetConceptCat("Text",false);
	Cats[Semantic].Cat=Session->GetConceptCat("Metadata",false);
	Cats[Metadata].Cat=Session->GetConceptCat("Semantic",false);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::ComputeMetaSim(GVector* vec1,GVector* vec2)
{
	double Num(0.0), Den(0.0), NbCommon(0),NbDiff(0);

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
			Den+=fabs(d);
		}
	}

	// Compute the rest of the NbDiff
	for(;!ptr2.End();ptr2.Next())
		NbDiff++;

	// Assign to the category
	if(NbCommon>0.0)
		Cats[Metadata].Sim+=NbCommon*(0.5+((Num*NbCommon)/(2*NbDiff*Den)));
	Cats[Metadata].NbConcepts+=NbCommon;
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::ComputeCosineSim(GVector* vec1,GVector* vec2,Cat& cat)
{
	double Num(0.0),Norm1(0.0),Norm2(0.0),NbCommon(0);

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
	if(NbCommon>0.0)
		cat.Sim+=NbCommon*(0.5+(Num/(2.0*sqrt(Norm1)*sqrt(Norm2))));
	cat.NbConcepts+=NbCommon;
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	bool GGenericSims<cObj1,cObj2>::ComputeSims(GConceptCat* filter)
{
	// Initialize all the similarities
	Cats[Textual].Sim  = Cats[Textual].NbConcepts  = 0.0;
	Cats[Metadata].Sim = Cats[Metadata].NbConcepts = 0.0;
	Cats[Semantic].Sim = Cats[Semantic].NbConcepts = 0.0;

	// Get two cursors over the vectors
	RCursor<GVector> Vec1(Desc1->GetVectors());
	RCursor<GVector> Vec2(Desc2->GetVectors());

	// Go trough the vectors of the first object
	for(Vec1.Start();!Vec1.End();Vec1.Next())
	{
		// Filter if necessary
		GConceptCat* Cat1(Vec1()->GetConcept()->GetType()->GetCategory());
		if(filter&&(Cat1!=filter))
			continue;

		// Textual content in the first object ?
		bool IsTextual(Cat1==Cats[Textual].Cat);

		// Go through the vectors of the second object
		for(Vec2.Start();!Vec2.End();Vec2.Next())
		{
			// Filter if necessary
			GConceptCat* Cat2(Vec2()->GetConcept()->GetType()->GetCategory());
			if(filter&&(Cat2!=filter))
				continue;

			// Look if first object is a textual content
			if(IsTextual)
			{
				// Yes -> Compute the similarities with all other textual content vectors
				if(Cat2!=Cat1)
					continue;
				ComputeCosineSim(Vec1(),Vec2(),Cats[Textual]);
			}
			else
			{
				// No -> Compare only with the same concept associated to the vector
				if(Vec2()->GetConceptId()!=Vec1()->GetConceptId())
					continue;
				if(Cat1==Cats[Metadata].Cat)
					ComputeMetaSim(Vec1(),Vec2());
				else if(Cat1==Cats[Semantic].Cat)
					ComputeCosineSim(Vec1(),Vec2(),Cats[Semantic]);
				// There is only one similarity to compute
				// -> Not necessary to parse the other vectors of the second object
				break;
			}
		}
	}
	if(Cats[Textual].NbConcepts)
		Cats[Textual].Sim  /= Cats[Textual].NbConcepts;
	if(Cats[Metadata].NbConcepts)
		Cats[Metadata].Sim /= Cats[Metadata].NbConcepts;
	if(Cats[Semantic].NbConcepts)
		Cats[Semantic].Sim /= Cats[Semantic].NbConcepts;
	return((Cats[Textual].Sim!=0.0)||(Cats[Metadata].Sim!=0.0)||(Cats[Semantic].Sim!=0.0));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityChoquet(void)
{
	if(!ComputeSims())
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
	Tab[0].Sim=Cats[Textual].Sim;
	Tab[0].What=0;
	Tab[1].Sim=Cats[Semantic].Sim;
	Tab[1].What=1;
	Tab[2].Sim=Cats[Metadata].Sim;
	Tab[2].What=2;
	qsort(Tab,static_cast<size_t>(3),sizeof(tSimSpace),tSimSpace::Compare);

	// First element (capacity=1)
	double Choquet(Tab[0].Sim);

	// Second element: Find the right capacity (in Cap)
	double Cap;
	if((Tab[1].What==0&&Tab[2].What==1)||(Tab[1].What==1&&Tab[2].What==0))
		Cap=TextualSemanticCapacity;
	else if((Tab[1].What==0&&Tab[2].What==2)||(Tab[1].What==2&&Tab[2].What==0))
		Cap=TextualMetadataCapacity;
	else if((Tab[1].What==1&&Tab[2].What==2)||(Tab[1].What==2&&Tab[2].What==1))
		Cap=SemanticMetadataCapacity;
	else
		Cap=0;
	Choquet+=(Tab[1].Sim-Tab[0].Sim)*Cap;

	// Third element : Find the right capacity
	if(Tab[2].What==0)
		Cap=Cats[Textual].Capacity;
	else if(Tab[2].What==1)
		Cap=Cats[Semantic].Capacity;
	else if(Tab[2].What==2)
		Cap=Cats[Metadata].Capacity;
	return(Choquet+(Tab[2].Sim-Tab[1].Sim)*Cap);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilaritySum(void)
{
	if(!ComputeSims())
		return(0.0);
	double Num((Cats[Textual].Sim*Cats[Textual].Capacity)+(Cats[Semantic].Sim*Cats[Textual].Capacity)+(Cats[Metadata].Sim*Cats[Metadata].Capacity));
	double Den(Cats[Textual].Capacity+Cats[Semantic].Capacity+Cats[Metadata].Capacity);
	return(Num/Den);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityProduct(void)
{
	if(!ComputeSims())
		return(0.0);
	double Sim(0.0);
	double NbComps(static_cast<double>(GetNbDiffElements()));
	NbComps=ProductFactor*NbComps*(NbComps-1);

	// Makes the product of the similarity (a null similarity is replaced by ProductFactor).
	Sim+=log10(Cats[Textual].Sim+ProductFactor);
	Sim+=log10(Cats[Semantic].Sim+ProductFactor);
	Sim+=log10(Cats[Metadata].Sim+ProductFactor);
	return(pow(10,Sim-3.0*log10(NbComps)));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityTextOnly(void)
{
	if(!ComputeSims(Cats[Textual].Cat))
		return(0.0);
   double d(Cats[Textual].Sim);
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
		case Product:
			sim=SimilarityProduct();
			break;
		case TextOnly:
			sim=SimilarityTextOnly();
			break;
		case Sum:
			sim=SimilaritySum();
			break;
		case Choquet:
			sim=SimilarityChoquet();
			break;
		default:
			ThrowGException("No similarity model defined.");
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
	InsertParam(new RParamValue("Product Factor",0.01));
	InsertParam(new RParamValue("Textual Capacity",0.01));
	InsertParam(new RParamValue("Semantic Capacity",0.01));
	InsertParam(new RParamValue("Metadata Capacity",0.01));
	InsertParam(new RParamValue("Textual/Semantic Capacity",0.01));
	InsertParam(new RParamValue("Textual/Metadata Capacity",0.01));
	InsertParam(new RParamValue("Semantic/Metadata Capacity",0.01));
}
