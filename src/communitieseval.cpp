/*

	GALILEI Research Project

	CommunitiesEval.cpp

	Communities Evaluation - Implementation.

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
	virtual RCursor<GCommunity> GetClusters(void) {return(Session->GetObjs(pCommunity));}
};


//------------------------------------------------------------------------------
CommunitiesEval::CommunitiesEval(GSession* session,GPlugInFactory* fac)
	: ClusteringEval<GCommunity,GProfile>(session,fac,otCommunity,otProfile)
{
	InsertObserver(HANDLER(CommunitiesEval::Handle),hCommunities[oeAdded]);
	InsertObserver(HANDLER(CommunitiesEval::Handle),hCommunities[oeModified]);
	InsertObserver(HANDLER(CommunitiesEval::Handle),hCommunities[oeUpdated]);
	InsertObserver(HANDLER(CommunitiesEval::Handle),hCommunities[oeAboutToBeDeleted]);
}


//------------------------------------------------------------------------------
void CommunitiesEval::Handle(const RNotification& notification)
{
	GCommunity* Community(dynamic_cast<GCommunity*>(notification.GetSender()));
	if(!Community)
		return;
  	Dirty();
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Communities Evaluation","Recall/Precision/Adjusted Rand Index","Recall/Precision/Adjusted Rand Index",CommunitiesEval)
