/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Implementation

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggrouping.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



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
void GALILEI::GGrouping::Grouping(void)
{
	GLang* l;
	GUser* u;

	// Go trough each language.
	for(Session->Langs->Start();!Session->Langs->End();Session->Langs->Next())
	{
		l=(*Session->Langs)();
		SubProfiles.Clear();
		Groups=Session->GetGroups(l);

		// Go through the users to find the subprofiles corresponding to the
		// language.
		for(Session->Users->Start();!Session->Users->End();Session->Users->Next())
		{
			u=(*Session->Users)();
			// Go through each profile.
			for(u->Start();!u->End();u->Next())
				SubProfiles.InsertPtr((*u)()->GetSubProfile(l));
		}

		// Make the grouping
		Run();
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGrouping::~GGrouping(void)
{
}
