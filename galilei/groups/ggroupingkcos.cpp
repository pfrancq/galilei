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
#include<groups/ggroupingsim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupvector.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<sessions/gslot.h>
#include<profiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GGroupingKCos
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::GGroupingKCos(GSession* s) throw(bad_alloc)
		: GGroupingSim("KMeansCosinus",s), Random(true), IterNumber(10), NbTests(1),
			FindGroupsNumber(false), MaxNbGroups(13), MinNbGroups(13), GlobalSimi(false),
				protos(0), protoserror(0), grpstemp(0), grpstemp2(0), grpsfinal(0)
{
	Epsilon= 0.0005;
	protos = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	protoserror = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	grpstemp = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	grpstemp2 = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	grpsfinal = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKCos::GetSettings(void)
{

	static char tmp[250];
	char c, d,e, f, g;

	if(Random) c='1'; else c='0';
	if (FindGroupsNumber) d='1'; else d='0';
	if (GlobalSimi) e='1'; else e='0';
	if (GlobalSim) f='1'; else f='0';
	if (FullSim) g='1'; else g='0';

	 sprintf(tmp,"%c %u %u %c %c %lf %c %c",c, IterNumber, NbTests, d, e, LevelSim, f, g);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::SetSettings(const char* s)
{
	char c, d, e, f,g;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %c %c %lf %c %c",&c, &IterNumber, &NbTests, &d, &e, &LevelSim, &f, &g);
	if(c=='1') Random=true; else Random=false;
	if(d=='1') FindGroupsNumber=true; else FindGroupsNumber=false;
	if(e=='1') GlobalSimi=true; else GlobalSimi=false;
	if(f=='1') GlobalSim=true; else GlobalSim=false;
	if(g=='1') FullSim=true; else FullSim=false;
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
			 if (IsValidProto(tmp))
				protos->InsertPtr(tmp);
		}
	}
	 else
		RelevantInitSubProfiles(GroupsNumber);
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingKCos::IsValidProto(GSubProfile* s) throw(bad_alloc)
{
	if (protos->GetPtr(s))
		return(false);
	for (protos->Start(); !protos->End(); protos->Next())
	 {
		double sim=Similarity(s, (*protos)());
		if ((sim<=1+Epsilon)&&(sim>=1-Epsilon))
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::RelevantInitSubProfiles(int nbsubs)
{
	GSubProfile* subprof;
	for (int i=0; i<nbsubs; i++)
	{
		SubProfiles.Start();
		double refsum=SumSimilarity((SubProfiles)());;
		subprof=(SubProfiles)();
		for (SubProfiles.Next();!SubProfiles.End(); SubProfiles.Next())
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

	// Calculates the similarities between subprofiles
	if (SubProfiles.NbPtr>0)
		ProfSim= new GProfilesSim(SubProfiles, GlobalSimi);

	//evaluate the number of groups
	EvaluateGroupsNumber();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		Clear();
		grpstemp->Clear();
		double finalcost;
		double palfinal;
		if (SubProfiles.NbPtr==0) return ;
		srand((unsigned)time(NULL));   //enables the total random parameters

		for (unsigned int test=0; test<NbTests; test++)
		{
			// initialization
			 Init();

			// cycle
			unsigned int itermax=0;
			int error=1;
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
			double pal=PalMeasure();
			if ((test==0)||(cost< finalcost))
			{
				grpstemp2->Clear();
				for (grpstemp->Start();!grpstemp->End(); grpstemp->Next())
					grpstemp2->InsertPtr((*grpstemp)());
				finalcost=cost;
				palfinal=pal;
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
	{
		Groups->InsertPtr((*grpsfinal)());
	}
	GlobalSimi=GlobalSim;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::EvaluateGroupsNumber()
{
	if (FindGroupsNumber==0)
	{
		MinNbGroups=24;
		MaxNbGroups=24;//SubProfiles.NbPtr-1;
	}
	else
		FindGroupsNumberLimits();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::FindGroupsNumberLimits()
{
	// run first fit heuristic
	GSubProfile* s;
	GGroup* g;

	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
	{
		s=SubProfiles();
		if(s->GetGroup()) continue;  // If already grouped -> next one.
		g=0;

		// Find the first group able to receive the subprofile.
		for(Groups->Start();!Groups->End();Groups->Next())
		{
			if(IsCoherent((*Groups)(),s))
			{
				g=(*Groups)();
				break;
			}
		}

		// If no group found create one and insert it in Groups.
		if(!g)
			g=NewGroup(Groups->GetLang());

		// Attach the subprofile to the group found/created.
		g->InsertSubProfile(s);
	}

	// set Max and Min Nbgroups.
	int nbgroup=Groups->NbPtr;
	MaxNbGroups=int(nbgroup + nbgroup*0.2);
	MinNbGroups=int(nbgroup-nbgroup*0.2);
	Groups->Clear();
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKCos::CostFunction()     // calculates the intra/min(inter)
{
	GSubProfile **s1, **s2;
	unsigned int i, j;
	double intra=0.0, mininter=1.0;

	for (protos->Start(); !protos->End(); protos->Next())
	{
		GSubProfile* proto = (*protos)();
		GGroup* gr=FindGroup(proto);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			double dist=(1.0)-Similarity((*gr)(),proto);
			intra+=(dist*dist);
		}
	}
	intra=intra/SubProfiles.NbPtr;


	for(s1=protos->Tab,i=protos->NbPtr;--i;s1++)
	{
		for(s2=s1+1,j=i+1;--j;s2++)
		{
			double dist=(1-0)-Similarity(*s1,*s2);
			dist=dist*dist;
			if(dist<=mininter)
				mininter=dist;
		}
	}

	return(intra/mininter);
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReAllocate()
{
	GGroup * g;
	int i=0;
	for (protos->Start(); !protos->End(); protos->Next())  // groupscontaining one proto are created
	{
		grpstemp->InsertPtr(g=new GGroupVector(i,Groups->GetLang()));
		g->InsertPtr((*protos)());
		i++;
	}
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		GSubProfile* s=(SubProfiles)();
		if (protos->GetPtr(s)) continue;
		GSubProfile* parent, *proto;
		protos->Start();
		double cos=Similarity(s,(*protos)());
		parent=(*protos)();
		for (protos->Next(); !protos->End(); protos->Next())
		{
			proto=(*protos)();
			if (Similarity(s,proto)>cos)
			{
				cos=Similarity(s,proto);
				parent=proto;
			}
		}
		GGroup* gr=FindGroup(parent);
		gr->InsertSubProfile(s);
	}
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReCenter()
{
	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GSubProfile* tmp=(*grpstemp)()->RelevantSubProfile(GlobalSimi);
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
		sum=sum+Similarity(s,(*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for(sub1=grp->Tab,i=grp->NbPtr+1;--i;sub1++)
		sum=sum+Similarity(s,(*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::Similarity(GSubProfile * s1, GSubProfile* s2)
{
	return(ProfSim->GetSim(s1,s2));
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
double GALILEI::GGroupingKCos::TestMeasure(int k)
{
	double test=0.0;
	GSubProfile ** s1;
	unsigned int i;
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		GSubProfile* sub=SubProfiles();
		RContainer<GSubProfile,unsigned int,false,false> * kneigh;
		kneigh=new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
		while(k>0)
		{
			GSubProfile* neigh;
			double sim=-1.0;
			for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; --i; s1++)
				if ((!kneigh->GetPtr(*s1))&&(sub->GetId()!=(*s1)->GetId()))
					if (Similarity(*s1,sub)>sim)
					{
						sim=Similarity(*s1,sub);
						neigh=(*s1);
					}
			kneigh->InsertPtr(neigh);
			k--;
		}
		for (kneigh->Start(); !kneigh->End(); kneigh->Next())
		 	if((*kneigh)()->GetGroup()->GetId()==sub->GetGroup()->GetId())
				test++;
	}
	return(test);	
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
		GSubProfile* center= gr->RelevantSubProfile(GlobalSimi);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			double dist=(1.0)-Similarity((*gr)(),center);
			ssw=ssw+(dist*dist);
		}
		double dist2=(1.0)-Similarity(center,mean);
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
double GALILEI::GGroupingKCos::PalMeasure()
{
	double sum=0.0;
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GGroup* g= (*grpstemp)();
		double sumgr=0.0;
		for (g->Start(); !g->End(); g->Next())
			sumgr+=g->ComputeSumSim((*g)(), GlobalSimi);
		sum+=sumgr/g->NbPtr;
	}
	sum/=grpstemp->NbPtr;
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::Variance(RContainer<GGroup,unsigned int,false,false>* grs)
{
	double sum=0.0;
	for (grs->Start(); !grs->End(); grs->Next())
	{
		GGroup* g=(*grs)();
		for (g->Start(); !g->End(); g->Next())
			sum+=Similarity((*g)(), g->RelevantSubProfile(GlobalSimi));
	}
	return(sum);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::~GGroupingKCos(void)
{
	if(protos) delete protos;
	if(protoserror) delete protoserror;
	if(grpstemp) delete grpstemp;
	if(grpstemp2) delete grpstemp2;
	if(grpsfinal) delete grpsfinal;
}


