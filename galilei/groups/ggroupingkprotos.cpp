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
//include files for GALILEI
#include<groups/ggroupingkprotos.h>
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
//  GGroupingKProtos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::GGroupingKProtos( GSession* s) throw(bad_alloc)
		: GGroupingKMeans(s,"KMeansPrototypes"),Landa(0.5),  NbPrototypes(1), protoserror(0), grpstemp(0), grpsfinal(0) , prototypes(0)
{
	protoserror = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
	grpstemp = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	grpsfinal = new RContainer<GGroup,unsigned int,false,false>  (10, 5);
	prototypes = new RContainer<GSubProfile,unsigned int,false,false> (10, 5);
}



//-----------------------------------------------------------------------------
const char* GALILEI::GGroupingKProtos::GetSettings(void)
{

	static char tmp[250];
	char c, d,e, h , k;
	if (GlobalSim) e='1'; else e='0';
	if(initial==Random) c='1'; else c='0';
	if (initial==Refined) d='1'; else d='0';
	if (initial==Relevant) h='1'; else h='0';
	if (initial==Scattered) k='1'; else k='0';

	 sprintf(tmp,"%c %u %u %u %c %c %c, %c",c, IterNumber, NbTests, NbPrototypes, d, e,  h, k);
	return(tmp);
}



//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::SetSettings(const char* s)
{
	char c, d, e, h, k;

	if(!(*s)) return;
	sscanf(s,"%c %u %u %u %c %c %c %c",&c, &IterNumber, &NbTests, &NbPrototypes,  &d, &e,&h, &k);
	if(e=='1') GlobalSim=true; else GlobalSim=false;
	if(c=='1') initial=Random ;
	if(d=='1') initial=Refined;
	if(h=='1') initial=Relevant;
	if(k=='1') initial=Scattered;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::RandomInitProtos(RContainer<GSubProfile,unsigned int,false,true>* dataset, unsigned int nbprototypes)
{
	GGroup* g;
	unsigned int i,nbok;

	prototypes->Clear();
	//set one center in each group.
	for (i=0,protos->Start(); !protos->End(); protos->Next(),i++)
	{
		grpstemp->InsertPtr(g=new GGroupVector(i,Groups->GetLang()));
 		g->InsertPtr((*protos)());
	}

	// set nbprottypes in each group
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		nbok=0;
		while (nbok<nbprototypes)
		{
			int u = Rand->Value(dataset->NbPtr);
			GSubProfile* tmp=dataset->GetPtrAt(u);
		 	if (IsValidProto(prototypes, tmp)&&IsValidProto(protos,tmp))
			{
				prototypes->InsertPtr(tmp);
			 	(*grpstemp)()->InsertPtr(tmp);
				nbok++;
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Init(void) throw(bad_alloc)
{
	InitCenters();
	RandomInitProtos(&SubProfiles, NbPrototypes);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Run(void) throw(GException)
{

	if (SubProfiles.NbPtr==0) return ;
	int grnumber;
	double var;

	// Calculates the similarities between subprofiles
	ProfSim= new GProfilesSim(SubProfiles, GlobalSim);

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

	//temp   - display KmeansPrototype parameters
	DisplayInfos();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		cout <<" ------------------------------------------------------------------------ init " <<endl;
		if (!initial==Random)
			Init();
		cout <<" ------------------------------------------------------------------------Run " <<endl;
		Execute(&SubProfiles, NbTests);


		// evaluate the clustering / each clutering for each number of group
		double finalcost=CostFunction(grpstemp2)/protos->NbPtr;
		double variance=Distortion(grpstemp2);
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

      }
	for (grpsfinal->Start(); !grpsfinal->End(); grpsfinal->Next())
	{
		Groups->InsertPtr((*grpsfinal)());
	}
	cout << "----------------->>>>>>end final cost function: "<<var<<endl;
	cout << "----------------->>>>>>end final nbgroup: "<< grnumber<<endl;
	GlobalSim=GlobalSim;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::DisplayInfos(void)
{
	cout <<endl;
	cout << " *** KMeansPrototypes *** "<< endl;
	cout << "NbSubProfiles"<<SubProfiles.NbPtr<<endl;
	cout << "nombre de test= "<<NbTests <<endl;
	cout << "global similarity= "<<GlobalSim<<endl;
	cout << " parameters max iter:"<<IterNumber<< endl;
	cout <<" nb tests: "<<NbTests<<endl<<endl;
	cout << " *** find groups number ***"<<endl;
	cout << "	find number of group :"<< FindGroupsNumber<<endl;
	cout << " *** initial parameters ***"<<endl;
	cout << " 	random :"<<  Random<<endl;
	cout << " 	scattered :"<< Scattered<<endl;
	cout << " 	relevant :"<< Relevant<<endl;
	cout << " 	refined :"<< Refined <<endl;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupingKProtos::Execute(RContainer<GSubProfile, unsigned int, false, true>* dataset, unsigned int nbtests)
{
	Clear();
	GGroup* gr;
	double finalcost;
	srand((unsigned)time(NULL));   //enables the total random parameters
	RContainer<GSubProfile,unsigned int,false,false>* subtodelete;

	for (unsigned int test=0; test<nbtests; test++)
	{
		cout << "**** ***************************test "<<test<<"***********************************"<<endl;
		// initialization
		grpstemp->Clear();
		if (initial==Random)
			Init();// cycle
		unsigned int iter=0;
		int error=1;

		while ((iter<IterNumber)&&(error!=0))
		{
			cout << " -------->  iter "<<iter<<endl;
			//Remove all subprofiles wich are neither center nor prototype from groups
			for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
			{
				subtodelete=new RContainer<GSubProfile,unsigned int,false,false>(10,5);
				gr=(*grpstemp)();
				for (gr->Start(); !gr->End(); gr->Next())
				{
					if (!protos->GetPtr((*gr)()))
						if(!prototypes->GetPtr((*gr)()))
							subtodelete->InsertPtr((*gr)());
				}
				for (subtodelete->Start(); !subtodelete->End(); subtodelete->Next())
					gr->DeletePtr((*subtodelete)());
			}

//			ShowGroups(grpstemp);

			// (re-)allocation
			cout << "Re allocating "<<endl;
			ReAllocate(dataset);

			//recentering
			cout << "Re centering "<<endl;
			ReCenter();

			// error calculation
			cout << "error calculating "<<endl;

			error=CalcError();
			iter++;
		}

	//	if (error==0) cout << "convergence"; else cout << "max iter reached ";
		double cost=CostFunction(grpstemp)/protos->NbPtr;
		cout << "cost function = "<< cost<<endl;
		if ((test==0)||(cost< finalcost))
		{
			grpstemp2->Clear();
			for (grpstemp->Start();!grpstemp->End(); grpstemp->Next())
				grpstemp2->InsertPtr((*grpstemp)());
			finalcost=cost;
			cout << "finalcost changed"<<endl;
		}

	}
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKProtos::CostFunction(RContainer<GGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
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
			double dist=(1-0)-Similarity(*s1,*s2);
			dist=dist*dist;
			if(dist<=mininter)
			{
				mininter=dist;
				if (dist==0)
				{
					cout <<" probleme distance sub "<<(*s1)->GetId() << " sub2 "<< (*s2)->GetId()<<endl;
				}
			}
		}
	}
	return(intra/mininter);
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKProtos::ReAllocate(RContainer<GSubProfile,unsigned int,false,true>* dataset)
{
	GGroup *parent;
	double distcenter, distprotos, level;

	for (dataset->Start(); !dataset->End(); dataset->Next())
	{
		GSubProfile* s=(*dataset)();
		if (protos->GetPtr(s)||prototypes->GetPtr(s)) continue;
		grpstemp->Start();
		level=Distance(s, (*grpstemp)(),1)+Distance(s, (*grpstemp)(),0);
		parent=(*grpstemp)();
		//find the nearest group for this subprofile
		for (grpstemp->Next(); !grpstemp->End(); grpstemp->Next())
		{
			distcenter=Distance(s, (*grpstemp)(),1);
			distprotos=Distance(s, (*grpstemp)(),0);
			if(Distance(s, (*grpstemp)(),0)+Distance(s, (*grpstemp)(),1)<level)
			{
				distcenter=Distance(s, (*grpstemp)(),1);
				distcenter=Distance(s, (*grpstemp)(),0);
				parent=(*grpstemp)();
			}
		}
		parent->InsertSubProfile(s);
	}
//	cout << " groupement apres reallocation : "<<endl;
//	ShowGroups(grpstemp);
}


//-----------------------------------------------------------------------------
double  GALILEI::GGroupingKProtos::Distance(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp, bool center)
{
	double distance;
	double  distlevel=1.0;
	for (grp->Start(); !grp->End(); grp->Next())
	{
		if (center)
		{
			if(protos->GetPtr((*grp)()))
			{
				distance=1.0-Similarity((*grp)(), s);
				return (Landa*distance*distance);
			}
		}
		else
		{
			if (prototypes->GetPtr((*grp)()))
				if (1.0-Similarity((*grp)(), s)<distlevel)
					distance=1.0-Similarity((*grp)(), s);
		}
	}

	return ((1.0-Landa)*distance*distance);
}


//-----------------------------------------------------------------------------
GSubProfile*  GALILEI::GGroupingKProtos::NearestPrototype(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp)
{
	unsigned int i;
	double sim=-1.0;
	GSubProfile *nearestproto, **s1;

	for (s1=grp->Tab, i=grp->NbPtr; i--;s1++)
	{
		if (prototypes->GetPtr(*s1))
			if (Similarity(*s1, s)>sim)
				nearestproto=(*s1);
	}
	return(nearestproto);
}


//-----------------------------------------------------------------------------
void  GALILEI::GGroupingKProtos::ReCenter()
{
	GGroup *gr, *prototypeclass;
	GSubProfile* newprototype;
	RContainer<GSubProfile,unsigned int,false,false>* newprototypes;
	RContainer<GSubProfile,unsigned int,false,false>* oldprototypes;
	newprototypes=new RContainer<GSubProfile,unsigned int,false,false> (10,5);
	oldprototypes=new RContainer<GSubProfile,unsigned int,false,false> (10,5);

	// recenter centers
	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GSubProfile* tmp=(*grpstemp)()->RelevantSubProfile(GlobalSim);
		protoserror->InsertPtr(tmp);
	}

	// recenter protos
//	for (protos->Start(); !protos->End(); protos->Next())
//		cout << "center " << (*protos)()->GetId()<<endl;
//	for (protoserror->Start(); !protoserror->End(); protoserror->Next())
//		cout << "new center " << (*protoserror)()->GetId()<<endl;
//	for (prototypes->Start(); !prototypes->End(); prototypes->Next())
//		cout << "prototype " << (*prototypes)()->GetId()<<endl;

	for (prototypes->Start(); !prototypes->End(); prototypes->Next())
	{
		prototypeclass=new GGroupVector(0, 0);
		gr=FindGroup(grpstemp, (*prototypes)());
//		cout << " recenter prototype of group "<< gr->GetId()<<endl;
		for(gr->Start(); !gr->End(); gr->Next())
		{
			GSubProfile* sub=(*gr)();
			if (protoserror->GetPtr(sub)) continue;  // if the subprofile is a center , it can't be a prototype
			if(NearestPrototype(sub, gr)==(*prototypes)())
				prototypeclass->InsertPtr(sub);
		}
		if(!prototypeclass->NbPtr) continue;
		newprototype=prototypeclass->RelevantSubProfile(GlobalSim);
//		cout << "new proto : "<< newprototype->GetId()<<endl;
		newprototypes->InsertPtr(newprototype);
		oldprototypes->InsertPtr((*prototypes)());
	}

	//update prototypes
	for(oldprototypes->Start(); !oldprototypes->End(); oldprototypes->Next())
		prototypes->DeletePtr((*oldprototypes)());
	for(newprototypes->Start(); !newprototypes->End(); newprototypes->Next())
		prototypes->InsertPtr((*newprototypes)());

	//temp
//	for(prototypes->Start(); !prototypes->End(); prototypes->Next())
//		cout << "nouvo protos "<< (*prototypes)()->GetId()<< "  "<<endl;
//	cout <<endl;
//	ShowGroups(grpstemp);
}


//-----------------------------------------------------------------------------
int  GALILEI::GGroupingKProtos::CalcError()
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
double GALILEI::GGroupingKProtos::TestMeasure()
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupingKProtos::CalcCalinsky()
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
		GSubProfile* center= gr->RelevantSubProfile(GlobalSim);
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
double GALILEI::GGroupingKProtos::StatMeasure()
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
double GALILEI::GGroupingKProtos::PalMeasure()
{
	double sum=0.0;
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		GGroup* g= (*grpstemp)();
		double sumgr=0.0;
		for (g->Start(); !g->End(); g->Next())
			sumgr+=g->ComputeSumSim((*g)(), GlobalSim);
		sum+=sumgr/g->NbPtr;
	}
	sum/=grpstemp->NbPtr;
	return(sum);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingKProtos::~GGroupingKProtos(void)
{
	if(protoserror) delete protoserror;
	if(grpstemp) delete grpstemp;
	if(grpstemp2) delete grpstemp2;
	if(grpsfinal) delete grpsfinal;
}


