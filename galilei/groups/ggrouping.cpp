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
#include <langs/glang.h>
#include <groups/ggrouping.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGrouping::GGrouping(const char* n,GSession* s) throw(bad_alloc)
	: GroupingName(n), Session(s), Lang(0), Groups(0), SubProfiles(s->GetNbUsers(),s->GetNbUsers()/2),
	  DeletedGroups(200,100)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
		DeletedGroups.InsertPtr(new GGroups(Langs()));
}


//-----------------------------------------------------------------------------
int GALILEI::GGrouping::Compare(const GGrouping* grp)
{
	return(GroupingName.Compare(grp->GroupingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGrouping::Compare(const GGrouping& grp)
{
	return(GroupingName.Compare(grp.GroupingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGrouping::Compare(const char* name)
{
	return(GroupingName.Compare(name));
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::Clear(void) throw(bad_alloc)
{
	unsigned int i;

	// Go through the groups and delete all invalid groups.
	for(i=Groups->NbPtr+1;--i;)
	{
		DeleteGroup(*Groups->Tab);
	}
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGrouping::NewGroup(GLang* lang)
{
	return(Session->NewGroup(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::DeleteGroup(GGroup* grp)
{
	grp->DeleteSubProfiles();
	Groups->DeletePtr(grp);
	Session->DeleteGroup(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGrouping::Grouping(GSlot* rec,bool modified)
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
		Lang=CurLang();
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
				DeleteGroup(Grp);
				Tab--;
			}
		}

		// Go through the profiles corresponding to the language and that are
		// to inserted.
		cur=Session->GetProfilesCursor();
		for(cur.Start();!cur.End();cur.Next())
		{
			sub=cur()->GetSubProfile(CurLang());
			if(sub->IsDefined())
				SubProfiles.InsertPtr(sub);
		}

		// Make the grouping
		Run();

		// Save the information about the groupement
		for(Groups->Start();!Groups->End();Groups->Next())
			Session->Save((*Groups)());
	}
	Lang=0;
}


//-----------------------------------------------------------------------------
GALILEI::GGrouping::~GGrouping(void)
{
}
