/*

	GALILEI Research Project

	GGrouping.cpp

	Generic Grouping Method - Implementation

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
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofiles.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// defines
//#define GROUP_SUBPROFILES_NOT_DEFINED



//-----------------------------------------------------------------------------
//
//  GGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGrouping::GGrouping(GFactoryGrouping* fac) throw(bad_alloc)
	: GPlugin<GFactoryGrouping>(fac), Lang(0), Groups(0), SubProfiles(100,50),
	  DeletedGroups(200,100), IdealGroups(0)
{
}

//-----------------------------------------------------------------------------
void GGrouping::Connect(GSession* session)
{
	GLangCursor Langs;

	Session=session;
	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
		DeletedGroups.InsertPtr(new GGroups(Langs()));
	IdealGroups=0;
}


//-----------------------------------------------------------------------------
void GGrouping::Disconnect(GSession*)
{
	Session=0;
	DeletedGroups.Clear();
	IdealGroups=0;
}


//-----------------------------------------------------------------------------
void GGrouping::Init(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
void GGrouping::Clear(void) throw(bad_alloc)
{
	unsigned int i;

	// Go through the groups and delete all invalid groups.
	for(i=Groups->NbPtr+1;--i;)
	{
		DeleteGroup(*Groups->Tab);
	}
}


//-----------------------------------------------------------------------------
GGroup* GGrouping::NewGroup(GLang* lang)
{
	GGroup* grp;

	grp=new GGroupVector(cNoRef,lang);
	if(SaveGroups)
		Session->NewGroup(lang,grp);
	else
		grp->SetId(Groups->Tab[Groups->NbPtr-1]->GetId()+1);
	Groups->InsertPtr(grp);
	return(grp);
}


//-----------------------------------------------------------------------------
void GGrouping::DeleteGroup(GGroup* grp)
{
	grp->DeleteSubProfiles();
	if(SaveGroups)
		Session->DeleteGroup(grp);
	Groups->DeletePtr(grp);
}


//-----------------------------------------------------------------------------
void GGrouping::Grouping(GSlot* rec,bool modified,bool /*save*/)
{
	RCursor<GLang,unsigned int> CurLang(Session->GetLangs());
	GGroupCalc* CalcDesc;
	GSubProfileCursor cur;
	GGroup* Grp;
	GGroup** Tab;
	unsigned int i;

	SaveGroups=/*save*/true;
	Modified=modified;

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
		cur=Session->GetSubProfilesCursor(CurLang());
		for(cur.Start();!cur.End();cur.Next())
		{
			#ifdef GROUP_SUBPROFILES_NOT_DEFINED
			if(cur()->IsDefined()||cur()->GetProfile()->GetNbJudgedDocs(CurLang()))
				SubProfiles.InsertPtr(cur());
			#else
			if(cur()->IsDefined())
				SubProfiles.InsertPtr(cur());
			#endif
		}

		// Make the grouping
		Run();

		// Compute the description of the groups and Save the information.
		CalcDesc=Session->GetCurrentGroupCalcMethod();
		for(Groups->Start();!Groups->End();Groups->Next())
			CalcDesc->Compute((*Groups)());
	}
	Lang=0;
	if(SaveGroups)
		Session->SaveGroups();
}


//-----------------------------------------------------------------------------
GGrouping::~GGrouping(void)
{
}
