/*

	GALILEI Research Project

	GQueryDocsGroup.cpp

	Query from Doc Sim with groups  - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
// include files for GALILEI
#include <tests/gquerydocsgroup.h>
#include <tests/ggroupsevaluate.h>
#include <tests/ggroupevaluatedoc.h>
#include <sessions/gsession.h>
#include <docs/gdocvector.h>
#include <docs/gdoc.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroupvector.h>
#include <groups/ggroupcalcgravitation.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GroupSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GQueryDocsGroup::GroupSim
{
public:
	GGroup* Grp;
	double Sim;

	GroupSim(GGroup* g,double s) : Grp(g), Sim(s) {}
	int Compare(const GroupSim*) const {return(-1);}
	static int sortOrder(const void *a,const void *b);
};


//-----------------------------------------------------------------------------
int GALILEI::GQueryDocsGroup::GroupSim::sortOrder(const void *a,const void *b)
{
	double af=(*((GroupSim**)(a)))->Sim;
	double bf=(*((GroupSim**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//-----------------------------------------------------------------------------
//
// class GQueryDocsGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GQueryDocsGroup::GQueryDocsGroup(GSession* ses) throw(bad_alloc)
	: Session(ses), Order(0), SizeOrder(6000), Groups(50)
{
	Order=new GIWordWeight*[SizeOrder];
}


//-----------------------------------------------------------------------------
void GALILEI::GQueryDocsGroup::Run(unsigned int first,unsigned int nb,bool trans)
{
	GGroupEvaluateDoc* group;
	GDoc* Doc;
	GDocVector* DocV;
	GIWordsWeights QueryWord(5000);
	GIWordsWeights QueryWord1(5);
	GGroupCalc* Calc=Session->GetCurrentGroupCalcMethod();
	GGroupsEvaluateCursor IdealDocs=Session->GetIdealDocsCursor();
	GGroupsCursor IdealGroups=Session->GetIdealGroupsCursor();

	// Init Part
	First=Second=SimQueryIntra=SimQueryInter=Recall=0.0;
	queries=comptintra=comptinter=0;
	Transform=trans;
	Best=first;
	Nb=nb;
	Index=new unsigned int[nb];

	// Compute Ideal Group Description
	for(IdealGroups.Start();!IdealGroups.End();IdealGroups.Next())
	{
		for(IdealGroups()->Start();!IdealGroups()->End();IdealGroups()->Next())
			Calc->Compute((*IdealGroups())());
	}

	// Do Queries
	for(IdealDocs.Start();!IdealDocs.End();IdealDocs.Next())
	{
		for(IdealDocs()->Start();!IdealDocs()->End();IdealDocs()->Next())
		{
			group=static_cast<GGroupEvaluateDoc*>((*IdealDocs())());
			QueryWord.Clear();

			// Sum the weight of each information entity in a new vector,
			// compute idf factor and ordered it in descending order
			for(group->Start();!group->End();group->Next())
			{
				Doc=Session->GetDoc(group->Current());
				if(!Doc->GetNbFdbks())
				{
					DocV=static_cast<GDocVector*>(Doc);
					GIWordWeightCursor WordCursor=DocV->GetWordWeightCursor();
					for(WordCursor.Start();!WordCursor.End();WordCursor.Next())
					{
						GIWordWeight* tmp = QueryWord.GetPtr(WordCursor());
						if(!tmp)
						{
							QueryWord.InsertPtr(new GIWordWeight(WordCursor()));
						}
						else
						{
							tmp->AddWeight(WordCursor()->GetWeight());
						}
					}
				}
			}
			QueryWord.Transform(otDocs,IdealDocs()->GetLang());
			if(SizeOrder<QueryWord.NbPtr)
			{
				delete[] Order;
				SizeOrder=static_cast<unsigned int>((QueryWord.NbPtr+1)*1.1);
				Order=new GIWordWeight*[SizeOrder];
			}
			memcpy(Order,QueryWord.Tab,QueryWord.NbPtr*sizeof(GIWordWeight*));
			qsort(static_cast<void*>(Order),QueryWord.NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);

			// Construct and apply the queries
			Lang=IdealDocs()->GetLang();
			GroupId=group->GetId();
			if(first>QueryWord.NbPtr) first=QueryWord.NbPtr;
			if(nb>first) nb=first;
			AddQuery(0,0);
		}
	}

	// Computes Average Values
	Recall/=comptintra;
	First/=queries;
	Second/=queries;
	SimQueryIntra/=comptintra;
	SimQueryInter/=comptinter;
	delete[] Index;
}


//-----------------------------------------------------------------------------
void GALILEI::GQueryDocsGroup::AddQuery(unsigned int pos,unsigned level)
{
	unsigned int i;
	unsigned int *idx;

	if(pos==Best) return;
	Index[level++]=pos;
	if(level==Nb)
	{
		GIWordsWeights Query(10);
		for(i=level+1,idx=Index;--i;idx++)
		{
			Query.InsertPtr(new GIWordWeight(Order[*idx]->GetId(),2.0));
		}
		queries++;
		DoQuery(Query);
	}
	else
	{
		AddQuery(pos+1,level);
	}
	AddQuery(pos+1,level-1);
}


//-----------------------------------------------------------------------------
void GALILEI::GQueryDocsGroup::DoQuery(GIWordsWeights& query)
{
	unsigned int pos;
	GroupSim** ptr;
	unsigned int g;
	GGroupCursor Group;
	GGroupsCursor IdealGroups=Session->GetIdealGroupsCursor();
	double sim;

	if(Transform)
		query.ModifyQueryGroups(otGroups,Lang);

	// Parse all the groups and insert those which have a sim with
	// the query greater than 0.0
	Groups.Clear();
	for(IdealGroups.Start();!IdealGroups.End();IdealGroups.Next())
	{
		Group=IdealGroups()->GetGroupCursor();
		for(Group.Start();!Group.End();Group.Next())
		{
			if(!Group()->NbPtr) continue;
			sim=query.Similarity(static_cast<GGroupVector*>(Group())->GetVector());
			if(sim>0.0)
				Groups.InsertPtr(new GroupSim(Group(),sim));
		}
	}
	qsort(static_cast<void*>(Groups.Tab),Groups.NbPtr,sizeof(GroupSim*),GroupSim::sortOrder);

	// Parse the groups in descending order of similarity with the current query
	for(ptr=Groups.Tab,g=Groups.NbPtr+1,pos=0,comptfind=0;--g;ptr++)
	{
		comptfind++;
		GGroup* temp=(*ptr)->Grp;
		sim=(*ptr)->Sim;
		if(temp->GetId()==GroupId)
		{
			Recall+=(1/comptfind);
			SimQueryIntra+=sim;
			comptintra++;
			pos=Groups.NbPtr-g;
		}
		else
		{
			SimQueryInter+=sim;
			comptinter++;
		}
	}
	if(pos==0)
		First++;
	if(pos<=1)
		Second++;
}


//-----------------------------------------------------------------------------
GALILEI::GQueryDocsGroup::~GQueryDocsGroup(void)
{
	if(Order) delete[] Order;
}
