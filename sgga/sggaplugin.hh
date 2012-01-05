/*

	GALILEI Research Project

	SGGAPlugIn.hh

	Generic Plug-in for SGGA - Template Implementation

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
#include <sggainst.h>
#include <sggagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class SGGAPlugIn
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::SGGAPlugIn(GSession* session,GPlugInFactory* fac,const RString& name,tObjType objtype,tObjType grouptype)
		: cPlugin(session,fac), RObject(name), ObjType(objtype),GroupType(grouptype)
{
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsCoherent(const cGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsCoherent(const cGroup* /*grp*/,const cObj* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsValid(cGroup* /*grp*/)
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
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	template<class cAlgoGroup> void SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::ConstructResults(GSession* session,R::RCursor<cAlgoGroup> Sol)
{
	size_t i;
	RString Label(GetObjType(GroupType,true,false)+" ");


	session->Reset(GroupType);
	for(Sol.Start(),i=1;!Sol.End();Sol.Next(),i++)
	{
		cGroup* g(new cGroup(session,Label+RString::Number(i)));
		session->AssignId(g);
		RCursor<cObj> Cur(Sol()->GetObjs());
		for(Cur.Start();!Cur.End();Cur.Next())
			g->InsertObj(Cur());
		session->InsertObj(g);
	}
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::RunGrouping(GSession* session,const R::RString& mes)
{
	// set the level of the MinSim
	try
	{
		// Create references objects
		if(Cout)
			cout<<"Create objects"<<endl;
		InitObjs(session->GetMaxObjId(static_cast<cObj*>(0)),20);

		if(Cout)
			cout<<"Do SGGA for "<<GetObjType(ObjType,false,true)<<endl;

		// Init the SGGA
		if(Cout)
			cout<<"Get minimum similarity"<<endl;
		double d;
		GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(0,&d);
		MinSimLevel=d;
		if(Cout)
		{
			cout<<"   Minimum Similarity="<<d<<endl;
			double nb;
			GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(1,&nb);
			cout<<"   Mean="<<nb<<endl;
			GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(2,&nb);
			cout<<"   Deviation="<<nb<<endl;
			cout<<"New SGGA"<<endl;
		}
		cInst Instance(session,session->GetObjs(static_cast<cObj*>(0)),this,0,ObjType);
		InsertObserver(reinterpret_cast<tNotificationHandler>(&SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::Gen),"RInst::Generation",&Instance);
		if(Cout)
			cout<<"Init SGGA"<<endl;
	//	Instance.SetVerify(true);
		Instance.Init();
		Instance.SetLocalOptimisation(LocalOptimisation);
		Instance.SetOptimisation(Optimisation);
		if(!InternalRandom)
			Instance.SetSeed(Seed);

		// Run
		if(Cout)
		{
			cout<<"Run SGGA (";
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
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::Gen(const R::RNotification& notification)
{
	if(Cout)
		cout<<"Gen "<<GetData<size_t>(notification)<<endl;
}


//------------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void SGGAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::CreateConfig(void)
{
	cPlugin::InsertParam(new RParamValue("Population Size",16));
	cPlugin::InsertParam(new RParamValue("Max Gen",30));
	cPlugin::InsertParam(new RParamValue("Step",false));
	cPlugin::InsertParam(new RParamValue("Step Gen",0));
	cPlugin::InsertParam(new RParamValue("Min Agreement",0.6));
	cPlugin::InsertParam(new RParamValue("Min Disagreement",0.6));
	cPlugin::InsertParam(new RParamValue("Max kMeans",30));
	cPlugin::InsertParam(new RParamValue("Convergence",0.05));
	cPlugin::InsertParam(new RParamValue("NbDivChromos",2));
	cPlugin::InsertParam(new RParamValue("LocalOptimisation",true));
	cPlugin::InsertParam(new RParamValue("Optimisation",true));
	cPlugin::InsertParam(new RParamValue("Incremental",true));
	cPlugin::InsertParam(new RParamValue("InternalRandom",true));
	cPlugin::InsertParam(new RParamValue("Seed",static_cast<int>(12345)));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
	cPlugin::InsertParam(new RParamValue("Cout",false));
	cPlugin::InsertParam(new RParamValue("NbMinObjs",0));
	cPlugin::InsertParam(new RParamValue("NbMaxObjs",cNoRef));
	cPlugin::InsertParam(new RParamValue("AllMinSim",true));
}
