/*

	GALILEI Research Project

	GCommunitysHistory.cpp

	History of Groups for a given language - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
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
#include<rdate.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gcommunitieshistory.h>
#include <gweightinfoshistory.h>
#include <gsubject.h>
#include <gsession.h>
#include <gsubjects.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GCommunitiesHistoryManager
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GCommunitiesHistoryManager::GCommunitiesHistoryManager(GSession* session,unsigned int max)
	: RContainer<GCommunitiesHistory,true,true>(max,max/2), Session(session)
{
}


//-------------------------------------------------------------------------------
R::RCursor<GCommunitiesHistory> GCommunitiesHistoryManager::GetCommunitiesHistory(void)
{
	R::RCursor<GCommunitiesHistory> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
void GCommunitiesHistoryManager::CheckModifiedCommunities(unsigned int minGen)
{
	R::RCursor<GCommunitiesHistory> Cur(*this);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->CheckModifiedCommunities(minGen);
}


//------------------------------------------------------------------------------
void GCommunitiesHistoryManager::CheckWellGroupedProfs(void)
{
	R::RCursor<GCommunitiesHistory> Cur(*this);

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SetCommunitiesSubject();
		Cur()->CheckWellGroupedProfs();
	}
}


//------------------------------------------------------------------------------
void GCommunitiesHistoryManager::CheckNewProfiles(void)
{
	R::RCursor<GCommunitiesHistory> Cur(*this);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->CheckNewProfiles();
}


//-----------------------------------------------------------------------------
void GALILEI::GCommunitiesHistoryManager::CreateCommunitiesRelationship(unsigned int maxgen)
{
	unsigned int  i, maxoccurs, nbchildren;
	GCommunitiesHistory *nextgrps;
	GWeightInfosHistory* subprof;
	bool treated;
	unsigned int** tab;
	R::RCursor<GCommunitiesHistory> Cur(*this);

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If the groups is the last one, no child
		if(Cur()->GetId()==maxgen) continue;

		nextgrps=GetPtr(Cur()->GetId()+1);
		if(!nextgrps)
			return;

		RCursor<GCommunityHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			nbchildren=0;

			// Initialize the table od groupid / nboccurs
			tab= new unsigned int*[Cur2()->GetNb()];
			for (i=0; i<Cur2()->GetNb(); i++)
				tab[i]=new unsigned int [2];
			for (i=0; i<Cur2()->GetNb(); i++)
				tab[i][0]=tab[i][1]=0;

			// Fill the table
			RCursor<GWeightInfosHistory> Cur3(*Cur2());
			for(Cur3.Start();!Cur3.End();Cur3.Next())
			{
				subprof=nextgrps->GetProfile(Cur3()->GetId());
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

			// Analyse table;
			maxoccurs=0;
			for(i=0;i<Cur2()->GetNb();i++)
				if (tab[i][1]>maxoccurs)
					maxoccurs=tab[i][1];

			// Find the childrens
			for(i=0;i<Cur2()->GetNb();i++)
				if (tab[i][1]==maxoccurs)
					Cur2()->InsertChildren(nextgrps->GetPtr(tab[i][0]));

			// Delete the table
			for(i=0;i<Cur2()->GetNb();i++)
				delete [] tab[i];
			delete [] tab;
		}
	}
}


//------------------------------------------------------------------------------
void GCommunitiesHistoryManager::InsertCommunitiesHistory(GCommunitiesHistory* gh)
{
	this->InsertPtr(gh);
	gh->SetManager(this);
}


//------------------------------------------------------------------------------
GSession* GCommunitiesHistoryManager::GetSession(void) const
{
	return(Session);
}

//------------------------------------------------------------------------------
GCommunitiesHistoryManager::~GCommunitiesHistoryManager(void)
{
}



//------------------------------------------------------------------------------
//
//  GCommunitiesHistory
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GCommunitiesHistory::GCommunitiesHistory(unsigned int id, RString date)
	: RContainer<GCommunityHistory,true,true>(20,10)
{
	Id=id;
	Date=RDate(date.Latin1());
}


//------------------------------------------------------------------------------
R::RDate GCommunitiesHistory::GetDate(void) const
{
	return(Date);
}


//------------------------------------------------------------------------------
GWeightInfosHistory* GCommunitiesHistory::GetProfile(unsigned int id)
{
	RCursor<GCommunityHistory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur.Next())
			if(Cur2()->GetId()==id)
				return(Cur2());
	}
	return(0);
}


//------------------------------------------------------------------------------
int GCommunitiesHistory::Compare(const GCommunitiesHistory& groups) const
{
	return(Id-groups.Id);
}


//------------------------------------------------------------------------------
int GCommunitiesHistory::Compare(const GCommunitiesHistory* groups) const
{
	return(Id-groups->Id);
}


//------------------------------------------------------------------------------
int GCommunitiesHistory::Compare(unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
void GCommunitiesHistory::SetManager(GCommunitiesHistoryManager* m)
{
	Manager=m;
}


//------------------------------------------------------------------------------
void GCommunitiesHistory::CheckModifiedCommunities(unsigned int minGen)
{
	RContainer<GWeightInfosHistory,false,true>* lastsubs;
	GCommunitiesHistory* lastgroups;
	GCommunityHistory* lastgroup;
	GWeightInfosHistory* sub, *lastsub;
	unsigned int  lastcurid;

	// if the grps is the frst historic one, return
	if (this->GetId()==minGen) return;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory,false,true>(10,5);
	lastgroups=Manager->GetPtr(this->GetId()-1);
	if (!lastgroups) return;
	RCursor<GCommunityHistory> Cur(*lastgroups);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			lastsubs->InsertPtr(Cur2());
	}

	// Check the actual groups
	RCursor<GCommunityHistory> Cur2(*this);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
	{
		RCursor<GWeightInfosHistory> Cur3(*Cur2());
		Cur3.Start();
		sub=Cur3();

		//get the equivalent last subprofile
		lastsub=lastsubs->GetPtr(sub->GetId());
		//if none has been found -> the subprofile is new
		if (!lastsub)
		{
			Cur2()->SetModified(true);
			continue;
		}
		//get the equivalent last group.
		lastgroup=lastgroups->GetPtr(lastsub->GetParent());
		//if the number of pointers is different, the group is modified
		if (lastgroup->GetNb()!=Cur2()->GetNb())
		{
			Cur2()->SetModified(true);
			continue;
		}

		//if a couple of subprofiles doesn't belong to a same group in the current and historic groups
		// then the group is modified.
		lastcurid=lastsub->GetParent()->GetId();
		while((Cur2()->IsModified()==false)&&(!Cur2.End()))
		{
			lastsub=lastsubs->GetPtr(Cur2()->GetId());
			// if the subprofiles is a new one, group is modified
			if (!lastsub)
				Cur2()->SetModified(true);

			//if the lastcurid has changed , group is modified
			if(lastcurid!=lastsub->GetParent()->GetId())
				Cur2()->SetModified(true);
			Cur2.Next();
		}
	}

	delete lastsubs;
}



//------------------------------------------------------------------------------
void GCommunitiesHistory::SetCommunitiesSubject(void)
{
	GCommunityHistory* grp;
	GSubject* mainsubject;
	unsigned int occur, maxoccur,knownsubject;
	R::RContainer<GSubject,false,true>* subjects;


	//find the dominant subject
	RCursor<GCommunityHistory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		grp=Cur();
		maxoccur=knownsubject=0;

		// find all the subjects contained in the group.
		subjects=new R::RContainer<GSubject,false,true>(5,2);
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			subjects->InsertPtr(Manager->GetSession()->GetSubjects()->GetSubject(Cur2()->GetProfile()));
		}

		// find the most dominant one
		RCursor<GSubject> Cur3(*subjects);
		Cur3.Start();
		while((maxoccur<(grp->GetNb()-knownsubject))&&!Cur2.End())
		{
			occur=0;
			Cur2.Set(*grp);
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if(Manager->GetSession()->GetSubjects()->GetSubject(Cur2()->GetProfile())->GetId()==Cur3()->GetId())
					occur++;
			}
			knownsubject+=occur;
			if(occur>maxoccur)
			{
				maxoccur=occur;
				mainsubject=Cur3();
			}
			Cur3.Next();
		}
		grp->SetSubject(mainsubject);
		delete subjects;
	}
}


//------------------------------------------------------------------------------
void GCommunitiesHistory::CheckWellGroupedProfs(void)
{
	RCursor<GCommunityHistory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			if(Manager->GetSession()->GetSubjects()->GetSubject(Cur2()->GetProfile())->GetId()==Cur()->GetSubject()->GetId())
				Cur2()->SetWellGrouped(true);
			else
				Cur2()->SetWellGrouped(false);
	}
}


//------------------------------------------------------------------------------
void GCommunitiesHistory::CheckNewProfiles(void)
{
	R::RContainer<GWeightInfosHistory,false,true>* lastsubs;
	GCommunitiesHistory* lastgroups;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GWeightInfosHistory,false,true>(10,5);
	lastgroups=Manager->GetPtr(this->GetId()-1);
	if (!lastgroups) return;
	RCursor<GCommunityHistory> Groups(*lastgroups);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		RCursor<GWeightInfosHistory> Cur2(*Groups());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			lastsubs->InsertPtr(Cur2());
	}

	//check wether each subprofile was in the last groups
	//if not, the profiles is considered as new.
	RCursor<GCommunityHistory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			if(!lastsubs->GetPtr(Cur2()))
				Cur2()->SetNewProfile(true);
	}

	delete lastsubs;
}


//------------------------------------------------------------------------------
GCommunitiesHistory::~GCommunitiesHistory(void)
{
}


