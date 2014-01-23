/*

	GALILEI Research Project

	ChoquetEstimation.cpp

	Estimation of the parameters of the Choquet Intregral - Implementation.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <gsession.h>
#include <gdoc.h>
#include <gmatrixmeasure.h>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <choquetestimation.h>



//-----------------------------------------------------------------------------
// General variables
const bool PrintConsolidate(true);



//-----------------------------------------------------------------------------
//
// class ChoquetEstimation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ChoquetEstimation::ChoquetEstimation(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetName()), GTool(session,fac), RTestChoquet(1),
	  Sims(4), AvgSim(2,4), MinSim(2), MaxSim(2)
{
	SetNbCriteria(0,NbCriteria);
	MinSim.InsertPtr(new RLowerTriangularMatrix(NbCriteria));
	MinSim.InsertPtr(new RLowerTriangularMatrix(NbCriteria));
	MaxSim.InsertPtr(new RLowerTriangularMatrix(NbCriteria));
	MaxSim.InsertPtr(new RLowerTriangularMatrix(NbCriteria));
}


//-----------------------------------------------------------------------------
void ChoquetEstimation::HandleSim(const RNotification& notification)
{
	sInfo Info(GetData<sInfo>(notification));
	Sims[cText]=Info.Text;
	Sims[cMetadata]=Info.Metadata;
	Sims[cSemantic]=Info.Semantic;
	Sims[cLink]=Info.Link;
}


//-----------------------------------------------------------------------------
void ChoquetEstimation::ComputeSims(void)
{
	// Init
//	SimIntra.Init(0.0);
//	SimInter.Init(0.0);
//	NbIntra=0;
//	NbInter=0;

	// Configure the measure plug-in and add an observer
	GMatrixMeasure* Measure(GALILEIApp->GetCurrentPlugIn<GMatrixMeasure>("Measures","Document Similarities"));
	if(!Measure)
		 mThrowGException("No current similarity measure for documents");
	if(Measure->GPlugIn::GetName()!="Tensor Space Model")
		 mThrowGException("Similarity measure for documents is not the tensor space model one");
	InsertObserver(HANDLER(ChoquetEstimation::HandleSim),"Sim",dynamic_cast<RObject*>(Measure));
	double mes;
	bool EmitSignal(Measure->FindParam<RParamValue>("EmitSignal")->GetBool());
	Measure->FindParam<RParamValue>("EmitSignal")->SetBool(true);
	Measure->ApplyConfig();
	bool ForceCompute(Measure->MustForceCompute());
	Measure->SetForceCompute(true);

  	// Compute SimIntra and SimInter;
	AvgSim.Init(0.0);
	MinSim[0]->Init(0.0);
	MinSim[1]->Init(0.0);
	MaxSim[0]->Init(0.0);
	MaxSim[1]->Init(0.0);
	size_t NbIntra(0);
	size_t NbInter(0);
	size_t MaxLine;

	RCursor<GDoc> Cur1(Session->GetUsedObjs(pDoc));
	RCursor<GDoc> Cur2(Session->GetUsedObjs(pDoc));
	for(Cur1.Start(),MaxLine=Cur1.GetNb()-1;Cur1.GetPos()<MaxLine;Cur1.Next())
	{
		for(Cur2.GoTo(Cur1.GetPos());!Cur2.End();Cur2.Next())
		{
			size_t idx;
			if(Session->GetObj(pSubject,Cur1())==Session->GetObj(pSubject,Cur2()))
			{
				idx=0;
				NbInter++;
			}
			else
			{
				idx=1;
				NbIntra++;
			}

			// Compute the similarity. The plug-in will send a message with the local
			// similarities that is copied in Sims (see HandleSim)
			Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&mes);

			// Compute Scores and AvgSim
			for(size_t l=0;l<NbCriteria;l++)
				AvgSim(idx,l)+=Sims[l];

			// Compute Min and Max
			for(size_t l=0;l<NbCriteria-1;l++)
				for(size_t m=l+1;m<NbCriteria;m++)
				{
					(*MinSim[idx])(m,l)+=min(Sims[l],Sims[m]);
					(*MaxSim[idx])(m,l)+=max(Sims[l],Sims[m]);
				}
		}
	}
	(*AvgSim[0])/=NbInter;
	(*AvgSim[1])/=NbIntra;
	(*MinSim[0])/=NbInter;
	(*MaxSim[0])/=NbInter;
	(*MinSim[1])/=NbIntra;
	(*MaxSim[1])/=NbIntra;
	if(PrintConsolidate)
	{
		AvgSim.Print(cout,"AvgSim ");
		MinSim[0]->Print(cout,"MinSim0");
		MinSim[1]->Print(cout,"MinSim1");
		MaxSim[0]->Print(cout,"MaxSim0");
		MaxSim[1]->Print(cout,"MaxSim1");
	}

	// Re-configure the measure plug-in  and delete an observer
	DeleteObserver("Sim",dynamic_cast<RObject*>(Measure));
	Measure->FindParam<RParamValue>("EmitSignal")->SetBool(EmitSignal);
	Measure->ApplyConfig();
	Measure->SetForceCompute(ForceCompute);
}


//------------------------------------------------------------------------------
double ChoquetEstimation::Sim(double sim) const
{
	if(fabs(sim)<1e-9)
		sim=0.0;
	if((fabs(sim)>=1) && (fabs(sim)<=1+1e-9))
		sim=1.0;

	if(sim<0.0)
	{
		cout<<"Problem with a similarity: "<<sim<<endl;
		sim=0.0;
	}
	else if(sim>1.0)
	{
		cout<<"Problem with a similarity: "<<sim<<endl;
		sim=1.0;
	}
	return(sim);
}


//------------------------------------------------------------------------------
double ChoquetEstimation::Compute(RChoquetPropagator& prop)
{
	double Intra(0.0);
	double Inter(0.0);

	// Compute Min/Max
	for(size_t l=0;l<3;l++)
		for(size_t m=l+1;m<4;m++)
		{
			if(prop.Iij(0,m,l)>0.0)
			{
				Inter+=prop.Iij(0,m,l)*((*MinSim[0])(m,l));
				Intra+=prop.Iij(0,m,l)*((*MinSim[1])(m,l));
			}
			else if(prop.Iij(0,m,l)<0.0)
			{
				Inter-=prop.Iij(0,m,l)*((*MaxSim[0])(m,l));
				Intra-=prop.Iij(0,m,l)*((*MaxSim[1])(m,l));
			}
		}

	// Compute Criteria
	for(size_t j=0;j<4;j++)
	{
		double sum(0);
		for(size_t k=0;k<4;k++)
		{
			if(k==j)
				continue;
			sum+=abs(prop.Iij(0,j,k));
		}
		Intra+=AvgSim(0,j)*(prop.vi(0,j)-0.5*sum);
		Inter+=AvgSim(1,j)*(prop.vi(0,j)-0.5*sum);
	}

	// Make the final computation
	Intra=Sim(Intra);
	Inter=Sim(Inter);
	double Res(0.0);
	if(Intra>Inter)
	{
		if(Intra!=0.0)
			Res=(Intra-Inter)/Intra;
		else
			Res=0.1;
	}
	else
	{
		if(Inter!=0.0)
			Res=(Intra-Inter)/Inter;
		else
			Res=-1.0;
	}
	if(fabs(Res)>2.0)
	{
		cout<<"Problem Res="<<Res<<endl;
		cout<<"Debug"<<endl;
		if(Res>2.0)
			Res=2.0;
		else
			Res=-2.0;
	}
	return(Res);
}


//-----------------------------------------------------------------------------
void ChoquetEstimation::SetParams(const RString& mes)
{
	// Get the measure and verify that it is a tensor space model one
	GMatrixMeasure* Measure(GALILEIApp->GetCurrentPlugIn<GMatrixMeasure>("Measures",mes));
	if((!Measure)||(Measure->GPlugIn::GetName()!="Tensor Space Model"))
		 return;
	Measure->FindParam<RParamValue>("Text")->SetDouble(vi(0,cText));
	Measure->FindParam<RParamValue>("Semantic")->SetDouble(vi(0,cSemantic));
	Measure->FindParam<RParamValue>("Metadata")->SetDouble(vi(0,cMetadata));
	Measure->FindParam<RParamValue>("Link")->SetDouble(vi(0,cLink));
	Measure->FindParam<RParamValue>("Text/Metadata")->SetDouble(Iij(0,cText,cMetadata));
	Measure->FindParam<RParamValue>("Text/Semantic")->SetDouble(Iij(0,cText,cSemantic));
	Measure->FindParam<RParamValue>("Text/Link")->SetDouble(Iij(0,cText,cLink));
	Measure->FindParam<RParamValue>("Metadata/Semantic")->SetDouble(Iij(0,cMetadata,cSemantic));
	Measure->FindParam<RParamValue>("Metadata/Link")->SetDouble(Iij(0,cMetadata,cLink));
	Measure->FindParam<RParamValue>("Semantic/Link")->SetDouble(Iij(0,cSemantic,cLink));
	Measure->ApplyConfig();
}


//-----------------------------------------------------------------------------
void ChoquetEstimation::RunGecode(void)
{
  // Compute the parameters and print them
	RTestChoquet::Run(1);
	for(size_t i=0;i<NbCriteria;i++)
		cout<<"v["<<i<<"]="<<vi(0,i)<<endl;;
	for(size_t i=0;i<NbCriteria;i++)
	{
		for(size_t j=i+1;j<NbCriteria;j++)
			cout<<"I["<<i<<","<<j<<"[="<<Iij(0,i,j)<<"\t";
		cout<<endl;
	}

	// Set the parameters for the different measure
	SetParams("Document/Community Similarities");

/*	//cout<<m.statistics().fail<<endl;
	GeCodeChoquet* m(new GeCodeChoquet(slot,SimIntra,SimInter,pow(10,ParamPrecision),pow(10,CostPrecision)));
	m->firstsol();
	BAB<GeCodeChoquet> e(m);
	delete m;
	GeCodeChoquet* Sol(0);
	while(GeCodeChoquet* s=e.next())
	{
		if(Sol)
			delete Sol;
		cout<<s->Msg()<<endl<<"\t";
		s->print();
		Sol=s;
	}
	SetParams(Sol,"Document Similarities",10);
	delete Sol;*/
	/*cout<<m.statistics().fail<<endl;
	cout<<e->statistics().node<<endl;
	cout<<e->statistics().depth<<endl;
	cout<<e->statistics().memory<<endl;*/
}


//-----------------------------------------------------------------------------
void ChoquetEstimation::Run(GSlot* slot)
{
	slot->WriteStr("Compute the local similarities");
	ComputeSims();
	slot->WriteStr("Run GeCode engine");
	RunGecode();
}


//-----------------------------------------------------------------------------
ChoquetEstimation::~ChoquetEstimation(void)
{
}


//------------------------------------------------------------------------------
CREATE_TOOL_FACTORY("Choquet Estimation","Choquet Estimation","Evaluate the parameters of a Choquet Integral",ChoquetEstimation)
