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
//include files for GALILEI
#include<groups/ggroupinggga.h>
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



//-----------------------------------------------------------------------------
//
//  GGroupingGGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingGGA::GGroupingGGA(GSession* s) throw(bad_alloc)
	: GGrouping("Grouping Genetic Algorithms",s), PopSize(16), MinSimLevel(0.1),
	  MaxGen(20), Step(false), StepGen(5)
{
}

//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingGGA::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(Step) c='1'; else c='0';
	sprintf(tmp,"%u %u %c %u %lf",PopSize,MaxGen,c,StepGen,MinSimLevel);
	return(tmp);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %c %u %lf",&PopSize,&MaxGen,&c,&StepGen,&MinSimLevel);
	if(c=='1') Step=true; else Step=false;
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
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingGGA::Run(void) throw(GException)
{
	unsigned int i;
	GGroupIR* gr;
	GGroup* g;
	unsigned int* tab;
	unsigned int* ptr;

	if(!SubProfiles.NbPtr) return;
	try
	{
	 	GGroupDataIR data;
		RGA::RObjs<GObjIR> Objs(SubProfiles.NbPtr);
		for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
		{
			Objs.InsertPtr(new GObjIR(i,SubProfiles()));
		}
		GProfilesSim Sims(SubProfiles);
		GInstIR Instance(MinSimLevel,MaxGen,PopSize,&Objs,&Sims,RGGA::FirstFit,0);
		Instance.Init(&data);
		Instance.Run();
		Clear();
		for(Instance.BestChromosome->Used.Start();!Instance.BestChromosome->Used.End();Instance.BestChromosome->Used.Next())
		{
			gr=Instance.BestChromosome->Used();
			Groups->InsertPtr(g=Session->NewGroup(Lang));
			ptr=tab=gr->GetObjectsId();
			while((*ptr)!=RGGA::NoObject)
				g->InsertSubProfile(Objs.Tab[*(ptr++)]->GetSubProfile());
			delete[] tab;
		}
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
}
