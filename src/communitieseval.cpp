/*

	GALILEI Research Project

	CommunitiesEval.cpp

	Communities Evaluation - Implementation.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gprofile.h>
#include <gcommunity.h>
#include <clusteringeval.h>



//------------------------------------------------------------------------------
//
//  CommunitiesEval
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class CommunitiesEval : public ClusteringEval<GCommunity,GProfile>
{
public:
	CommunitiesEval(GSession* session,GPlugInFactory* fac);
	virtual R::RCString GetClassName(void) const {return("CommunitiesEval");}
	void Handle(const R::RNotification& notification);
	virtual RCursor<GCommunity> GetClusters(void) {return(Session->GetCommunities());}
};


//------------------------------------------------------------------------------
CommunitiesEval::CommunitiesEval(GSession* session,GPlugInFactory* fac)
	: ClusteringEval<GCommunity,GProfile>(session,fac,otCommunity,otProfile)
{
	InsertObserver(HANDLER(CommunitiesEval::Handle),"ObjectChanged");
}


//------------------------------------------------------------------------------
void CommunitiesEval::Handle(const RNotification& notification)
{
	GEvent& Event(GetData<GEvent&>(notification));
    if(GroupType==Event.Object->GetObjType())
    	Dirty();
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Communities Evaluation","Recall/Precision/Adjusted Rand Index","Recall/Precision/Adjusted Rand Index",CommunitiesEval)
