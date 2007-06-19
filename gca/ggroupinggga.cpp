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
#include <gprofile.h>
#include <gsubprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gmeasure.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
//  GGroupingGGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupingGGA::GGroupingGGA(GFactoryGrouping* fac)
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
	Params.ParamsSim.Set(Factory->FindParam<RParamStruct>("Sim Criterion"));
	Params.ParamsAgreement.Set(Factory->FindParam<RParamStruct>("Agreement Criterion"));
	Params.ParamsDisagreement.Set(Factory->FindParam<RParamStruct>("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Connect(GSession* session)
{
	GGrouping::Connect(session);
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Disconnect(GSession* session)
{
	GGrouping::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Init(void)
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
void GGroupingGGA::ConstructGroupsFromChromo(GChromoIR* chromo)
{
	
	unsigned int* tab;
	unsigned int* ptr;

	Session->ClearGroups(Lang);
	RCursor<GGroupIR> gr(chromo->Used);
	for(gr.Start();!gr.End();gr.Next())
	{
		GGroup* g=new GGroup(cNoRef,Lang,true,RDate(""),RDate(""));
		Session->AssignId(g);
		ptr=tab=gr()->GetObjectsId();
		while((*ptr)!=NoObject)
			g->InsertSubProfile(((*Objs)[*(ptr++)])->GetSubProfile());
		delete[] tab;
		Session->InsertGroup(g);
	}
}


//-----------------------------------------------------------------------------
void GGroupingGGA::Run(void)
{
	// If no subprofiles -> nothing to group
	if(!SubProfiles.GetNb()) return;

	// set the level of the MinSim
	try
	{
		GGroupDataIR data;
		double d;
		unsigned int i;
		GObjIR* obj;

		// Get the minimum of similarity		
		GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("SubProfiles Similarities")->Info(0,Lang,&d);
  		Params.MinSimLevel=d;
		cout<<"Min (0.139054?)="<<d<<endl;
		
		// Create the GA objects 
		Objs=new RObjs<GObjIR>(SubProfiles.GetNb());
		RCursor<GSubProfile> Cur2(SubProfiles);
		for(Cur2.Start(),i=0;!Cur2.End();Cur2.Next(),i++)
			Objs->InsertPtr(obj=new GObjIR(i,Cur2()));
			
		// Launch the GA
		Instance=new GInstIR(Session,Lang,Objs,&Params,Session->GetDebug());
		Instance->Init(&data);
		Instance->Run();
		
		// Cleanup and save the result
		ConstructGroupsFromChromo(Instance->BestChromosome);
		delete Objs;
		Objs=0;
		delete Instance;
	}
	catch(RGAException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
	catch(GException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
	catch(RException& e)
	{
		cerr<<e.GetMsg()<<endl;
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
void GGroupingGGA::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Population Size",16));
	params->InsertParam(new RParamValue("Max Gen",30));
	params->InsertParam(new RParamValue("Step",false));
	params->InsertParam(new RParamValue("Step Gen",0));
	params->InsertParam(new RParamValue("Min Agreement",0.6));
	params->InsertParam(new RParamValue("Min Disagreement",0.6));
	params->InsertParam(new RParamValue("Max kMeans",30));
	params->InsertParam(new RParamValue("Convergence",0.05));
	params->InsertParam(new RParamValue("NbDivChromo",2));
	params->InsertParam(RPromCriterionParams::CreateParam("Sim Criterion"));
	params->InsertParam(RPromCriterionParams::CreateParam("Agreement Criterion"));
	params->InsertParam(RPromCriterionParams::CreateParam("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
GGroupingGGA::~GGroupingGGA(void)
{
	delete Objs;
}


//------------------------------------------------------------------------------
CREATE_GROUPING_FACTORY("Genetic Community Algorithm",GGroupingGGA)
