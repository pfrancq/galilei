/*

	GALILEI Research Project

	GGroupsHistory.cpp

	History of Groups for a given language - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include files for R
#include<rstd/rdate.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <historic/ggroupshistory.h>
#include <historic/gweightinfoshistory.h>
#include <groups/ggroups.h>
#include <profiles/gsubprofile.h>
#include <groups/gsubject.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GGroupsHistoryManager
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GGroupsHistoryManager::GGroupsHistoryManager(unsigned int max) throw(std::bad_alloc)
	: RContainer<GGroupsHistory,true,true>(max,max/2)
{
}


//-------------------------------------------------------------------------------
GGroupsHistoryCursor GGroupsHistoryManager::GetGroupsHistoryCursor(void)
{
	GGroupsHistoryCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GGroupsHistoryManager::CheckModifiedGroups(unsigned int minGen) throw(std::bad_alloc)
{
	GGroupsHistoryCursor Cur(this);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->CheckModifiedGroups(minGen);
}


//------------------------------------------------------------------------------
void GGroupsHistoryManager::CheckWellGroupedSubProfs(void) throw(std::bad_alloc)
{
	GGroupsHistoryCursor Cur(this);

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SetGroupsSubject();
		Cur()->CheckWellGroupedSubProfs();
	}
}


//------------------------------------------------------------------------------
void GGroupsHistoryManager::CheckNewProfiles(void) throw(std::bad_alloc)
{
	GGroupsHistoryCursor Cur(this);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->CheckNewProfiles();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsHistoryManager::CreateGroupsRelationship(unsigned int maxgen) throw(std::bad_alloc)
{
	unsigned int  i, maxoccurs, nbchildren;
	GGroupsHistory* curgrps, *nextgrps;
	GGroupHistory* grp;
	GWeightInfosHistory* subprof;
	bool treated;
	unsigned int** tab;
	GGroupsHistoryCursor Cur(this);

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If the groups is the last one, no child
		if(Cur()->GetId()==maxgen) continue;

		curgrps=Cur();
		nextgrps=GetPtr(curgrps->GetId()+1);
		if (!nextgrps)
			return;

		for (curgrps->Start(); !curgrps->End(); curgrps->Next())
		{
			grp=(*curgrps)();
			nbchildren=0;
			//initialize the table od groupid / nboccurs
			tab= new unsigned int* [grp->NbPtr];
			for (i=0; i<grp->NbPtr; i++)
				tab[i]=new unsigned int [2];
			for (i=0; i<grp->NbPtr; i++)
				tab[i][0]=tab[i][1]=0;

			//fill the table
			for (grp->Start(); !grp->End(); grp->Next())
			{
				subprof=nextgrps->GetSubProfile((*grp)()->GetId());
				treated=false;
				i=0;
				while(!treated&&i<nbchildren)
				{
					if (tab[i][0]==subprof->GetParent()->GetId())
					{
						tab[i][1]++;
						treated=true;
					}
					i++;
				}
				if(!treated)
				{
					tab[i][0]=subprof->GetParent()->GetId();
					tab[i][1]++;
					nbchildren++;
				}
			}

			//analyse table;
			maxoccurs=0;
			for (i=0; i<grp->NbPtr;i++)
				if (tab[i][1]>maxoccurs)
					maxoccurs=tab[i][1];
			//and find the childrens
			for (i=0; i<grp->NbPtr;i++)
				if (tab[i][1]==maxoccurs)
					grp->InsertChildren(nextgrps->GetPtr(tab[i][0]));
			//delete the table
			for (i=0; i<grp->NbPtr; i++)
				delete [] tab[i];
			delete [] tab;
		}
	}
}


//------------------------------------------------------------------------------
void GGroupsHistoryManager::InsertGroupsHistory(GGroupsHistory* gh) throw(std::bad_alloc)
{
	this->InsertPtr(gh);
	gh->SetManager(this);
}


//------------------------------------------------------------------------------
GGroupsHistoryManager::~GGroupsHistoryManager(void)
{
}



//------------------------------------------------------------------------------
//
//  GGroupsHistory
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GGroupsHistory::GGroupsHistory(unsigned int id, RString date) throw(std::bad_alloc)
	: RContainer<GGroupHistory,true,true>(20,10)
{
	Id=id;
	Date=RDate(date.Latin1());
}


//------------------------------------------------------------------------------
R::RDate GGroupsHistory::GetDate(void) const
{
	return(Date);
}


//------------------------------------------------------------------------------
GWeightInfosHistory* GGroupsHistory::GetSubProfile(unsigned int id) throw(GException)
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


//------------------------------------------------------------------------------
int GGroupsHistory::Compare(const GGroupsHistory& groups) const
{
	return(Id-groups.Id);
}


//------------------------------------------------------------------------------
int GGroupsHistory::Compare(const GGroupsHistory* groups) const
{
	return(Id-groups->Id);
}


//------------------------------------------------------------------------------
int GGroupsHistory::Compare(unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
void GGroupsHistory::SetManager(GGroupsHistoryManager* m)
{
	Manager=m;
}


//------------------------------------------------------------------------------
void GGroupsHistory::CheckModifiedGroups(unsigned int minGen) throw(std::bad_alloc)
{
	RContainer<GWeightInfosHistory,false,true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp, *lastgroup;
	GWeightInfosHistory* sub, *lastsub;
	unsigned int  lastcurid;

	// if the grps is the frst historic one, return
	if (this->GetId()==minGen) return;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory,false,true>(10,5);
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
		//if none has been found -> the subprofile is new
		if (!lastsub)
		{
			grp->SetModified(true);
			continue;
		}
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
		while(grp->IsModified()==false&&!grp->End())
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



//------------------------------------------------------------------------------
void GGroupsHistory::SetGroupsSubject(void) throw(std::bad_alloc)
{
	GGroupHistory* grp;
	GSubject* mainsubject;
	unsigned int occur, maxoccur,knownsubject;
	R::RContainer<GSubject,false,true>* subjects;


	//find the dominant subject
	for (Start(); !End(); Next())
	{
		grp=(*this)();
		maxoccur=knownsubject=0;

		// find all the subjects contained in the group.
		subjects=new R::RContainer<GSubject,false,true>(5,2);
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


//------------------------------------------------------------------------------
void GGroupsHistory::CheckWellGroupedSubProfs(void) throw(std::bad_alloc)
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


//------------------------------------------------------------------------------
void GGroupsHistory::CheckNewProfiles(void) throw(std::bad_alloc)
{
	R::RContainer<GWeightInfosHistory,false,true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory,false,true>(10,5);
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


//------------------------------------------------------------------------------
GGroupsHistory::~GGroupsHistory(void)
{
}


