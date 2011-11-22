/*

	GALILEI Research Project

	NNGGAChromo.cpp

	Chromosome - Implementation

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
#include <guser.h>
#include <gcommunity.h>
#include <gtopic.h>


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <nnggachromo.h>
#include <nnggainst.h>
#include <nnggagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class NNGGAChromoDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NNGGAChromoDoc::NNGGAChromoDoc(NNGGAInstDoc* inst,size_t id)
	: RChromoNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>(inst,id)
{
}


//-----------------------------------------------------------------------------
void NNGGAChromoDoc::ConstructChromo(R::RCursor<GTopic> groups)
{
	GDoc** objs;
	size_t i;

	for(groups.Start();!groups.End();groups.Next())
	{
		// Reserve a GA group
		NNGGAGroupDoc* grp(ReserveGroup());

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
void NNGGAChromoDoc::RandomConstruct(void)
{
	// Look if already a solution in the session
	if((Instance->Params->Incremental)&&(Instance->Session->GetNbObjs(pTopic)))
		ConstructChromo(Instance->Session->GetObjs(pTopic));

	// Call classical heuristic for non-assigned objects
	RChromoNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>::RandomConstruct();
}



//-----------------------------------------------------------------------------
//
// class NNGGAChromoProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NNGGAChromoProfile::NNGGAChromoProfile(NNGGAInstProfile* inst,size_t id)
	: RChromoNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>(inst,id)
{
}


//-----------------------------------------------------------------------------
void NNGGAChromoProfile::ConstructChromo(R::RCursor<GCommunity> groups)
{
	GProfile** objs;
	size_t i;

	for(groups.Start();!groups.End();groups.Next())
	{
		// Reserve a GA group
		NNGGAGroupProfile* grp(ReserveGroup());

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
void NNGGAChromoProfile::RandomConstruct(void)
{
	// Look if already a solution in the session
	if((Instance->Params->Incremental)&&(Instance->Session->GetNbObjs(pCommunity)))
		ConstructChromo(Instance->Session->GetObjs(pCommunity));

	// Call classical heuristic for non-assigned objects
	RChromoNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>::RandomConstruct();
}
