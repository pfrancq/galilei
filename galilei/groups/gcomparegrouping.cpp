/*

	GALILEI Research Project

	GCompareGrouping.h

	Compare a ideal groupement with a computed one - Implementation

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for R Project
//#include <rstd/rcontainercursor.h>
//using namespace RStd;


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
void GALILEI::GCompareGrouping::Compare(GCompareGroupingSignalsReceiver* rec)
{
	GroupScoreCursor Grp;
	GSubProfileCursor Sub;
	GGroup* grp;
	unsigned int NbGrp;

	Precision=Recall=0.0;
	Grp.Set(GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		NbGrp=Grp()->Group->NbPtr;
		Grp()->Precision=Grp()->Recall=0.0;
		if(NbGrp==1)
		{
			grp=GetIdealGroup(Grp()->Group->Tab[0]);
			if((!grp)||(grp->GetLang()!=Grp()->Group->GetLang())) continue;
			Grp()->Precision=1.0;
			if(grp->NbPtr==1)
				Grp()->Recall=1.0;
			else
				Grp()->Recall=1.0/((double)(grp->NbPtr-1));
		}
		else
		{
			Sub=Grp()->Group->GetSubProfileCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				grp=GetIdealGroup(Sub());
				if((!grp)||(grp->GetLang()!=Grp()->Group->GetLang())) continue;
				if(grp->NbPtr==1)
				{
					Grp()->Precision+=1.0/((double)(NbGrp-1));
					Grp()->Recall+=1.0;
				}
				else
				{
					Grp()->Precision+=(NbGrp-1)/((double)(grp->GetNbSubProfiles(Grp()->Group)-1));
					Grp()->Recall+=(grp->NbPtr-1)/((double)(Grp()->Group->GetNbSubProfiles(grp)-1));
				}
			}
			Grp()->Precision/=NbGrp;
			Grp()->Recall/=NbGrp;
		}
		Precision+=Grp()->Precision;
		Recall+=Grp()->Recall;
	}
	Precision/=(double)GroupsScore->NbPtr;
	Recall/=(double)GroupsScore->NbPtr;
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
