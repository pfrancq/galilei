/*

	GALILEI Project Library

	GIRHeuristic.cpp

	Heuristic for an IR Problem - Implemenation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		

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
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <groups/girheuristic.h>
#include <groups/gchromoir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <groups/ginstir.h>
using namespace GALILEI;
using namespace RGGA;



//-----------------------------------------------------------------------------
//
// class GIRHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIRHeuristic::GIRHeuristic(RRandom* r,RStd::RCursor<GObjIR,unsigned int>* objs)
	: RGGA::RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>("IR Heuristic",r,objs)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GIRHeuristic::Init(GChromoIR* groups)
{
	RGGA::RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>::Init(groups);
}


//-----------------------------------------------------------------------------
GGroupIR* GALILEI::GIRHeuristic::FindGroup(void) throw(RGA::eGA)
{
	GGroupIR* grp;
	double maxsim;
	double ratio;
	double maxratio;
	double sim;
	GGroupIRCursor Cur;

//	if(CurObj->GetId()==16||CurObj->GetId()==5)
//		cout<<"Debug"<<endl;
	Cur.Set(Groups->Used);
	for(Cur.Start(),maxsim=maxratio=-1.0,grp=0;!Cur.End();Cur.Next())
	{
		if(!Cur()->CanInsert(CurObj)) continue;
		sim=Cur()->ComputeAvgSim(CurObj);
		ratio=Cur()->GetMaxRatioSame(CurObj);
		if(ratio>=Groups->Instance->Params->MinCommonOK)
		{
			// Take the group with the highest ratio
			if(ratio>maxratio)
			{
				maxratio=ratio;
				grp=Cur();
				maxsim=2.0;
			}
		}
		else
		{
			if(sim>maxsim)
			{
				maxsim=sim;
				grp=Cur();
			}
		}
//		return(Groups->Used());
	}
	if(!grp)
		grp=Groups->ReserveGroup();
	return(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GIRHeuristic::PostRun(void) throw(RGA::eGA)
{
	GObjIR* obj;
	GGroupIRCursor Cur1,Cur2;
	GGroupIR* grp=0;
	double tmp,max;
	unsigned int i;

	for(i=0;i<Groups->Used.NbPtr;i++)
	{
		Cur1.Set(Groups->Used);
		Cur2.Set(Groups->Used);
		for(Cur1.GoTo(i);!Cur1.End();Cur1.Next(),i++)
		{
			if(Cur1()->GetNbObjs()!=1) continue;
			obj=Cur1()->GetObjPos(0);
			if(!obj->GetSubProfile()->GetProfile()->IsSocial()) continue;
			for(Cur2.Start(),max=-1.0,grp=0;!Cur2.End();Cur2.Next())
			{
				if(Cur1()==Cur2()) continue;
//				if(!Cur2()->CanInsert(obj)) continue;
				tmp=Cur2()->ComputeAvgSim(obj);
				if(tmp>max)
				{
					max=tmp;
					grp=Cur2();
				}
			}
			if(grp)
			{
				Groups->ReleaseGroup(Cur1());
				grp->Insert(obj);
				i--;
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GIRHeuristic::~GIRHeuristic(void)
{
}
