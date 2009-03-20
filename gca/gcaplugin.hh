/*

	GCA Project

	GCAPlugIn.hh

	Generic Plug-in for GCA - Template Implementation

	Copyright 2008 by the Université Libre de Bruxelles.

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
// class kMeans
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeans::kMeans(const R::RString& n,R::RRandom* r,R::RCursor<GCAObj> objs,const R::RString& mes,R::RDebug* debug)
	: R::RGroupingKMeans<CGroup,GCAObj,CGroups>(n,r,objs,debug)
{
	Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Similarities");
}


//-----------------------------------------------------------------------------
double kMeans::Similarity(const GCAObj* obj1,const GCAObj* obj2)
{
	double d;
	Measure->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}



//-----------------------------------------------------------------------------
//
//  class GCAPlugIn
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	GCAPlugIn<cObj,cGroup,cFactory>::GCAPlugIn(const RString& name,tObjType objtype,tObjType grouptype)
		: RObject(name), Objs(20), ObjType(objtype),GroupType(grouptype)
{
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::ApplyConfig(cFactory* factory)
{
	PopSize=factory->GetUInt("Population Size");
	MaxGen=factory->GetUInt("Max Gen");
	Step=factory->GetBool("Step");
	StepGen=factory->GetUInt("Step Gen");
	MinAgreement=factory->GetDouble("Min Agreement");
	MinDisagreement=factory->GetDouble("Min Disagreement");
	MaxKMeans=factory->GetUInt("Max kMeans");
	Convergence=factory->GetDouble("Convergence");
	NbDivChromo=factory->GetUInt("NbDivChromo");
	LocalOptimisation=factory->GetBool("LocalOptimisation");
	Optimisation=factory->GetBool("Optimisation");
	Incremental=factory->GetBool("Incremental");
	InternalRandom=factory->GetBool("InternalRandom");
	Seed=factory->GetInt("Seed");
	ParamsSim=factory->R::RConfig::FindParam<R::RParamStruct>("Sim Criterion");
	ParamsAgreement=factory->R::RConfig::FindParam<R::RParamStruct>("Agreement Criterion");
	ParamsDisagreement=factory->R::RConfig::FindParam<R::RParamStruct>("Disagreement Criterion");
	RString what(factory->Get("Clustering Method"));
	ClusteringMethod=0;
	if(what=="GA")
		ClusteringMethod=1;
	if(what=="k-Means")
		ClusteringMethod=2;
	NbClusters=factory->GetUInt("NbClusters");
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	bool GCAPlugIn<cObj,cGroup,cFactory>::IsCoherent(const cGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	bool GCAPlugIn<cObj,cGroup,cFactory>::IsCoherent(const cGroup* /*grp*/,const cObj* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	bool GCAPlugIn<cObj,cGroup,cFactory>::IsValid(cGroup* /*grp*/)
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
template<class cObj,class cGroup,class cFactory>
	template<class cAlgoGroup> void GCAPlugIn<cObj,cGroup,cFactory>::ConstructResults(GSession* session,R::RCursor<cAlgoGroup> Sol)
{
	size_t* tab;
	size_t* ptr;

	session->ClearGroups(GroupType);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		cGroup* g=static_cast<cGroup*>(session->NewGroup(GroupType));
		session->AssignId(g);
		ptr=tab=Sol()->GetObjectsId();
		while((*ptr)!=cNoRef)
			g->InsertObj(static_cast<cObj*>(Objs[*(ptr++)]->GetElement()));
		delete[] tab;
		session->InsertGroup(g,GroupType);
	}
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::DoGCA(GSession* session,const R::RString& mes)
{
	double d;

	cout<<"Do GCA for "<<GetObjType(ObjType)<<"s"<<endl;

	// Init the GCA
	cout<<"Get minimum similarity"<<endl;
	GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Similarities")->Info(0,&d);
	MinSimLevel=d;
	cout<<"   Minimum Similarity="<<d<<endl;
	cout<<"New GCA"<<endl;
	GCAInst Instance(session,Objs,this,session->GetDebug(),ObjType,mes,Incremental);
	InsertObserver(reinterpret_cast<tNotificationHandler>(&GCAPlugIn<cObj,cGroup,cFactory>::Gen),"RInst::Generation",&Instance);
	cout<<"Init GCA"<<endl;
	Instance.Init();
	Instance.SetLocalOptimisation(LocalOptimisation);
	Instance.SetOptimisation(Optimisation);
	if(!InternalRandom)
		Instance.SetSeed(Seed);

	// Run
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
	Instance.Run();
	cout<<"Build solutions"<<endl;
	ConstructResults<GCAGroup>(session,Instance.BestChromosome->Used);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::DokMeans(GSession* session,const R::RString& mes,R::RCursor<cGroup> groups)
{
	cout<<"Do kMeans for "<<GetObjType(ObjType)<<"s"<<endl;
	std::auto_ptr<RRandom> Rand(RRandom::Create(RRandom::Good,1));
	if(InternalRandom)
		Rand->Reset(12345);
	else
		Rand->Reset(Seed);

	kMeans kMeans("k-MeansObj",Rand.get(),Objs,mes);
	kMeans::tInitial start;

	CGroups Sol(Objs,NbClusters);
	Sol.Init();
	if(Incremental&&groups.GetNb())
	{
		cout<<"Prepare incremental kMeans"<<endl;
		for(groups.Start();!groups.End();groups.Next())
		{
			CGroup* grp(0);
			RCursor<cObj> Cur(groups()->GetObjs());
			if(Cur.GetNb())
				grp=Sol.ReserveGroup();
			for(Cur.Start();!Cur.End();Cur.Next())
				grp->Insert(Objs.GetPtr(Cur()->GetId()-1));
		}
		start=kMeans::Incremental;
	}
	else
		start=kMeans::kMeansPlusPlus;
	cout<<"Run kMeans ("<<NbClusters<<" clusters)"<<endl;
	kMeans.Run(&Sol,MaxKMeans,NbClusters,start);
	cout<<"kMeans iterates "<<kMeans.GetNbIterations()<<" times"<<endl;
	ConstructResults<CGroup>(session,Sol);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::RunGrouping(GSession* session,const R::RString& mes,R::RCursor<cGroup> groups)
{
	// set the level of the MinSim
	try
	{
		// If no element to group -> skip it
		RCursor<cObj> Cur(GetObjs());
		if(!Cur.GetNb())
			return;
		Objs.Clear(Cur.GetNb());

		// Create objects
		cout<<"Create objects...";
		size_t i(0);
		for(Cur.Start();!Cur.End();Cur.Next(),i++)
			Objs.InsertPtr(new GCAObj(i,Cur()));
		cout<<" "<<i+1<<" created"<<endl;

		switch(ClusteringMethod)
		{
			case 1:
				DoGCA(session,mes);
				break;
			case 2:
				DokMeans(session,mes,groups);
				break;
		}
	}
	catch(RGAException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(GException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(exception& e)
	{
		throw GException(e.what());
	}
	catch(...)
	{
		throw GException("Unknown Problem");
	}
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::Gen(const R::RNotification& notification)
{
	cout<<"Gen "<<GetData<size_t>(notification)<<endl;
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Population Size",16));
	params->InsertParam(new RParamValue("Max Gen",30));
	params->InsertParam(new RParamValue("Step",false));
	params->InsertParam(new RParamValue("Step Gen",0));
	params->InsertParam(new RParamValue("Min Agreement",0.6));
	params->InsertParam(new RParamValue("Min Disagreement",0.6));
	params->InsertParam(new RParamValue("Max kMeans",30));
	params->InsertParam(new RParamValue("Convergence",0.05));
	params->InsertParam(new RParamValue("NbDivChromos",2));
	params->InsertParam(new RParamValue("NbClusters",20));
	params->InsertParam(new RParamValue("Clustering Method","GA"));
	params->InsertParam(new RParamValue("LocalOptimisation",true));
	params->InsertParam(new RParamValue("Optimisation",true));
	params->InsertParam(new RParamValue("Incremental",true));
	params->InsertParam(new RParamValue("InternalRandom",true));
	params->InsertParam(new RParamValue("Seed",static_cast<int>(12345)));
	params->InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	GCAPlugIn<cObj,cGroup,cFactory>::~GCAPlugIn(void)
{
}
