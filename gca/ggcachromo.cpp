/*

	Genetic Community Algorithm

	GGCAChromo.cpp

	Chromosome - Implementation

	Copyright 2002-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <gsubjects.h>
#include <guser.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggcachromo.h>
#include <ggcainst.h>
#include <ggcagroup.h>
#include <ggroup.h>
#include <ggcaobj.h>



//-----------------------------------------------------------------------------
//
// class GGCAChromo
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAChromo::GGCAChromo(GGCAInst* inst,unsigned int id)
	: RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj,GGCAGroupData>(inst,id),
	  ToDel(0), CritSimJ(0.0), CritAgreement(0.0), CritDisagreement(1.0), Protos(Used.GetMaxNb()),
	  thProm(0), thSols(0)
{
	(*Fitness)=0.0;
}


//-----------------------------------------------------------------------------
void GGCAChromo::Init(GGCAThreadData* thData)
{
	// Parent Initialisation
	RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj,GGCAGroupData>::Init(thData);

	// Current
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;
	thTests=thData->Tests;
	thProm=&thData->Prom;
	thSols=thData->Sols;
	ToDel=&thData->ToDel;
}


//-----------------------------------------------------------------------------
int GGCAChromo::Compare(const GGCAChromo* c) const
{
	return(Id-c->Id);
}


//-----------------------------------------------------------------------------
void GGCAChromo::ConstructChromo(GSession* grps)
{
	R::RCursor<GGroup> Grp;
	RCursor<GSubProfile> SubProfile;
	GGCAGroup* grp;
	GGCAObj** objs;
	unsigned int i;

	Grp=grps->GetGroups(Instance->Lang);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		// Reserve a GA group
		grp=ReserveGroup();

		// Go through the current profiles and store them in thObjs1
		SubProfile=Grp()->GetSubProfiles();
		for(SubProfile.Start(),objs=thObjs1,NbObjs1=0;!SubProfile.End();SubProfile.Next(),objs++,NbObjs1++)
			(*objs)=Instance->GetObj(SubProfile());

		// Mix randomly thObjs1
		Instance->RandOrder<GGCAObj*>(thObjs1,NbObjs1);

		// Put the objects in the group if possible
		for(objs=thObjs1,i=NbObjs1+1;--i;objs++)
		{
			if((*objs)&&(grp->CanInsert(*objs)))
				grp->Insert(*objs);
		}

		// Verify that the groups is not empty
		if(!grp->GetNbObjs())
			ReleaseGroup(grp);
	}
}


//-----------------------------------------------------------------------------
void GGCAChromo::RandomConstruct(void)
{
	// Look if already a solution in the session
	if(Instance->Session->GetNbGroups(Instance->Lang))
		ConstructChromo(Instance->Session);

	// Call classic heuristic for non-assigned objects
	RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj,GGCAGroupData>::RandomConstruct();
}


//-----------------------------------------------------------------------------
void GGCAChromo::Evaluate(void)
{
	unsigned int i;
	double min;
	double tmp;

	CritAgreement=CritDisagreement=CritSimJ=0.0;
	if(!Used.GetNb())
		return;
	R::RCursor<GGCAGroup> Cur(Used);
	R::RCursor<GGCAGroup> Cur2(Used);
	for(Cur.Start(),i=0,min=2.0;!Cur.End();Cur.Next(),i++)
	{
		if(!i)
			Cur()->Evaluate(CritSimJ,CritAgreement,CritDisagreement);
		if(i<Used.GetNb()-1)
		{
			for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
			{
				if(!i)
					Cur2()->Evaluate(CritSimJ,CritAgreement,CritDisagreement);
				tmp=1-Instance->GetSim(Cur()->GetRelevant()->GetSubProfile(),Cur2()->GetRelevant()->GetSubProfile());
				tmp=tmp*tmp;
				if(tmp<min)
					min=tmp;
			}
		}
	}

	tmp=static_cast<double>(Instance->Objs->GetNb())*static_cast<double>(Instance->Objs->GetNb()-1)/2;
	CritAgreement/=tmp;
	CritDisagreement/=tmp;
	CritSimJ=min/(CritSimJ*Used.GetNb());
}


//-----------------------------------------------------------------------------
void GGCAChromo::ReAllocate(void)
{
	unsigned int nb;
	double sim,maxsim;
	GGCAGroup* grp;
	GGCAObj** Cur;


	// Put the prototypes in Protos
	Protos.Clear();
	R::RCursor<GGCAGroup> Grp(Used);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		Grp()->ComputeRelevant();
		Protos.InsertPtr(Grp()->GetRelevant());
	}

	// Clear the chromosome
	Clear();

	// Insert the Prototypes in a group
	RCursor<GGCAObj> CurP(Protos);
	for(CurP.Start();!CurP.End();CurP.Next())
	{
		grp=ReserveGroup();
		grp->Insert(CurP());
		grp->SetRelevant(CurP());
	}

	// Mix randomly thObjs1
//	Instance->RandOrder<GGCAObj*>(thObjs1,Objs->GetNb());

	// Go through the ranbomly ordered subprofiles and put them in the group of the
	// most similar prototype.
	for(Cur=thObjs1,nb=Objs->GetNb()+1;--nb;Cur++)
	{
		// If the subprofile is a prototype -> already in a group
		if(GetGroup(*Cur)) continue;

		// Look first if one of the object with a ratio are already grouped
		RCursor<GGCAMaxRatio> Best(*Instance->Ratios[(*Cur)->GetId()]);
		for(Best.Start(),grp=0;(!Best.End())&&(!grp);Best.Next())
			grp=GetGroup(Best()->ObjId);

		// If no group find, -> Go through each groups
		if(!grp)
		{
			R::RCursor<GGCAGroup> Grp(Used);
			for(Grp.Start(),maxsim=-1.0;!Grp.End();Grp.Next())
			{
				// If all the hard constraints are not respected -> skip the group.
				if(!Grp()->CanInsert(*Cur))
					continue;

				// Compute similarity with the relevant profile of the group.
				sim=Grp()->ComputeRelSim(*Cur);
				if(sim>maxsim)
				{
					maxsim=sim;
					grp=Grp();
				}
			}
		}

		// If no group find -> Create a new group and make the current subprofile the
		// prototype of it.
		if(!grp)
		{
			grp=ReserveGroup();
			grp->SetRelevant(*Cur);
		}

		// Insert the subprofile in the current group.
		grp->Insert(*Cur);
	}
}


//-----------------------------------------------------------------------------
unsigned int GGCAChromo::CalcNewProtosNb(void)
{
	unsigned int count;
	R::RCursor<GGCAGroup> Grp;
	GGCAObj* OldProto;

	// Computed the prototypes for each groups and count the number in Protos
	Grp.Set(Used);
	for(Grp.Start(),count=0;!Grp.End();Grp.Next())
	{
		OldProto=Grp()->GetRelevant();
		Grp()->ComputeRelevant();
		if(OldProto!=Grp()->GetRelevant())
			count++;
	}
	return(count);
}


//-----------------------------------------------------------------------------
void GGCAChromo::DoKMeans(void)
{
	unsigned int itermax;
	double error,minerror;
	GGCAObj** obj;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("DoKMeans","GChomoIR");

	// Copy the subprofiles into thObjs1
	R::RCursor<GGCAGroup> Grp(Used);
	for(Grp.Start(),obj=thObjs1;!Grp.End();Grp.Next())
	{
		RCursor<GGCAObj> ptr=GetObjs(Grp());
		for(ptr.Start();!ptr.End();ptr.Next(),obj++)
		{
			(*obj)=ptr();
		}
	}

	// Mix randomly thObjs1
	Instance->RandOrder<GGCAObj*>(thObjs1,Objs->GetNb());

	// Max Iterations
	minerror=Instance->Params->Convergence/100.0;
	for(itermax=Instance->Params->MaxKMeans+1,error=1.0;(--itermax)&&(error>minerror);)
	{
		ReAllocate();
		error=static_cast<double>(CalcNewProtosNb())/static_cast<double>(Used.GetNb());
	}

	if(Instance->Debug)
	{
		char tmp[60];
		sprintf(tmp,"Number of k-Means runs: %u",itermax);
		Instance->Debug->PrintInfo(tmp);
		Instance->Debug->EndFunc("DoKMeans","GChomoIR");
	}
}


//-----------------------------------------------------------------------------
void GGCAChromo::DivideWorstSubProfiles(void)
{
	R::RCursor<GGCAGroup> Grp;
	unsigned int i;
	GGCAObj** ptr;
	GGCAObj** ptr2;
	GGCAObj** ptr3;
	double sim,minsim;
	GSubProfile* sub;
	GSubProfile* sub1;
	GSubProfile* sub2;
	GGCAGroup* grp;
	GGCAObj* worst1=0;
	GGCAObj* worst2=0;

	// Find the group containing the two most dissimilar subprofiles
	Grp.Set(Used);
	for(Grp.Start(),minsim=1.0,grp=0;!Grp.End();Grp.Next())
	{
		if(Grp()->GetNbObjs()<3) continue;
		RCursor<GGCAObj> CurObj(GetObjs(*Grp()));
		RCursor<GGCAObj> CurObj2(GetObjs(*Grp()));
		for(CurObj.Start(),i=0;i<Grp()->GetNbObjs()-1;CurObj.Next(),i++)
		{
			for(CurObj2.GoTo(i+1);!CurObj2.End();CurObj2.Next())
			{
				sim=Instance->GetSim(CurObj()->GetSubProfile(),CurObj2()->GetSubProfile());
				if(sim<minsim)
				{
					minsim=sim;
					grp=Grp();
					worst1=CurObj();
					worst2=CurObj2();
				}
			}
		}
	}

	// Put in worst1 in thObj1 and worst2 in thObjs2
	if(!grp) return;
	NbObjs1=NbObjs2=1;
	ptr2=thObjs1;
	ptr3=thObjs2;
	(*(ptr2++))=worst1;
	(*(ptr3++))=worst2;

	// Copy in thObjs1 the subprofiles most similar to worst1 and in thObjs2
	// the subprofiles most similar to worst2
	sub1=worst1->GetSubProfile();
	sub2=worst2->GetSubProfile();
	RCursor<GGCAObj> CurObj(GetObjs(*grp));
	for(CurObj.Start();!CurObj.End();CurObj.Next())
	{
		if((CurObj()==worst1)||(CurObj()==worst2)) continue;
		sub=CurObj()->GetSubProfile();
		sim=Instance->GetSim(sub1,sub);
		minsim=Instance->GetSim(sub2,sub);
		if(sim>minsim)
		{
			NbObjs1++;
			(*(ptr2++))=CurObj();
		}
		else
		{
			NbObjs2++;
			(*(ptr3++))=CurObj();
		}
	}

	// Release grp
	ReleaseGroup(grp);

	// Reserve a new group and insert the elements of thObjs1
	grp=ReserveGroup();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
		grp->Insert(*ptr);

	// Reserve a new group and insert the elements of thObjs2
	grp=ReserveGroup();
	for(ptr=thObjs2,i=NbObjs2+1;--i;ptr++)
		grp->Insert(*ptr);
}


//-----------------------------------------------------------------------------
void GGCAChromo::MergeBestSubProfiles(void)
{
	R::RCursor<GGCAObj> Cur,Cur2;
	unsigned int i,j;
	GGCAGroup* grp1;
	GGCAGroup* grp2;
	GGCAGroup* bestgrp1;
	GGCAGroup* bestgrp2;
	double sim,maxsim;
	GSubProfile* sub;
	GGCAObj** ptr;

	// Find the two groups containing the most similar objects.
	Cur=(*Objs);
	Cur2=(*Objs);
	for(Cur.Start(),i=Cur.GetNb(),j=0,maxsim=-1.0,bestgrp1=bestgrp2=0;--i;Cur.Next(),j++)
	{
		grp1=GetGroup(Cur());
		sub=Cur()->GetSubProfile();
		for(Cur2.GoTo(j+1);!Cur2.End();Cur2.Next())
		{
			grp2=GetGroup(Cur2());
			if(grp1==grp2) continue;
			sim=Instance->GetSim(sub,Cur2()->GetSubProfile());
			if(sim>maxsim)
			{
				maxsim=sim;
				bestgrp1=grp1;
				bestgrp2=grp2;
			}
		}
	}
	if((!bestgrp1)||(!bestgrp2)) return;

	// Put the objects of bestgrp1 in bestgrp2 only if there are not already have the same user
	RCursor<GGCAObj> CurObj(GetObjs(*bestgrp1));
	for(CurObj.Start(),ptr=thObjs1,NbObjs1=0;!CurObj.End();CurObj.Next(),ptr++,NbObjs1++)
		(*ptr)=CurObj();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
	{
		if(bestgrp2->HasSameUser(*thObjs1))
			continue;
		bestgrp1->Delete(*ptr);
		bestgrp2->Insert(*ptr);
	}

	// Release the groups
	if(!bestgrp1->GetNbObjs())
		ReleaseGroup(bestgrp1);
}


//-----------------------------------------------------------------------------
void GGCAChromo::TreatSocialSubProfiles(bool rel)
{
	GGCAObj* obj;
	R::RCursor<GGCAGroup> Cur1(Used),Cur2(Used);
	GGCAGroup* grp=0;
	double tmp,max;

//	cout<<"Do not treat social"<<endl;
	return;
	
	// Look for the groups to delete
	ToDel->Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		// Look if this group contains only 1 social profile
		if(Cur1()->GetNbObjs()!=1)
			continue;
		obj=Cur1()->GetObjPos(0);
		if(!obj->GetSubProfile()->GetProfile()->IsSocial())
			continue;

		// Find a new group
		for(Cur2.Start(),max=-1.0,grp=0;!Cur2.End();Cur2.Next())
		{
			if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
				continue;
			if(Cur2()->HasSameUser(obj))
				continue;
			Cur2()->ComputeRelevant();
			if(rel)
				tmp=Instance->GetSim(obj->GetSubProfile(),Cur2()->GetRelevant()->GetSubProfile());
			else
				tmp=Cur2()->ComputeHomogeneity(obj);
			if(tmp>max)
			{
				max=tmp;
				grp=Cur2();
			}
		}
		if(grp)
		{
			Cur1()->Delete(obj);
			grp->Insert(obj);
			ToDel->InsertPtr(Cur1());
		}
	}

	// Delete
	Cur1.Set(*ToDel);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
		ReleaseGroup(Cur1());
}


//-----------------------------------------------------------------------------
void GGCAChromo::Optimisation(void)
{
	unsigned int i;
	unsigned int nb;
	RPromSol** s;
	RPromSol** tab;
	GGCAChromo* LastDiv;
	GGCAChromo* LastMerge;

	// Do not optimize
	if(!Instance->Params->NbDivChromo)
		return;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("Optimisation","GChomoIR");

	// Copy the current chromosome in thTests
	Evaluate();
	(*thTests[0])=(*this);
	LastDiv=LastMerge=this;
	for(i=Instance->Params->NbDivChromo+1;--i;)
	{
		(*thTests[i*2-1])=(*LastDiv);
		LastDiv=thTests[i*2-1];
		LastDiv->DivideWorstSubProfiles();
		(*thTests[i*2])=(*LastMerge);
		LastMerge=thTests[i*2];
		LastMerge->MergeBestSubProfiles();
	}

	// Do K-Means on the temporaries chromosomes.
	nb=(Instance->Params->NbDivChromo*2)+1;
	for(i=0;i<nb;i++)
	{
		thTests[i]->DoKMeans();
		thTests[i]->TreatSocialSubProfiles(true);
		thTests[i]->Evaluate();
	}

	// Use PROMETHEE to determine the best solution.
	s=thSols;
	thProm->Assign(*s,this);
	for(i=0,s++;i<nb;i++,s++)
	{
		thProm->Assign(*s,thTests[i]);
	}
	thProm->ComputePrometheeII();
	if(thProm->GetBestSol()->GetId())
		(*this)=(*thTests[thProm->GetBestSol()->GetId()-1]);

	if(Instance->Debug)
	{
		for(tab=s=thProm->GetSols(),i=nb+2;--i;s++)
		{
			if((*s)->GetId())
				Instance->WriteChromoInfo(thTests[(*s)->GetId()-1]);
			else
				Instance->WriteChromoInfo(this);
		}
		delete[] tab;
		Instance->Debug->EndFunc("Optimisation","GChomoIR");
	}
}


//-----------------------------------------------------------------------------
void GGCAChromo::Modify(void)
{
	RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj,GGCAGroupData>::Mutation();
}



//-----------------------------------------------------------------------------
bool GGCAChromo::SameGroup(unsigned int obj1,unsigned int obj2) const
{
	return(ObjectsAss[obj1]==ObjectsAss[obj2]);
}


//-----------------------------------------------------------------------------
GGCAChromo& GGCAChromo::operator=(const GGCAChromo& chromo)
{
	RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj,GGCAGroupData>::operator=(chromo);
	CritSimJ=chromo.CritSimJ;
	CritAgreement=chromo.CritAgreement;
	CritDisagreement=chromo.CritDisagreement;
	Fi=chromo.Fi;
	FiPlus=chromo.FiPlus;
	FiMinus=chromo.FiMinus;
	return(*this);
}


//-----------------------------------------------------------------------------
GGCAChromo::~GGCAChromo(void)
{
}
