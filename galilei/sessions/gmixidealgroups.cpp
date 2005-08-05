/*

	GALILEI Research Project

	GMixIdealGroups.cpp

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R
#include <random.h>
#include <rcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gmixidealgroups.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <glang.h>
#include <ggroups.h>
#include <ggroup.h>
#include <gsubject.h>
#include <gsubprofile.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GMixIdealGroups
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
GALILEI::GMixIdealGroups::GMixIdealGroups(GSession* sess,GGroups* idealgroups, int nbgroups, int level, bool m, bool s, bool r, bool i)
	:  IdealGroups(idealgroups), CurrentLang(0), Tab(0),  TabS(0)
{
	//init parameters
	Random=r;
	Ideal=i;
	Merge=m;
	Split=s;
	Session=sess;
	Level=level;
	NbMixedGroups=nbgroups;
	Rand=new RRandomGood(0);
	Rand->Reset(10);
	Tab=new GGroup*[Session->GetProfilesNb()];
	TabS=new GSubProfile*[Session->GetProfilesNb()];
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::SetSettings(const char* s)
{
	char c, d, f;

	if(!(*s)) return;
	sscanf(s,"%u %u %c %c %c",&NbMixedGroups, &Level, &c, &d, &f);
	if(c=='1') Random=true; else Random=false;
	if(d=='1') Merge=true; else Merge=false;
	if(f=='1') Split=true; else Split=false;
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GMixIdealGroups::GetIdealGroup(GSubProfile* s)
{
	return(IdealGroups->GetGroup(s));
}


//-----------------------------------------------------------------------------
bool GALILEI::GMixIdealGroups::MergeGroups(void)
{
	GGroup* g;
	RCursor<GSubProfile> Cur;

	// At least two groups must be presented
	if(MixedGroups->GetNbGroups(CurrentLang)<2) return(false);

	// First group is the first of Tab
	RandOrderTab();
	g=Tab[0];

	// Merge the groups (Put elements of g2 in g1 and delete g2).
	Cur=Tab[1]->GetSubProfiles();
	for(Cur.Start();!Cur.End();Cur.Next())
		g->InsertPtr(Cur());
	MixedGroups->DeleteGroup(Tab[1]);

	return(true);
}


//-----------------------------------------------------------------------------
bool GALILEI::GMixIdealGroups::SplitGroups(void)
{
	unsigned int r1, i;
	GGroup* g1;
	GGroup* g2;
	GGroup** ptr;
	GSubProfile** s;
	R::RCursor<GGroup> grpscur;
	RCursor<GSubProfile> subcur;

	// find  a group to split with at least two subprofiles
	RandOrderTab();
	grpscur=MixedGroups->GetGroups(CurrentLang);
	for(i=grpscur.GetNb()+1,g1=0,ptr=Tab;(--i)&&(!g1);ptr++)
		if((*ptr)->GetNbSubProfiles()>1)
			g1=(*ptr);

	if(!g1) return(false);

	// Copy the subprofiles of the group in TabS and reorder them randomly
	subcur=g1->GetSubProfiles();
	for (i=0, subcur.Start(); !subcur.End(); subcur.Next(),i++)
		TabS[i]=subcur();
	Rand->RandOrder<GSubProfile*>(TabS,g1->GetNbSubProfiles());

	// choose a place to split and put the first elements in g2

	g2=new GGroup(MixedGroups->GetNewId(), g1->GetLang(), false,RDate(""),RDate(""));
	r1=Rand->Value(g1->GetNbSubProfiles());
	for(s=TabS,i=r1+1;--i;)
	{
		g2->InsertSubProfile(*s);
		g1->DeleteSubProfile(*s);
	}
	MixedGroups->InsertGroup(g2);

	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::RandomGroups(void)
{
	unsigned int r;
	GGroup *igrp, *grp;
	R::RCursor<GGroup> igrpscur, grpscur;
	RCursor<GSubProfile> subs;

	MixedGroups->ClearGroups();

	// create the same structure than idealgroupment but without subprofiles
	igrpscur=IdealGroups->GetGroups();
	for (igrpscur.Start(); !igrpscur.End(); igrpscur.Next())
	{
		igrp=igrpscur();
		grp=new GGroup(igrp->GetId(), igrp->GetLang(), false,RDate(""),RDate(""));
		MixedGroups->InsertGroup(grp);
	}

	// put subprofiles in randomly chosen groups;
	for (igrpscur.Start(); !igrpscur.End(); igrpscur.Next())
	{
		r=Rand->Value(MixedGroups->GetNbGroups(igrpscur()->GetLang()));
		grpscur=MixedGroups->GetGroups(igrpscur()->GetLang());
		grpscur.Start();
		while(r--)
			grpscur.Next();
		subs=igrpscur()->GetSubProfiles();
 		for (subs.Start(); !subs.End(); subs.Next())
			grpscur()->InsertSubProfile(subs());
	}

}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::Run(GSlot* g)
{
	unsigned int r, nbgroups;
	unsigned  int i,u;
	unsigned int id;
	bool b;
	bool DoMerge,DoSplit;               // Split or merge  must be done?
	bool OneGroup;                      // Solution is one group already found?

	if((!Merge)&&(!Split)) return;
	nbgroups=NbMixedGroups;
	if(Random)
		nbgroups=nbgroups-1;
	if(Ideal)
		nbgroups=nbgroups-1;
	DoMerge=DoSplit=true;
	OneGroup=false;

	for(i=0,id=0;i<nbgroups;)
	{
		// If merges and splits must be done, a probability of 18/20 is used that
		//  both is done and 1/20 that one is done only
		if(Merge&&Split)
		{
			r=Rand->Value(20);
			DoMerge=DoSplit=true;
			if(r==19)
				DoMerge=false;
			if(r==20)
				DoSplit=false;
		}

		// Do a Split
		if(Split&&DoSplit)
		{
			if(g)
				g->NextChromosome(i);
			InitMixedGroups(1);
			if(!MixedGroups->GetNbGroups(CurrentLang)) break;
			r=Rand->Value(Level)+1;
			for(u=r+1,b=true;(--u)&&b;)
				b=SplitGroups();
		}

		// Do merges. If both merge are needed, do some merges of each type
		if(Merge&&DoMerge)
		{
			if(!DoSplit)               // If no split was done -> reused ideal
			{
				if(g)
					g->NextChromosome(i);
				InitMixedGroups(2);
			}
			if(!MixedGroups->GetNbGroups(CurrentLang)) break;
			r=Rand->Value(Level)+1;
			for(u=r+1,b=true;(--u)&&b;)
				b=MergeGroups();

			// If only 1 group and this solution already found -> make one split
			if(MixedGroups->GetNbGroups(CurrentLang)==1)
			{
				if(OneGroup)
				{
					SplitGroups();
				}
				else
					OneGroup=true;
			}
		}

		if(DoMerge||DoSplit)
		{
			StockInDatabase(id++);
			if(++i==nbgroups) break;
		}
	}
	if(Random)
	{
		if(g)
			g->NextChromosome(i++);
		RandomGroups();
		StockInDatabase(id++);
		nbgroups++;
	}
	if(Ideal)
	{
		if(g)
			g->NextChromosome(i);
		InitMixedGroups(cNoRef);
		StockInDatabase(id);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::InitMixedGroups(unsigned int mingroups)
{
	GGroup *igrp, *grp;
	R::RCursor<GGroup> grpscur;
	RCursor<GSubProfile> subs;

	MixedGroups=new GGroups(10);
	// Copy the ideal solution into MixedGroups
	grpscur=IdealGroups->GetGroups();
	for (grpscur.Start(); !grpscur.End(); grpscur.Next())
	{
			igrp=grpscur();
			grp=new GGroup(igrp->GetId(), igrp->GetLang(), false,RDate(""),RDate(""));
			subs=igrp->GetSubProfiles();
			for (subs.Start(); !subs.End();subs.Next())
				grp->InsertSubProfile(subs());
			MixedGroups->InsertGroup(grp);
	}

	// Select a language containing at least mingroups
	CurrentLang=0;
	if(mingroups==cNoRef) return;
	grpscur=MixedGroups->GetGroups();
	for (grpscur.Start(); (!grpscur.End())&&(!CurrentLang); grpscur.Next())
	{
		if(MixedGroups->GetNbGroups(grpscur()->GetLang())>=mingroups)
			CurrentLang=grpscur()->GetLang();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::RandOrderTab(void)
{
	unsigned int i;

	if(!CurrentLang) return;
	R::RCursor<GGroup> grpscur;
	grpscur=MixedGroups->GetGroups(CurrentLang);
	for (i=0, grpscur.Start(); !grpscur.End(); grpscur.Next(), i++)
		Tab[i]=grpscur();
	Rand->RandOrder<GGroup*>(Tab,grpscur.GetNb());
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::StockInDatabase(int nbmixedgroups)
{
	Session->GetStorage()->SaveMixedGroups(MixedGroups, nbmixedgroups,false);
}


//-----------------------------------------------------------------------------
GALILEI::GMixIdealGroups::~GMixIdealGroups(void)
{
	if(Tab)
		delete[] Tab;
	if(MixedGroups)
		delete[] MixedGroups;
	if(CurrentLang)
		delete CurrentLang;
	if(TabS)
		delete[] TabS;
	if (Rand)
		delete Rand;
}
