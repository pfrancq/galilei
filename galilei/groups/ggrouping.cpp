/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Implementation

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
#include<ggroups/ggrouping.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GGroupingSignalsReceiver
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingSignalsReceiver::GGroupingSignalsReceiver(void)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSignalsReceiver::NextGroupLang(GLang*)
{
}



//-----------------------------------------------------------------------------
//
//  GGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGrouping::GGrouping(GSession* s) throw(bad_alloc)
	: Session(s), Groups(0), SubProfiles(s->GetNbUsers(),s->GetNbUsers()/2)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::Grouping(GGroupingSignalsReceiver* rec)
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Session->GetLangs());

	// Go trough each language.
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		SubProfiles.Clear();
		Groups=Session->GetGroups(CurLang());
		if(rec)
			rec->NextGroupLang(CurLang());

		// Go through the users to find the subprofiles corresponding to the
		// language.
		RContainerCursor<GUser,unsigned int,true,true> CurUsr(Session->GetUsers());
		for(CurUsr.Start();!CurUsr.End();CurUsr.Next())
		{
			// Go through each profile.
			for(CurUsr()->Start();!CurUsr()->End();CurUsr()->Next())
				SubProfiles.InsertPtr((*CurUsr())()->GetSubProfile(CurLang()));
		}

		// Make the grouping
		Run();
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGrouping::~GGrouping(void)
{
}
