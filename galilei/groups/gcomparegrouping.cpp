/*

	GALILEI Research Project

	GCompareGrouping.h

	Compare a ideal groupement with a computed one - Implementation

	(C) 2001 by P. Francq.

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
//include files for GALILEI
#include<groups/gcomparegrouping.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GCompareGroupingSignalsReceiver
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GCompareGroupingSignalsReceiver::GCompareGroupingSignalsReceiver(void)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGroupingSignalsReceiver::NextProfile(GProfile*)
{
}



//-----------------------------------------------------------------------------
//
//  GroupScore
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GCompareGrouping::GroupScore
{
public:
	GGroup* Group;
	double Precision;
	double Recall;

	GroupScore(GGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const GroupScore* grp) const {return(Group->Compare(grp->Group));}
	int Compare(const GGroup* grp) const {return(Group->Compare(grp));}
};



//-----------------------------------------------------------------------------
//
//  GCompareGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GCompareGrouping::GCompareGrouping(GSession* s,RContainer<GGroups,unsigned int,true,true>* groups) throw(bad_alloc)
	: Session(s), Groups(groups), Precision(0.0), Recall(0.0), GroupsScore(0)
{
	GGroupsCursor Cur;
	GGroupCursor Grp;

	GroupsScore=new RContainer<GroupScore,unsigned int,true,true>(100,50);
	Cur=s->GetGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Grp=Cur()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
			GroupsScore->InsertPtr(new GroupScore(Grp()));
	}
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GCompareGrouping::GetIdealGroup(GSubProfile* sub) const
{
	GGroups* grps;

	grps=Groups->GetPtr<const GLang*>(sub->GetLang());
	if(!grps) return(0);
	return(grps->GetGroup(sub));
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::ComputeRecallPrecision(GCompareGroupingSignalsReceiver* rec)
{
	GroupScoreCursor Grp;
	GSubProfileCursor Sub;
	GGroup* thGrp;
	unsigned int NbGrp;
	unsigned int InGrp;
	unsigned int InthGrp;

	Precision=Recall=0.0;
	Grp.Set(GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		NbGrp=Grp()->Group->NbPtr;
		Grp()->Precision=Grp()->Recall=0.0;
		if(!NbGrp) continue;
		if(NbGrp==1)
		{
			thGrp=GetIdealGroup(Grp()->Group->Tab[0]);
			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
			Grp()->Precision=1.0;
			if(thGrp->NbPtr==1)
				Grp()->Recall=1.0;
		}
		else
		{
			Sub=Grp()->Group->GetSubProfileCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				thGrp=GetIdealGroup(Sub());
				if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
				if(thGrp->NbPtr==1)
				{
					Grp()->Recall+=1.0;
				}
				else
				{
					InthGrp=thGrp->GetNbSubProfiles(Grp()->Group)-1;
					if(InthGrp)
						Grp()->Precision+=((double)(InthGrp))/((double)(NbGrp-1));
					InGrp=Grp()->Group->GetNbSubProfiles(thGrp)-1;
					if(InGrp)
						Grp()->Recall+=((double)(InGrp))/((double)(thGrp->NbPtr-1));
				}
			}
			Grp()->Precision/=NbGrp;
			Grp()->Recall/=NbGrp;
		}
		Precision+=Grp()->Precision;
		Recall+=Grp()->Recall;
	}
	if(GroupsScore->NbPtr)
	{
		Precision/=(double)GroupsScore->NbPtr;
		Recall/=(double)GroupsScore->NbPtr;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::ComputeTotal(GCompareGroupingSignalsReceiver* rec)
{
	GGroup* thGrp;
	GGroup* thGrp2;
	unsigned int NbComp;
	bool IsInGrp,IsInthGrp;
	GSubProfile** Prof1;
	GSubProfile* P1;
	GSubProfile** Prof2;
	GSubProfile* P2;
	unsigned int i,j;

	Total=0.0;
	NbComp=0;
	for(i=Session->GetSubProfiles()->NbPtr,Prof1=Session->GetSubProfiles()->Tab;--i;Prof1++)
	{
		P1=(*Prof1);
		thGrp=GetIdealGroup(P1);
		if(!P1->GetGroup())
		{
			if(thGrp)
				NbComp+=thGrp->NbPtr-1;
			continue;
		}
		for(j=i+1,Prof2=Prof1+1;--j;Prof2++)
		{
			P2=(*Prof2);
			if((P1->GetLang()!=P2->GetLang())||(!P2->GetGroup())) continue;
			thGrp2=GetIdealGroup(P2);
			if(!thGrp2) continue;
			NbComp++;
			IsInGrp=P1->GetGroup()->IsIn<const GSubProfile*>(P2);
			IsInthGrp=thGrp->IsIn<const GSubProfile*>(P2);
			if((IsInGrp&&IsInthGrp)||((!IsInGrp)&&(!IsInthGrp))) Total+=1.0;
		}
	}
	if(NbComp)
		Total/=NbComp;
	else
		Total=1.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::Compare(GCompareGroupingSignalsReceiver* rec)
{
	ComputeRecallPrecision(rec);
	ComputeTotal(rec);
}


//-----------------------------------------------------------------------------
double GALILEI::GCompareGrouping::GetPrecision(GGroup* grp)
{
	GroupScore* g=GroupsScore->GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Precision);
}


//-----------------------------------------------------------------------------
double GALILEI::GCompareGrouping::GetRecall(GGroup* grp)
{
	GroupScore* g=GroupsScore->GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Recall);
}


//-----------------------------------------------------------------------------
GALILEI::GCompareGrouping::~GCompareGrouping(void)
{
	if(GroupsScore) delete GroupsScore;
}
