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
#include<groups/ggrouping.h>
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
//  GGroupingKMeans
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKMeans::GGroupingKMeans(GSession* s, const char* n) throw(bad_alloc)
		:  GGrouping(n,s), Random(false),  Relevant(false), Scattered(false), Refined(true), IterNumber(10), NbTests(1),
			FindGroupsNumber(false), MaxNbGroups(13), MinNbGroups(13), GlobalSimi(false),
				protos(0), grpstemp2(0)
{
	Epsilon= 0.0005;
	protos = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	grpstemp2 = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	Rand = new RRandomGood(1) ;
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingKMeans::IsValidProto(RContainer<GSubProfile,unsigned int,false,false>* prototypes,GSubProfile* s) throw(bad_alloc)
{
	if (prototypes->GetPtr(s))
		return(false);
	for (prototypes->Start(); !prototypes->End(); prototypes->Next())
	 {
		double sim=Similarity(s, (*prototypes)());
		if ((sim<=1+Epsilon)&&(sim>=1-Epsilon))
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::RelevantInitSubProfiles(unsigned int nbsubs)
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
void GALILEI::GGroupingKMeans::ScatteredInitSubProfiles(unsigned int nbsubs)
{
	int k=0;
	GSubProfile* sub;
	GGroup* g=((GGroup*)new GGroupVector(0,Groups->GetLang()));
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
		g->InsertPtr(SubProfiles());
	sub = g->RelevantSubProfile(GlobalSimi);
	protos->InsertPtr(sub);
	while (k<nbsubs-1)
	{
		SubProfiles.Start();
		GSubProfile* subprof;
		double refsum=Similarity((SubProfiles)(), sub);;
		subprof=(SubProfiles)();
		for (SubProfiles.Next();!SubProfiles.End(); SubProfiles.Next())
		{
			if (!protos->GetPtr((SubProfiles)()))
			{
				double sum=Similarity((SubProfiles)(), sub);
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
		MinNbGroups=13;
		MaxNbGroups=13;//SubProfiles.NbPtr-1;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKMeans::RandomInitSubProfiles(RContainer<GSubProfile,unsigned int,false,true>* dataset,unsigned int nbgroups)
{
	protos->Clear();
	while (protos->NbPtr!=nbgroups)
	{
		unsigned int u = Rand->Value(dataset->NbPtr);
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
	if (Random)
		RandomInitSubProfiles(&SubProfiles,GroupsNumber);
	if (Relevant)
		RelevantInitSubProfiles(GroupsNumber);
	if (Scattered)
		ScatteredInitSubProfiles(GroupsNumber);
	if(Refined)
		RefiningCenters(3,100);
}

//-----------------------------------------------------------------------------
double GALILEI::GGroupingKMeans::SumSimilarity(GSubProfile * s)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for (sub1=SubProfiles.Tab,i=SubProfiles.NbPtr;--i;sub1++)
		sum=sum+Similarity(s,(*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKMeans::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
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
void GALILEI::GGroupingKMeans::RefiningCenters(unsigned int nbsub, int level)
{
	unsigned int nbsubprofpersubsamp = int (SubProfiles.NbPtr*level/100);
	RContainer<GGroup,unsigned int,false,false>* subsamples ;
	subsamples= new RContainer<GGroup,unsigned int,false,false> (10,5);
	RContainer<GGroup,unsigned int,false,false>* initialcenters ;
	initialcenters= new RContainer<GGroup,unsigned int,false,false> (20,10);
	GGroupVector* allcenters= new GGroupVector(0, Groups->GetLang());;
	for (int k=0; k<nbsub; k++)
	{
		RContainer<GSubProfile,unsigned int,false,false>* startingprotos ;
		startingprotos = new RContainer<GSubProfile,unsigned int,false,false> (10,5);
//		cout << "subsample "<< k <<endl;
		GGroupVector* gr= new GGroupVector (k,Groups->GetLang());
		subsamples->InsertPtr(gr);
		GGroup* g = static_cast <GGroup*> (gr);
		while (g->NbPtr<nbsubprofpersubsamp)
		{
			int r=int(Rand->Value(SubProfiles.NbPtr));
			GSubProfile* sub = SubProfiles.GetPtrAt(r);
			g->InsertSubProfile(sub);
		}

		//init the clustering over the subsample
		RandomInitSubProfiles(gr, GroupsNumber);
		for (protos->Start(); !protos->End(); protos->Next())
			startingprotos->InsertPtr((*protos)());
		

		// executes the clustering
//		cout << "executing kmeans on subsample of " << ((GGroup*)gr)->NbPtr<<" elements"<<endl;
		bool okkmeans=false;
		while( okkmeans==false)
		{
			cout << "ok means false "<<endl;
			Execute(gr, 1, 1);
			okkmeans=VerifyKMeansMod(startingprotos, gr);
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
	double cost;
;
	int finalcentersid;
	for (int i=0; i<nbsub; i++)
	{
		protos->Clear();
		GGroup* initialcenter = initialcenters->GetPtr(i);
		// initializing the proto with a set of found initial centers
		for  (initialcenter->Start(); !initialcenter->End(); initialcenter->Next())
		   protos->InsertPtr((*initialcenter)());
		// excutes a kmeans over allcenters with this initial centers
		Execute(allcenters, 1, true);
		// evaluate the distortion
		double subcostfunction = GroupsVariance(grpstemp2);
//		cout << " distortion for this inital centers "<< i<< "  "<<subcostfunction <<endl;
		if ((i==0)||(subcostfunction>cost))
		{
			cost = subcostfunction;
			finalcentersid=i;
		}
	}
	// set the final intial centers as prototypes.
	protos->Clear();
	GGroup* g = initialcenters->GetPtr(finalcentersid);	
	for (g->Start(); !g->End(); g->Next())
		protos->InsertPtr((*g)());

//	for (protos->Start(); !protos->End(); protos->Next())
//			cout << "	end final initial points "<< (*protos)()->GetId()<<endl;
}


//-----------------------------------------------------------------------------
bool  GALILEI::GGroupingKMeans::VerifyKMeansMod(RContainer<GSubProfile,unsigned int,false,false>* startingprotos, GGroup*)
{
	RContainer<GSubProfile,unsigned int,false,false>* wrongprotos;
	wrongprotos= new RContainer<GSubProfile,unsigned int,false,false>(10,5);
	RContainer<GSubProfile,unsigned int,false,false>* farthestsubprofiles;
	farthestsubprofiles = new RContainer<GSubProfile,unsigned int,false,false> (10,5);

	// check for 'empty' clusters.
	for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
		if ((*grpstemp2)()->NbPtr==1)
			wrongprotos->InsertPtr((*grpstemp2)()->RelevantSubProfile(GlobalSimi));

	for (wrongprotos->Start(); !wrongprotos->End(); wrongprotos->Next())
		protos->DeletePtr((*wrongprotos)());
	while (protos->NbPtr<GroupsNumber)
	{
		int r=int(Rand->Value(SubProfiles.NbPtr));
//		cout <<" hasard "<< r <<endl;
		GSubProfile* sub=SubProfiles.GetPtrAt(r);
		if (IsValidProto(protos, sub))
			protos->InsertPtr(sub);
	}
	cout << "protos test = ";
	for (protos->Start(); !protos->End(); protos->Next())
		cout << (*protos)()->GetId()<< "   ";
	cout <<endl<<endl<<endl;

	return(wrongprotos->NbPtr==0);

}

//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKMeans::GroupsVariance(RContainer<GGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
{
	double var=0;
	// calculation of variance
	for (grps->Start(); !grps->End(); grps->Next())
	{
		GGroup* gr = (*grps)() ;
		GSubProfile* proto= gr->RelevantSubProfile(GlobalSimi);
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
	GSubProfile* center=grp->RelevantSubProfile(GlobalSimi);
	for (grp->Start(); !grp->End(); grp->Next())
		variance+=(1-Similarity(center, (*grp)()))*(1-Similarity(center, (*grp)()));
	cout << " variance group = "<< variance/grp->NbPtr;
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
void GALILEI::GGroupingKMeans::ShowGroups(RContainer<GGroup,unsigned int,false,false>* grs)
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


