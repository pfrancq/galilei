/*

	GALILEI Research Project

	GGroupingSim.cpp

	Heuristic using Similarity - Implementation

	Copyright 2001 by the Université Libre de Bruxelles.

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
//#include <rstd/rcontainercursor.h>
//using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingsim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GSimParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSimParams::GSimParams(void)
	: GGroupingParams("Sim")
{

}


//-----------------------------------------------------------------------------
const char* GALILEI::GSimParams::GetSettings(void)
{
	static char tmp[100];
	char c,c1;

	if(FullSim) c='1'; else c='0';
	if(GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%c %f %c",c,LevelSim,c1);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSimParams::SetSettings(const char* s)
{
	char c,c1;

	if(!(*s)) return;
	sscanf(s,"%c %lf %c",&c,&LevelSim,&c1);
	if(c=='1') FullSim=true; else FullSim=false;
	if(c1=='1') GlobalSim=true; else GlobalSim=false;
}



//-----------------------------------------------------------------------------
//
//  GGroupingSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::GGroupingSim(GSession* s, GSimParams* p) throw(bad_alloc)
	: GGrouping("First-Fit Heuristic",s), Params(p)

{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::GGroupingSim(const char* n,GSession* s, GSimParams* p) throw(bad_alloc)
	: GGrouping(n,s), Params(p)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingSim::GetSettings(void)
{
	static char tmp[100];
	char c,c1;

	if(Params->FullSim) c='1'; else c='0';
	if(Params->GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%c %f %c",c,Params->LevelSim,c1);
	return(tmp);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::SetSettings(const char* s)
{
	char c,c1;

	if(!(*s)) return;
	sscanf(s,"%c %lf %c",&c,&Params->LevelSim,&c1);
	if(c=='1') Params->FullSim=true; else Params->FullSim=false;
	if(c1=='1') Params->GlobalSim=true; else Params->GlobalSim=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsCoherent(const GGroup* grp) const
{
	GSubProfile** s1;
	GSubProfile** s2;
	unsigned int i,j;

	if(Params->GlobalSim)
	{
		if(Params->FullSim)
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->GlobalSimilarity(*s2)<Params->LevelSim)
						return(false);
			return(true);
		}
		else
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->GlobalSimilarity(*s2)>=Params->LevelSim)
						return(true);
			return(false);
		}
	}
	else
	{
		if(Params->FullSim)
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->Similarity(*s2)<Params->LevelSim)
						return(false);
			return(true);
		}
		else
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->Similarity(*s2)>=Params->LevelSim)
						return(true);
			return(false);
		}
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsCoherent(GGroup* grp,const GSubProfile* sub) const
{
	GSubProfileCursor cur=grp->GetSubProfileCursor();

	if(Params->GlobalSim)
	{
		if(Params->FullSim)
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->GlobalSimilarity(sub)<Params->LevelSim)
					return(false);
			return(true);
		}
		else
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->GlobalSimilarity(sub)>=Params->LevelSim)
					return(true);
			return(false);
		}
	}
	else
	{
		if(Params->FullSim)
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->Similarity(sub)<Params->LevelSim)
					return(false);
			return(true);
		}
		else
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->Similarity(sub)>=Params->LevelSim)
					return(true);
			return(false);
		}
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsValid(GGroup* grp)
{
	GSubProfileCursor cur=grp->GetSubProfileCursor();
	bool ndef=true;

	// At least one subprofile must be defined
	for(cur.Start();(!cur.End())&&ndef;cur.Next())
		ndef=cur()->IsDefined();
	if(ndef)
		return(false);
	return(IsCoherent(grp));
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::Run(void) throw(GException)
{
	GSubProfile* s;
	GGroup* g;

	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
	{
		s=SubProfiles();
		if(s->GetGroup()) continue;  // If already grouped -> next one.
		g=0;

		// Find the first group able to receive the subprofile.
		for(Groups->Start();!Groups->End();Groups->Next())
		{
			if(IsCoherent((*Groups)(),s))
			{
				g=(*Groups)();
				break;
			}
		}

		// If no group found create one and insert it in Groups.
		if(!g)
			g=NewGroup(Groups->GetLang());

		// Attach the subprofile to the group found/created.
		g->InsertSubProfile(s);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::~GGroupingSim(void)
{
}
