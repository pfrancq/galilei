/*

	GALILEI Research Project

	SGGA Chromo.cpp

	SGGA Chromosome - Implementation

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gsubject.h>
#include <guser.h>
#include <gcommunity.h>
#include <gtopic.h>


//-----------------------------------------------------------------------------
// include files for SGGA
#include <sggachromo.h>
#include <sggainst.h>
#include <sggagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class SGGAChromoDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
SGGAChromoDoc::SGGAChromoDoc(SGGAInstDoc* inst,size_t id)
	: RChromoSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>(inst,id)
{
}


//-----------------------------------------------------------------------------
void SGGAChromoDoc::ConstructChromo(R::RCursor<GTopic> groups)
{
	GDoc** objs;
	size_t i;

	for(groups.Start();!groups.End();groups.Next())
	{
		// Reserve a GA group
		SGGAGroupDoc* grp(ReserveGroup());

		// Go through the current objects and store them in thObjs1
		RCursor<GDoc> Cur(groups()->GetObjs());
		for(Cur.Start(),objs=thObjs1,NbObjs1=0;!Cur.End();Cur.Next(),objs++,NbObjs1++)
			(*objs)=Cur();

		// Mix randomly thObjs1
		Instance->RandOrder(thObjs1,NbObjs1);

		// Put the objects in the group if possible
		for(objs=thObjs1,i=NbObjs1+1;--i;objs++)
		{
			if((*objs)&&(grp->CanInsert(*objs)))
				grp->Insert(*objs);
		}

		// Verify that the groups is not empty
		if(!grp->GetNbObjs())
			ReleaseGroup(grp);
	}
}


//-----------------------------------------------------------------------------
void SGGAChromoDoc::RandomConstruct(void)
{
	// Look if already a solution in the session
	if((Instance->Params->Incremental)&&(Instance->Session->GetNbObjs(pTopic)))
		ConstructChromo(Instance->Session->GetObjs(pTopic));

	// Call classical heuristic for non-assigned objects
	RChromoSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>::RandomConstruct();
}



//-----------------------------------------------------------------------------
//
// class SGGAChromoProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
SGGAChromoProfile::SGGAChromoProfile(SGGAInstProfile* inst,size_t id)
	: RChromoSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>(inst,id)
{
}


//-----------------------------------------------------------------------------
void SGGAChromoProfile::ConstructChromo(R::RCursor<GCommunity> groups)
{
	GProfile** objs;
	size_t i;

	for(groups.Start();!groups.End();groups.Next())
	{
		// Reserve a GA group
		SGGAGroupProfile* grp(ReserveGroup());

		// Go through the current objects and store them in thObjs1
		RCursor<GProfile> Cur(groups()->GetObjs());
		for(Cur.Start(),objs=thObjs1,NbObjs1=0;!Cur.End();Cur.Next(),objs++,NbObjs1++)
			(*objs)=Cur();

		// Mix randomly thObjs1
		Instance->RandOrder(thObjs1,NbObjs1);

		// Put the objects in the group if possible
		for(objs=thObjs1,i=NbObjs1+1;--i;objs++)
		{
			if((*objs)&&(grp->CanInsert(*objs)))
				grp->Insert(*objs);
		}

		// Verify that the groups is not empty
		if(!grp->GetNbObjs())
			ReleaseGroup(grp);
	}
}


//-----------------------------------------------------------------------------
void SGGAChromoProfile::RandomConstruct(void)
{
	// Look if already a solution in the session
	if((Instance->Params->Incremental)&&(Instance->Session->GetNbObjs(pCommunity)))
		ConstructChromo(Instance->Session->GetObjs(pCommunity));

	// Call classical heuristic for non-assigned objects
	RChromoSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>::RandomConstruct();
}
