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
#include<sessions/gslot.h>
using namespace GALILEI;



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
void GALILEI::GCompareGrouping::ComputeRecallPrecision(GSlot* /*rec*/)
{
	GroupScoreCursor Grp;
	GSubProfileCursor Sub;
	GGroup* thGrp;
	unsigned int NbGrp;
	unsigned int InGrp;
	unsigned int InthGrp;
	unsigned int NbProf;

	Precision=Recall=0.0;
	NbProf=0;
	Grp.Set(GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		NbGrp=Grp()->Group->NbPtr;
		NbProf+=NbGrp;
		Grp()->Precision=Grp()->Recall=0.0;
		if(!NbGrp) continue;
		if(NbGrp==1)
		{
			thGrp=GetIdealGroup(Grp()->Group->Tab[0]);
			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
			Grp()->Precision=1.0;
			if(thGrp->NbPtr==1)
				Grp()->Recall=1.0;
			Precision+=Grp()->Precision;
			Recall+=Grp()->Recall;
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
					Recall+=1.0;
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
			Precision+=Grp()->Precision;
			Recall+=Grp()->Recall;
			Grp()->Precision/=NbGrp;
			Grp()->Recall/=NbGrp;
		}
	}
	if(NbProf)
	{
		Precision/=(double)NbProf;
		Recall/=(double)NbProf;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::ComputeTotal(GSlot* /*rec*/)
{
	GGroupsCursor ComputedGroup=Session->GetGroupsCursor();
	GGroups* Grp;
	GGroups* GrpComputed;
	GGroup* grp;
	GGroup* grpcomputed;
	int u,v,subprofileid,NbTot;
	int** ptr;
 	int* ptr2;
	int i,j;

	// Compute number of elements in ideal and computed groups.
	u=0;
	v=0;
	NbTot=0;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		Grp=(*Groups)();
		u=u+Grp->NbPtr;
	}
	for(ComputedGroup.Start();!ComputedGroup.End();ComputedGroup.Next())
	{
		Grp=ComputedGroup();
		v=v+Grp->NbPtr;
	}

	//initalisation of the matrix
	int** matrix;
	matrix=new int*[u];
	for(int i=0;i<u;i++)
	{
		matrix[i]=new int[v];
		for(int j=0;j<v;j++)
		{
			matrix[i][j]=0;
		}
	}

	// Element i,j of the matrix is the number of profiles who are in the i eme ideal groups and j eme computed group
	int u1,v1;
	u1=0;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		Grp=(*Groups)();
		for(Grp->Start();!Grp->End();Grp->Next())
		{
			grp=(*Grp)();
			for(grp->Start();!grp->End();grp->Next())
			{
				subprofileid=(*grp)()->GetId();
				v1=0;
				for(ComputedGroup.Start();!ComputedGroup.End();ComputedGroup.Next())
				{
					GrpComputed=ComputedGroup();
					for(GrpComputed->Start();!GrpComputed->End();GrpComputed->Next())
					{
						grpcomputed=(*GrpComputed)();
						for(grpcomputed->Start();!grpcomputed->End();grpcomputed->Next())
						{
							if (subprofileid==(*grpcomputed)()->GetId())
							{
								matrix[u1][v1]++;
								NbTot++;
							}
						}
						v1++;
					}
				}
			}
			u1++;
		}
	}

	// calculation of the different terms of the total = a-(b*c)/d)/((1/2*(b+c))-(b*c)/d)
	double a,b,c,d,num,den;

	a=0;
	
	for(i=u+1,ptr=matrix;--i;ptr++)
	{
		for(j=v+1,ptr2=(*ptr);--j;ptr2++)
		{
			a+=(((*ptr2)*((*ptr2)-1))/2);
		}
	}

	b=0;

	for(int i=0;i<u;i++)
	{
		int sum=0;
		for(int j=0;j<v;j++)
		{
			sum+=matrix[i][j];
		}
		b+=((sum*(sum-1))/2);
	}

	c=0;

	for(int j=0;j<v;j++)
	{
		int sum=0;
		for(int i=0;i<u;i++)
		{
			sum+=matrix[i][j];
		}
		c+=((sum*(sum-1))/2);
	}

	d=(NbTot*(NbTot-1))/2;

	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);


	Total=num/den;

	for(int i=0;i<u;i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::Compare(GSlot* rec)
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
