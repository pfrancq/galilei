                                                                                                                                                                                                                                                                                                                                                        /*

	GALILEI Research Project

	GGroupingKMeans.cpp

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
#include<groups/ggroupingkmeans.h>
#include <groups/ggroupingsim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupvector.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<sessions/gslot.h>
#include<profiles/gprofile.h>
#include <profiles/gprofilessim.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupingKMeans
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKMeans::GGroupingKMeans(GSession* s, const char* n) throw(bad_alloc)
		:  GGrouping(n,s), IterNumber(10), NbTests(1), FindGroupsNumber(false), MaxNbGroups(13),
				 MinNbGroups(13), GlobalSim(false),protos(0), grpstemp2(0)
{
	Epsilon= 0.0005;
	protos = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	grpstemp2 = new RContainer<GGroup,unsigned int,false,false>  (10, 5);

	Rand = new RRandomGood(1) ;
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingKMeans::IsValidProto(RContainer<GSubProfile,unsigned int,false,false>* prototypes,GSubProfile* s) throw(bad_alloc)
{
	double sim;

	if (prototypes->GetPtr(s))
		return(false);
	for (prototypes->Start(); !prototypes->End(); prototypes->Next())
	 {
		sim=Similarity(s, (*prototypes)());
		if ((sim<=1+Epsilon)&&(sim>=1-Epsilon))
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::RelevantInitSubProfiles(int nbsubs)
{
	double sum, refsum;
	GSubProfile* subprof;
	for (int i=0; i<nbsubs; i++)
	{
		SubProfiles.Start();
		refsum=SumSimilarity((SubProfiles)());;
		subprof=(SubProfiles)();

		for (SubProfiles.Next();!SubProfiles.End(); SubProfiles.Next())
		{
			if (!protos->GetPtr((SubProfiles)()))
			{
				sum=SumSimilarity((SubProfiles)());
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
void GALILEI::GGroupingKMeans::ScatteredInitSubProfiles(int nbsubs)
{
	int k=0;
	double sum, refsum;
	GSubProfile* sub;
	GGroup* g=((GGroup*)new GGroupVector(0,Groups->GetLang()));
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
		g->InsertPtr(SubProfiles());
	sub = g->RelevantSubProfile(GlobalSim);
	protos->InsertPtr(sub);
	while (k<nbsubs-1)
	{
		SubProfiles.Start();
		GSubProfile* subprof;
		refsum=Similarity((SubProfiles)(), sub);;
		subprof=(SubProfiles)();
		for (SubProfiles.Next();!SubProfiles.End(); SubProfiles.Next())
		{
			if (!protos->GetPtr((SubProfiles)()))
			{
				sum=Similarity((SubProfiles)(), sub);
				if (sum<refsum)
				{
					refsum=sum;
					subprof=(SubProfiles)();
				}
			}
		}
		protos->InsertPtr(subprof);
		k++;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::EvaluateGroupsNumber()
{
	if (FindGroupsNumber==0)
	{
		MinNbGroups=GroupsNumber;
		MaxNbGroups=GroupsNumber;//SubProfiles.NbPtr-1;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::RandomInitSubProfiles(RContainer<GSubProfile,unsigned int,false,true>* dataset, unsigned int nbgroups)
{
	int u;
	protos->Clear();
	while (protos->NbPtr!=nbgroups)
	{
		u = Rand->Value(dataset->NbPtr);
//		int u = int(rand()%(dataset->NbPtr));
		GSubProfile* tmp=dataset->GetPtrAt(u);
		 if (IsValidProto(protos, tmp))
			protos->InsertPtr(tmp);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::InitCenters(void) throw(bad_alloc)
{
	protos->Clear();
	if (initial==Random)
		RandomInitSubProfiles(&SubProfiles,GroupsNumber);
	if (initial==Relevant)
		RelevantInitSubProfiles(GroupsNumber);
	if (initial==Scattered)
		ScatteredInitSubProfiles(GroupsNumber);
	if(initial==Refined)
		RefiningCenters(15,80);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKMeans::SumSimilarity(GSubProfile * s)
{
	double sum=0.0;
	unsigned i;
	GSubProfile** sub1;

	for (sub1=SubProfiles.Tab,i=SubProfiles.NbPtr;--i;sub1++)
		sum=sum+Similarity(s,(*sub1));

	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKMeans::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	unsigned i;
	GSubProfile** sub1;

	for(sub1=grp->Tab,i=grp->NbPtr+1;--i;sub1++)
		sum=sum+Similarity(s,(*sub1));

	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKMeans::Similarity(GSubProfile * s1, GSubProfile* s2)
{
	return(ProfSim->GetSim(s1,s2));
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::RefiningCenters(int nbsub, int level)
{
	int r, finalcentersid;;
	bool okkmeans;
	double cost, subcostfunction;
	GGroup* g;
	GGroupVector* gr;
	unsigned int nbsubprofpersubsamp = static_cast <int> (SubProfiles.NbPtr*level/100);
	RContainer<GGroup,unsigned int,false,false>* subsamples ;
	RContainer<GGroup,unsigned int,false,false>* initialcenters ;

	GGroupVector* allcenters= new GGroupVector(0, Groups->GetLang());
	subsamples= new RContainer<GGroup,unsigned int,false,false> (10,5);
	initialcenters= new RContainer<GGroup,unsigned int,false,false> (20,10);

	for (int k=0; k<nbsub; k++)
	{
		RContainer<GSubProfile,unsigned int,false,false>* startingprotos ;
		startingprotos = new RContainer<GSubProfile,unsigned int,false,false> (10,5);
		gr= new GGroupVector (k,Groups->GetLang());
		subsamples->InsertPtr(gr);
		g = static_cast <GGroup*> (gr);
		while (g->NbPtr<nbsubprofpersubsamp)
		{
			r=int(Rand->Value(SubProfiles.NbPtr));
			GSubProfile* sub = SubProfiles.GetPtrAt(r);
			g->InsertSubProfile(sub);
		}

		//init the clustering over the subsample
		RandomInitSubProfiles(gr, GroupsNumber);
		for (protos->Start(); !protos->End(); protos->Next())
			startingprotos->InsertPtr((*protos)());

		// executes the clustering
//		cout << "executing kmeans on subsample of " << ((GGroup*)gr)->NbPtr<<" elements"<<endl;
		okkmeans=false;
		while( okkmeans==false)
		{
//			cout << "ok means false "<<endl;
			Execute(gr, 1);
			okkmeans=VerifyKMeansMod();
		}
//		for (protos->Start(); !protos->End(); protos->Next())
//			cout << "	final proto "<< (*protos)()->GetId()<<endl;
            // save the centers
		GGroupVector* centers= new GGroupVector (k,Groups->GetLang());
		initialcenters->InsertPtr(centers);
		for (protos->Start(); !protos->End(); protos->Next())
		{
			(centers)->InsertSubProfile((*protos)());
			(allcenters)->InsertSubProfile((*protos)());
		}
//		cout <<" nombre de centres en tout: "<<((GGroup*)allcenters)->NbPtr <<endl<<endl;
	}

	// executes a KMeans over all the estimates of the centroids
	for (int i=0; i<nbsub; i++)
	{
		protos->Clear();
		GGroup* initialcenter = initialcenters->GetPtr(i);
		// initializing the proto with a set of found initial centers
		for  (initialcenter->Start(); !initialcenter->End(); initialcenter->Next())
		   protos->InsertPtr((*initialcenter)());
		// excutes a kmeans over allcenters with this initial centers
		Execute(allcenters, 1);
		// evaluate the distortion
		subcostfunction = Distortion(grpstemp2);
		if ((i==0)||(subcostfunction>cost))
		{
			cost = subcostfunction;
			finalcentersid=i;
		}
	}
	// set the final intial centers as prototypes.
	protos->Clear();
	g = initialcenters->GetPtr(finalcentersid);
	for (g->Start(); !g->End(); g->Next())
		protos->InsertPtr((*g)());

}


//-----------------------------------------------------------------------------
bool  GALILEI::GGroupingKMeans::VerifyKMeansMod()
{
	int r;
	RContainer<GSubProfile,unsigned int,false,false>* wrongprotos;
	wrongprotos= new RContainer<GSubProfile,unsigned int,false,false>(10,5);
	RContainer<GSubProfile,unsigned int,false,false>* farthestsubprofiles;
	farthestsubprofiles = new RContainer<GSubProfile,unsigned int,false,false> (10,5);

	// check for 'empty' clusters.
	for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
		if ((*grpstemp2)()->NbPtr==1)
			wrongprotos->InsertPtr((*grpstemp2)()->RelevantSubProfile(GlobalSim));

	for (wrongprotos->Start(); !wrongprotos->End(); wrongprotos->Next())
		protos->DeletePtr((*wrongprotos)());
	while (protos->NbPtr<GroupsNumber)
	{
		r=int(Rand->Value(SubProfiles.NbPtr));
		GSubProfile* sub=SubProfiles.GetPtrAt(r);
		if (IsValidProto(protos, sub))
			protos->InsertPtr(sub);
	}

	return(wrongprotos->NbPtr==0);

}

//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKMeans::Distortion(RContainer<GGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
{
	double var=0;
	// calculation of variance
	for (grps->Start(); !grps->End(); grps->Next())
	{
		GGroup* gr = (*grps)() ;
		GSubProfile* proto= gr->RelevantSubProfile(GlobalSim);
		for (gr->Start(); !gr->End(); gr->Next())
		{
			var+=Similarity((*gr)(), proto);
		}
	}
	return(var);
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKMeans::GroupVariance(GGroup* grp)     // calculates the intra/min(inter)
{
	double variance=0.0;
	GSubProfile* center=grp->RelevantSubProfile(GlobalSim);
	for (grp->Start(); !grp->End(); grp->Next())
		variance+=(1-Similarity(center, (*grp)()))*(1-Similarity(center, (*grp)()));
	return(variance/grp->NbPtr);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroupingKMeans::FindGroup(RContainer<GGroup,unsigned int,false,false>* grps, GSubProfile* s)
{
	for (grps->Start(); !grps->End(); grps->Next())
		if( (*grps)()->GetPtr(s))
			return((*grps)());
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::ShowGroups(RContainer<GGroup,unsigned int,true,true>* grs)
{
	GGroup* g;
	for (grs->Start(); !grs->End(); grs->Next())
	{
		g=(*grs)();
		cout << " Groupe " << g->GetId()<<endl;
		for (g->Start(); !g->End(); g->Next())
			cout << "subprof "<<(*g)()->GetId();
		cout <<endl;
	}
	cout <<endl <<endl;
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKMeans::~GGroupingKMeans(void)
{
	if(protos) delete protos;
}


