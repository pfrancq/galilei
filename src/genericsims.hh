/*

	GALILEI Research Project

	GenericSims.cpp

	Generic Tensor Space Model Similarity Measure - Inline Implementation.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
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
	: GSimPlugIn(session,fac,lines,cols), Cats(0), Desc1(0), Desc2(0), Web(0)
{
	Cats=new GCat[GetNbConceptCats()];
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
	Cats[ccText].Capacity=FindParam<RParamValue>("Textual Capacity")->GetDouble();
	Cats[ccSemantic].Capacity=FindParam<RParamValue>("Semantic Capacity")->GetDouble();
	Cats[ccMetadata].Capacity=FindParam<RParamValue>("Metadata Capacity")->GetDouble();
	TextualSemanticCapacity=FindParam<RParamValue>("Textual/Semantic Capacity")->GetDouble();
	TextualMetadataCapacity=FindParam<RParamValue>("Textual/Metadata Capacity")->GetDouble();
	SemanticMetadataCapacity=FindParam<RParamValue>("Semantic/Metadata Capacity")->GetDouble();
	NbHops=FindParam<RParamValue>("NbHops")->GetUInt();
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
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	bool GGenericSims<cObj1,cObj2>::ComputeSims(bool filter,tConceptCat cat)
{
	// Initialize all the similarities
	Cats[ccText].Sim  = Cats[ccText].Nb  = 0.0;
	Cats[ccMetadata].Sim = Cats[ccMetadata].Nb = 0.0;
	Cats[ccSemantic].Sim = Cats[ccSemantic].Nb = 0.0;
	Cats[ccLink].Sim = Cats[ccLink].Nb = 0.0;

	// Get two cursors over the vectors
	RCursor<GVector> Vec1(Desc1->GetVectors());
	RCursor<GVector> Vec2(Desc2->GetVectors());

	// Go trough the vectors of the first object
	for(Vec1.Start();!Vec1.End();Vec1.Next())
	{
		// Filter if necessary
		tConceptCat Cat1(Vec1()->GetMetaConcept()->GetType()->GetCategory());
		if(filter&&(cat!=Cat1))
			continue;

		// Go through the vectors of the second object
		for(Vec2.Start();!Vec2.End();Vec2.Next())
		{
			// Compute the similarities only between vectors having the same meta-concept
			if(Vec1()->GetMetaConcept()!=Vec2()->GetMetaConcept())
				continue;

			GComputeSim* ComputeSim(Sims[Cat1]);
			double Sim(0.0);
			size_t Nb(0.0);
			ComputeSim->Compute(Vec1(),Vec2(),Sim,Nb);
			Cats[Cat1].Sim+=Nb*Sim;
			Cats[Cat1].Nb+=Nb;
		}
	}
	if(Cats[ccText].Nb)
		Cats[ccText].Sim/=Cats[ccText].Nb;
	if(Cats[ccMetadata].Nb)
		Cats[ccMetadata].Sim/=Cats[ccMetadata].Nb;
	if(Cats[ccSemantic].Nb)
		Cats[ccSemantic].Sim/=Cats[ccSemantic].Nb;
	if(Cats[ccLink].Nb)
		Cats[ccLink].Sim/=Cats[ccLink].Nb;
	return((Cats[ccText].Sim!=0.0)||(Cats[ccMetadata].Sim!=0.0)||(Cats[ccSemantic].Sim!=0.0)||(Cats[ccLink].Sim!=0.0));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityChoquet(void)
{
	if(!ComputeSims(false))
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
	Tab[0].Sim=Cats[ccText].Sim;
	Tab[0].What=0;
	Tab[1].Sim=Cats[ccSemantic].Sim;
	Tab[1].What=1;
	Tab[2].Sim=Cats[ccMetadata].Sim;
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
		Cap=Cats[ccText].Capacity;
	else if(Tab[2].What==1)
		Cap=Cats[ccSemantic].Capacity;
	else if(Tab[2].What==2)
		Cap=Cats[ccMetadata].Capacity;
	return(Choquet+(Tab[2].Sim-Tab[1].Sim)*Cap);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilaritySum(void)
{
	if(!ComputeSims(false))
		return(0.0);
	double Num((Cats[ccText].Sim*Cats[ccText].Capacity)+(Cats[ccSemantic].Sim*Cats[ccText].Capacity)+(Cats[ccMetadata].Sim*Cats[ccMetadata].Capacity));
	double Den(Cats[ccText].Capacity+Cats[ccSemantic].Capacity+Cats[ccMetadata].Capacity);
	return(Num/Den);
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityProduct(void)
{
	if(!ComputeSims(false))
		return(0.0);
	double Sim(0.0);
	double NbComps(static_cast<double>(GetNbDiffElements()));
	NbComps=ProductFactor*NbComps*(NbComps-1);

	// Makes the product of the similarity (a null similarity is replaced by ProductFactor).
	Sim+=log10(Cats[ccText].Sim+ProductFactor);
	Sim+=log10(Cats[ccSemantic].Sim+ProductFactor);
	Sim+=log10(Cats[ccMetadata].Sim+ProductFactor);
	return(pow(10,Sim-3.0*log10(NbComps)));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityTextOnly(void)
{
	if(!ComputeSims(true,ccText))
		return(0.0);
   double d(Cats[ccText].Sim);
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
	if(!Web)
		Web=Session->GetInsertConceptType(ccLink,"Web","World Wide Web");

	Desc1=dynamic_cast<cObj1*>(obj1);
	Desc2=dynamic_cast<cObj2*>(obj2);
	if(GetLinesType()==otDoc)
		Concept1=Session->GetConcept(Web,obj1->GetName(),true);
	if(GetColsType()==otDoc)
		Concept2=Session->GetConcept(Web,obj2->GetName(),true);

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
	InsertParam(new RParamValue("NbHops",3));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	GGenericSims<cObj1,cObj2>::~GGenericSims(void)
{
	delete[] Cats;
}
