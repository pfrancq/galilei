                                                                                                                                                                                                                                                                                                                                                        /*

	GALILEI Research Project

	GGroupingCure.cpp

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
#include <math.h>

//-----------------------------------------------------------------------------
//include files for test

#include <rio/rtextfile.h>          // 		!a supprimer
using namespace RIO;
//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingcure.h>
#include <groups/ggroupingsim.h>
#include <groups/gcomparegrouping.h>
//#include <groups/gevaluategroupsnumber.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupvector.h>
#include<langs/glang.h>
#include<sessions/gsession.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofilevector.h>
#include <profiles/gprofilessim.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GCureParameters
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GCureParams::GCureParams(void)
	: GGroupingParams("CURE")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GCureParams::GetSettings(void)
{
	static char tmp[200];

	sprintf(tmp,"%u %u", NbGroups, NbProtos);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GCureParams::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u", &NbGroups, &NbProtos) ;
}


//-----------------------------------------------------------------------------
//
//  GGroupingCure
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GGroupingCure::GGroupingCure(GSession* s, GCureParams* p) throw(bad_alloc)
		:  GGrouping("CURE",s), Params(p)
{
	Prototypes = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	Grps = new  RContainer<GGroup,unsigned int,false,true> (10, 5);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingCure::GetSettings(void)
{
	static char tmp[250];
	sprintf(tmp,"%u %u",Params->NbGroups, Params->NbProtos);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingCure::SetSettings(const char* s)
{

	if(!(*s)) return;
	sscanf(s,"%u %u",&Params->NbGroups, &Params->NbProtos);
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingCure::InitGroups(void)
{
	int i;
	GGroup* newgroup;

	// create groups containing one subprofile
	for (i=0,SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next(),i++)
	{
		newgroup=new GGroupVector(i,SubProfiles()->GetLang());
		newgroup->InsertPtr(SubProfiles());
		Grps->InsertPtr(newgroup);
	}
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingCure::InitProtos(void)
{
	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
		Prototypes->InsertPtr(SubProfiles());
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingCure::DisplayInfos(void)
{
	cout << " *** CURE *** "<< endl;
	cout << "NbSubProfiles "<<SubProfiles.NbPtr<<endl;
	cout << "NbGroups wanted:  "<<Params->NbGroups<<endl;
	cout << "Number of Prototypes per groups: "<<Params->NbProtos<<endl;
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingCure::Run(void) throw(GException)
{
	unsigned int i,j;
	double dist;
	GGroup *gr1, *gr2,**g1, **g2;

	if (SubProfiles.NbPtr==0) return;
	DisplayInfos();
	InitGroups();
	InitProtos();

	while (Grps->NbPtr>Params->NbGroups)
	{
		// find the two closest groups.
//		cout << "calculating closest groups ....."<<endl;
		dist=2.0;
		for(g1=Grps->Tab,i=Grps->NbPtr;--i;g1++)
			for(g2=g1+1,j=i+1;--j;g2++)
			{
				double gdist=GroupsDistance((*g1),(*g2));
				if(gdist<dist)
				{
					dist=gdist;
					gr1=(*g1);
					gr2=(*g2);
				}
			}
//		cout << "closest groups found : " << gr1->GetId() << "and "<< gr2->GetId() <<endl;
		//merge the groups
		MergeGroups(gr1, gr2);
//		cout << "number of group at this time = " << Grps->NbPtr<<endl;
//		GEvaluateGroupsNumber* eval= new GEvaluateGroupsNumber(Grps,Session);
//		cout<< eval->CalculateSDbw()<<","<<endl;
//		delete eval;
	}

	Groups->Clear();
	for (Grps->Start(); !Grps->End(); Grps->Next())
		Groups->InsertPtr((*Grps)());
//	cout <<endl;	
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingCure::MergeGroups(GGroup* g1, GGroup* g2)
{
//	cout << "merging groups ....."<<endl;

	//add elements of g2 in g1  & remove g2 prototypes from protos container.
	for (g2->Start(); !g2->End(); g2->Next())
	{
		GSubProfile* toadd= (*g2)();
		g1->InsertPtr(toadd);
		if (Prototypes->GetPtr(toadd))
			Prototypes->DeletePtr(toadd);
	}
	// create the protos for the new g1
	CreateGroupsPrototypes(g1);

	//remove g2 from Grps
	Grps->DeletePtr(g2);

//	cout << "groups merged ....."<<endl;
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingCure::CreateGroupsPrototypes(GGroup* g)
{
//	cout << "creating new protos ...."<<endl;

	double mindist;
	double maxdist=-1.0;
	GSubProfile *temppoint, *maxpoint;
	RContainer<GSubProfile,unsigned int,true,true>* protos;
	protos=new RContainer<GSubProfile,unsigned int,true,true> (10,5);
	GSubProfile* mean=g->RelevantSubProfile(false);

	if (g->NbPtr<=Params->NbProtos+1)
		for (g->Start(); !g->End(); g->Next())
			protos->InsertPtr((*g)());
	else
	{
		while(protos->NbPtr<Params->NbProtos)
		{
			mindist=-1.0;
			for (g->Start(); !g->End(); g->Next())
			{
				if (protos->GetPtr((*g)())) continue;
				if (protos->NbPtr==0)
				{
					// find the farthest subprofile from the relevant.
					mindist=SubProfDistance(mean,(*g)());
					temppoint=(*g)();
				}
				else
				{
					// find the farthest subprofiles from the already chosen subprofiles
					protos->Start();
					double temp=SubProfDistance((*protos)(), (*g)());
					temppoint=(*g)();
					for (protos->Next(); !protos->End(); protos->Next())
						if (SubProfDistance((*protos)(), (*g)())<temp)
						{
							temp=SubProfDistance((*protos)(), (*g)());
							temppoint=(*g)();
						}
				}
				if (mindist>maxdist)
				{
					maxdist=mindist;
					maxpoint=temppoint;
				}
			}
			protos->InsertPtr(temppoint);
		}
	}

	for (protos->Start(); !protos->End(); protos->Next())
		Prototypes->InsertPtr((*protos)());

//	cout << "new protos created ....."<<endl;
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingCure::GroupsDistance(GGroup* g1, GGroup* g2)
{
	double dist=1.0;
	for (g1->Start(); !g1->End(); g1->Next())
	{
		if (!Prototypes->GetPtr((*g1)())) continue;
		for (g2->Start(); !g2->End(); g2->Next())
		{
			if (!Prototypes->GetPtr((*g2)())) continue;
			if (SubProfDistance((*g1)(), (*g2)())<dist)
				dist=SubProfDistance((*g1)(), (*g2)());
		}
	}
	return(dist);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingCure::SubProfDistance(GSubProfile* s1, GSubProfile* s2)
{
	double dist;

	dist=1.0-Session->GetSimProf(s1, s2);
	if (dist <1e-10) return 0.0;
	else return(dist);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingCure::Similarity(GSubProfile* s1, GSubProfile* s2)
{
	return(ProfSim->GetSim(s1,s2));
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingCure::CostFunction(RContainer<GGroup,unsigned int,false,true>* grps)     // calculates the intra/min(inter)
{
	unsigned int i,j;
	double intra, inter;
	GSubProfile **s1;
	GGroup *grp, **g1, **g2;
	intra=0.0;
	for (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		for(s1=grp->Tab,i=grp->NbPtr;--i;s1++)
		{
				intra+=SubProfDistance((*s1),grp->RelevantSubProfile(0));
		}
	}
	intra/=SubProfiles.NbPtr;
	return(intra);
//
//	inter=1.0;
//	for(g1=grps->Tab,i=grps->NbPtr;--i;g1++)
//		for(g2=g1+1,j=i+1;--j;g2++)
//			if(SubProfDistance((*g1)->RelevantSubProfile(0),(*g2)->RelevantSubProfile(0))<inter)
//				inter=SubProfDistance((*g1)->RelevantSubProfile(0),(*g2)->RelevantSubProfile(0));
//
//	return(intra/inter);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingCure::ShowGroups(RContainer<GGroup,unsigned int,false,true>*gs)
{
	for (gs->Start(); !gs->End(); gs->Next())
	{
		GGroup* g= (*gs)();
		cout << "Groupe "<< g->GetId() <<endl;
		for (g->Start(); !g->End(); g->Next())
			cout << "sub" << (*g)()->GetId() << "     ";
		cout <<endl;
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingCure::~GGroupingCure(void)
{
	if (Prototypes)
		delete Prototypes;
	if (Grps)
		delete Grps;
}

