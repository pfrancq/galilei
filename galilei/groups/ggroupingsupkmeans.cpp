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
#include <profiles/guser.h>
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
	Debug=false;
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
	FoundGroups= new RContainer<GGroups,unsigned int,false,false>(10,5);
	HardConstraints= new RContainer<GGroup,unsigned int,false,false>(10,5);

	Session->GetRandom()->Reset(0);
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
void GALILEI::GGroupingSupKMeans::AdaptMinSimToDatabase(double distcoeff)
{
	unsigned int i,j,comp;
	double minsim, etype, tmpsim;
	GSubProfile** s1, **s2;

	comp=0;
	minsim=0.0;
	etype=0.0 ;

	//tmp david
	int comp2=0;
	RTextFile* text= new RTextFile("../../../simprofs.txt", RIO::Create);
	for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; i--; s1++)
		(*text) << "\t"<<(*s1)->GetProfile()->GetId();
	(*text) <<endl;
	for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; i--; s1++)
	{
		comp2++;
		(*text)  << (*s1)->GetProfile()->GetId();
		for (s2=SubProfiles.Tab, j=SubProfiles.NbPtr; j--; s2++)
		{
				(*text)  << "\t"<<Session->GetSimProf(*s1,*s2) ;
		}
		(*text) <<endl;
	}
	cout << "file written"<<endl;
	// end tmp dav

	for (s1=SubProfiles.Tab, i=SubProfiles.NbPtr; i--; s1++)
		for (s2=s1+1, j=0; j<i; s2++,j++)
		{
			tmpsim=Session->GetSimProf(*s1,*s2) ;
			minsim+=tmpsim;
			etype+=tmpsim*tmpsim;
			comp++;

		}
	minsim/=comp;
	etype=(etype/double(comp))-(minsim*minsim);
	etype=sqrt(etype);
	cout <<" MinSim = "<<minsim<<" \t Ecart type = "<<etype<<endl;
	Params->MinSim=minsim+distcoeff*etype;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::Run(void) throw(GException)
{
	unsigned int sample, nbsamples, nbiter;
	unsigned int **cor, comp;

	if (!SubProfiles.NbPtr) return;

	//tmp dav
	double dav;

	cout << "dav = " <<dav<<endl;
	AdaptMinSimToDatabase(1.5);

//	if (Params->Debug)
//	{
		cout <<"**********************************"<<endl;
		cout << "SubProfiles NbPtr= "<<SubProfiles.NbPtr<<endl;
		cout << "Params->InitMode  = "<<Params->InitMode<<endl;
		cout << "Params->NbIters  = "<<Params->NbIters<<endl;
		cout << "Params->MinSim  = "<<Params->MinSim<<endl;
		cout << "Params->PcDiff  = "<<Params->NbPcDiff<<endl;
		cout << "Params->PcSame  = "<<Params->NbPcSame<<endl;
		cout << "Params->DoubleKMeans  = "<<Params->DoubleKMeans<<endl;
		cout << "Params->Relevant  = "<<(Params->InitMode==GSupKMeansParams::Relevant)<<endl;
		cout << "Params->Initialisation  = "<<Params->UsedAsInitialization<<endl;
		cout << "Params->NbSamples  = "<<Params->NbSamples<<endl;
		cout << "Params->SameGroupRate = "<<Params->SameGroupRate<<endl;
		cout << "Params->Debug = "<<Params->Debug<<endl;
		cout <<"**********************************"<<endl;
//	}

	if (Params->UsedAsInitialization)  nbsamples=Params->NbSamples;
	else nbsamples=1;
	for (sample=0;sample<nbsamples; sample++)
	{
		// initialization
		Init();
		//execution
		nbiter=Execute(Params->DoubleKMeans);
			if (Params->Debug) cout << " executed in "<< nbiter<<" loops."<<endl;
	}

	if (Params->Debug)
	{
		cout << " ---------------------------------- END of SUPKMEANS ----------------------"<<endl;
		cout <<FoundGroups->NbPtr<<"  groups registered"<<endl;
	}

	if (Params->UsedAsInitialization)
	{
		if (Params->Debug) cout << " ----------------------------------  ----------------------"<<endl;
		cor= new unsigned int*[SubProfiles.NbPtr];
		for (comp=0; comp<SubProfiles.NbPtr; comp++)
			cor[comp]=new unsigned int[2];
		// init correlation table
		for (comp=0; comp<SubProfiles.NbPtr;comp++)
 		{
			cor[comp][0]=comp;
			cor[comp][1]=SubProfiles.GetPtrAt(comp)->GetId();
		}
		SetConstraintsFromSamples(cor);
		if (Params->Debug)
		{
			cout <<HardConstraints->NbPtr<<"  groups for HardConstraints"<<endl;
			for (HardConstraints->Start();!HardConstraints->End(); HardConstraints->Next())
			{
				GGroup* g=(*HardConstraints)();
				cout << "group"<<g->GetId();
				for (g->Start(); !g->End(); g->Next())
					cout <<"     prof "<<(*g)()->GetProfile()->GetId();
				cout <<endl;
			}
		}
		for (comp=0; comp<SubProfiles.NbPtr;comp++)
			delete[] cor[comp];
	        delete[] cor;
	}
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupingSupKMeans::Execute(bool doublekmeans)
{
	unsigned int iter;
	unsigned int error;
	GGroups* groups;
	GGroup* group;

	iter=0;
	error=1;
	while (iter<Params->NbIters&&error!=0)
	{
//		if (Params->Debug)
			cout <<"---------------------------------- Iter "<< iter<<"-----------------------------"<<endl;

		Groups->Clear();

		// execute the hard constraints kmeans
		ReAllocate(true);
		// error calculation and error calculation:
		error=ReCenterCalcError(false);
		if (Params->Debug) cout <<" erreur = "<<error<<endl;

		// execute the standard kmeans
		if (doublekmeans)
		{
			if (Params->Debug)
				cout << "------------------------ double kmeans ---------------------"<<endl;
			ReAllocate(false);
			if (Params->Debug)
				DisplayGroups();
			ReCenterCalcError(true);
			if (Params->Debug)
				cout <<" erreur = "<<error<<endl;
		}
		iter++;
	}
//	if (Params->Debug)
//	{
//		DisplayGroups();
		cout <<endl<<"**********************************"<<endl;
//		cout <<"Clustering done in :"<<iter<< " loops"<<endl;
		cout <<"Number of groups found = "<<Groups->NbPtr<<endl;
		cout <<"**********************************"<<endl;
//	 }

	if (Params->UsedAsInitialization)
	{
		groups= new GGroups (Groups->GetLang());
		for (Groups->Start(); !Groups->End(); Groups->Next())
		{
			group=new GGroupVector((*Groups)()->GetId(),Groups->GetLang()) ;
			for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
				group->InsertPtr((*(*Groups)())());
			groups->InsertPtr(group);
		}
		FoundGroups->InsertPtr(groups);
	}

	return(iter);

}


//-----------------------------------------------------------------------------
void GALILEI::GGroupingSupKMeans::Init(void) throw(bad_alloc)
{
	GGroup* group;
	unsigned int random;

	Protos->Clear();
	ProtosDblKMeans->Clear() ;
	Groups->Clear();

	if (Params->Debug)  cout <<" --- initialization ---"<<endl;
	if (Params->Debug)  cout << "InitMode= "<<Params->InitMode<<endl;

	switch (Params->InitMode)
	{
		case (GSupKMeansParams::Relevant) :
			group=new GGroupVector(1,Groups->GetLang());
			for(SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
				group->InsertPtr(SubProfiles());
			Protos->InsertPtr(group->RelevantSubProfile(0));
			if (Params->Debug)
			{
				cout << "proto init=" <<group->RelevantSubProfile(0)->GetProfile()->GetId()<<endl;
				cout <<" --- Relevant initialization Ended ---"<<endl;
			}
			delete group;
			break;

		case(GSupKMeansParams::Random) :
			random=Session->GetRandom()->Value(SubProfiles.NbPtr) ;
			Protos->InsertPtr(SubProfiles.GetPtrAt(random)) ;
			if (Params->Debug)
			{
				cout << "proto init=" <<SubProfiles.GetPtrAt(random)->GetProfile()->GetId()<<endl;
				cout <<" ---Random  initialization Ended ---"<<endl;
			}
			break;

		default :
			if (Params->Debug)  cout << " ERROR !!! no initialization !!!"<<endl;
	}
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
		if (Params->Debug)  cout << "groupe " << g->GetId() <<" recentered in profile "<< (*Protos)()->GetProfile()->GetId()<<endl;

	}

	for (SubProfiles.Start(); !SubProfiles.End(); SubProfiles.Next())
	{
		if (Protos->GetPtr(SubProfiles()))    continue;
		if (Params->Debug) cout <<"*********** Profile "<<SubProfiles()->GetProfile()->GetId()<<"**************"<<endl;
		foundgroup=ClosestGroup(SubProfiles(),  hardconstraint);
		if(!foundgroup)
		{
			g=new GGroupVector(Groups->NbPtr,Groups->GetLang());
			g->InsertPtr(SubProfiles());
			Groups->InsertPtr(g);
			Protos->InsertPtr(SubProfiles());
			if (Params->Debug)  cout <<"             added to a NEW group"<< g->GetId()<<endl;
		}
		else
		{
			foundgroup->InsertPtr(SubProfiles());
			if (Params->Debug)  cout <<"             added to group"<< foundgroup->GetId()<<endl;
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
		if (Params->Debug) cout << "groupe " << (*Groups)()->GetId() <<" recentered in profile "<< tmpsub->GetProfile()->GetId()<<endl;
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
						if (Params->Debug) cout << "same judgements"<<endl;
					}
					else
						if (Params->Debug) cout <<"  diiferents judgements"<<endl;
				}

		if(nbcomp)
		{
			percent=int(100.0*(double(percent)/double(nbcomp)));
			if (Params->Debug)  cout << "pcsame"<<"%=  "<< percent<< "("<<nbcomp<<")"<<"   entre  "<< (*group)()->GetProfile()->GetId()<<"  ET  "<<sub->GetProfile()->GetId()<<endl;
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
			if (Params->Debug)  cout << "pcdiff"<<"%=  "<< percent<< "("<<nbcomp<<")"<<"   entre  "<< (*group)()->GetProfile()->GetId()<<"  ET  "<<sub->GetProfile()->GetId()<<endl;
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
		if (Params->Debug)  cout <<(*group)()->GetId()<<endl;;
		sims+=SubProfDistance((*group)(), sub);
	}
	sims/=group->NbPtr;
	if (Params->Debug)  cout <<"sims ="<<sims << " avec "<<group->NbPtr << "elements ds ce group";

	return((sims)<(1.0-Params->MinSim)) ;

}

//-----------------------------------------------------------------------------
double GALILEI::GGroupingSupKMeans::SubProfDistance(GSubProfile* s1, GSubProfile* s2)
{
	double dist;

	dist=1.0-Session->GetSimProf(s1, s2);
	if (Params->Debug)  cout <<" distance ="<<dist<<endl;
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
		if (Params->Debug) cout << "distance avec groupe "<< (*Groups)()->GetId()<<" = "<<tmpdist<<endl;
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
	if (Params->Debug)  cout << "compare subprofile  "<< sub->GetId() << " with  group  "<<g->GetId()<<" with "<< hardconstraint << " hardconstraints"<<endl;

	if (hardconstraint)
	{
		//hard constraints
		if (IsAPcSameValidGroup(g,sub))
		{
			if (Params->Debug) cout << "                is a pcsame valid group"<<endl;
			return(0.0);
		}
		if (IsAPcDiffValidGroup(g,sub))
		{
			if (Params->Debug) cout << "                is a pcdiff  valid group"<<endl;
			 return(2.0);
		}
		if (!IsAMinSimValidGroup(g,sub))
		{
			if (Params->Debug) cout << "                not simvalid group"<<endl;
	   	  return(2.0);
		}
	}

	if (Params->Debug) cout <<"calculating cosinus distance" ;
	proto=0;
	Protos->Start();
	// finding the prototype of the group
	while(!proto&&!Protos->End())
	{
		if (g->GetPtr((*Protos)()))
			proto=(*Protos)();
		Protos->Next();
	}
	if (Params->Debug) cout << " with proto "<< proto->GetProfile()->GetId()<<" ="<<SubProfDistance(proto,sub)<<endl;

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
void GALILEI::GGroupingSupKMeans::SetConstraintsFromSamples(unsigned int** cor)
{
	GGroups* sample;
	GGroup* group;
	GSubProfile **sub1, **sub2;
	unsigned int**matrix, i, j, k, id1,id2;
	bool added;

	HardConstraints->Clear();

	//init the matrix
	matrix=new unsigned int*[SubProfiles.NbPtr] ;
	for (i=0; i<SubProfiles.NbPtr;i++)
		matrix[i]=new unsigned int [SubProfiles.NbPtr];
	for (i=0; i<SubProfiles.NbPtr;i++)
		for (j=0; j<SubProfiles.NbPtr;j++)
			matrix[i][j]=0;

	//fill the matrix
	for( FoundGroups->Start();!FoundGroups->End(); FoundGroups->Next())
	{
		sample=(*FoundGroups)();
		for (sample->Start(); !sample->End(); sample->Next() )
		{
			group=(*sample)();
			for (i=group->NbPtr, sub1=group->Tab; i--; sub1++)
			{
				for (sub2=sub1+1, j=0; j<i; sub2++,j++)
				{
					id1=(*sub1)->GetId();
					id2=(*sub2)->GetId();
					for (k=0; k<SubProfiles.NbPtr;k++)
					{
						if(cor[k][1]==id1)
							id1=cor[k][0];
						if(cor[k][1]==id2)
							id2=cor[k][0];
					}
					if(id1<id2) matrix[id1][id2]+=1;
					else  matrix[id2][id1]+=1;
				}
			}
		}
	}

	//build hard constraints
	for (i=0; i<SubProfiles.NbPtr;i++)
	{
		for (j=i+1; j<SubProfiles.NbPtr;j++)
		{
			if ((100.0*double(matrix[i][j])/ double(Params->NbSamples))>=Params->SameGroupRate)
			{
				if (Params->Debug) cout << "contraintes ok"<<endl;
				added=false;
				HardConstraints->Start() ;
				while(added==false&&!HardConstraints->End())
				{
					if( (*HardConstraints)()->GetPtr(SubProfiles.GetPtrAt(i)->GetId()) )
					{
						(*HardConstraints)()->InsertPtr(SubProfiles.GetPtrAt(j));
						added=true;
					}
					HardConstraints->Next();
				}
				if (added==false)
				{
					group=new GGroupVector(HardConstraints->NbPtr,Groups->GetLang());
					group->InsertPtr(SubProfiles.GetPtrAt(i)) ;
					group->InsertPtr(SubProfiles.GetPtrAt(j));
					HardConstraints->InsertPtr(group);
				}
			}
		}
	}
	for (i=0; i<SubProfiles.NbPtr;i++)
		delete[] matrix[i];
	delete[] matrix;

}


//-----------------------------------------------------------------------------
GALILEI::GGroupingSupKMeans::~GGroupingSupKMeans(void)
{
	if (FoundGroups)  delete FoundGroups;
	if (Protos) delete Protos;
	if (ProtosDblKMeans) delete ProtosDblKMeans;
}


