/*

	Genetic Documents Algorithm

	GGDAGrouping.cpp

	GDA Plug-in - Implementation

	Copyright 2006-2008 by the Université Libre de Bruxelles.

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
#include <ggdagrouping.h>
#include <ggcachromo.h>
#include <ggcainst.h>
#include <ggcagroup.h>
#include <ggcaobj.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  GGDAGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGDAGrouping::GGDAGrouping(GFactoryPostDoc* fac)
	: GPostDoc(fac), Objs(20)
{
}


//-----------------------------------------------------------------------------
void GGDAGrouping::ApplyConfig(void)
{
	Params.PopSize=Factory->GetUInt("Population Size");
	Params.MaxGen=Factory->GetUInt("Max Gen");
	Params.Step=Factory->GetBool("Step");
	Params.StepGen=Factory->GetUInt("Step Gen");
	Params.MinAgreement=Factory->GetDouble("Min Agreement");
	Params.MinDisagreement=Factory->GetDouble("Min Disagreement");
	Params.MaxKMeans=Factory->GetUInt("Max kMeans");
	Params.Convergence=Factory->GetDouble("Convergence");
	Params.NbDivChromo=Factory->GetUInt("NbDivChromo");
	Params.ParamsSim=Factory->FindParam<RParamStruct>("Sim Criterion");
	Params.ParamsAgreement=Factory->FindParam<RParamStruct>("Agreement Criterion");
	Params.ParamsDisagreement=Factory->FindParam<RParamStruct>("Disagreement Criterion");
	RString what(Factory->Get("Clustering Method"));
	ClusteringMethod=0;
	if(what=="GA")
		ClusteringMethod=1;
	if(what=="k-Means")
		ClusteringMethod=2;
	NbClusters=Factory->GetUInt("NbClusters");
}


//-----------------------------------------------------------------------------
void GGDAGrouping::Connect(GSession* session)
{
	GPostDoc::Connect(session);
}


//-----------------------------------------------------------------------------
void GGDAGrouping::Disconnect(GSession* session)
{
	GPostDoc::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GGDAGrouping::Init(void)
{
}


//-----------------------------------------------------------------------------
bool GGDAGrouping::IsCoherent(const GCommunity* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGDAGrouping::IsCoherent(const GCommunity* /*grp*/,const GProfile* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGDAGrouping::IsValid(GCommunity* /*grp*/)
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
//	return(true);
	return(true);
}


//-----------------------------------------------------------------------------
void GGDAGrouping::ConstructResults(RCursor<GGCAGroup> Sol)
{
	size_t* tab;
	size_t* ptr;

	RTextFile Out("/var/log/galilei/res.txt");
	Out.Open(RIO::Create);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		Out<<"New group - "<<Sol()->GetNbObjs()<<endl;
		ptr=tab=Sol()->GetObjectsId();
		while((*ptr)!=NoObject)
		{
			GDoc* doc=static_cast<GDoc*>((Objs[*(ptr++)])->GetElement());
			//Out<<"\t"<<doc->GetId()<<doc->GetName()<<endl;
			Out<<"\t"<<doc->GetId()<<doc->GetURL()<<endl;
		}
		delete[] tab;
	}
	Out.Close();
}


//-----------------------------------------------------------------------------
void GGDAGrouping::DoGDA()
{
	double d;

	cout<<"Do GA"<<endl;

	// Run the GA
	cout<<"Get Mininmum"<<endl;
	GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities")->Info(0,&d);
	Params.MinSimLevel=d;
	cout<<"Min Sim="<<d<<endl;
	cout<<"New GDA"<<endl;
	GGCAInst Instance(Session,Objs,&Params,Session->GetDebug(),otDoc);
	cout<<"Init GDA"<<endl;
	Instance.Init();
	cout<<"Run GDA"<<endl;
	Instance.Run();
	cout<<"Build solutions"<<endl;
	ConstructResults(Instance.BestChromosome->Used);
}



//-----------------------------------------------------------------------------
class kMeansDoc;
class kMeansGroups;
class kMeansGroup : public RGroup<kMeansGroup,GGCAObj,kMeansGroups>
{
public:
	kMeansGroup(kMeansGroups* owner,size_t id);
	friend class KMeansDoc;
};


//-----------------------------------------------------------------------------
class kMeansGroups : public RGroups<kMeansGroup,GGCAObj,kMeansGroups>
{
public:
	kMeansGroups(RCursor<GGCAObj> objs,size_t max)
		: RGroups<kMeansGroup,GGCAObj,kMeansGroups>(objs,max)
		{}
	friend class KMeansDoc;
};

kMeansGroup::kMeansGroup(kMeansGroups* owner,size_t id)
	: RGroup<kMeansGroup,GGCAObj,kMeansGroups>(owner,id)
{
}


//-----------------------------------------------------------------------------
class kMeansDoc : public RGroupingKMeans<kMeansGroup,GGCAObj,kMeansGroups>
{
	GMeasure* Measure;

public:
	kMeansDoc(const RString& n,RRandom* r,RCursor<GGCAObj> objs,RDebug* debug=0)
		: RGroupingKMeans<kMeansGroup,GGCAObj,kMeansGroups>(n,r,objs,debug)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities");
	}

	virtual double Similarity(GGCAObj* obj1,GGCAObj* obj2)
	{
		double d;
		Measure->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
		return(d);
	}
};


//-----------------------------------------------------------------------------
void GGDAGrouping::DokMeans(void)
{
	cout<<"Do kMeans"<<endl;

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
	Out.Close();
}


//-----------------------------------------------------------------------------
void GGDAGrouping::Run(void)
{
	// set the level of the MinSim
	try
	{
		// If no document in an active language -> skip it
		size_t nbdocs=Session->GetNbDocs();
		if(!nbdocs)
			return;

		// Create objects
		cout<<"Create objects";
		Objs.Clear(nbdocs);
		RCursor<GDoc> Docs(Session->GetDocs());
		size_t i(0);
		for(Docs.Start();!Docs.End();Docs.Next(),i++)
			Objs.InsertPtr(new GGCAObj(i,Docs()));
		cout<<endl;

		switch(ClusteringMethod)
		{
			case 1:
				DoGDA();
				break;
			case 2:
				DokMeans();
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
		throw GException("Unknow Problem");
	}
}


//------------------------------------------------------------------------------
void GGDAGrouping::CreateParams(RConfig* params)
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
	params->InsertParam(new RParamValue("Clustering Method","GDA"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
GGDAGrouping::~GGDAGrouping(void)
{
}


//------------------------------------------------------------------------------
CREATE_POSTDOC_FACTORY("Genetic Documents Algorithm",GGDAGrouping)
