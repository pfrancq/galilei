                                                                                                                                                                                                                                                                                             /*

	GALILEI Research Project

	GGroupingSupKMeans.cpp

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
#include <groups/ggroupingsupkmeans.h>
#include <groups/ggroupingkcos.h>
#include <groups/ggroupingsim.h>
#include <profiles/gprofilessim.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <profiles/gsubprofilevector.h>
#include <groups/gcomparegrouping.h>
#include <docs/gdoc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//include files for R
#include <rmath/random.h>
#include <rio/rtextfile.h>



using namespace RMath;


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//
//  GSupKMeansParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSupKMeansParams::GSupKMeansParams(void)
	: GGroupingParams("SUPKMeans")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSupKMeansParams::GetSettings(void)
{
	char c;
	static char tmp[200];

	if(DoubleKMeans) c='1'; else c='0';
	sprintf(tmp,"%u %u %f %c", NbPcSame, NbPcDiff, MinSim, c);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSupKMeansParams::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %lf %c",& NbPcSame, &NbPcDiff, &MinSim, &c) ;
	if(c=='1') DoubleKMeans=true; else DoubleKMeans=false;
}


//-----------------------------------------------------------------------------
//
//  GGroupingSupKMeans
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingSupKMeans::GGroupingSupKMeans( GSession* s, GSupKMeansParams* p, GKMeansParams* p2) throw(bad_alloc)
		: GGrouping("SUPKMeans",s), Params(p), Params2(p2)
{
	Protos= new RContainer<GSubProfile,unsigned int,false,false>(10,5);
 	ProtosDblKMeans= new RContainer<GSubProfile,unsigned int,false,false>(10,5);

}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingSupKMeans::GetSettings(void)
{
	char c;
	static char tmp[200];

	if(Params->DoubleKMeans) c='1'; else c='0';
	sprintf(tmp,"%u %u %f %c", Params->NbPcSame, Params->NbPcDiff, Params->MinSim, c);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %lf %c",&Params->NbPcSame, &Params->NbPcDiff, &Params->MinSim,&c) ;
	if(c=='1') Params->DoubleKMeans=true; else Params->DoubleKMeans=false;}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::Run(void) throw(GException)
{
	unsigned int nbiters;

	if (!SubProfiles.NbPtr) return;

	GSubProfile** s1, **s2;
	unsigned int i,j,comp=0;
	double minsim=0.0, etype=0.0 ;
	for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; i--; s1++)
		for (s2=s1+1, j=0; j<i; s2++,j++)
		{
//			cout <</* "prof"<<(*s1)->GetProfile()->GetId() <<" prof "<< (*s2)->GetProfile()->GetId()<< " sim = "<<*/Session->GetSimProf(*s1,*s2) <<","<<endl;
			minsim+=Session->GetSimProf(*s1,*s2) ;
			etype+=(Session->GetSimProf(*s1,*s2))*(Session->GetSimProf(*s1,*s2));
			comp++;
			
		}
		minsim/=comp;
		cout <<" moyene sim= "<<minsim<<endl;
		etype=(etype/double(comp))-(minsim*minsim);
		etype=sqrt(etype);
		cout <<"etype = "<<etype<<endl;
		Params->MinSim=minsim+1.5*etype;


	cout <<"**********************************"<<endl;
	cout << "SubProfiles NbPtr= "<<SubProfiles.NbPtr<<endl;
	cout << "Params->NbIters  = "<<Params->NbIters<<endl;
	cout << "Params->MinSim  = "<<Params->MinSim<<endl;
	cout << "Params->PcDiff  = "<<Params->NbPcDiff<<endl;
	cout << "Params->PcSame  = "<<Params->NbPcSame<<endl;
	cout << "Params->DoubleKMeans  = "<<Params->DoubleKMeans<<endl;
	cout <<"**********************************"<<endl;


	nbiters=Execute(Params->DoubleKMeans);


	cout <<endl<<"**********************************"<<endl;
	cout <<"Clustering done in :"<<nbiters<< " loops"<<endl;
	cout <<"Number of groups found = "<<Groups->NbPtr<<endl;
	cout <<"**********************************"<<endl;




	bool bkmeans=true;
	if (!bkmeans)  return;
	Params2->NbGroups=Groups->NbPtr;
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupingSupKMeans::Execute(bool doublekmeans)
{
	unsigned int iter;
	unsigned int error;

	// initialization
	Init();
//	//tmp dav
//	GSubProfile** tab;
//	tab=new GSubProfile*[SubProfiles.NbPtr];
//	memcpy(tab,SubProfiles.Tab,sizeof(GSubProfile*)*(SubProfiles.NbPtr));
//	Session->GetRandom()->Reset(0);
//	Session->GetRandom()->RandOrder<GSubProfile*>(tab,SubProfiles.NbPtr);
//	unsigned int i;
//	cout <<endl;
//	Session->GetRandom()->RandOrder<GSubProfile*>(tab,SubProfiles.NbPtr);
//for (i=0; i<50;i++)
//{
//	Protos->Clear();
//	ProtosDblKMeans->Clear() ;
//	Groups->Clear();
//	Protos->InsertPtr((tab[i]));
//	cout << "proto init=" <<tab[i]->GetId()<<endl;
	//end /tmp dav


	iter=0;
	error=1;
	while (iter<Params->NbIters&&error!=0)
	{
//		cout <<"---------------------------------- Iter "<< iter<<"-----------------------------"<<endl;

		Groups->Clear();

		// execute the hard constraints kmeans
		ReAllocate(true);
//		DisplayGroups();
		// error calculation and error calculation:
		error=ReCenterCalcError(false);
		cout <<" erreur = "<<error<<endl;


		// execute the standard kmeans
		if (doublekmeans)
		{
//			cout << "------------------------ double kmeans ---------------------"<<endl;
			ReAllocate(false);
//			DisplayGroups();
			ReCenterCalcError(true);
//			cout <<" erreur = "<<error<<endl;
		}
		iter++;
	}
//	DisplayGroups();
//	cout <<"Number of groups found = "<<Groups->NbPtr<<endl;
//	cout <<"**********************************"<<endl;

//}

	return(iter);

}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::Init(void) throw(bad_alloc)
{
	GGroup* group;

	Protos->Clear();
	ProtosDblKMeans->Clear() ;
	Groups->Clear();

	cout <<" --- initialization ---"<<endl;
	group=new GGroupVector(1,Groups->GetLang());
	for(SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		group->InsertPtr(SubProfiles());
	}
	Protos->InsertPtr(group->RelevantSubProfile(0));
	cout << "proto init=" <<group->RelevantSubProfile(0)->GetProfile()->GetId()<<endl;
	cout <<" --- initialization Ended ---"<<endl;
	delete group;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::ReAllocate(bool hardconstraint)
{
	unsigned int i;
	GGroup *g, *foundgroup;

	Groups->Clear() ;
	for (i=0,Protos->Start(); !Protos->End(); Protos->Next(),i++)  // groupscontaining one proto are created
	{
		g=new GGroupVector(i,Groups->GetLang());
		g->InsertPtr((*Protos)());
		Groups->InsertPtr(g);
//		cout << "groupe " << g->GetId() <<" recentered in profile "<< (*Protos)()->GetProfile()->GetId()<<endl;;              // tmp Dav

	}

	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{                                                                                    
		if (Protos->GetPtr(SubProfiles()))    continue;
//		cout <<"*********** Profile "<<SubProfiles()->GetProfile()->GetId()<<"**************"<<endl;;
		foundgroup=ClosestGroup(SubProfiles(),  hardconstraint);
		if(!foundgroup)
		{
			g=new GGroupVector(Groups->NbPtr,Groups->GetLang());
			g->InsertPtr(SubProfiles());
			Groups->InsertPtr(g);
			Protos->InsertPtr(SubProfiles());
//			cout <<"             added to a NEW group"<< g->GetId()<<endl;
		}
		else
		{
			foundgroup->InsertPtr(SubProfiles());
//			cout <<"             added to group"<< foundgroup->GetId()<<endl;
		}	
	}
}


//-----------------------------------------------------------------------------
unsigned int  GALILEI::GGroupingSupKMeans::ReCenterCalcError(bool doublekmeans)
{
	GSubProfile* tmpsub;
	RContainer<GSubProfile, unsigned int, false,false>* protoserror;
	 int err;

	//rezcenter
	protoserror=new RContainer<GSubProfile, unsigned int, false,false>(10,5);
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		tmpsub=(*Groups)()->RelevantSubProfile(0);
		protoserror->InsertPtr(tmpsub);
//		cout << "groupe " << (*Groups)()->GetId() <<" recentered in profile "<< tmpsub->GetProfile()->GetId()<<endl;              // tmp Dav
	}

	//calc error
	if (doublekmeans)              // traditionnal comparison
	{
		err=Protos->NbPtr;
		for (Protos->Start(); !Protos->End(); Protos->Next())
		{
			if (protoserror->GetPtr((*Protos)()))
				err=err-1;
		}
	}
	else     // special comprasion for doublekmeans (compare with the protos of the last double kmeans)
	{
		err=protoserror->NbPtr;
		for (protoserror->Start(); !protoserror->End(); protoserror->Next())
		{
			if (ProtosDblKMeans->GetPtr((*protoserror)()))
				err=err-1;
		}
	}
	//replace protos
	Protos->Clear();
	for (protoserror->Start(); !protoserror->End(); protoserror->Next())
	{
		Protos->InsertPtr((*protoserror)());
	}
	if (!doublekmeans)   // fill the ProtosDblKMeans Container with the new DblKMeans Protos
	{
		ProtosDblKMeans->Clear() ;
		for (protoserror->Start(); !protoserror->End(); protoserror->Next())
			ProtosDblKMeans->InsertPtr((*protoserror)());
	}

	delete protoserror;
	return(abs(err));
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroupingSupKMeans::FindGroup(GSubProfile* subprof)
{
	for (Groups->Start(); !Groups->End(); Groups->Next())
		if( (*Groups)()->GetPtr(subprof))
			return((*Groups)());

	return(0) ;
}

//-----------------------------------------------------------------------------
bool  GALILEI::GGroupingSupKMeans::IsAPcSameValidGroup(GGroup* group, GSubProfile* sub)
{
	GProfDocCursor profdoc1;
	GProfDocCursor profdoc2;
	unsigned int nbcomp, percent;

//	cout << group->NbPtr<<endl;
	profdoc1=sub->GetProfile()->GetProfDocCursor();


	for (group->Start(); ! group->End(); group->Next())
	{
		nbcomp=percent=0;
		profdoc2=(*group)()->GetProfile()->GetProfDocCursor();

		for (profdoc1.Start(); !profdoc1.End(); profdoc1.Next())
			for (profdoc2.Start(); !profdoc2.End(); profdoc2.Next())
				if (profdoc1()->GetDoc()->GetId()==profdoc2()->GetDoc()->GetId())
				{
					nbcomp++;              // number of common documents
					if ((profdoc1()->GetFdbk() ==djOK)&&(profdoc2()->GetFdbk()==djOK))
					{
						percent++;       // number of same judgements on a common document
//						cout << "same judgements"<<endl;
					}
//					else
//						cout <<"  diiferents judgements"<<endl;
				}

		if(nbcomp)
		{
			percent=int(100.0*(double(percent)/double(nbcomp)));
//			cout << "pcsame"<<"%=  "<< percent<< "("<<nbcomp<<")"<<"   entre  "<< (*group)()->GetProfile()->GetId()<<"  ET  "<<sub->GetProfile()->GetId()<<endl;
			if (percent >=Params->NbPcSame)
			{
				return(true) ;
			}
		}
	}

	return(false);
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSupKMeans::IsAPcDiffValidGroup(GGroup* group, GSubProfile* sub)
{
	GProfDocCursor profdoc1;
	GProfDocCursor profdoc2;
	unsigned int nbcomp, percent;

//	cout << group->NbPtr<<endl;

	profdoc1=sub->GetProfile()->GetProfDocCursor();


	for (group->Start(); ! group->End(); group->Next())
	{
		nbcomp=percent=0;
		profdoc2=(*group)()->GetProfile()->GetProfDocCursor();

		for (profdoc1.Start(); !profdoc1.End(); profdoc1.Next())
			for (profdoc2.Start(); !profdoc2.End(); profdoc2.Next())
				if (profdoc1()->GetDoc()->GetId()==profdoc2()->GetDoc()->GetId())
				{
					nbcomp++;              // number of common documents
					if (profdoc1()->GetFdbk()!=profdoc2()->GetFdbk())
						percent++;       // number of same (or diff) judgements on a common document
				}

		if(nbcomp)
		{
			percent=int(100*(percent/nbcomp));
//			cout << "pcdiff"<<"%=  "<< percent<< "("<<nbcomp<<")"<<"   entre  "<< (*group)()->GetProfile()->GetId()<<"  ET  "<<sub->GetProfile()->GetId()<<endl;
			if (percent >=Params->NbPcDiff)
			{
				return(true) ;            // return 1 if pcsame>rate or return 0 if pcdiff >rate
			}
		}
	}

	return(false);
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupingSupKMeans::IsAMinSimValidGroup(GGroup* group, GSubProfile* sub)
{
	double sims;

	sims=0.0;
	for (group->Start(); !group->End(); group->Next())
	{
//		cout <<(*group)()->GetId()<<endl;;
		sims+=SubProfDistance((*group)(), sub);
	}
	sims/=group->NbPtr;
//	cout <<"sims ="<<sims << " avec "<<group->NbPtr << "elements ds ce group";

	return((sims)<(1.0-Params->MinSim)) ;
	
}

//-----------------------------------------------------------------------------
double GALILEI::GGroupingSupKMeans::SubProfDistance(GSubProfile* s1, GSubProfile* s2)
{
	double dist;

	dist=1.0-Session->GetSimProf(s1, s2);
//	cout <<" distance ="<<dist<<endl;
	if (dist <1e-10) return 0.0;
	else return(dist);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroupingSupKMeans::ClosestGroup(GSubProfile* subprof, bool hardconstraint)
{
	GGroup* closestGroup;
	double dist, tmpdist;
	
	Groups->Start();
	closestGroup=(*Groups)();
	dist=Distance(closestGroup, subprof, hardconstraint) ;
	if (dist==0.0) return(closestGroup);

	for (Groups->Next(); !Groups->End(); Groups->Next())
	{
		tmpdist=Distance((*Groups)(), subprof, hardconstraint) ;
//		cout << "distance avec groupe "<< (*Groups)()->GetId()<<" = "<<tmpdist<<endl;
		if (tmpdist==0.0)   return((*Groups)());
		else
		{
			if (tmpdist<dist)
			{
				dist=tmpdist;
				closestGroup=(*Groups)();
			}
		}                                           
	}

	if(dist==2.0) return(0);
	else return(closestGroup);
}

//-----------------------------------------------------------------------------
double GALILEI::GGroupingSupKMeans::Distance(GGroup* g, GSubProfile* sub, bool hardconstraint)
{
	GSubProfile* proto;
//	cout << "compare subprofile  "<< sub->GetId() << " with  group  "<<g->GetId()<<" with "<< hardconstraint << " hardconstraints"<<endl;

	if (hardconstraint)
	{
		//hard constraints
//		if (IsAPcSameValidGroup(g,sub))
		if (IsAPcSameValidGroup(g,sub))  //tmp dav (v erifie deux conditions pour pcsame)
		{
//			cout << "                is a pcsame valid group"<<endl;
			return(0.0);
		}
		if (IsAPcDiffValidGroup(g,sub))
		{
//			cout << "                is a pcdiff  valid group"<<endl;
			 return(2.0);
		}
		if (!IsAMinSimValidGroup(g,sub))
		{
//			cout << "                not simvalid group"<<endl;
	   	  return(2.0);
		}
	}

//	cout <<"calculating cosinus distance" ;
	proto=0;
	Protos->Start();
	// finding the prototype of the group
	while(!proto&&!Protos->End())
	{
		if (g->GetPtr((*Protos)()))
			proto=(*Protos)();
		Protos->Next();
	}
//	cout << " with proto "<< proto->GetProfile()->GetId()<<" ="<<SubProfDistance(proto,sub)<<endl;
	return(SubProfDistance(proto,sub));
	

	
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::DisplayGroups()
{
	for (Groups->Start();!Groups->End(); Groups->Next())
	{
		GGroup* g=(*Groups)();
		cout << "group"<<g->GetId();
		for (g->Start(); !g->End(); g->Next())
			cout <<"     prof "<<(*g)()->GetProfile()->GetId();
		cout <<endl;
	}
	cout <<"Number of groups found = "<<Groups->NbPtr<<endl;
	cout <<"**********************************"<<endl;

}

//-----------------------------------------------------------------------------
GALILEI::GGroupingSupKMeans::~GGroupingSupKMeans(void)
{

}


