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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingsim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupingSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::GGroupingSim(GSession* s) throw(bad_alloc)
	: GGrouping("First-Fit Heuristic",s), FullSim(true), LevelSim(0.3),
	  GlobalSim(false)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::GGroupingSim(const char* n,GSession* s) throw(bad_alloc)
	: GGrouping(n,s), FullSim(true), LevelSim(0.3),
	  GlobalSim(false)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingSim::GetSettings(void)
{
	static char tmp[100];
	char c,c1;

	if(FullSim) c='1'; else c='0';
	if(GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%c %lf %c",c,LevelSim,c1);
	return(tmp);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::SetSettings(const char* s)
{
	char c,c1;

	if(!(*s)) return;
	sscanf(s,"%c %lf %c",&c,&LevelSim,&c1);
	if(c=='1') FullSim=true; else FullSim=false;
	if(c1=='1') GlobalSim=true; else GlobalSim=false;
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

	if(GlobalSim)
	{
		if(FullSim)
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->GlobalSimilarity(*s2)<LevelSim)
						return(false);
			return(true);
		}
		else
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->GlobalSimilarity(*s2)>=LevelSim)
						return(true);
			return(false);
		}
	}
	else
	{
		if(FullSim)
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->Similarity(*s2)<LevelSim)
						return(false);
			return(true);
		}
		else
		{
			for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
				for(s2=s1+1,j=i+1;--j;s2++)
					if((*s1)->Similarity(*s2)>=LevelSim)
						return(true);
			return(false);
		}
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsCoherent(const GGroup* grp,const GSubProfile* sub) const
{
	RContainerCursor<GSubProfile,unsigned int,false,true> cur(grp);

	if(GlobalSim)
	{
		if(FullSim)
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->GlobalSimilarity(sub)<LevelSim)
					return(false);
			return(true);
		}
		else
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->GlobalSimilarity(sub)>=LevelSim)
					return(true);
			return(false);
		}
	}
	else
	{
		if(FullSim)
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->Similarity(sub)<LevelSim)
					return(false);
			return(true);
		}
		else
		{
			for(cur.Start();!cur.End();cur.Next())
				if(cur()->Similarity(sub)>=LevelSim)
					return(true);
			return(false);
		}
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsValid(GGroup* grp)
{
	RContainerCursor<GSubProfile,unsigned int,false,true> cur(grp);
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
