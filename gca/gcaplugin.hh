/*

	Genetic Documents Algorithm

	GCA.hh

	Genetic Clustering Algorithm - Template Implementation

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
#include <rgroupingkmeans.h>


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
//  GCA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	GCA<cObj,cGroup,cFactory>::GCA(const RString& name,tObjType type)
		: RObject(name), Objs(20), Type(type)
{
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCA<cObj,cGroup,cFactory>::ApplyConfig(cFactory* factory)
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
	bool GCA<cObj,cGroup,cFactory>::IsCoherent(const cGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	bool GCA<cObj,cGroup,cFactory>::IsCoherent(const cGroup* /*grp*/,const cObj* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	bool GCA<cObj,cGroup,cFactory>::IsValid(cGroup* /*grp*/)
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
	void GCA<cObj,cGroup,cFactory>::ConstructResults(GSession* session,R::RCursor<GCAGroup> Sol)
{
	size_t* tab;
	size_t* ptr;

	session->ClearGroups(Type);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		cGroup* g=static_cast<cGroup*>(session->NewGroup(Type));
		session->AssignId(g);
		ptr=tab=Sol()->GetObjectsId();
		while((*ptr)!=NoObject)
			g->InsertObj(static_cast<cObj*>(Objs[*(ptr++)]->GetElement()));
		delete[] tab;
		session->InsertGroup(g,Type);
	}
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCA<cObj,cGroup,cFactory>::DoGCA(GSession* session)
{
	double d;

	cout<<"Do GA"<<endl;

	// Run the GA
	cout<<"Get Mininmum"<<endl;

	switch(Type)
	{
		case otDoc :
			GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities")->Info(0,&d);
			break;
		case otProfile :
			GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities")->Info(0,&d);
			break;
		default:
			break;

	}
	Params.MinSimLevel=d;
	cout<<"Min Sim="<<d<<endl;
	cout<<"New GDA"<<endl;
	GCAInst Instance(session,Objs,&Params,session->GetDebug(),Type);
	cout<<"Init GDA"<<endl;
	Instance.Init();
	cout<<"Run GDA"<<endl;
	Instance.Run();
	cout<<"Build solutions"<<endl;
	ConstructResults(session,Instance.BestChromosome->Used);
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCA<cObj,cGroup,cFactory>::DokMeans(GSession*)
{
/*	cout<<"Do kMeans"<<endl;

	cout<<"Get Mininmum"<<endl;
	double d;
	GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities")->Info(0,&d);
	Params.MinSimLevel=d;
	cout<<"Min Sim="<<d<<endl;

	RRandomGood Rand(1);
	Rand.Reset(1);

	kMeansDoc kMeans("k-MeansDocs",&Rand,Objs);
	kMeansGroups Sol(Objs,NbClusters);
	Sol.Init();
	cout<<"Run kMeans"<<endl;
	kMeans.Run(&Sol,Params.MaxKMeans,NbClusters);
	cout<<kMeans.GetNbIterations()<<" iterations runned"<<endl;

	cout<<"Save solutions"<<endl;
	size_t* tab;
	size_t* ptr;

	RTextFile Out("/var/log/galilei/res.txt");
	Out.Open(RIO::Create);
	RCursor<kMeansGroup> Cur(Sol);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Out<<"New group - "<<Cur()->GetNbObjs()<<endl;
		ptr=tab=Cur()->GetObjectsId();
		while((*ptr)!=NoObject)
		{
			GDoc* doc=static_cast<GDoc*>((Objs[*(ptr++)])->GetElement());
			Out<<"\t"<<doc->GetId()<<doc->GetName()<<endl;
		}
		delete[] tab;
	}
	Out.Close();*/
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	void GCA<cObj,cGroup,cFactory>::Run(GSession* session)
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
				DoGCA(session);
				break;
			case 2:
				DokMeans(session);
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
	void GCA<cObj,cGroup,cFactory>::CreateParams(RConfig* params)
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
	GCA<cObj,cGroup,cFactory>::~GCA(void)
{
}
