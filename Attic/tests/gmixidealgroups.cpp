/*

	GALILEI Research Project

	GMixIdealGroups.cpp

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
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
// include files for R
#include <rmath/random.h>
#include <rstd/rcontainer.h>
using namespace RMath;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <tests/gmixidealgroups.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <groups/gsubject.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <langs/glang.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GMixIdealGroups
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
GALILEI::GMixIdealGroups::GMixIdealGroups(GSession* sess,RContainer<GGroups,unsigned int,true,true>* idealgroups, int nbgroups, int level, bool m, bool s, bool r, bool i)
	:  IdealGroups(idealgroups), MixedGroups(IdealGroups->NbPtr,2), Tab(0), Tabs(0), TabS(0)
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
	Tabs=new GGroups*[IdealGroups->NbPtr];
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
	GGroupsCursor Cur;
	GGroupCursor Cur2;

	Cur.Set(IdealGroups);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur2.Set(Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(Cur2()->IsIn(s))
				return(Cur2());
		}
	}
	return(0);
}


//-----------------------------------------------------------------------------
bool GALILEI::GMixIdealGroups::MergeGroups(void)
{
	GGroup* g;
	GSubProfileCursor Cur;

	// At least two groups must be presented
	if(Current->NbPtr<2) return(false);

	// First group is the first of Tab
	RandOrderTab();
	g=Tab[0];

	// Merge the groups (Put elements of g2 in g1 and delete g2).
	Cur.Set(Tab[1]);
	for(Cur.Start();!Cur.End();Cur.Next())
		g->InsertPtr(Cur());
	Current->DeletePtr(Tab[1]);

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

	// find  a group to split with at least two subprofiles
	RandOrderTab();
	for(i=Current->NbPtr+1,g1=0,ptr=Tab;--i;ptr++)
	{
		if((*ptr)->NbPtr>1)
		{
			g1=(*ptr);
			break;
		}
	}
	if(!g1) return(false);

	// Copy the subprofiles of the group in TabS and reorder them randomly
	memcpy(TabS,g1->Tab,g1->NbPtr*sizeof(GSubProfile*));
	Rand->RandOrder<GSubProfile*>(TabS,g1->NbPtr);

	// choose a place to split and put the first elements in g2
	g2=new GGroupVector(Current->GetPtrAt(Current->NbPtr-1)->GetId()+1, g1->GetLang());
	r1=Rand->Value(g1->NbPtr);
	for(s=TabS,i=r1+1;--i;)
	{
		g2->InsertPtr(*s);
		g1->DeletePtr(*s);
	}
	Current->InsertPtr(g2);

	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::RandomGroups(void)
{
	unsigned int r;
	GGroups *igrps, *grps;
	GGroup *igrp, *grp;

	MixedGroups.Clear();

	// create the same structure than idealgroupment but without subprofiles
	for(IdealGroups->Start(); !IdealGroups->End(); IdealGroups->Next())
	{
		igrps=(*IdealGroups)();
		grps=new GGroups(igrps->GetLang());
		for (igrps->Start(); !igrps->End(); igrps->Next())
		{
			igrp=(*igrps)();
			grp=new GGroupVector(igrp->GetId(), igrp->GetLang());
			grps->InsertPtr(grp);
		}
		MixedGroups.InsertPtr(grps);
	}

	// put subprofiles in randomly chosen groups;
	for(IdealGroups->Start(); !IdealGroups->End(); IdealGroups->Next())
	{
		igrps=(*IdealGroups)();
		grps=MixedGroups.GetPtr((igrps->GetLang()));
		for (igrps->Start(); !igrps->End(); igrps->Next())
		{
			igrp=(*igrps)();
			for (igrp->Start(); !igrp->End(); igrp->Next())
			{
				r=Rand->Value(grps->NbPtr);
				grps->GetPtrAt(r)->InsertPtr((*igrp)());
			}
		}
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
				g->receiveNextChromosome(i);
			InitMixedGroups(1);
			if(!Current) break;
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
					g->receiveNextChromosome(i);
				InitMixedGroups(2);
			}
			if(!Current) break;
			r=Rand->Value(Level)+1;
			for(u=r+1,b=true;(--u)&&b;)
				b=MergeGroups();

			// If only 1 group and this solution already found -> make one split
			if(Current->NbPtr==1)
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
			g->receiveNextChromosome(i++);
		RandomGroups();
		StockInDatabase(id++);
		nbgroups++;
	}
	if(Ideal)
	{
		if(g)
			g->receiveNextChromosome(i);
		InitMixedGroups(cNoRef);
		StockInDatabase(id);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::InitMixedGroups(unsigned int mingroups)
{
	unsigned int i;
	GGroups *igrps, *grps,**ptr;
	GGroup *igrp, *grp;

	// Copy the ideal solution into MixedGroups
	for (IdealGroups->Start(); !IdealGroups->End(); IdealGroups->Next())
	{
		igrps=(*IdealGroups)();
		grps=new GGroups(igrps->GetLang());
		for (igrps->Start(); !igrps->End(); igrps->Next())
		{
			igrp=(*igrps)();
			grp=new GGroupVector(igrp->GetId(), igrp->GetLang());
			for (igrp->Start(); !igrp->End(); igrp->Next())
				grp->InsertPtr((*igrp)());
			grps->InsertPtr(grp);
		}
		MixedGroups.InsertPtr(grps);
	}

	// Select a language containing at least mingroups
	Current=0;
	if(mingroups==cNoRef) return;
	memcpy(Tabs,MixedGroups.Tab,MixedGroups.NbPtr*sizeof(GGroups*));
	Rand->RandOrder<GGroups*>(Tabs,MixedGroups.NbPtr);
	for(i=MixedGroups.NbPtr+1,ptr=Tabs;--i;ptr++)
	{
		if((*ptr)->NbPtr>=mingroups)
		{
			Current=(*ptr);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::RandOrderTab(void)
{
	if(!Current) return;
	memcpy(Tab,Current->Tab,Current->NbPtr*sizeof(GGroup*));
	Rand->RandOrder<GGroup*>(Tab,Current->NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::StockInDatabase(int nbmixedgroups)
{
	Session->SaveMixedGroups(&MixedGroups, nbmixedgroups);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::IShow()
{
	cout <<endl <<endl;
	for (IdealGroups->Start(); !IdealGroups->End(); IdealGroups->Next())
	{
		GGroups * grps = (*IdealGroups)();
		cout << "ideal groups "<< grps->GetLang()->GetCode()<<endl;;
		for (grps->Start(); !grps->End(); grps->Next())
		{
			GGroup * grp= (*grps)();
			cout << "ideal group "<< grp->GetId()<<endl;
			for(grp->Start(); !grp->End(); grp->Next())
			{
				cout << (*grp)()->GetId()<< "   ";
			}
			cout << endl;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::Show()
{
	cout <<endl <<endl;
	for (MixedGroups.Start(); !MixedGroups.End(); MixedGroups.Next())
	{
		GGroups * grps = MixedGroups();
		cout << "groups "<< grps->GetLang()->GetCode()<<endl;;
		for (grps->Start(); !grps->End(); grps->Next())
		{
			GGroup * grp= (*grps)();
			cout << "group "<< grp->GetId()<<endl;
			for(grp->Start(); !grp->End(); grp->Next())
			{
				cout << (*grp)()->GetId()<< "   ";
			}
			cout << endl;
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GMixIdealGroups::~GMixIdealGroups(void)
{
	if(Tab)
		delete[] Tab;
	if(Tabs)
		delete[] Tabs;
	if(TabS)
		delete[] TabS;
}
