/*

	GALILEI Research Project

	GGroupsHistory.cpp

	History of Groups for a given language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include files for GALILEI
#include <historic/ggroupshistory.h>
#include <historic/gweightinfoshistory.h>
#include <groups/ggroups.h>
#include <profiles/gsubprofile.h>
#include <groups/gsubject.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGroupsHistoryManager
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GGroupsHistoryManager::GGroupsHistoryManager(unsigned int i)
	: RContainer<GGroupsHistory,unsigned int,true,true>(i,i/2)
{
}


//------------------------------------------------------------------------------
GGroupsHistoryCursor& GALILEI::GGroupsHistoryManager::GetGroupsHistoryCursor(void)
{
	GGroupsHistoryCursor *cur=GGroupsHistoryCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistoryManager::CheckModifiedGroups(unsigned int minGen)
{
	for (Start(); !End(); Next())
		(*this)()->CheckModifiedGroups(minGen);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistoryManager::CheckWellGroupedSubProfs(void)
{
	for (Start(); !End();Next())
	{
		(*this)()->SetGroupsSubject();
		(*this)()->CheckWellGroupedSubProfs();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistoryManager::CheckNewProfiles(void)
{
	for (Start(); !End(); Next())
		(*this)()->CheckNewProfiles();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistoryManager::InsertGroupsHistory(GGroupsHistory* gh)
{
	this->InsertPtr(gh);
	gh->SetManager(this);
}



//-----------------------------------------------------------------------------
//
//  GGroupsHistory
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GGroupsHistory::GGroupsHistory(unsigned int id)
	: RContainer<GGroupHistory,unsigned int,true,true>(20,10)
{
	Id=id;
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupsHistory::GetId(void)
{
	return Id;
}


//-----------------------------------------------------------------------------
GWeightInfosHistory* GALILEI::GGroupsHistory::GetSubProfile(unsigned int id)
{
	GGroupHistory* grp;

	for (Start(); !End(); Next())
	{
		grp=(*this)();
		for (grp->Start(); !grp->End(); grp->Next())
		 	if ((*grp)()->GetId()==id)
		  		return((*grp)());
	}
	return(0);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(const GGroupsHistory& groups) const
{
	return(Id-groups.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(const GGroupsHistory* groups) const
{
	return(Id-groups->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsHistory::Compare(unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistory::CheckModifiedGroups(unsigned int minGen)
{
	RContainer<GWeightInfosHistory, unsigned int, false, true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp, *lastgroup;
	GWeightInfosHistory* sub, *lastsub;
	unsigned int  lastcurid;

	// if the grps is the frst historic one, return
	if (this->GetId()==minGen) return;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory, unsigned int, false, true>(10,5);
	lastgroups=Manager->GetPtr(this->GetId()-1);
	if (!lastgroups) return;
	for (lastgroups->Start(); !lastgroups->End(); lastgroups->Next())
		for ((*lastgroups)()->Start(); !(*lastgroups)()->End();(*lastgroups)()->Next())
			lastsubs->InsertPtr((*(*lastgroups)())());

	// check the actual groups
	for (Start(); !End(); Next())
	{
		grp=(*this)();
		grp->Start();
		sub=(*grp)();

		//get the equivalent last subprofile
		lastsub=lastsubs->GetPtr(sub->GetId());
		//get the equivalent last group.
		lastgroup=lastgroups->GetPtr(lastsub->GetParent());
		//if the number of pointers is different, the group is modified
		if (lastgroup->NbPtr!=grp->NbPtr)
		{
			grp->SetModified(true);
			continue;
		}

		//if a couple of subprofiles doesn't belong to a same group in the current and historic groups
		// then the group is modified.
		lastcurid=lastsub->GetParent()->GetId();
		while(grp->GetModified()==false&&!grp->End())
		{
			lastsub=lastsubs->GetPtr((*grp)()->GetId());
			// if the subprofiles is a new one, group is modified
			if (!lastsub) grp->SetModified(true);

			//if the lastcurid has changed , group is modified
			if(lastcurid!=lastsub->GetParent()->GetId())
				grp->SetModified(true);
			grp->Next();
		}
	}
	delete lastsubs;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistory::SetGroupsSubject(void)
{
	GGroupHistory* grp;
	GSubject* mainsubject;
	unsigned int occur, maxoccur,knownsubject;
	R::RContainer<GSubject, unsigned int, false, true>* subjects;


	//find the dominant subject
	for (Start(); !End(); Next())
	{
		grp=(*this)();
		maxoccur=knownsubject=0;

		// find all the subjects contained in the group.
		subjects=new R::RContainer<GSubject, unsigned int, false, true>(5,2);
		for (grp->Start(); !grp->End(); grp->Next())
		{
			subjects->InsertPtr((*grp)()->GetSubProfile()->GetSubject());
		}

		// find the most dominant one
		subjects->Start();
		while((maxoccur<(grp->NbPtr-knownsubject))&&!subjects->End())
		{
			occur=0;
			for (grp->Start(); !grp->End(); grp->Next())
			{
				if ((*grp)()->GetSubProfile()->GetSubject()->GetId()==(*subjects)()->GetId())
					occur++;
			}
			knownsubject+=occur;
			if(occur>maxoccur)
			{
				maxoccur=occur;
				mainsubject=(*subjects)();
			}
			subjects->Next();
		}
		grp->SetSubject(mainsubject);
		delete subjects;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistory::CheckWellGroupedSubProfs(void)
{
	GGroupHistory* grp;
	for (Start(); !End(); Next())
	{
		grp=(*this)();
		for (grp->Start(); !grp->End(); grp->Next())
			if((*grp)()->GetSubProfile()->GetSubject()->GetId()==grp->GetSubject()->GetId())
				(*grp)()->SetWellGrouped(true);
			else
				(*grp)()->SetWellGrouped(false);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistory::CheckNewProfiles(void)
{
	R::RContainer<GWeightInfosHistory, unsigned int, false, true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory, unsigned int, false, true>(10,5);
	lastgroups=Manager->GetPtr(this->GetId()-1);
	if (!lastgroups) return;
	for (lastgroups->Start(); !lastgroups->End(); lastgroups->Next())
		for ((*lastgroups)()->Start(); !(*lastgroups)()->End();(*lastgroups)()->Next())
			lastsubs->InsertPtr((*(*lastgroups)())());

	//check wether each subprofile was in the last groups
	//if not, the profiles is considered as new.
	for  (Start(); !End(); Next())
	{
		grp=(*this)();
		for (grp->Start(); !grp->End(); grp->Next())
			if (!lastsubs->GetPtr((*grp)()))
				(*grp)()->SetNewSubProfile(true);
	}

	delete lastsubs;
}


//-----------------------------------------------------------------------------
GALILEI::GGroupsHistory::~GGroupsHistory(void)
{
}


