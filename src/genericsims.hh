/*

	GALILEI Research Project

	GenericSims.cpp

	Generic Tensor Space Model Similarity Measure - Inline Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
	: GSimPlugIn(session,fac,lines,cols), Cats(0), ComputeChoquet(4), Values(4), Desc1(0), Desc2(0), URI(0)
{
	Cats=new GCat[GetNbConceptCats()];
	Notification=GetNotificationHandle("Sim");
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	void GGenericSims<cObj1,cObj2>::HandlerNotFound (const RNotification&)
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
	ComputeChoquet.SetCriteria(0,FindParam<RParamValue>("Token")->GetDouble());
	ComputeChoquet.SetCriteria(1,FindParam<RParamValue>("Metadata")->GetDouble());
	ComputeChoquet.SetCriteria(2,FindParam<RParamValue>("Structure")->GetDouble());
	ComputeChoquet.SetCriteria(3,FindParam<RParamValue>("Link")->GetDouble());
	ComputeChoquet.SetInteraction(0,1,FindParam<RParamValue>("Token/Metadata")->GetDouble());
	ComputeChoquet.SetInteraction(0,2,FindParam<RParamValue>("Token/Structure")->GetDouble());
	ComputeChoquet.SetInteraction(0,3,FindParam<RParamValue>("Token/Link")->GetDouble());
	ComputeChoquet.SetInteraction(1,2,FindParam<RParamValue>("Metadata/Structure")->GetDouble());
	ComputeChoquet.SetInteraction(1,3,FindParam<RParamValue>("Metadata/Link")->GetDouble());
	ComputeChoquet.SetInteraction(2,3,FindParam<RParamValue>("Structure/Link")->GetDouble());
	NbHops=FindParam<RParamValue>("NbHops")->GetUInt();
	tSimType sim(Undefined);
	if(type=="Product")
		sim=Product;
	else if(type=="Text Only")
		sim=TextOnly;
	else if(type=="Sum")
		sim=Sum;
	else if(type=="2-Additive Choquet Integral")
		sim=Choquet;
	if(sim==Undefined)
	{
		cerr<<"No valid similarity measure"<<endl;
		SimType=Choquet;
	}
	else
		SimType=sim;
	EmitSignal=FindParam<RParamValue>("EmitSignal")->GetBool();
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	bool GGenericSims<cObj1,cObj2>::ComputeSims(bool filter,tConceptCat cat)
{
	// Initialize all the similarities
	Cats[ccToken].Sim  = Cats[ccToken].Nb  = 0.0;
	Cats[ccMetadata].Sim = Cats[ccMetadata].Nb = 0.0;
	Cats[ccStructure].Sim = Cats[ccStructure].Nb = 0.0;
	Cats[ccLink].Sim = Cats[ccLink].Nb = 0.0;

	// Get two cursors over the vectors
	RConstCursor<GVector> Vec1(Desc1->GetVectors());
	RConstCursor<GVector> Vec2(Desc2->GetVectors());

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
	if(Cats[ccToken].Nb>0.1)
		Cats[ccToken].Sim/=Cats[ccToken].Nb;
	else
		Cats[ccToken].Sim=0.5;
	if(Cats[ccMetadata].Nb>0.1)
		Cats[ccMetadata].Sim/=Cats[ccMetadata].Nb;
	else
		Cats[ccMetadata].Sim=0.5;
	if(Cats[ccStructure].Nb>0.1)
		Cats[ccStructure].Sim/=Cats[ccStructure].Nb;
	else
		Cats[ccStructure].Sim=0.5;
	if(Cats[ccLink].Nb>0.1)
		Cats[ccLink].Sim/=Cats[ccLink].Nb;
	else
		Cats[ccLink].Sim=0.5;
	if(EmitSignal)
	{

		sInfo Info;
		Info.Text=Cats[ccToken].Sim;
		Info.Metadata=Cats[ccMetadata].Sim;
		Info.Semantic=Cats[ccStructure].Sim;
		Info.Link=Cats[ccLink].Sim;
		PostNotification(Notification,Info);
	}
	return((Cats[ccToken].Sim!=0.0)||(Cats[ccMetadata].Sim!=0.0)||(Cats[ccStructure].Sim!=0.0)||(Cats[ccLink].Sim!=0.0));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityChoquet(void)
{
	if(!ComputeSims(false))
		return(0.0);

	// Prepare the vector and compute the value
	Values[0]=Cats[ccToken].Sim;
	Values[1]=Cats[ccMetadata].Sim;
	Values[2]=Cats[ccStructure].Sim;
	Values[3]=Cats[ccLink].Sim;
	return(ComputeChoquet.Compute(Values));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilaritySum(void)
{
	if(!ComputeSims(false))
		return(0.0);
	double Num( (Cats[ccToken].Sim*ComputeChoquet.GetCriteria(0))+
	            (Cats[ccMetadata].Sim*ComputeChoquet.GetCriteria(1))+
	            (Cats[ccStructure].Sim*ComputeChoquet.GetCriteria(2))+
	            (Cats[ccLink].Sim*ComputeChoquet.GetCriteria(3)) );
	double Den(ComputeChoquet.GetCriteria(0)+ComputeChoquet.GetCriteria(1)+ComputeChoquet.GetCriteria(2)+ComputeChoquet.GetCriteria(3));
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
	Sim+=log10(Cats[ccToken].Sim+ProductFactor);
	Sim+=log10(Cats[ccStructure].Sim+ProductFactor);
	Sim+=log10(Cats[ccMetadata].Sim+ProductFactor);
	Sim+=log10(Cats[ccLink].Sim+ProductFactor);
	return(pow(10,Sim-4.0*log10(NbComps)));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	double GGenericSims<cObj1,cObj2>::SimilarityTextOnly(void)
{
	if(!ComputeSims(true,ccToken))
		return(0.0);
   double d(Cats[ccToken].Sim);
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
	if(!URI)
		URI=Session->InsertObj(pConceptType,ccLink,"URI","Uniform Resource Identifier");

	Desc1=dynamic_cast<cObj1*>(obj1);
	Desc2=dynamic_cast<cObj2*>(obj2);
	if(GetLinesType()==otDoc)
		Concept1=Session->GetObj(pConcept,URI,obj1->GetName(),true);
	if(GetColsType()==otDoc)
		Concept2=Session->GetObj(pConcept,URI,obj2->GetName(),true);

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
			mThrowGException("No similarity model defined.");
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
	InsertParam(new RParamValue("Token",0.1));
	InsertParam(new RParamValue("Metadata",0.1));
	InsertParam(new RParamValue("Structure",0.1));
	InsertParam(new RParamValue("Link",0.1));
	InsertParam(new RParamValue("Token/Metadata",0.1));
	InsertParam(new RParamValue("Token/Structure",0.1));
	InsertParam(new RParamValue("Token/Link",0.1));
	InsertParam(new RParamValue("Metadata/Structure",0.1));
	InsertParam(new RParamValue("Metadata/Link",0.1));
	InsertParam(new RParamValue("Structure/Link",0.1));
	InsertParam(new RParamValue("NbHops",3));
	InsertParam(new RParamValue("EmitSignal",false));
}


//------------------------------------------------------------------------------
template<class cObj1,class cObj2>
	GGenericSims<cObj1,cObj2>::~GGenericSims(void)
{
	delete[] Cats;
}
