/*

	GALILEI Research Project

	GGroupingKProtos.cpp

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
//include files for R Project
#include <rgga/rgroupingkmeans.h>
using namespace R;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroupingkprotos.h>
#include<groups/ggroupingsim.h>
#include<groups/ggroups.h>
#include<groups/ggroup.h>
#include<groups/ggroupir.h>
#include<groups/ggroupvector.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GKProtosParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GKProtosParams::GKProtosParams(void)
	: GGroupingParams("kMeansProtos")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GKProtosParams::GetSettings(void)
{
	static char tmp[200];

	sprintf(tmp,"%u %u %u %u %u %f %u", NbGroups, NbProtos, MaxIters, NbSubSamples, SubSamplesRate, Lambda, VerifyKMeansLimit);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GKProtosParams::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u %u %u %u %lf %u", &NbGroups, &NbProtos, &MaxIters, &NbSubSamples, &SubSamplesRate, &Lambda, &VerifyKMeansLimit);
}



//-----------------------------------------------------------------------------
//
//  GGroupingKProtos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::GGroupingKProtos(GSession* s, GKProtosParams* params) throw(bad_alloc)
	: GGrouping("kMeansProtos",s), Params(params)
{
//	NbProtos=2;
//	NbGroups=13;
//	NbIters=10;
//	NbSubSamples=5;
//	SubSamplesRate=100;
//	VerifyKMeansLimit=1000;
//	Lambda=0.5;

	Protos = new RContainer<GSubProfile,unsigned int,false,false> (Params->NbProtos*Params->NbGroups,2);
	Centers = new RContainer<GSubProfile,unsigned int,false,false> (Params->NbGroups,2); 
	CentersError = new RContainer<GSubProfile,unsigned int,false,false> (Params->NbGroups,2); 
	Grps= new RContainer<GGroup,unsigned int, false, true> (Params->NbGroups, 2);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKProtos::GetSettings(void)
{
	static char tmp[250];

	sprintf(tmp,"%u %u %u %u %u %f %u", Params->NbGroups, Params->NbProtos, Params->MaxIters, Params->NbSubSamples, Params->SubSamplesRate, Params->Lambda, Params->VerifyKMeansLimit);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u %u %u %u %lf %u", &Params->NbGroups, &Params->NbProtos, &Params->MaxIters, &Params->NbSubSamples, &Params->SubSamplesRate, &Params->Lambda, &Params->VerifyKMeansLimit);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Initialization(void)         // initialization is made by a 2 levels KMeans algorithm (one level for centers, one level for protos)
{
	GGroup *g, *gr;
	GSubProfile* subprof;
	unsigned int i;
	RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups>* kmeans;
	RContainer <GSubProfile,unsigned int, false,true>* subs;
	kmeans=new  RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups> (&SubProfiles) ;

	// clear the containers.
	Centers->Clear();
	Protos->Clear();
	//execute a kmeans to find centers
	cout << " ****************** INITIALIZATION  BY KMEANS ****************"<<endl;

	kmeans->SetNbTests(1);
	kmeans->SetGroupsNumber(Params->NbGroups);
	kmeans->SetInitial(RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups>::Refined);
	kmeans->SetEpsilon(0.0005);
	kmeans->SetIterNumber(Params->MaxIters);
	kmeans->SetSubSamplesNumber(Params->NbSubSamples);
	kmeans->SetSubSamplesRate(Params->SubSamplesRate);
	kmeans->SetVerifyKMeansMaxIters(Params->VerifyKMeansLimit);
	kmeans->Run();
	RContainer<GGroup,unsigned int,false,false>* grps=kmeans->GetGrouping();
	cout << " ****************** END OF INITIALIZATION ****************"<<endl;

	//executing kmeans to find protos
	for (i=0,grps->Start(); !grps->End(); grps->Next(),i++)
	{
		gr=(*grps)();
		subprof=gr->RelevantSubProfile(0);
		Centers->InsertPtr(subprof);
		Grps->InsertPtr(g=new GGroupVector(i,Groups->GetLang()));
		g->InsertPtr(subprof);
		if (gr->NbPtr>=Params->NbProtos)
		{
			// execute a kmeans to find protos
			subs = new  RContainer <GSubProfile,unsigned int, false,true>(10,5);
			for (gr->Start(); !gr->End(); gr->Next())
				if (!Centers->GetPtr((*gr)()))
					subs->InsertPtr((*gr)());
			kmeans=new  RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups> (subs) ;
			kmeans->SetNbTests(1);
			kmeans->SetGroupsNumber(Params->NbProtos);
			kmeans->SetInitial(RGroupingKMeans<GGroup,GSubProfile,GGroupDataIR,GGroups>::Refined);
			kmeans->SetEpsilon(0.0);
			kmeans->SetIterNumber(Params->MaxIters);
			kmeans->SetSubSamplesNumber(1);
			kmeans->SetSubSamplesRate(100);
			kmeans->SetVerifyKMeansMaxIters(Params->VerifyKMeansLimit);
			kmeans->Run();
			RContainer<GGroup,unsigned int,false,false>* grps2=kmeans->GetGrouping();
			for (grps2->Start(); !grps2->End(); grps2->Next())
			{
				subprof=(*grps2)()->RelevantSubProfile(0);
				Protos->InsertPtr(subprof);
				g->InsertPtr(subprof);
			}
			delete(subs);
			delete(kmeans);
		}
		else
		{
			for (gr->Start(); !gr->End(); gr->Next())
			{
				subprof=(*gr)();
				Protos->InsertPtr(subprof);
				g->InsertPtr(subprof);
			}
		}
		
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Run(void) throw(GException)
{
	unsigned int iter=0;
	unsigned int error=Params->NbGroups;
	GGroup *g, *gr ;
	
	if (!SubProfiles.NbPtr) return;
	
//	DisplayInfos();
	Initialization();
	
 	while(iter<Params->MaxIters&&error!=0)
 	{
 		ReAllocate();
 		ReCenter();
 		error=CalcError();
 		iter++;
 	}
//	cout << "End of KMeans Protos"<<endl;
//	cout << "number of iterations= "<<iter<< "    error= "<<error<<endl;
 	//save grouping into Groups
 	Groups->Clear();
 	for (Grps->Start(); !Grps->End(); Grps->Next())
 	{
 		gr=(*Grps)();
 		g=new GGroupVector(gr->GetId(), Groups->GetLang());
 		for (gr->Start(); !gr->End(); gr->Next())
 			g->InsertPtr((*gr)());
 		Groups->InsertPtr(g);
 	}

}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::ReAllocate(void)
{
	GGroup *parent;
	double distcenter, distprotos;
	double distcentertest, distprotostest;

	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		GSubProfile* s=SubProfiles();
		if (Centers->GetPtr(s)||Protos->GetPtr(s)) continue;
		Grps->Start();
		distcenter=Distance(s, (*Grps)(),1);
		distprotos=Distance(s, (*Grps)(),0);
		parent=(*Grps)();
		//find the nearest group for this subprofile
		for (Grps->Next(); !Grps->End(); Grps->Next())
		{
			distcentertest=Distance(s, (*Grps)(),1);
			distprotostest=Distance(s, (*Grps)(),0);
			if(distcentertest+distprotostest<distcenter+distprotos)
			{
				distcenter=distcentertest;
				distprotos=distprotostest;
				parent=(*Grps)();
			}
		}
		parent->InsertSubProfile(s);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::ReCenter(void)
{
	GGroup *gr, *prototypeclass;
	GSubProfile* newprototype;
	RContainer<GSubProfile,unsigned int,false,false>* newprototypes;
	RContainer<GSubProfile,unsigned int,false,false>* oldprototypes;
	newprototypes=new RContainer<GSubProfile,unsigned int,false,false> (10,5);
	oldprototypes=new RContainer<GSubProfile,unsigned int,false,false> (10,5);

	// recenter centers
	CentersError->Clear();
	for (Grps->Start(); !Grps->End(); Grps->Next())
	{
		GSubProfile* tmp=(*Grps)()->RelevantSubProfile(0);
		CentersError->InsertPtr(tmp);
	}

	//recenter prototypes
	for (Protos->Start(); !Protos->End(); Protos->Next())
	{
		prototypeclass=new GGroupVector(0, 0);
		gr=FindGroup(Grps, (*Protos)());
//		cout << " recenter prototype of group "<< gr->GetId()<<endl;
		for(gr->Start(); !gr->End(); gr->Next())
		{
			GSubProfile* sub=(*gr)();
			if (Protos->GetPtr(sub)) continue;  // if the subprofile is a center , it can't be a prototype
			if(NearestPrototype(sub, gr)->GetId()==(*Protos)()->GetId())
				prototypeclass->InsertPtr(sub);
		}
		if(!prototypeclass->NbPtr) continue;
		newprototype=prototypeclass->RelevantSubProfile(0);
//		cout << "new proto : "<< newprototype->GetId()<<endl;
		newprototypes->InsertPtr(newprototype);
		oldprototypes->InsertPtr((*Protos)());
	}

	//update prototypes
	for(oldprototypes->Start(); !oldprototypes->End(); oldprototypes->Next())
		Protos->DeletePtr((*oldprototypes)());
	for(newprototypes->Start(); !newprototypes->End(); newprototypes->Next())
		Protos->InsertPtr((*newprototypes)());

	//temp
//	for(prototypes->Start(); !prototypes->End(); prototypes->Next())
//		cout << "nouvo protos "<< (*prototypes)()->GetId()<< "  "<<endl;
//	cout <<endl;
//	ShowGroups(grpstemp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::DisplayInfos(void)
{
	cout <<endl;
	cout << " *** KMeansPrototypes *** "<< endl;
	cout << "NbSubProfiles"<<SubProfiles.NbPtr<<endl;
	cout << "Lambda= "<<Params->Lambda <<endl;
	cout << " *** initial parameters ***"<<endl;
	cout << "nombre de SubSamples= "<<Params->NbSubSamples <<endl;
	cout << "taux des SubSamples= "<<Params->SubSamplesRate <<endl;
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKProtos::Distance(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp, bool center)
{
	double distance=0.0;
	bool firstprotofound=true;
	for (grp->Start(); !grp->End(); grp->Next())
	{
		if (center)
			if(Centers->GetPtr((*grp)()))
			{
				distance=1.0-Similarity((*grp)(), s);
				return(Params->Lambda*distance*distance);
			}
		else
		{
			if (Protos->GetPtr((*grp)()))
			{
				if (firstprotofound=true)
				{
					distance=1.0-Similarity((*grp)(), s);
					firstprotofound=false;
				}
				else
					if(1.0-Similarity((*grp)(), s)<distance)
						distance=1.0-Similarity((*grp)(), s);
			}
		}
	}
	return((1-Params->Lambda)*distance*distance);
}



//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKProtos::Similarity(GSubProfile* s1, GSubProfile* s2)
{
	return(Session->GetSimProf(s1,s2));
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroupingKProtos::FindGroup(RContainer<GGroup,unsigned int,false,true>* grps, GSubProfile* s)
{
	for (grps->Start(); !grps->End(); grps->Next())
		if( (*grps)()->GetPtr(s))
			return((*grps)());
	return(0);
}


//-----------------------------------------------------------------------------
GSubProfile*  GALILEI::GGroupingKProtos::NearestPrototype(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp)
{
	unsigned int i;
	double sim, simtest;;
	bool firstprotofound=true;
	GSubProfile *nearestproto, **s1;

	for (s1=grp->Tab, i=grp->NbPtr; i--;s1++)
	{
		if (!Protos->GetPtr(*s1)) continue;
		if (firstprotofound)
		{
			sim=Similarity(*s1, s);
			nearestproto=(*s1);
		}
		else
		{
			simtest=Similarity(*s1, s);
			if(simtest>sim)
			{
				sim=simtest;
				nearestproto=(*s1);
			}
		}
	}
	return(nearestproto);
}


//-----------------------------------------------------------------------------
int  GALILEI::GGroupingKProtos::CalcError()
{
	int err=Centers->NbPtr;
	for (Centers->Start(); !Centers->End(); Centers->Next())
	{
		if (CentersError->GetPtr((*Centers)()))
			err=err-1;
	}
	Centers->Clear();
	for (CentersError->Start(); !CentersError->End(); CentersError->Next())
	{
		Centers->InsertPtr((*CentersError)());
	}
//	cout << "error : "<< err<<endl;
	return(err);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::~GGroupingKProtos(void)
{
	if(Protos) delete Protos;

}


