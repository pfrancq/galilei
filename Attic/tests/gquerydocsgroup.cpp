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
class GroupSim
{
public:
	GGroup* Grp;
	double Sim;

	GroupSim(GGroup* g,double s) : Grp(g), Sim(s) {}
	int Compare(const GroupSim*) const {return(-1);}
	static int sortOrder(const void *a,const void *b);
};


//-----------------------------------------------------------------------------
int GroupSim::sortOrder(const void *a,const void *b)
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
	: Session(ses), Order(0), SizeOrder(6000)
{
	Order=new GIWordWeight*[SizeOrder];
}


//-----------------------------------------------------------------------------
void GALILEI::GQueryDocsGroup::Run(unsigned int first,unsigned int nb,bool trans)
{
	unsigned int comptintra;
	unsigned int comptinter;
	unsigned int comptfind;
	unsigned int pos;
	unsigned int queries;
	GGroupsEvaluateCursor IdealDocs;
	GGroupEvaluateDoc* group;
	GDoc* Doc;
	GDocVector* DocV;
	GIWordsWeights QueryWord(5000);
	GIWordsWeights QueryWord1(5);
	RContainer<GroupSim,unsigned int,false,false> Groups(50);
	GroupSim** ptr;
	unsigned int j;
	GGroupsCursor IdealGroups;
	GGroupCursor Group;
	GGroupCalc* Calc=Session->GetCurrentGroupCalcMethod();

	// Init Part
	IdealGroups=Session->GetIdealGroupsCursor();
	First=Second=SimQueryIntra=SimQueryInter=Recall=0.0;
	queries=comptintra=comptinter=0;

	// Compute Ideal Group Description
	for(IdealGroups.Start();!IdealGroups.End();IdealGroups.Next())
	{
		for(IdealGroups()->Start();!IdealGroups()->End();IdealGroups()->Next())
			Calc->Compute((*IdealGroups())());
	}

	// Do Queries
	IdealDocs=Session->GetIdealDocsCursor();
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
			for(unsigned int i=0;i<first;i++)
			{
				queries++;
				QueryWord1.Clear();
				QueryWord1.InsertPtr(new GIWordWeight(Order[i]->GetId(),2.0));
				if(trans)
					QueryWord1.ModifyQueryGroups(otGroups,IdealDocs()->GetLang());

				// Parse all the groups and insert those which have a sim with
				// the query greater than 0.0
				Groups.Clear();
				for(IdealGroups.Start();!IdealGroups.End();IdealGroups.Next())
				{
					Group=IdealGroups()->GetGroupCursor();
					for(Group.Start();!Group.End();Group.Next())
					{
						GGroup* Grp=Group();
						if(!Grp->NbPtr) continue;
						double simidf=QueryWord1.Similarity(static_cast<GGroupVector*>(Group())->GetVector());
						if(simidf>0.0)
							Groups.InsertPtr(new GroupSim(Grp,simidf));
					}
				}
				qsort(static_cast<void*>(Groups.Tab),Groups.NbPtr,sizeof(GroupSim*),GroupSim::sortOrder);

				// Parse the groups in descending order of similarity with the current query
				for(ptr=Groups.Tab,j=Groups.NbPtr+1,pos=0,comptfind=0;--j;ptr++)
				{
					comptfind++;
					GGroup* temp=(*ptr)->Grp;
					double sim=(*ptr)->Sim;
					if(temp->GetId()==group->GetId())
					{
						Recall+=(1/comptfind);
						SimQueryIntra+=sim;
						comptintra++;
						pos=Groups.NbPtr-j;
					}
					else
					{
						SimQueryInter+=sim;
						comptinter++;
					}
				}
				if(pos==0)
				{
					First++;
				}
				if(pos<=1)
				{
					Second++;
				}
			}
		}
	}

	// Computes Average Values
	Recall/=comptintra;
	First/=queries;
	Second/=queries;
	SimQueryIntra/=comptintra;
	SimQueryInter/=comptinter;
}


//-----------------------------------------------------------------------------
GALILEI::GQueryDocsGroup::~GQueryDocsGroup(void)
{
	if(Order) delete[] Order;
}
