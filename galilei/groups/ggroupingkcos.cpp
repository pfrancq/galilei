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
//#include <groups/gevaluategroupsnumber.h>
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
#include <rgga/rgroupingkmeans.h>
using namespace RMath;
using namespace RGGA;


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//
//  GKMeansParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GKMeansParams::GKMeansParams(void)
	: GGroupingParams("KMeansCosinus")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GKMeansParams::GetSettings(void)
{
	static char tmp[200];

	sprintf(tmp,"%u %u %u %u %f", NbGroups, NbIters, NbSubSamples, SubSamplesRate, Epsilon);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GKMeansParams::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u %u %u %lf", &NbGroups, &NbIters, &NbSubSamples, &SubSamplesRate, &Epsilon) ;
}


//-----------------------------------------------------------------------------
//
//  GGroupingKCos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::GGroupingKCos( GSession* s, GKMeansParams* p) throw(bad_alloc)
		: GGrouping("KMeansCosinus",s), Params(p)
{
}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKCos::GetSettings(void)
{

	char c;
	static char tmp[250];
	if(Params->Init==GKMeansParams::Refined) c='1';

	sprintf(tmp,"%c %u %u %u %u  %f",c, Params->NbIters, Params->NbGroups, Params->NbSubSamples, Params->SubSamplesRate,   Params->Epsilon);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %u %u  %lf",&c, &(Params->NbIters), &(Params->NbGroups), &(Params->NbSubSamples), &(Params->SubSamplesRate),   &(Params->Epsilon));
	if(c=='1') Params->Init=GKMeansParams::Refined ;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKCos::Run(void) throw(GException)
{
//	unsigned int i;
	if (!SubProfiles.NbPtr)  return;
	GGroup* g,*gr;
	RContainer<GGroup,unsigned int,false,false>* grps;
//	for (i=13; i<14; i++)
//	{
		RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups>* kmeans;
		kmeans=new  RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups> (&SubProfiles) ;
		// init parameters of kmeans
		kmeans->SetNbTests(1);
		kmeans->SetGroupsNumber(Params->NbGroups);
		kmeans->SetInitial(RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups>::Refined);
		kmeans->SetEpsilon(Params->Epsilon);
		kmeans->SetIterNumber(Params->NbIters);
		kmeans->SetSubSamplesNumber(Params->NbSubSamples);
		kmeans->SetSubSamplesRate(Params->SubSamplesRate);
		kmeans->SetVerifyKMeansMaxIters(1000);
		kmeans->SetGroupsNumber(Params->NbGroups);

		// run the kmeans
	 	kmeans->Run();
		grps=kmeans->GetGrouping();
		RContainer<GGroup,unsigned int,false,true>* grps2;
		grps2=new  RContainer<GGroup,unsigned int,false,true>(10,5);
		for (grps->Start(); !grps->End(); grps->Next())
			grps2->InsertPtr((*grps)());
//		GEvaluateGroupsNumber* eval;
//		eval=new GEvaluateGroupsNumber(grps2,Session);
//		cout << eval->CalculateSDbw()<<","<<endl;

		// save the grouping in the session
		for (grps->Start(); !grps->End(); grps->Next())
		{
			gr=(*grps)();
			g=new GGroupVector(gr->GetId(), Groups->GetLang());
			for (gr->Start(); !gr->End(); gr->Next())
				g->InsertPtr((*gr)());
			Groups->InsertPtr(g);
		}

		delete kmeans;
		delete grps2;
//		delete eval;
//	}


}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKCos::~GGroupingKCos(void)
{

}


