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
#include <math.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingkcos.h>
#include<groups/ggroupingsim.h>
#include <profiles/gprofilessim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupvector.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<sessions/gslot.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofilevector.h>
#include <groups/gcomparegrouping.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//include files for R
#include <rmath/random.h>
using namespace RMath;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//
//  GGroupingKCos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::GGroupingKCos( GSession* s) throw(bad_alloc)
		: GGroupingKMeans(s,"KMeansCosinus"), protoserror(0), grpstemp(0), grpsfinal(0)
{
	protoserror = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	grpstemp = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	grpsfinal = new RContainer<GGroup,unsigned int,false,false>  (10, 5);

}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKCos::GetSettings(void)
{

	static char tmp[250];
	char c, d,e, h , k;
	if(initial==Random) c='1'; else c='0';
	if (initial==FindGroupsNumber) d='1'; else d='0';
	if (initial==GlobalSim) e='1'; else e='0';
	if (initial==Relevant) h='1'; else h='0';
	if (initial==Scattered) k='1'; else k='0';

	sprintf(tmp,"%c %u %u %c %c  %c, %c",c, IterNumber, NbTests, d, e, h, k);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::SetSettings(const char* s)
{
	char c, d,e,  h, k;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %c %c %c %c",&c, &IterNumber, &NbTests, &d, &e,   &h, &k);
	if(c=='1') initial=Random;
	if(d=='1') initial=Refined;
	if(e=='1') GlobalSim=true; else GlobalSim=false;
	if(h=='1') initial=Relevant;
	if(k=='1') initial=Scattered;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Init(void) throw(bad_alloc)
{
	InitCenters();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Run(void) throw(GException)
{
	int grnumber;
	double var;

	if (SubProfiles.NbPtr==0) return ; 

	// Calculates the similarities between subprofiles
	ProfSim= new GProfilesSim(SubProfiles, GlobalSim);

	//evaluate the number of groups
	EvaluateGroupsNumber();

	//temp   - display KmeansCosinus parameters
//	DisplayInfos();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
//		cout <<" ------------------------------------------------------------------------ init " <<endl;
		if (initial==Refined)
			Init();
//		cout <<" ------------------------------------------------------------------------Run " <<endl;

		Execute(&SubProfiles, NbTests);

		// evaluate the clustering / each clutering for each number of group
		double finalcost=CostFunction(grpstemp2)/protos->NbPtr;
//		double distortion=Distortion(grpstemp2);
//		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final cost function: "<< finalcost<<endl;
//		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final distortion: "<< distortion<<endl;

		if ((GroupsNumber==MinNbGroups)||(finalcost<var))
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
	cout << "----------------->>>>>>end final cost function: "<<var<<endl;
	cout << "----------------->>>>>>end final nbgroup: "<< grnumber<<endl;
}


//-----------------------------------------------------------------------------
//void GALILEI::GGroupingKCos::Run(void) throw(GException)
//{
//	unsigned int i;
//	RRandomGood* r;
// 	GGroupDataIR data;
//	Objs=new RGA::RObjs<GObjIR>(SubProfiles.NbPtr);
//	for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
//	{
//		Objs->InsertPtr(new GObjIR(i,SubProfiles()));
//	}
//	GProfilesSim Sims(SubProfiles,GlobalSim);
//	r=new RRandomGood(0);
//	RGroupingKMeans<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* kmeans;
//	kmeans=new  RGroupingKMeans<GGroupIR,GObjIR,GGroupDataIR,GChromoIR> (r, Objs) ;
//	kmeans->Run();
//}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::DisplayInfos(void)
{
	cout << " *** KMeansCos *** "<< endl;
	cout << "NbSubProfiles"<<SubProfiles.NbPtr<<endl;
	cout << "nombre de test= "<<NbTests <<endl;
	cout << "global similarity= "<<GlobalSim<<endl;
	cout << " parameters max iter:"<<IterNumber<< " nb tests: "<<NbTests<<endl<<endl;
	cout << " *** initial parameters ***"<<endl;
	cout << "initial condi :"<< initial<<endl;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Execute(RContainer<GSubProfile, unsigned int, false, true>* dataset, unsigned int nbtests)
{
	double cost, finalcost;
	unsigned int iter, error;

	srand((unsigned)time(NULL));   //enables the total random parameters
	Clear();
	grpstemp->Clear();

	for (unsigned int test=0; test<nbtests; test++)
	{
		// initialization
		if (initial==Random)
			RandomInitSubProfiles(&SubProfiles, GroupsNumber); // cycle
		iter=0;
		error=1;
		while ((iter<IterNumber)&&(error!=0))
		{
//			cout << "            iter "<<itermax<<endl;
			//Groups->Clear();
			grpstemp->Clear();

			// (re-)allocation
			ReAllocate(dataset);

			//recentering
			ReCenter();

			// error calculation
			error=CalcError();
			iter++;
		}
		cost=CostFunction(grpstemp)/protos->NbPtr;
		if ((test==0)||(cost< finalcost))
		{
			grpstemp2->Clear();
			for (grpstemp->Start();!grpstemp->End(); grpstemp->Next())
				grpstemp2->InsertPtr((*grpstemp)());
			finalcost=cost;
		}
	}
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKCos::CostFunction(RContainer<GGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
{
	unsigned int i, j;
	double intra=0.0, mininter=1.0;
	GSubProfile **s1, **s2;

	for (protos->Start(); !protos->End(); protos->Next())
	{
		GSubProfile* proto = (*protos)();
		GGroup* gr=FindGroup(grps,proto);
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
			double dist=(1.0)-Similarity(*s1,*s2);
			dist=dist*dist;
			if(dist<mininter)
				mininter=dist;
		}
	}
	return(intra/mininter);
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReAllocate(RContainer<GSubProfile,unsigned int,false,true>* dataset)
{
	int i=0;
	GGroup * g;

	for (protos->Start(); !protos->End(); protos->Next())  // groupscontaining one proto are created
	{
		grpstemp->InsertPtr(g=new GGroupVector(i,Groups->GetLang()));
		g->InsertPtr((*protos)());
		i++;
	}

	for (dataset->Start(); !dataset->End(); dataset->Next())
	{
		GSubProfile* s=(*dataset)();
		if (protos->GetPtr(s)) continue;
		GSubProfile* parent, *proto;
		protos->Start(); proto=(*protos)();
		double dist=Distance(s,proto);
		parent=(*protos)();
		for (protos->Next(); !protos->End(); protos->Next())
		{
			proto=(*protos)();
			if (Distance(s,proto)<dist)
			{
				dist=Distance(s,proto);
				parent=proto;
			}
		}
		GGroup* gr=FindGroup(grpstemp, parent);
		gr->InsertSubProfile(s);
	}
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReCenter()
{
	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GSubProfile* tmp=(*grpstemp)()->RelevantSubProfile(GlobalSim);
		protoserror->InsertPtr(tmp);
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::Distance(GSubProfile *s1, GSubProfile *proto)
{
	double dist;
	dist=(1-Similarity(s1,proto));

	return(dist);
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
double GALILEI::GGroupingKCos::TestMeasure()
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::CalcCalinsky()
{
	GSubProfile* mean;
	double refsum, sum, dist, dist2,cal;
	double ssw=0.0, ssb=0.0;
	int k, n;

	SubProfiles.Start();
	refsum=SumSimilarity((SubProfiles)());
	mean=(SubProfiles)();
	for (SubProfiles.Next();!SubProfiles.End(); SubProfiles.Next())
	{
		sum=SumSimilarity((SubProfiles)());
		if (sum>refsum)
		{
			refsum=sum;
			mean=(SubProfiles)();
		}
	}

	// ssw & ssb calculation
	for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
	{
		GGroup* gr=(*grpstemp2)();
		GSubProfile* center= gr->RelevantSubProfile(GlobalSim);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			dist=(1.0)-Similarity((*gr)(),center);
			ssw=ssw+(dist*dist);
		}
		dist2=(1.0)-Similarity(center,mean);
		ssb=ssb+((gr->NbPtr)*(dist2*dist2));
	}

	//index calculation
	k=GroupsNumber;
	n=SubProfiles.NbPtr;
	cal=(ssb/(k-1))/(ssw/(n-k));
	return(cal);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::~GGroupingKCos(void)
{
	if(protoserror) delete protoserror;
	if(grpstemp) delete grpstemp;
	if(grpstemp2) delete grpstemp2;
	if(grpsfinal) delete grpsfinal;
}


