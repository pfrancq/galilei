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
GALILEI::GGroupingGGA::GGroupingGGA(GSession* s,GIRParams* p) throw(bad_alloc)
	: GGrouping("Grouping Genetic Algorithms",s), Objs(0), Params(p)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetParam(const char* param,const char* value)
{
	if(!strcmp(param,"Global"))
	{
		Params->GlobalSim=atoi(value);
	}
	else
	if(!strcmp(param,"SimLevel"))
	{
		Params->MinSimLevel=atof(value);
	}
	else
	if(!strcmp(param,"SameBehaviors"))
	{
		Params->MinCommonOK=atof(value);
	}
	else
	if(!strcmp(param,"DiffBehaviors"))
	{
		Params->MinCommonDiff=atof(value);
	}
	else
	if(!strcmp(param,"k-Means"))
	{
		Params->MaxKMeans=atoi(value);
	}
	else
	if(!strcmp(param,"CritSim"))
	{
		Params->ParamsSim.Set(value);
	}
	else
	if(!strcmp(param,"CritInfo"))
	{
		Params->ParamsInfo.Set(value);
	}
	else
	if(!strcmp(param,"CritEntropy"))
	{
		Params->ParamsEntropy.Set(value);
	}
	else
	if(!strcmp(param,"CritSameBehaviors"))
	{
		Params->ParamsSameFeedbacks.Set(value);
	}
	else
	if(!strcmp(param,"CritDiffBehaviors"))
	{
		Params->ParamsDiffFeedbacks.Set(value);
	}
	else
	if(!strcmp(param,"CritSocial"))
	{
		Params->ParamsSocial.Set(value);
	}
	else
	if(!strncmp(param,"Sim(",4))
	{
		int i;
		char tmp[60];
		strcmp(tmp,&param[4]);
		tmp[strlen(tmp)-1]=0;       // Skip last ')';
		GSimMeasure* ptr=Params->Measures.GetPtr<const char*>(tmp);
		if(!ptr) return;
		sscanf(value,"%i %lf",&i,&(ptr->Weight));
		ptr->Use=i;
	}
	else
	if(!strcmp(param,"SimType"))
	{
		if(!strcmp(value,"Criteria"))
		{
			Params->SimMeasures=sctCrits;
		}
		else
		if(!strcmp(value,"Correlation"))
		{
			Params->SimMeasures=sctCorl;
		}
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingGGA::GetSettings(void)
{
	static char tmp[200];
	char c,c1;

	if(Params->Step) c='1'; else c='0';
	if(Params->GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%u %c %u %u %c %u %f %f %f %u %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
	        Params->SimMeasures,c1,Params->PopSize,Params->MaxGen,c,Params->StepGen,Params->MinSimLevel,Params->MinCommonOK,Params->MinCommonDiff,Params->MaxKMeans,
	        Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight,
	        Params->ParamsInfo.P,Params->ParamsInfo.Q,Params->ParamsInfo.Weight,
	        Params->ParamsSameFeedbacks.P,Params->ParamsSameFeedbacks.Q,Params->ParamsSameFeedbacks.Weight,
	        Params->ParamsDiffFeedbacks.P,Params->ParamsDiffFeedbacks.Q,Params->ParamsDiffFeedbacks.Weight,
	        Params->ParamsSocial.P,Params->ParamsSocial.Q,Params->ParamsSocial.Weight);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetSettings(const char* s)
{
	char c,c1;
	unsigned int t;

	if(!(*s)) return;
	sscanf(s,"%u %c %u %u %c %u %lf %lf %lf %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &t,&c1,&Params->PopSize,&Params->MaxGen,&c,&Params->StepGen,&Params->MinSimLevel,&Params->MinCommonOK,&Params->MinCommonDiff,&Params->MaxKMeans,
	       &Params->ParamsSim.P,&Params->ParamsSim.Q,&Params->ParamsSim.Weight,
	       &Params->ParamsInfo.P,&Params->ParamsInfo.Q,&Params->ParamsInfo.Weight,
	       &Params->ParamsSameFeedbacks.P,&Params->ParamsSameFeedbacks.Q,&Params->ParamsSameFeedbacks.Weight,
	       &Params->ParamsDiffFeedbacks.P,&Params->ParamsDiffFeedbacks.Q,&Params->ParamsDiffFeedbacks.Weight,
	       &Params->ParamsSocial.P,&Params->ParamsSocial.Q,&Params->ParamsSocial.Weight);
	Params->SimMeasures=static_cast<SimCritType>(t);
	if(c=='1') Params->Step=true; else Params->Step=false;
	if(c1=='1') Params->GlobalSim=true; else Params->GlobalSim=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::Init(void) throw(bad_alloc)
{
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
		GProfilesSim Sims(SubProfiles,Params->GlobalSim);
		if(Modified)
			Instance=new GInstIR(Session,Lang,Groups,Objs,&Sims,Params,0/*&file*/);
		else
			Instance=new GInstIR(Session,Lang,0,Objs,&Sims,Params,0/*&file*/);
		Instance->Init(&data);
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
