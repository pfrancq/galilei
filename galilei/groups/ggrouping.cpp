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
#include <rstd/rcursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggrouping.h>
#include<ggroups/ggroup.h>
#include<ggroups/ggroups.h>
#include<gprofiles/guser.h>
#include<gprofiles/gprofile.h>
#include<gprofiles/gsubprofile.h>
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
void GALILEI::GGrouping::Grouping(GGroupingSignalsReceiver* rec,bool modified)
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Session->GetLangs());
	GProfileCursor cur;
	GSubProfile* sub;
	GGroup* Grp;
	GGroup** Tab;
	unsigned int i;

	// Go trough each language.
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		SubProfiles.Clear();
		Groups=Session->GetGroups(CurLang());
		if(rec)
			rec->NextGroupLang(CurLang());

		// Go through the groups and delete all invalid groups.
		for(i=Groups->NbPtr+1,Tab=Groups->Tab;--i;Tab++)
		{
			Grp=(*Tab);
			if((!modified)||(((Grp->GetState()!=osUpToDate)&&(Grp->GetState()!=osUpdated))&&(!IsValid(Grp))))
			{
				Grp->DeleteSubProfiles();
				Session->DeleteGroup(Grp);
				Groups->DeletePtr(Grp);
				Tab--;
			}
		}

		// Go through the profiles corresponding to the language and that are
		// to inserted.
		cur=Session->GetProfilesCursor();
		for(cur.Start();!cur.End();cur.Next())
		{
			sub=cur()->GetSubProfile(CurLang());
			if((!sub->GetGroup())&&(sub->IsDefined()))
				SubProfiles.InsertPtr(sub);
		}

		// Make the grouping
		Run();

		// Save the information about the groupement
		for(Groups->Start();!Groups->End();Groups->Next())
			Session->Save((*Groups)());
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGrouping::~GGrouping(void)
{
}
