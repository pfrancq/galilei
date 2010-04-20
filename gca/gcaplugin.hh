/*

	GALILEI Research Project

	GCAPlugIn.hh

	Generic Plug-in for GCA - Template Implementation

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <memory>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcursor.h>
#include <rdebug.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroup.h>
#include <guser.h>
#include <gdoc.h>
#include <glang.h>
#include <gsession.h>
#include <gmeasure.h>
#include <ggalileiapp.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gcainst.h>
#include <gcagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class GCAPlugIn
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::GCAPlugIn(const RString& name,tObjType objtype,tObjType grouptype)
		: RObject(name), ObjType(objtype), GroupType(grouptype)
{
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::ApplyConfig(GPlugInFactory* factory)
{
	PopSize=factory->FindParam<RParamValue>("Population Size")->GetUInt();
	MaxGen=factory->FindParam<RParamValue>("Max Gen")->GetUInt();
	Step=factory->FindParam<RParamValue>("Step")->GetBool();
	StepGen=factory->FindParam<RParamValue>("Step Gen")->GetUInt();
	MinAgreement=factory->FindParam<RParamValue>("Min Agreement")->GetDouble();
	MinDisagreement=factory->FindParam<RParamValue>("Min Disagreement")->GetDouble();
	MaxKMeans=factory->FindParam<RParamValue>("Max kMeans")->GetUInt();
	Convergence=factory->FindParam<RParamValue>("Convergence")->GetDouble();
	NbDivChromo=factory->FindParam<RParamValue>("NbDivChromos")->GetUInt();
	LocalOptimisation=factory->FindParam<RParamValue>("LocalOptimisation")->GetBool();
	Optimisation=factory->FindParam<RParamValue>("Optimisation")->GetBool();
	Incremental=factory->FindParam<RParamValue>("Incremental")->GetBool();
	InternalRandom=factory->FindParam<RParamValue>("InternalRandom")->GetBool();
	Seed=factory->FindParam<RParamValue>("Seed")->GetInt();
	ParamsSim=factory->FindParam<R::RParamStruct>("Sim Criterion");
	ParamsAgreement=factory->FindParam<R::RParamStruct>("Agreement Criterion");
	ParamsDisagreement=factory->FindParam<R::RParamStruct>("Disagreement Criterion");
	RString what(factory->FindParam<RParamValue>("Clustering Method")->Get());
	ClusteringMethod=0;
	if(what=="GA")
		ClusteringMethod=1;
	if(what=="k-Means")
		ClusteringMethod=2;
	NbClusters=factory->FindParam<RParamValue>("NbClusters")->GetUInt();
	Cout=factory->FindParam<RParamValue>("Cout")->GetBool();
	NbMinObjs=factory->FindParam<RParamValue>("NbMinObjs")->GetUInt();
	NbMaxObjs=factory->FindParam<RParamValue>("NbMaxObjs")->GetUInt();
	AllMinSim=factory->FindParam<RParamValue>("AllMinSim")->GetBool();
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	bool GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::IsCoherent(const cGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	bool GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::IsCoherent(const cGroup* /*grp*/,const cObj* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	bool GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::IsValid(cGroup* /*grp*/)
{
//	GSubProfileCursor Cur1,Cur2;
//	size_t i,j;
//
//	Cur1=grp->GetSubProfileCursor();
//	Cur2=grp->GetSubProfileCursor();
//	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
//	{
//		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
//		{
//			if(GlobalSim)
//			{
//				if(Cur1()->GlobalSimilarity(Cur2())<MinSimLevel)
//					return(false);
//			}
//			else
//			{
//				if(Cur1()->Similarity(Cur2())<MinSimLevel)
//					return(false);
//			}
//		}
//	}
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	template<class cAlgoGroup> void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::ConstructResults(GSession* session,R::RCursor<cAlgoGroup> Sol)
{
	size_t i;
	RString Label(GetObjType(GroupType,true,false)+" ");


	session->ClearGroups(GroupType);
	for(Sol.Start(),i=1;!Sol.End();Sol.Next(),i++)
	{
		cGroup* g(static_cast<cGroup*>(session->NewGroup(GroupType,Label+RString::Number(i))));
		session->AssignId(g);
		RCursor<cObj> Cur(Sol()->GetObjs());
		for(Cur.Start();!Cur.End();Cur.Next())
			g->InsertObj(Cur());
		session->InsertGroup(g,GroupType);
	}
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::DoGCA(GSession* session,const R::RString& mes,R::RCursor<cObj> Objs)
{
	const RMaxVector* Vec;
	GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(3,1,&Vec);
	RCursor<RMaxValue> Cur(*Vec);
	for(Cur.Start();!Cur.End();Cur.Next())
		cout<<"Sim (1,"<<Cur()->Id<<")="<<Cur()->Value<<endl;

	return;
	double d;

	if(Cout)
		cout<<"Do GCA for "<<GetObjType(ObjType,false,true)<<endl;

	// Init the GCA
	if(Cout)
		cout<<"Get minimum similarity"<<endl;
	GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(0,&d);
	MinSimLevel=d;
	if(Cout)
	{
		cout<<"   Minimum Similarity="<<d<<endl;
		double nb;
		GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(1,&nb);
		cout<<"     Mean Similarity="<<nb<<endl;
		GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(2,&nb);
		cout<<"     Deviation Similarity="<<nb<<endl;
		GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Agreements")->Info(0,&nb);
		cout<<"   Minimum Agreement="<<nb<<endl;
		GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Disagreements")->Info(0,&nb);
		cout<<"   Minimum Disagreement="<<nb<<endl;
		cout<<"New GCA"<<endl;
	}
	cInst Instance(session,Objs,this,session->GetDebug(),ObjType);
	InsertObserver(reinterpret_cast<tNotificationHandler>(&GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::Gen),"RInst::Generation",&Instance);
	if(Cout)
		cout<<"Init GCA"<<endl;
//	Instance.SetVerify(true);
	Instance.Init();
	Instance.SetLocalOptimisation(LocalOptimisation);
	Instance.SetOptimisation(Optimisation);
	if(!InternalRandom)
		Instance.SetSeed(Seed);

	// Run
	if(Cout)
	{
		cout<<"Run GCA (";
		if(Optimisation)
			cout<<"optimization";
		else
			cout<<"no optimization";
		if(LocalOptimisation)
			cout<<",local optimization";
		else
			cout<<",no local optimization";
		if(Incremental)
			cout<<",incremental mode)"<<endl;
		else
			cout<<",from scratch)"<<endl;
	}
	Instance.Run();
	if(Cout)
		cout<<"Build solutions"<<endl;
	ConstructResults<cGAGroup>(session,Instance.BestChromosome->Used);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::DokMeans(GSession* session,R::RCursor<cGroup> groups,R::RCursor<cObj> Objs)
{
	cout<<"Do kMeans for "<<Objs.GetNb()<<" "<<GetObjType(ObjType,false,true)<<endl;
	std::auto_ptr<RRandom> Rand(RRandom::Create(RRandom::Good,1));
	if(InternalRandom)
		Rand->Reset(12345);
	else
		Rand->Reset(Seed);

	cKMeans kMeans("k-MeansObj",Rand.get(),Objs);
	typename cKMeans::tInitial start;

	cKMeansGrps Sol(Objs,NbClusters);
	Sol.Init();
	if(Incremental&&groups.GetNb())
	{
		cout<<"Prepare incremental kMeans"<<endl;
		for(groups.Start();!groups.End();groups.Next())
		{
			cKMeansGrp* grp(0);
			RCursor<cObj> Cur(groups()->GetObjs());
			if(Cur.GetNb())
				grp=Sol.ReserveGroup();
			for(Cur.Start();!Cur.End();Cur.Next())
				grp->Insert(Cur());
		}
		start=cKMeans::Incremental;
	}
	else
		start=cKMeans::kMeansPlusPlus;
	cout<<"Run kMeans ("<<NbClusters<<" clusters)"<<endl;
	kMeans.Run(&Sol,MaxKMeans,NbClusters,start);
	cout<<"kMeans iterates "<<kMeans.GetNbIterations()<<" times (max="<<MaxKMeans<<")"<<endl;
	ConstructResults<cKMeansGrp>(session,Sol);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::RunGrouping(GSession* session,const R::RString& mes,R::RCursor<cGroup> groups,R::RCursor<cObj> Objs)
{
	switch(ClusteringMethod)
	{
		case 1:
			DoGCA(session,mes,Objs);
			break;
		case 2:
			DokMeans(session,groups,Objs);
			break;
	}
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::Gen(const R::RNotification& notification)
{
	if(Cout)
		cout<<"Gen "<<GetData<size_t>(notification)<<endl;
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	void GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::CreateParams(GPlugInFactory* fac)
{
	fac->InsertParam(new RParamValue("Population Size",16));
	fac->InsertParam(new RParamValue("Max Gen",30));
	fac->InsertParam(new RParamValue("Step",false));
	fac->InsertParam(new RParamValue("Step Gen",0));
	fac->InsertParam(new RParamValue("Min Agreement",0.6));
	fac->InsertParam(new RParamValue("Min Disagreement",0.6));
	fac->InsertParam(new RParamValue("Max kMeans",30));
	fac->InsertParam(new RParamValue("Convergence",0.05));
	fac->InsertParam(new RParamValue("NbDivChromos",2));
	fac->InsertParam(new RParamValue("NbClusters",20));
	fac->InsertParam(new RParamValue("Clustering Method","GA"));
	fac->InsertParam(new RParamValue("LocalOptimisation",true));
	fac->InsertParam(new RParamValue("Optimisation",true));
	fac->InsertParam(new RParamValue("Incremental",true));
	fac->InsertParam(new RParamValue("InternalRandom",true));
	fac->InsertParam(new RParamValue("Seed",static_cast<int>(12345)));
	fac->InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	fac->InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	fac->InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
	fac->InsertParam(new RParamValue("Cout",false));
	fac->InsertParam(new RParamValue("NbMinObjs",0));
	fac->InsertParam(new RParamValue("NbMaxObjs",cNoRef));
	fac->InsertParam(new RParamValue("AllMinSim",true));
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cInst,class cGAGroup,class cKMeans,class cKMeansGrp,class cKMeansGrps>
	GCAPlugIn<cObj,cGroup,cInst,cGAGroup,cKMeans,cKMeansGrp,cKMeansGrps>::~GCAPlugIn(void)
{
}
