/*

	GALILEI Research Project

	GGroupingKCos.cpp

	Heuristic using Similarity - Implementation

	(C) 2001 by David Wartel.

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
// include specific files
#include <stdlib.h>
#include <time.h>

//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingkcos.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<profiles/gprofile.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------
//
//  GGroupingKCos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::GGroupingKCos(GSession* s) throw(bad_alloc)
	: GGrouping("KMeansCosinus",s), Random(true), GroupsNumber(13), IterNumber(10)
		, NbTests(1), FindGroupsNumber(false), MaxNbGroups(13), MinNbGroups(13)
{
	protos = new RContainer<GSubProfile,unsigned int,false,false> (GroupsNumber, 2);
	protoserror = new RContainer<GSubProfile,unsigned int,false,false> (GroupsNumber, 2);
	grpstemp = new RContainer<GGroup,unsigned int,false,false>  (GroupsNumber, 2);
	grpstemp2 = new RContainer<GGroup,unsigned int,false,false>  (GroupsNumber, 2);
	grpsfinal = new RContainer<GGroup,unsigned int,false,false>  (GroupsNumber, 2);

}

//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKCos::GetSettings(void)
{
	static char tmp[100];
	char c, d;

	if(Random) c='1'; else c='0';
	if (FindGroupsNumber) d='1'; else d='0';
	
	sprintf(tmp,"%c %u %u %c %u %u",c,GroupsNumber, IterNumber, FindGroupsNumber, MaxNbGroups, MinNbGroups);
	return(tmp);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::SetSettings(const char* s)
{
	char c, d;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %c %u %u",&c,&GroupsNumber, &IterNumber, &d, &MaxNbGroups, &MinNbGroups);
	if(c=='1') Random=true; else Random=false;
	if(d=='1') FindGroupsNumber=true; else FindGroupsNumber=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Init(void) throw(bad_alloc)
{
	protos->Clear();
	if (Random)
	{
		while (protos->NbPtr!=GroupsNumber)
		{
			int u = int(rand()%(SubProfiles.NbPtr));
			GSubProfile* tmp=SubProfiles.GetPtrAt(u);
			if (!protos->GetPtr(tmp))
				protos->InsertPtr(tmp);
		}
	}
	else
		RelevantInitSubProfiles(GroupsNumber);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::RelevantInitSubProfiles(int nbsubs)
{
	GSubProfile* subprof;
	for (int i=0; i<nbsubs; i++)
	{ 
		double refsum=0.0;
		for (SubProfiles.Start();!SubProfiles.End(); SubProfiles.Next())
		{
			if (!protos->GetPtr((SubProfiles)()))
			{
				double sum=SumSimilarity((SubProfiles)());
				if (sum>refsum)
				{
					refsum=sum;
					subprof=(SubProfiles)();
				} 
			}	
		}
		protos->InsertPtr(subprof);
	}
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Run(void) throw(GException)
{
	int grnumber;
	double var=100.00 ;
	if (FindGroupsNumber==0)
	{
		MinNbGroups=GroupsNumber;
		MaxNbGroups=GroupsNumber;
	}

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		Clear();
		grpstemp->Clear();
		double finalcost;
		if(SubProfiles.NbPtr==0) return;
		srand((unsigned)time(NULL));   //enables the total random parameters

		for (unsigned int test=0; test<NbTests; test++)
		{
			// initialization
			Init();

			// cycle
			unsigned int itermax=0;
			unsigned int error=1;
			while ((itermax<IterNumber)&&(error!=0))
			{
				//Groups->Clear();
				grpstemp->Clear();
		
				// (re-)allocation
				ReAllocate();

				//recentering
				ReCenter();

				// error calculation
				error=CalcError();
				itermax++;
			}
			double cost=CostFunction()/protos->NbPtr;
			if ((test==0)||(cost< finalcost))
			{
				grpstemp2->Clear();
				for (grpstemp->Start();!grpstemp->End(); grpstemp->Next())
     				grpstemp2->InsertPtr((*grpstemp)());
				finalcost=cost;
			}
		}
		if (finalcost<var)
		{
			var=finalcost;
			grnumber=GroupsNumber;
			grpsfinal->Clear();
			for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
				grpsfinal->InsertPtr((*grpstemp2)());
		}
	}
	for (grpsfinal->Start(); !grpsfinal->End(); grpsfinal->Next())
		Groups->InsertPtr((*grpsfinal)());
	GroupsNumber=grnumber;
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKCos::CostFunction()
{
	double cost=TestMeasure();
	return (cost);
}
                                      

//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReAllocate()
{
	GGroup * g;
	for (protos->Start(); !protos->End(); protos->Next())  // groups containing one proto are created
	{
		grpstemp->InsertPtr(g=NewGroup(Groups->GetLang()));
		g->InsertPtr((*protos)());
	}
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		double cos= 0.0;
		GSubProfile* s=(SubProfiles)();
		GSubProfile* parent, *proto;
		for (protos->Start(); !protos->End(); protos->Next())
		{
			proto=(*protos)();
			if (s->Similarity(proto)>cos)
			{
				cos=s->Similarity((*protos)());
				parent=proto;
			}
		}
		GGroup* gr=FindGroup(parent);
		gr->InsertPtr(s);
	}	

}
//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReCenter()
{
 	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GSubProfile* tmp=RelevantSubProfile((*grpstemp)());
		protoserror->InsertPtr(tmp); 
	}
}


//-----------------------------------------------------------------------------
int  GALILEI::GGroupingKCos::CalcError()
{
	int err=protos->NbPtr;
	for (protos->Start(); !protos->End(); protos->Next())
	{
		if (protoserror->GetPtr((*protos)()))
			err=err-1;
	}
	protos->Clear();
	for (protoserror->Start(); !protoserror->End(); protoserror->Next())
	{
	     protos->InsertPtr((*protoserror)());
	}
	return(err);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::SumSimilarity(GSubProfile * s)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for (sub1=SubProfiles.Tab,i=SubProfiles.NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for(sub1=grp->Tab,i=grp->NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GGroupingKCos::RelevantSubProfile(GGroup* grp)
{
	GSubProfile* finalsub;
	GSubProfile* sub1;
	double refsum=0.0;
	for(grp->Start(); !grp->End(); grp->Next())
	{
		sub1 = (*grp)();
		double sum=GroupSumSimilarity(sub1, grp);
		if (sum>=refsum)
		{
			finalsub=sub1;
			refsum=sum;
		}	
	}
	return(finalsub);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroupingKCos::FindGroup(GSubProfile* s)
{
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
		if( (*grpstemp)()->GetPtr(s))
			return((*grpstemp)());
	return(0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::TestMeasure()
{
	GSubProfile **s1, **s2;
	double intra=0.0, inter=1.0;
	for (protos->Start(); !protos->End(); protos->Next())
	{
		GSubProfile* proto = (*protos)();
		GGroup* gr=FindGroup(proto);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			double dist=(1.0)-((*gr)()->Similarity(proto));
			intra=intra+(dist*dist);
		}
	}
	intra=intra/SubProfiles.NbPtr;
	unsigned int i, j;
	for(s1=protos->Tab,i=protos->NbPtr;--i;s1++)
	{
		for(s2=s1+1,j=i+1;--j;s2++)
		{
			double dist=((1-0)-(*s1)->Similarity(*s2));
			dist=dist*dist;
			if(dist<=inter)
				inter=dist;
		}
	}
	return(intra/inter);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::CalcCalinsky()
{                                 
	GSubProfile* mean;
	double refsum=0.0;
	for (SubProfiles.Start();!SubProfiles.End(); SubProfiles.Next())
	{
		double sum=SumSimilarity((SubProfiles)());
		if (sum>refsum)
		{
			refsum=sum;
			mean=(SubProfiles)();
		} 
	}

   // ssw & ssb calculation
	double ssw=0.0, ssb=0.0;
	for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
	{
		GGroup* gr=(*grpstemp2)();
		GSubProfile* center= RelevantSubProfile(gr);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			double dist=(1.0)-(*gr)()->Similarity(center);
			ssw=ssw+(dist*dist);
		}
		double dist2=(1.0)-(center->Similarity(mean));
		ssb=ssb+((gr->NbPtr)*(dist2*dist2));
	}

	//index calculation
	double cal;
	int k=GroupsNumber;
	int n=SubProfiles.NbPtr;
	cal=(ssb/(k-1))/(ssw/(n-k)); 
	return(cal);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::StatMeasure()
{ 
	GGroup* g1;
	GSubProfile* s1;
	GSubProfile** s2;
	unsigned int j;
	double stat=0.0;
	int compt=0;

	for(Groups->Start(); !Groups->End(); Groups->Next())
	{
		g1=(*Groups)();
		for (g1->Start(); !g1->End(); g1->Next())
		{
			s1=(*g1)();
			for(s2=g1->Tab,j=g1->NbPtr;j--;s2++)
			{
				if (s1->GetId()!=(*s2)->GetId())
				{
					double tmp=s1->GetCommonDocs(*s2);
					stat=stat+(tmp/s1->GetNbJudgedDocs());
					compt=compt+1;
				}
			}
		}
	}
	stat=stat/compt;
	return(stat*100);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::~GGroupingKCos(void)
{
}
