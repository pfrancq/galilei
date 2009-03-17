/*

	Genetic Community Algorithm

	GCAChromo.cpp

	Chromosome - Implementation

	Copyright 2002-2009 by the Université Libre de Bruxelles.

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
#include <gsubjects.h>
#include <guser.h>
#include <gcommunity.h>
#include <gtopic.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcachromo.h>
#include <gcainst.h>
#include <gcagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GCAChromo
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAChromo::GCAChromo(GCAInst* inst,size_t id)
	: RChromoSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>(inst,id)
{
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
template<class cObj,class cGroup>
	void GCAChromo::ConstructChromo(R::RCursor<cGroup> groups)
{
	GCAObj** objs;
	size_t i;

	for(groups.Start();!groups.End();groups.Next())
	{
		// Reserve a GA group
		GCAGroup* grp(ReserveGroup());

		// Go through the current objects and store them in thObjs1
		RCursor<cObj> Cur(groups()->GetObjs());
		for(Cur.Start(),objs=thObjs1,NbObjs1=0;!Cur.End();Cur.Next(),objs++,NbObjs1++)
			(*objs)=Instance->GetObj(Cur()->GetId());

		// Mix randomly thObjs1
		Instance->RandOrder<GCAObj*>(thObjs1,NbObjs1);

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
void GCAChromo::RandomConstruct(void)
{
	// Look if already a solution in the session
	if(Instance->Incremental)
	{
		switch(Instance->Type)
		{
			case otProfile:
				if(Instance->Session->GetNbCommunities())
					ConstructChromo<GProfile,GCommunity>(Instance->Session->GetCommunities());
				break;
			case otDoc:
				if(Instance->Session->GetNbTopics())
					ConstructChromo<GDoc,GTopic>(Instance->Session->GetTopics());
				break;
			default:
				cout<<"Existing clustering not take into account"<<endl;
		}
	}

	// Call classical heuristic for non-assigned objects
	RChromoSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>::RandomConstruct();
}


//-----------------------------------------------------------------------------
GCAChromo::~GCAChromo(void)
{
}
