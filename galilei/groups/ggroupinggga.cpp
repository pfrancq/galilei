/*

	GALILEI Research Project

	GGroupingGGA.cpp

	Heuristic using a GGA - Implementation

	(C) 2002 by P. Francq.

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
	  MaxGen(20), Step(false), StepGen(5), ParamsSim(0.2,0.05,1.0),
	  ParamsNb(0.2,0.05,1.0), ParamsOK(0.2,0.05,1.0), ParamsDiff(0.2,0.05,1.0),
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
	sprintf(tmp,"%c %u %u %c %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	        c1,PopSize,MaxGen,c,StepGen,MinSimLevel,
	        ParamsSim.P,ParamsSim.Q,ParamsSim.Weight,
	        ParamsNb.P,ParamsNb.Q,ParamsNb.Weight,
	        ParamsOK.P,ParamsOK.Q,ParamsOK.Weight,
	        ParamsDiff.P,ParamsDiff.Q,ParamsDiff.Weight,
	        ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetSettings(const char* s)
{
	char c,c1;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %c %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &c1,&PopSize,&MaxGen,&c,&StepGen,&MinSimLevel,
	       &ParamsSim.P,&ParamsSim.Q,&ParamsSim.Weight,
	       &ParamsNb.P,&ParamsNb.Q,&ParamsNb.Weight,
	       &ParamsOK.P,&ParamsOK.Q,&ParamsOK.Weight,
	       &ParamsDiff.P,&ParamsDiff.Q,&ParamsDiff.Weight,
	       &ParamsSocial.P,&ParamsSocial.Q,&ParamsSocial.Weight);
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
	else if(!strcmp(crit,"Nb Profiles"))
	{
		ParamsNb.P=p;
		ParamsNb.Q=q;
		ParamsNb.Weight=w;
	}
	else if(!strcmp(crit,"OK Factor"))
	{
		ParamsOK.P=p;
		ParamsOK.Q=q;
		ParamsOK.Weight=w;
	}
	else if(!strcmp(crit,"Diff Factor"))
	{
		ParamsDiff.P=p;
		ParamsDiff.Q=q;
		ParamsDiff.Weight=w;
	}
	else if(!strcmp(crit,"Social Factor"))
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
	else if(!strcmp(crit,"Nb Profiles"))
	{
		p=ParamsNb.P;
		q=ParamsNb.Q;
		w=ParamsNb.Weight;
	}
	else if(!strcmp(crit,"OK Factor"))
	{
		p=ParamsOK.P;
		q=ParamsOK.Q;
		w=ParamsOK.Weight;
	}
	else if(!strcmp(crit,"Diff Factor"))
	{
		p=ParamsDiff.P;
		q=ParamsDiff.Q;
		w=ParamsDiff.Weight;
	}
	else if(!strcmp(crit,"Social Factor"))
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
//	unsigned int i,j;
//	GSubProfile** sub1;
//	GSubProfile** sub2;
//
//	for(i=NbSubObjects+1,ptr=Owner->GetObjs(SubObjects);--i;ptr++)
//		if(Owner->Sims->GetSim(sub,(*ptr)->GetSubProfile())<Owner->MinSimLevel)
//			return(false);
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::ConstructGroupsFromChromo(GChromoIR* chromo) throw(bad_alloc)
{
	unsigned int i;
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
	RDebugXML file(RString("/home/pfrancq/text")+Lang->GetCode()+".xml","GIR","Pascal Francq");

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
			Instance=new GInstIR(Session,Lang,MinSimLevel,MaxGen,PopSize,Groups,Objs,GlobalSim,&Sims,RGGA::FirstFit,&file);
		else
			Instance=new GInstIR(Session,Lang,MinSimLevel,MaxGen,PopSize,0,Objs,GlobalSim,&Sims,RGGA::FirstFit,&file);
		Instance->Init(&data);
		Instance->SetCriterionParam("Similarity",ParamsSim.P,ParamsSim.Q,ParamsSim.Weight);
		Instance->SetCriterionParam("Nb Profiles",ParamsNb.P,ParamsNb.Q,ParamsNb.Weight);
		Instance->SetCriterionParam("OK Factor",ParamsOK.P,ParamsOK.Q,ParamsOK.Weight);
		Instance->SetCriterionParam("Diff Factor",ParamsDiff.P,ParamsDiff.Q,ParamsDiff.Weight);
		Instance->SetCriterionParam("Social Factor",ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
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
