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
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupvector.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<sessions/gslot.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofilevector.h>

//include files for R
#include <rmath/random.h>
using namespace RMath;


using namespace GALILEI;




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
	char c, d,e, f, g, h , k;
	if(Random) c='1'; else c='0';
	if (FindGroupsNumber) d='1'; else d='0';
	if (GlobalSimi) e='1'; else e='0';
	if (Relevant) h='1'; else h='0';
	if (Scattered) k='1'; else k='0';

	 sprintf(tmp,"%c %u %u %c %c  %c, %c",c, IterNumber, NbTests, d, e, h, k);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::SetSettings(const char* s)
{
	char c, d, e, f,g, h, k;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %c %c %c %c %c",&c, &IterNumber, &NbTests, &d, &e, &f,  &h, &k);
	if(c=='1') Random=true; else Random=false;
	if(d=='1') FindGroupsNumber=true; else FindGroupsNumber=false;
	if(e=='1') GlobalSimi=true; else GlobalSimi=false;
	if(h=='1') Relevant=true; else Relevant=false;
	if(k=='1') Scattered=true; else Scattered=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Init(void) throw(bad_alloc)
{
	InitCenters();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Run(void) throw(GException)
{

	if (SubProfiles.NbPtr==0) return ;
	int grnumber;
	double var;

	// Calculates the similarities between subprofiles
	ProfSim= new GProfilesSim(SubProfiles, GlobalSimi);

	//temp
	int nbdiff=SubProfiles.NbPtr;
	GSubProfile **s1, **s2;
	unsigned int i,j;
	for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; --i;s1++)
	{
		int sim=0;
		for(s2=s1+1,j=i+1;--j;s2++)
		 {
			if ((Similarity(*s1,(*s2))>0.9999)&&(!sim))
			 {
				nbdiff--;
				sim=1;
			}
		}
	}
	cout <<" nbdiff profiles="<<nbdiff <<endl;

	//evaluate the number of groups
	EvaluateGroupsNumber();

	//temp   - display KmeansCosinus parameters
	DisplayInfos();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		cout <<" ------------------------------------------------------------------------ init " <<endl;
		if (Refined)
			Init();
		cout <<" ------------------------------------------------------------------------Run " <<endl;
		Execute(&SubProfiles, NbTests, false);

		
		// evaluate the clustering / each clutering for each number of group
		double finalcost=CostFunction(grpstemp2)/protos->NbPtr;
		double variance=GroupsVariance(grpstemp2);
		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final cost function: "<< finalcost<<endl;
		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final variance: "<< variance<<endl;

		if ((GroupsNumber==MinNbGroups)||(finalcost<var))
		{
			var=finalcost;
			grnumber=GroupsNumber;
			grpsfinal->Clear();
			for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
				grpsfinal->InsertPtr((*grpstemp2)());
	 	}
		cout << "index Calinsky "<< CalcCalinsky()<<endl;
		
      }
	for (grpsfinal->Start(); !grpsfinal->End(); grpsfinal->Next())
	{
		Groups->InsertPtr((*grpsfinal)());
	}
	cout << "----------------->>>>>>end final cost function: "<<var<<endl;
	cout << "----------------->>>>>>end final nbgroup: "<< grnumber<<endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::DisplayInfos(void)
{
	cout << " *** KMeansCos *** "<< endl;
	cout << "NbSubProfiles"<<SubProfiles.NbPtr<<endl;
	cout << "nombre de test= "<<NbTests <<endl;
	cout << "global similarity= "<<GlobalSimi<<endl;
	cout << " parameters max iter:"<<IterNumber<< " nb tests: "<<NbTests<<endl<<endl;
	cout << " *** initial parameters ***"<<endl;
	cout << " 	random :"<<  Random<<endl;
	cout << " 	scattered :"<< Scattered<<endl;
	cout << " 	relevant :"<< Relevant<<endl;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Execute(RContainer<GSubProfile, unsigned int, false, true>* dataset, unsigned int nbtests, bool init)
{
	Clear();
	grpstemp->Clear();
	double variances[GroupsNumber];
	double finalcost;
	srand((unsigned)time(NULL));   //enables the total random parameters

	

	for (unsigned int test=0; test<nbtests; test++)
	{
		cout << "**** test "<<test<<"****"<<endl;
		// initialization
		if (Random)
			RandomInitSubProfiles(&SubProfiles, GroupsNumber); // cycle
		//init variances
		for (int i=0; i<GroupsNumber; i++)
			variances[i]=0.0;
		unsigned int iter=0;
		int error=1;
		while ((iter<IterNumber)&&(error!=0))
		{
//			cout << "            iter "<<itermax<<endl;
			//Groups->Clear();
			grpstemp->Clear();

			//tmp
//			cout << "variances" <<endl;
//			for (int i=0; i<GroupsNumber; i++)
//				cout << variances[i]<< "   "<<endl;
//			cout <<endl;
			
			// (re-)allocation
//			cout << "reallocating..."<<endl;
			ReAllocate(dataset, variances, init);

			// Calulate the variances of each groups
			for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
				variances[(*grpstemp)()->GetId()]=GroupVariance((*grpstemp)());

			//recentering
//			cout << "recentering..."<<endl;
			ReCenter();

			// error calculation
			error=CalcError();
			iter++;
		}

		if (error==0) cout << "convergence"; else cout << "max iter reached ";
		double cost=CostFunction(grpstemp)/protos->NbPtr;
//		cout << "cost function = "<< cost<<endl;
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
	GSubProfile **s1, **s2;
	double intra=0.0, mininter=1.0;
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

	unsigned int i, j;
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
void  GALILEI::GGroupingKCos::ReAllocate(RContainer<GSubProfile,unsigned int,false,true>* dataset, double variances[], bool init)
{
	GGroup * g;
	int i=0;
	for (protos->Start(); !protos->End(); protos->Next())  // groupscontaining one proto are created
	{
		grpstemp->InsertPtr(g=new GGroupVector(i,Groups->GetLang()));
		g->InsertPtr((*protos)());
//		cout << "  proto   "<< (*protos)()->GetId();
		i++;
	}
//	cout <<endl;
	for (dataset->Start(); !dataset->End(); dataset->Next())
	{
		GSubProfile* s=(*dataset)();
		if (protos->GetPtr(s)) continue;
		GSubProfile* parent, *proto;
		protos->Start();
		double dist=Distance(s,(*protos)(), variances, init);
		parent=(*protos)();
		for (protos->Next(); !protos->End(); protos->Next())
		{
			proto=(*protos)();
			if (Distance(s,proto, variances, init)<dist)
			{
				dist=Distance(s,proto, variances, init);
				parent=proto;
			}
		}

		GGroup* gr=FindGroup(grpstemp, parent);
		gr->InsertSubProfile(s);
//		cout << "insert profile" << s->GetId() <<" in group"<<gr->GetId()<< " parent "<<parent->GetId() <<endl;
	}
		//tmp
//		ShowGroups(grpstemp);

}
//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKCos::ReCenter()
{
	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GSubProfile* tmp=(*grpstemp)()->RelevantSubProfile(GlobalSimi);
		protoserror->InsertPtr(tmp);
//		cout << "new proto " << tmp->GetId() << "from group "<< (*grpstemp)()->GetId()<<endl;
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKCos::Distance(GSubProfile *s1, GSubProfile *proto, double variances[], bool init)
{
	double newdist, dist, variance;
	GGroup* gr;

	gr=FindGroup(grpstemp, proto);
	dist=(1-Similarity(s1,proto));
	variance= variances[gr->GetId()];
	if(variance==0.0||init==true)
		return(dist);
	newdist=0.5*grpstemp->NbPtr*log(variance);
	newdist+=0.5*dist*dist/variance;
	cout<< "newdist carré = "<<newdist<<endl;
	return(newdist);	
	
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
//	cout << "error : "<< err<<endl;
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
GALILEI::GGroupingKCos::~GGroupingKCos(void)
{
	if(protoserror) delete protoserror;
	if(grpstemp) delete grpstemp;
	if(grpstemp2) delete grpstemp2;
	if(grpsfinal) delete grpsfinal;
}


