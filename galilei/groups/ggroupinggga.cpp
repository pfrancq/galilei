/*

	GALILEI Research Project

	GGroupingGGA.cpp

	Heuristic using a GGA - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <rga/rdebug.h>
#include <groups/ggroupinggga.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/guser.h>
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RPromethee;
using namespace RGA;



//-----------------------------------------------------------------------------
//
//  GGroupingGGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingGGA::GGroupingGGA(GSession* s) throw(bad_alloc)
	: GGrouping("Grouping Genetic Algorithms",s), PopSize(16), MinSimLevel(0.1),
	  MinCommonOK(1.0), MinCommonDiff(1.0), MaxGen(20), Step(false), StepGen(5),
	  MaxKMeans(10), SimMeasure(stAvgSim), ParamsSim(0.2,0.05,1.0), ParamsInfo(0.2,0.05,1.0),
	  ParamsSameFeedbacks(0.2,0.05,1.0), ParamsDiffFeedbacks(0.2,0.05,1.0),
	  ParamsSocial(0.2,0.05,1.0), GlobalSim(false), Objs(0)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingGGA::GetSettings(void)
{
	static char tmp[200];
	char c,c1;

	if(Step) c='1'; else c='0';
	if(GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%u %c %u %u %c %u %f %f %f %u %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
	        SimMeasure,c1,PopSize,MaxGen,c,StepGen,MinSimLevel,MinCommonOK,MinCommonDiff,MaxKMeans,
	        ParamsSim.P,ParamsSim.Q,ParamsSim.Weight,
	        ParamsInfo.P,ParamsInfo.Q,ParamsInfo.Weight,
	        ParamsSameFeedbacks.P,ParamsSameFeedbacks.Q,ParamsSameFeedbacks.Weight,
	        ParamsDiffFeedbacks.P,ParamsDiffFeedbacks.Q,ParamsDiffFeedbacks.Weight,
	        ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetSettings(const char* s)
{
	char c,c1;
	unsigned int t;

	if(!(*s)) return;
	sscanf(s,"%u %c %u %u %c %u %lf %lf %lf %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &t,&c1,&PopSize,&MaxGen,&c,&StepGen,&MinSimLevel,&MinCommonOK,&MinCommonDiff,&MaxKMeans,
	       &ParamsSim.P,&ParamsSim.Q,&ParamsSim.Weight,
	       &ParamsInfo.P,&ParamsInfo.Q,&ParamsInfo.Weight,
	       &ParamsSameFeedbacks.P,&ParamsSameFeedbacks.Q,&ParamsSameFeedbacks.Weight,
	       &ParamsDiffFeedbacks.P,&ParamsDiffFeedbacks.Q,&ParamsDiffFeedbacks.Weight,
	       &ParamsSocial.P,&ParamsSocial.Q,&ParamsSocial.Weight);
	SimMeasure=static_cast<SimType>(t);
	if(c=='1') Step=true; else Step=false;
	if(c1=='1') GlobalSim=true; else GlobalSim=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetCriterionParam(const char* crit,double p,double q,double w)
{
	if(!strcmp(crit,"Similarity"))
	{
		ParamsSim.P=p;
		ParamsSim.Q=q;
		ParamsSim.Weight=w;
	}
	else if(!strcmp(crit,"Information"))
	{
		ParamsInfo.P=p;
		ParamsInfo.Q=q;
		ParamsInfo.Weight=w;
	}
	else if(!strcmp(crit,"Same Feedbacks"))
	{
		ParamsSameFeedbacks.P=p;
		ParamsSameFeedbacks.Q=q;
		ParamsSameFeedbacks.Weight=w;
	}
	else if(!strcmp(crit,"Diff Feedbacks"))
	{
		ParamsDiffFeedbacks.P=p;
		ParamsDiffFeedbacks.Q=q;
		ParamsDiffFeedbacks.Weight=w;
	}
	else if(!strcmp(crit,"Social"))
	{
		ParamsSocial.P=p;
		ParamsSocial.Q=q;
		ParamsSocial.Weight=w;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::GetCriterionParam(const char* crit,double& p,double& q,double& w)
{
	if(!strcmp(crit,"Similarity"))
	{
		p=ParamsSim.P;
		q=ParamsSim.Q;
		w=ParamsSim.Weight;
	}
	else if(!strcmp(crit,"Information"))
	{
		p=ParamsInfo.P;
		q=ParamsInfo.Q;
		w=ParamsInfo.Weight;
	}
	else if(!strcmp(crit,"Same Feedbacks"))
	{
		p=ParamsSameFeedbacks.P;
		q=ParamsSameFeedbacks.Q;
		w=ParamsSameFeedbacks.Weight;
	}
	else if(!strcmp(crit,"Diff Feedbacks"))
	{
		p=ParamsDiffFeedbacks.P;
		q=ParamsDiffFeedbacks.Q;
		w=ParamsDiffFeedbacks.Weight;
	}
	else if(!strcmp(crit,"Social"))
	{
		p=ParamsSocial.P;
		q=ParamsSocial.Q;
		w=ParamsSocial.Weight;
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingGGA::IsCoherent(const GGroup* /*grp*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingGGA::IsCoherent(const GGroup* /*grp*/,const GSubProfile* /*sub*/) const
{
	return(true);
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingGGA::IsValid(GGroup* /*grp*/)
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
void GALILEI::GGroupingGGA::ConstructGroupsFromChromo(GChromoIR* chromo) throw(bad_alloc)
{
	GGroupIR* gr;
	GGroup* g;
	unsigned int* tab;
	unsigned int* ptr;

	Clear();
	for(chromo->Used.Start();!chromo->Used.End();chromo->Used.Next())
	{
		gr=chromo->Used();
		g=NewGroup(Lang);
		ptr=tab=gr->GetObjectsId();
		while((*ptr)!=RGGA::NoObject)
			g->InsertSubProfile(Objs->Tab[*(ptr++)]->GetSubProfile());
		delete[] tab;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::Run(void) throw(GException)
{
	unsigned int i;
//	RDebugXML file(RString("/home/pfrancq/text")+Lang->GetCode()+".xml","GIR","Pascal Francq");

	if(!SubProfiles.NbPtr) return;
	try
	{
	 	GGroupDataIR data;
		Objs=new RGA::RObjs<GObjIR>(SubProfiles.NbPtr);
		for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
		{
			Objs->InsertPtr(new GObjIR(i,SubProfiles()));
		}
		GProfilesSim Sims(SubProfiles,GlobalSim);
		if(Modified)
			Instance=new GInstIR(Session,Lang,MinSimLevel,MaxGen,PopSize,Groups,Objs,GlobalSim,&Sims,SimMeasure,0/*&file*/);
		else
			Instance=new GInstIR(Session,Lang,MinSimLevel,MaxGen,PopSize,0,Objs,GlobalSim,&Sims,SimMeasure,0/*&file*/);
		Instance->Init(&data);
		Instance->SetCriterionParam("Similarity",ParamsSim.P,ParamsSim.Q,ParamsSim.Weight);
		Instance->SetCriterionParam("Information",ParamsInfo.P,ParamsInfo.Q,ParamsInfo.Weight);
		Instance->SetCriterionParam("Same Feedbacks",ParamsSameFeedbacks.P,ParamsSameFeedbacks.Q,ParamsSameFeedbacks.Weight);
		Instance->SetCriterionParam("Diff Feedbacks",ParamsDiffFeedbacks.P,ParamsDiffFeedbacks.Q,ParamsDiffFeedbacks.Weight);
		Instance->SetCriterionParam("Social",ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
		Instance->SetMinRatios(MinCommonOK,MinCommonDiff);
		Instance->SetMaxKMeans(MaxKMeans);
		#ifdef RGADEBUG
			if(IdealGroups) Instance->SetIdealGroups(IdealGroups);
		#endif
		Instance->Run();
		ConstructGroupsFromChromo(Instance->BestChromosome);
		delete Objs;
		Objs=0;
		delete Instance;
	}
	catch(eGA& e)
	{
		throw GException(e.Msg);
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


//-----------------------------------------------------------------------------
GALILEI::GGroupingGGA::~GGroupingGGA(void)
{
	if(Objs) delete Objs;
}
