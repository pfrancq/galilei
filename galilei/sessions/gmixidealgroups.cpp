/*

	GALILEI Research Project

	GMixIdealGroups.cpp

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#include <langs/glang.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GMixIdealGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GMixIdealGroups::GMixIdealGroups(GSession* sess, RContainer<GGroupIdParentId,unsigned int,true,true>* parents, RContainer<GGroups,unsigned int,true,true>* idealgroups)
{
	//init parameters
	Random=MergeDiff=MergeSame=Split=true;
	Session=sess;
	Level=3;
	NbMixedGroups=10;
	Rand=new RRandomGood(0);
	IdealGroups= new RContainer<GGroups,unsigned int,true,true> (2,2);
	MixedGroups= new RContainer<GGroups,unsigned int,true,true>(2,2);
	Parents= new RContainer<GGroupIdParentId,unsigned int,true,true>(10,5);

	// fill Parents & IdealGroups
	for (parents->Start(); !parents->End(); parents->Next())
		Parents->InsertPtr((*parents)());
	for (idealgroups->Start(); !idealgroups->End(); idealgroups->Next())
		IdealGroups->InsertPtr((*idealgroups)());

}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::SetSettings(const char* s)
{
	char c, d, e, f;

	if(!(*s)) return;
	sscanf(s,"%u %u %c %c %c %c",&NbMixedGroups, &Level, &c, &d, &e, &f);
	if(c=='1') Random=true; else Random=false;
	if(d=='1') MergeSame=true; else MergeSame=false;
	if(e=='1') MergeDiff=true; else MergeDiff=false;
	if(f=='1') Split=true; else Split=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::MergeGroups(bool sametheme)
{
	int r1;
	bool ok;
	GGroups* grps;
	GGroup *g1,*g2;
	RContainer<GGroup,unsigned int,true,true> * tempgroups;
	tempgroups = new RContainer<GGroup,unsigned int,true,true>(10,5);
	
	// find a 'groups' containing at least 2 'group's;
	r1=Rand->Value(MixedGroups->NbPtr);
	grps=MixedGroups->GetPtrAt(r1);
	while (grps->NbPtr<2)
	{
		r1=Rand->Value(MixedGroups->NbPtr);
		grps=MixedGroups->GetPtrAt(r1);
	}

	// find a group having another group of the same theme (sametheme=true), or not (sametheme=false)
	ok=false;
	while(!ok)
	{
		r1=Rand->Value(grps->NbPtr);
		g1=grps->GetPtrAt(r1);
		tempgroups = new RContainer<GGroup,unsigned int,true,true>(10,5);
		for (grps->Start(); !grps->End(); grps->Next())
		{
			if( g1!=(*grps)())
				if( (Parents->GetPtr(g1->GetId())->ParentId == Parents->GetPtr((*grps)()->GetId())->ParentId)==sametheme)
				tempgroups->InsertPtr((*grps)());
			
		}
		ok=(tempgroups->NbPtr>0);
	}	
	// find another group to merge
	r1=Rand->Value(tempgroups->NbPtr);
	g2=tempgroups->GetPtrAt(r1);
	// merge the groups
	for (g2->Start(); !g2->End(); g2->Next())
		g1->InsertPtr((*g2)());
	grps->DeletePtr(g2);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::SplitGroups(void)
{
	int r1, i;
	GGroups* grps;
	GGroup *g1, *g2;
	RContainer<GSubProfile,unsigned int,true,true> * subs;
	subs=new RContainer<GSubProfile,unsigned int,true,true>(10,5);
	// find a 'groups' coontaining a least 1 group;
	r1=Rand->Value(MixedGroups->NbPtr);
	grps=MixedGroups->GetPtrAt(r1);
	while (grps->NbPtr<1)
	{
		r1=Rand->Value(MixedGroups->NbPtr);
		grps=MixedGroups->GetPtrAt(r1);
	}
	//find  a group to split
	r1=Rand->Value(grps->NbPtr);
	g1=grps->GetPtrAt(r1);
	while (g1->NbPtr<2)
	{
		r1=Rand->Value(grps->NbPtr);
		g1=grps->GetPtrAt(r1);
	}
	//create a new group;
	g2=new GGroupVector ( grps->GetPtrAt(grps->NbPtr-1)->GetId()+1, g1->GetLang());
	//choose a place to split not to leave empty group
	r1=Rand->Value(g1->NbPtr-2)+1;
	// split
	for (g1->Start(), i=0; i<r1; g1->Next(), i++)
		subs->InsertPtr((*g1)());
	for (subs->Start(); !subs->End(); subs->Next())
	{
		g2->InsertPtr((*subs)());
		g1->DeletePtr((*subs)());
	}
	grps->InsertPtr(g2);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::RandomGroups(void)
{
	int r;
	GGroups *igrps, *grps;
	GGroup *igrp, *grp;

	MixedGroups->Clear();
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
		MixedGroups->InsertPtr(grps);
	}

	// put subprofiles in randomly chosen groups;

	for(IdealGroups->Start(); !IdealGroups->End(); IdealGroups->Next())
	{
		igrps=(*IdealGroups)();
		grps=MixedGroups->GetPtr((igrps->GetLang()));
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
void GALILEI::GMixIdealGroups::Run(void)
{
	int r, nbgroups;
	if (Random)
		nbgroups=NbMixedGroups-1;
	else
		nbgroups=NbMixedGroups;
	// set the correct Level
	Level++;
	
	for (int i=0; i<nbgroups; i++)
	{
		InitMixedGroups();
		if(MergeSame)
		{
			r=Rand->Value(Level);
			for (int u=0; u<r; u++)
				MergeGroups(true);
		}
		if(MergeDiff)
		{
			r=Rand->Value(Level);
			for (int u=0; u<r; u++)
				MergeGroups(false);
		}
		if(Split)
		{
			r=Rand->Value(Level);
			for (int u=0; u<r; u++)
				SplitGroups();
		}
		if (Split||MergeSame||MergeDiff)
			StockInDatabase(i);
	}
	// totaly random
	RandomGroups();
	StockInDatabase(NbMixedGroups-1);
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::InitMixedGroups(void)
{

	GGroups *igrps, *grps;
	GGroup *igrp, *grp;
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
		MixedGroups->InsertPtr(grps);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GMixIdealGroups::StockInDatabase(int nbmixedgroups)
{
	Session->SaveMixedGroups(MixedGroups, nbmixedgroups);
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
	for (MixedGroups->Start(); !MixedGroups->End(); MixedGroups->Next())
	{
		GGroups * grps = (*MixedGroups)();
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
	if (Parents)
		delete (Parents);
	if (MixedGroups)
		delete (MixedGroups);
	if (IdealGroups)
		delete (IdealGroups);
}
