/*

	GALILEI Research Project

	GCAPlugIn.hh

	Generic Plug-in for GCA - Template Implementation

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::GCAPlugIn(GSession* session,GPlugInFactory* fac,const RString& name,tObjType objtype,tObjType grouptype)
		: cPlugin(session,fac), RObject(name), ObjType(objtype), GroupType(grouptype)
{
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsCoherent(const cGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsCoherent(const cGroup* /*grp*/,const cObj* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	bool GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::IsValid(cGroup* /*grp*/)
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
	template<class cAlgoGroup> void GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::ConstructResults(GSession* session,R::RCursor<cAlgoGroup> Sol)
{
	size_t i;
	RString Label(GetObjType(GroupType,true,false)+" ");


	session->Clear(GroupType);
	for(Sol.Start(),i=1;!Sol.End();Sol.Next(),i++)
	{
		cGroup* g(new cGroup(session,Label+RString::Number(i)));
		session->AssignId(g);
		RCursor<cObj> Cur(Sol()->GetObjs());
		for(Cur.Start();!Cur.End();Cur.Next())
			g->InsertObj(Cur());
		session->Insert(g);
	}
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::RunGrouping(GSession* session,const R::RString& mes,R::RCursor<cObj> Objs)
{
	const RMaxVector* Vec;
	GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities")->Info(3,1,&Vec);
	RCursor<RMaxValue> Cur(*Vec);
	for(Cur.Start();!Cur.End();Cur.Next())
		cout<<"Sim (1,"<<Cur()->Id<<")="<<Cur()->Value<<endl;

//	return;
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
	cInst Instance(session,Objs,this,0,ObjType);
	InsertObserver(reinterpret_cast<tNotificationHandler>(&GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::Gen),"RInst::Generation",&Instance);
	if(Cout)
		cout<<"Init GCA"<<endl;
//	Instance.SetVerify(true);
	Instance.Init();
	if(!InternalRandom)
		Instance.SetSeed(Seed);

	// Run
	if(Cout)
	{
		cout<<"Run GCA (";
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


//------------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::Gen(const R::RNotification& notification)
{
	if(Cout)
		cout<<"Gen "<<GetData<size_t>(notification)<<endl;
}


//------------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	void GCAPlugIn<cPlugin,cObj,cGroup,cInst,cGAGroup>::CreateConfig(void)
{
	cPlugin::InsertParam(new RParamValue("Population Size",16,"Size of the population for the GA"));
	cPlugin::InsertParam(new RParamValue("Max Gen",30,"Number of generations to run"));
	cPlugin::InsertParam(new RParamValue("Step",false,"Must the GA be run step by step"));
	cPlugin::InsertParam(new RParamValue("Step Gen",0,"Number of generations to run for a step"));
	cPlugin::InsertParam(new RParamValue("Min Agreement",0.6,"Minimum agreement ratio"));
	cPlugin::InsertParam(new RParamValue("Min Disagreement",0.6,"Minimum disagreement ratio"));
	cPlugin::InsertParam(new RParamValue("Incremental",true,"Should the algorithm be incremental"));
	cPlugin::InsertParam(new RParamValue("InternalRandom",true,"Use an internal random number generator"));
	cPlugin::InsertParam(new RParamValue("Seed",static_cast<int>(12345),"Internal random number generator seed"));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion","Promethée parameters for the similarity criterion"));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion","Promethée parameters for the agreement criterion"));
	cPlugin::InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion","Promethée parameters for the disagreement criterion"));
	cPlugin::InsertParam(new RParamValue("Cout",false,"Cout some information?"));
	cPlugin::InsertParam(new RParamValue("NbMinObjs",0,"Minimum number of objects per cluster (0 means no minimum)"));
	cPlugin::InsertParam(new RParamValue("NbMaxObjs",cNoRef,"Maximum number of objects per cluster"));
}
