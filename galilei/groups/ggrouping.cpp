/*

	GALILEI Research Project

	GGrouping.cpp

	Generic Grouping Method - Implementation

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <groups/ggroupcalc.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <sessions/gslot.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// defines
//#define GROUP_SUBPROFILES_NOT_DEFINED



//------------------------------------------------------------------------------
//
//  GGrouping
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGrouping::GGrouping(GFactoryGrouping* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryGrouping>(fac), Lang(0), Groups(0), SubProfiles(100,50),
	  IdealGroups(0)
{
}


//------------------------------------------------------------------------------
void GGrouping::Connect(GSession* session) throw(GException)
{
	Session=session;
	Groups=session;
	IdealGroups=0;
}


//------------------------------------------------------------------------------
void GGrouping::Disconnect(GSession*) throw(GException)
{
	Session=0;
	Groups=0;
	IdealGroups=0;
}


//-----------------------------------------------------------------------------
void GGrouping::Grouping(GSlot* rec,bool modified,bool save, bool savehistory) throw(GException)
{
	GFactoryLangCursor CurLang;
	GGroupCalc* CalcDesc;
	GSubProfileCursor cur;
	GGroupCursor Groups;

	Modified=modified;

	// Go trough each language.
	CurLang=Session->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		Lang=CurLang()->GetPlugin();
		if(!Lang) continue;

		SubProfiles.Clear();
		if(rec)
			rec->NextGroupLang(Lang);

		// Go through the profiles corresponding to the language and that are
		// to inserted.
		cur=Session->GetSubProfilesCursor(Lang);
		for(cur.Start();!cur.End();cur.Next())
		{
			#ifdef GROUP_SUBPROFILES_NOT_DEFINED
			if(cur()->IsDefined()||cur()->GetProfile()->GetNbJudgedDocs(Lang))
				SubProfiles.InsertPtr(cur());
			#else
			if(cur()->IsDefined())
				SubProfiles.InsertPtr(cur());
			#endif
		}

		// Make the grouping
		Run();
	}
	Lang=0;

	// Compute the description of the groups and Save the information.
	CalcDesc=Session->GetGroupCalcMng()->GetCurrentMethod();
	Groups=Session->GetGroupsCursor();
	for(Groups.Start();!Groups.End();Groups.Next())
		CalcDesc->Compute(Groups());

	if(save)
		Session->GetStorage()->SaveGroups(Session);
	if(savehistory)
		Session->GetStorage()->SaveGroupsHistory(Session);

}


//------------------------------------------------------------------------------
GGrouping::~GGrouping(void)
{
}
