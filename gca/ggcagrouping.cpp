/*

	GALILEI Research Project

	GCAGrouping.h

	GCA Plug-in - Implementation

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gmeasure.h>
#include <ggalileiapp.h>

//-----------------------------------------------------------------------------
// include files for GCA
#include <ggcagrouping.h>
#include <ggcachromo.h>
#include <ggcainst.h>
#include <ggcagroup.h>
#include <ggcaobj.h>



//-----------------------------------------------------------------------------
//
//  GGCAGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAGrouping::GGCAGrouping(GFactoryGrouping* fac)
	: GGrouping(fac), RObject("Profiles Grouping"), Objs(20)
{
}


//-----------------------------------------------------------------------------
void GGCAGrouping::ApplyConfig(void)
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
}


//-----------------------------------------------------------------------------
void GGCAGrouping::Connect(GSession* session)
{
	GGrouping::Connect(session);
}


//-----------------------------------------------------------------------------
void GGCAGrouping::Disconnect(GSession* session)
{
	GGrouping::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GGCAGrouping::Init(void)
{
}


//-----------------------------------------------------------------------------
bool GGCAGrouping::IsCoherent(const GGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGCAGrouping::IsCoherent(const GGroup* /*grp*/,const GProfile* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GGCAGrouping::IsValid(GGroup* /*grp*/)
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
void GGCAGrouping::ConstructGroupsFromChromo(GGCAChromo* chromo)
{
	
	unsigned int* tab;
	unsigned int* ptr;

	Session->ClearGroups();
	RCursor<GGCAGroup> gr(chromo->Used);
	for(gr.Start();!gr.End();gr.Next())
	{
		GGroup* g=new GGroup(cNoRef,true,RDate(""),RDate(""));
		Session->AssignId(g);
		ptr=tab=gr()->GetObjectsId();
		while((*ptr)!=NoObject)
			g->InsertProfile((Objs[*(ptr++)])->GetProfile());
		delete[] tab;
		Session->InsertGroup(g);
	}
}


//-----------------------------------------------------------------------------
void GGCAGrouping::BestChromo(const RNotification& notification)
{
    cout<<"Best Chromo at Gen : "<<GetData<unsigned int>(notification)<<endl;
}


//-----------------------------------------------------------------------------
void GGCAGrouping::Run(void)
{
	// If no subprofiles -> nothing to group
	if(!Profiles.GetNb()) return;

	// set the level of the MinSim
	try
	{
		double d;
		unsigned int i;
		GGCAObj* obj;

		// Get the minimum of similarity		
		GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities")->Info(0,&d);
  		Params.MinSimLevel=d;
		cout<<"MinSim="<<d<<endl;
		
		// Create the GA objects 
  		Objs.Clear(Profiles.GetNb());  		
		RCursor<GProfile> Cur2(Profiles);
		for(Cur2.Start(),i=0;!Cur2.End();Cur2.Next(),i++)
			Objs.InsertPtr(obj=new GGCAObj(i,Cur2()));
			
		// Launch the GA
		Instance=new GGCAInst(Session,RCursor<GGCAObj>(Objs),&Params,Session->GetDebug());
		Instance->Init();
		Instance->Run();
		
		// Cleanup and save the result
		ConstructGroupsFromChromo(Instance->BestChromosome);
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
void GGCAGrouping::CreateParams(RConfig* params)
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
	params->InsertParam(RPromLinearCriterion::CreateParam("Sim Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Agreement Criterion"));
	params->InsertParam(RPromLinearCriterion::CreateParam("Disagreement Criterion"));
}


//-----------------------------------------------------------------------------
GGCAGrouping::~GGCAGrouping(void)
{
}


//------------------------------------------------------------------------------
CREATE_GROUPING_FACTORY("Genetic Community Algorithm",GGCAGrouping)
