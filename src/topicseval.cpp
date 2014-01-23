/*

	GALILEI Research Project

	TopicsEval.cpp

	Topics Evaluation - Implementation.

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
#include <gdoc.h>
#include <gtopic.h>
#include <clusteringeval.h>



//------------------------------------------------------------------------------
//
//  TopicsEval
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class TopicsEval : public ClusteringEval<GTopic,GDoc>
{
public:
	TopicsEval(GSession* session,GPlugInFactory* fac);
	virtual R::RCString GetClassName(void) const {return("TopicsEval");}
	virtual bool IsObjAloneInIdealGroup(GDoc* obj);
	virtual void ComputeBestLocalRecallPrecision(RCursor<GDoc>& objs,ClusterScore<GTopic>* grp,size_t ingroup);
	void Handle(const R::RNotification& notification);
	virtual RCursor<GTopic> GetClusters(void) {return(Session->GetObjs(pTopic));}
};


//------------------------------------------------------------------------------
TopicsEval::TopicsEval(GSession* session,GPlugInFactory* fac)
	: ClusteringEval<GTopic,GDoc>(session,fac,otTopic,otDoc)
{
	InsertObserver(HANDLER(TopicsEval::Handle),hTopics[oeAdded]);
	InsertObserver(HANDLER(TopicsEval::Handle),hTopics[oeModified]);
	InsertObserver(HANDLER(TopicsEval::Handle),hTopics[oeUpdated]);
	InsertObserver(HANDLER(TopicsEval::Handle),hTopics[oeAboutToBeDeleted]);
}


//------------------------------------------------------------------------------
bool TopicsEval::IsObjAloneInIdealGroup(GDoc* obj)
{
	RCursor<GSubject> ThGrp(Session->GetObjs(pSubject,obj));
	for(ThGrp.Start();!ThGrp.End();ThGrp.Next())
		if(ThGrp()->GetNbUsedObjs(otDoc)==1)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void TopicsEval::ComputeBestLocalRecallPrecision(RCursor<GDoc>& objs,ClusterScore<GTopic>* grp,size_t ingroup)
{
	for(objs.Start();!objs.End();objs.Next())
	{
		double bestrecall(0.0),bestprecision(0.0),bestFMeasure(0.0);
		RCursor<GSubject> ThGrp(Session->GetObjs(pSubject,objs()));
		for(ThGrp.Start();!ThGrp.End();ThGrp.Next())
		{
			double recall(0.0),precision(0.0);
			size_t InThGrp(ThGrp()->GetNbUsedObjs(ObjType));
			if(InThGrp==1)
				recall=1.0;
			else
			{
				size_t ElseInThGrp(ThGrp()->GetNbUsedObjs(grp->Group)-1);
				precision+=((double)(ElseInThGrp))/((double)(ingroup-1));
				size_t ElseInGrp(grp->Group->GetNbObjs(ThGrp())-1);
				recall+=((double)(ElseInGrp))/((double)(InThGrp-1));
			}
			if((recall==1)&&(precision==1))
			{
				bestrecall=recall;
				bestprecision=precision;
				break;
			}
			else
			if((recall>bestrecall)||(precision>bestprecision))
			{
				double FMeasure((2*recall*precision)/(recall+precision));
				if(FMeasure>bestFMeasure)
				{
					bestrecall=recall;
					bestprecision=precision;
					bestFMeasure=FMeasure;
				}
			}
		}
		grp->Precision+=bestprecision;
		grp->Recall+=bestrecall;
	}
}


//------------------------------------------------------------------------------
void TopicsEval::Handle(const RNotification& notification)
{
	GTopic* Topic(dynamic_cast<GTopic*>(notification.GetSender()));
	if(!Topic)
		return;
  	Dirty();
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Topics Evaluation","Recall/Precision/Adjusted Rand Index","Recall/Precision/Adjusted Rand Index",TopicsEval)
