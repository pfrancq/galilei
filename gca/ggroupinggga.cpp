/*

	GALILEI Research Project

	GGroupingGGA.cpp

	Heuristic using a GGA - Implementation

	Copyright 2002-2005 by the Université Libre de Bruxelles.

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
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <rdebug.h>
#include <ggroupinggga.h>
#include <gchromoir.h>
#include <ginstir.h>
#include <ggroupir.h>
#include <gobjir.h>
#include <ggroup.h>
#include <guser.h>
#include <gsubprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gmeasure.h>
#include <gpluginmanagers.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
//  GGroupingGGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupingGGA::GGroupingGGA(GFactoryGrouping* fac) throw(bad_alloc)
	: GGrouping(fac), Objs(0)
{
}


//-----------------------------------------------------------------------------
void GGroupingGGA::ApplyConfig(void)
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
	Params.ParamsSim=(*Factory->GetProm("Sim Criterion"));
	Params.ParamsAgreement=(*Factory->GetProm("Agreement Criterion"));
	Params.ParamsDisagreement=(*Factory->GetProm("Disagreement Criterion"));
	Params.isf=Factory->GetBool("isf");
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Connect(GSession* session) throw(GException)
{
	GGrouping::Connect(session);
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Disconnect(GSession* session) throw(GException)
{
	GGrouping::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
bool GGroupingGGA::IsCoherent(const GGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGroupingGGA::IsCoherent(const GGroup* /*grp*/,const GSubProfile* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGroupingGGA::IsValid(GGroup* /*grp*/)
{
//	GSubProfileCursor Cur1,Cur2;
//	unsigned int i,j;
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
void GGroupingGGA::ConstructGroupsFromChromo(GChromoIR* chromo) throw(bad_alloc)
{
	GGroup* g;
	unsigned int* tab;
	unsigned int* ptr;

	Session->ClearGroups(Lang);
	RCursor<GGroupIR> gr(chromo->Used);
	for(gr.Start();!gr.End();gr.Next())
	{
		g=new GGroup(cNoRef,Lang,true,RDate(""),RDate(""));
		Session->AssignId(g);
		ptr=tab=gr()->GetObjectsId();
		while((*ptr)!=NoObject)
			g->InsertSubProfile(((*Objs)[*(ptr++)])->GetSubProfile());
		delete[] tab;
		Session->InsertGroup(g);
	}
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Run(void) throw(GException)
{
	unsigned int i;
	GObjIR* obj;

	if(!SubProfiles.GetNb()) return;

	// set the level of the MinSim
	try
	{
		GGroupDataIR data;
  		Params.MinSimLevel=GPluginManagers::GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities")->GetMinMeasure(Lang);
		Objs=new RObjs<GObjIR>(SubProfiles.GetNb());
		RCursor<GSubProfile> Cur2(SubProfiles);
		for(Cur2.Start(),i=0;!Cur2.End();Cur2.Next(),i++)
			Objs->InsertPtr(obj=new GObjIR(i,Cur2()));
		Instance=new GInstIR(Session,Lang,Objs,&Params,Session->GetDebug());
		Instance->Init(&data);
		Instance->Run();
		ConstructGroupsFromChromo(Instance->BestChromosome);
		delete Objs;
		Objs=0;
		delete Instance;
	}
	catch(eGA& e)
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
	catch(bad_alloc)
	{
		throw GException("Memory Problems");
	}
	catch(...)
	{
		throw GException("Unknow Problem");
	}
}


//------------------------------------------------------------------------------
void GGroupingGGA::CreateParams(GParams* params)
{
	RPromCriterionParams def;

	params->InsertPtr(new GParamUInt("Population Size",16));
	params->InsertPtr(new GParamUInt("Max Gen",30));
	params->InsertPtr(new GParamBool("Step",false));
	params->InsertPtr(new GParamUInt("Step Gen",0));
	params->InsertPtr(new GParamDouble("Min Agreement",0.6));
	params->InsertPtr(new GParamDouble("Min Disagreement",0.6));
	params->InsertPtr(new GParamUInt("Max kMeans",30));
	params->InsertPtr(new GParamDouble("Convergence",0.05));
	params->InsertPtr(new GParamUInt("NbDivChromo",2));
	params->InsertPtr(new GParamProm("Sim Criterion",&def));
	params->InsertPtr(new GParamProm("Agreement Criterion",&def));
	params->InsertPtr(new GParamProm("Disagreement Criterion",&def));
	params->InsertPtr(new GParamBool("isf",true));
}


//-----------------------------------------------------------------------------
GGroupingGGA::~GGroupingGGA(void)
{
	delete Objs;
}


//------------------------------------------------------------------------------
CREATE_GROUPING_FACTORY("Genetic Community Algorithm",GGroupingGGA)
