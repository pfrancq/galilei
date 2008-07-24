/*

	Genetic Community Algorithm

	GCAHeuristic.h

	Heuristic - Implemenation

	Copyright 2002-2007 by the Université Libre de Bruxelles.

	Authors:
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
// include files for GALILEI
#include <gprofile.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcaheuristic.h>
#include <gcachromo.h>
#include <gcagroup.h>
#include <gcaobj.h>
#include <gcainst.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GCAHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAHeuristic::GCAHeuristic(RRandom* r,RCursor<GCAObj> objs,R::RContainer<GCAMaxRatios,true,false>& ratios,RDebug* debug)
	: RGroupingHeuristic<GCAGroup,GCAObj,GCAChromo>("IR Heuristic",r,objs,debug),
	  ToDel(Objs.GetNb()<11?10:Objs.GetNb()/4), Ratios(ratios)
{
}


//-----------------------------------------------------------------------------
void GCAHeuristic::Init(GCAChromo* groups)
{
	RGroupingHeuristic<GCAGroup,GCAObj,GCAChromo>::Init(groups);
}


//-----------------------------------------------------------------------------
GCAGroup* GCAHeuristic::FindGroup(void)
{
	GCAGroup* grp;
	double maxsim;
	double sim;

	// Look first if one of the object with a ratio are already grouped
	// -> If yes, return the group
	RCursor<GCAMaxRatio> Best(*Ratios[CurObj->GetId()]);
	for(Best.Start(),grp=0;(!Best.End())&&(!grp);Best.Next())
		grp=Groups->GetGroup(Best()->ObjId);
	if(grp)
		return(grp);

	// Go through each groups
	R::RCursor<GCAGroup> Cur(Groups->Used);
	for(Cur.Start(),maxsim=-1.0;!Cur.End();Cur.Next())
	{
		// If all the hard constraints are not respected -> skip the group.
		if(!Cur()->CanInsert(CurObj))
			continue;

		// Compute the relevant profile of the current group
		Cur()->ComputeRelevant();

		// Compute average similarity with the profiles already in the group.
		sim=Cur()->ComputeHomogeneity(CurObj);
		if(sim>maxsim)
		{
			maxsim=sim;
			grp=Cur();
		}
	}

	// If no group find -> Reserve another one.
	if(!grp)
		grp=Groups->ReserveGroup();

	// Return the group.
	return(grp);
}


//-----------------------------------------------------------------------------
void GCAHeuristic::PostRun(void)
{
	GCAObj* obj;
	R::RCursor<GCAGroup> Cur1(Groups->Used),Cur2(Groups->Used);
	GCAGroup* grp=0;
	double tmp,max;

	// Look for the groups to delete
	ToDel.Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		// Look if this group contains only 1 social profile
		if(Cur1()->GetNbObjs()!=1)
			continue;
		obj=Cur1()->GetObjPos(0);
		if(!obj->IsSocial())
			continue;

		// Find a new group
		for(Cur2.Start(),max=-1.0,grp=0;!Cur2.End();Cur2.Next())
		{
			if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
				continue;
			if(Cur2()->HasSameUser(obj))
				continue;
			Cur2()->ComputeRelevant();
			tmp=Cur2()->ComputeHomogeneity(obj);
			if(tmp>max)
			{
				max=tmp;
				grp=Cur2();
			}
		}
		if(grp)
		{
			Cur1()->Delete(obj);
			grp->Insert(obj);
			ToDel.InsertPtr(Cur1());
		}
	}

	// Delete
	Cur1.Set(ToDel);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
		Groups->ReleaseGroup(Cur1());
}


//-----------------------------------------------------------------------------
GCAHeuristic::~GCAHeuristic(void)
{
}
