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
// class kMeansGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeansGroup::kMeansGroup(kMeansGroups* owner,size_t id)
	: R::RGroup<kMeansGroup,GCAObj,kMeansGroups>(owner,id)
{
}



//-----------------------------------------------------------------------------
//
// class kMeansGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeansGroups::kMeansGroups(R::RCursor<GCAObj> objs,size_t max)
	: R::RGroups<kMeansGroup,GCAObj,kMeansGroups>(objs,max)
{
}



//-----------------------------------------------------------------------------
//
// class kMeansObj
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeansObj::kMeansObj(const R::RString& n,R::RRandom* r,R::RCursor<GCAObj> objs,const R::RString& mes,R::RDebug* debug)
	: R::RGroupingKMeans<kMeansGroup,GCAObj,kMeansGroups>(n,r,objs,debug)
{
	Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Similarities");
}


//-----------------------------------------------------------------------------
double kMeansObj::Similarity(GCAObj* obj1,GCAObj* obj2)
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
	GCAPlugIn<cObj,cGroup,cFactory>::GCAPlugIn(const RString& name,tObjType type)
		: RObject(name), Objs(20), Type(type)
{
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::ApplyConfig(cFactory* factory)
{
	Params.PopSize=factory->GetUInt("Population Size");
	Params.MaxGen=factory->GetUInt("Max Gen");
	Params.Step=factory->GetBool("Step");
	Params.StepGen=factory->GetUInt("Step Gen");
	Params.MinAgreement=factory->GetDouble("Min Agreement");
	Params.MinDisagreement=factory->GetDouble("Min Disagreement");
	Params.MaxKMeans=factory->GetUInt("Max kMeans");
	Params.Convergence=factory->GetDouble("Convergence");
	Params.NbDivChromo=factory->GetUInt("NbDivChromo");
	Params.ParamsSim=factory->R::RConfig::FindParam<R::RParamStruct>("Sim Criterion");
	Params.ParamsAgreement=factory->R::RConfig::FindParam<R::RParamStruct>("Agreement Criterion");
	Params.ParamsDisagreement=factory->R::RConfig::FindParam<R::RParamStruct>("Disagreement Criterion");
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

	session->ClearGroups(Type);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		cGroup* g=static_cast<cGroup*>(session->NewGroup(Type));
		session->AssignId(g);
		ptr=tab=Sol()->GetObjectsId();
		while((*ptr)!=cNoRef)
			g->InsertObj(static_cast<cObj*>(Objs[*(ptr++)]->GetElement()));
		delete[] tab;
		session->InsertGroup(g,Type);
	}
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::DoGCA(GSession* session,const R::RString& mes)
{
	double d;

	cout<<"Do GCA for "<<GetObjType(Type)<<"s"<<endl;

	// Init the GCA
	cout<<"Get minimum similarity"<<endl;
	GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Similarities")->Info(0,&d);
	Params.MinSimLevel=d;
	cout<<"   Minimum Similarity="<<d<<endl;
	cout<<"New GCA"<<endl;
	GCAInst Instance(session,Objs,&Params,session->GetDebug(),Type,mes);
	cout<<"Init GCA"<<endl;
	Instance.Init();

	// Run
	cout<<"Run GCA"<<endl;
	Instance.Run();
	cout<<"Build solutions"<<endl;
	ConstructResults<GCAGroup>(session,Instance.BestChromosome->Used);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::DokMeans(GSession* session,const R::RString& mes)
{
	cout<<"Do kMeans for "<<GetObjType(Type)<<"s"<<endl;
	RRandomGood Rand(1);
	Rand.Reset(1);

	kMeansObj kMeans("k-MeansObj",&Rand,Objs,mes);
	kMeansGroups Sol(Objs,NbClusters);
	Sol.Init();
	cout<<"Run kMeans ("<<NbClusters<<" clusters)"<<endl;
	kMeans.Run(&Sol,Params.MaxKMeans,NbClusters);
	cout<<"kMeans iterates "<<kMeans.GetNbIterations()<<" times"<<endl;
	ConstructResults<kMeansGroup>(session,Sol);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCAPlugIn<cObj,cGroup,cFactory>::Run(GSession* session,const R::RString& mes)
{
	// set the level of the MinSim
	try
	{
		// If no document in an active language -> skip it
		size_t nb=session->GetNbElements(Type);
		if(!nb)
			return;

		// Create objects
		cout<<"Create objects";
		Objs.Clear(nb);
		RCursor<cObj> Cur(GetObjs());
		size_t i(0);
		for(Cur.Start();!Cur.End();Cur.Next(),i++)
			Objs.InsertPtr(new GCAObj(i,Cur()));
		cout<<endl;

		switch(ClusteringMethod)
		{
			case 1:
				DoGCA(session,mes);
				break;
			case 2:
				DokMeans(session,mes);
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
	params->InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	GCAPlugIn<cObj,cGroup,cFactory>::~GCAPlugIn(void)
{
}
