/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Implementation

	(C) 2001 by P. Francq.

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
#include <rstd/rcursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggrouping.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <sessions/gsession.h>
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
void GALILEI::GGrouping::Grouping(tSubProfileDesc t,GGroupingSignalsReceiver* rec,bool modified)
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Session->GetLangs());
	GProfileCursor cur;
	GSubProfile* sub;
	GGroup* Grp;
	GGroup** Tab;
	unsigned int i;

	// Description
	SubProfileDesc=t;

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
			if((!sub->GetGroup())&&(sub->GetPtr<const tSubProfileDesc>(SubProfileDesc)->IsDefined()))
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
