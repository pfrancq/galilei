/*

	GALILEI Research Project

	GGroupingSim.cpp

	Heuristic using Similarity - Implementation

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroupingsim.h>
#include<gprofiles/gsubprofile.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupingSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::GGroupingSim(GSession* s,const bool f,const double l) throw(bad_alloc)
	: GGrouping(s), Level(l), Full(f)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsCoherent(const GGroup* grp) const
{
	GSubProfileRef** s1;
	GSubProfileRef** s2;
	unsigned int i,j;

	if(Full)
	{
		for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
			for(s2=s1+1,j=i+1;--j;s2++)
				if((*s1)->GetSubProfile()->Similarity((*s2)->GetSubProfile())<Level)
					return(false);
		return(true);
	}
	else
	{
		for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
			for(s2=s1+1,j=i+1;--j;s2++)
				if((*s1)->GetSubProfile()->Similarity((*s2)->GetSubProfile())>=Level)
					return(true);
		return(false);
	}
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSim::IsCoherent(const GGroup* grp,const GSubProfile* sub) const
{
	RContainerCursor<GSubProfileRef,unsigned int,true,true> cur(grp);

	if(Full)
	{
		for(cur.Start();!cur.End();cur.Next())
			if(cur()->GetSubProfile()->Similarity(sub)<Level)
				return(false);
		return(true);
	}
	else
	{
		for(cur.Start();!cur.End();cur.Next())
			if(cur()->GetSubProfile()->Similarity(sub)>=Level)
				return(true);
		return(false);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSim::Run(void)
{
	GSubProfile* s;
	GGroup* g;

	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
	{
		s=SubProfiles();
		g=Groups->GetGroup(s);

		// Verify if the subprofile is already attached
		if(g)
		{
			// If group is still coherent -> leave it
			if(IsCoherent(g))
				continue;
			
			// Sub Profile must be deleted from the group
			g->DeleteSubProfile(g->GetPtr<GSubProfile*>(s));
		}

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
			Groups->InsertPtr(g=Session->NewGroup(Groups->GetLang()));

		// Attach the subprofile to the group found/created.
		g->InsertSubProfile(new GSubProfileRef(s));
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingSim::~GGroupingSim(void)
{
}
