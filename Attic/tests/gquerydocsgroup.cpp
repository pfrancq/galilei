/*

	GALILEI Research Project

	GQueryDocsGroup.cpp

	Query from Doc Sim with groups  - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
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
#include <tests/gquery.h>
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
// class GQueryDocsGroup
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GQueryDocsGroup::GQueryDocsGroup(GSession* ses) throw(bad_alloc)
{
	double simintra=0.0;
	double siminter=0.0;
	double comptintra=0.0;
	double comptinter=0.0;
	double comptgood=0.0;
	double compttot=0.0;
	RContainer<GGroupsEvaluate,unsigned int,false,false>* idealgroup=new RContainer<GGroupsEvaluate,unsigned int,false,false>(2,2);
	GGroupsEvaluate* groups;
	GGroupEvaluateDoc* group;
	GDoc* Doc;
	GDocVector* DocV;
	idealgroup=ses->GetIdealDoc();
	GIWordWeight** Order=new GIWordWeight*[5000];
	GIWordsWeights* QueryWord = new GIWordsWeights(5000);
	GIWordsWeights* QueryWord1 = new GIWordsWeights(1);

	for(idealgroup->Start();!idealgroup->End();idealgroup->Next())
	{
		groups=(*idealgroup)();
		//for each group in this languages
		for(groups->Start();!groups->End();groups->Next())
		{
			group=static_cast<GGroupEvaluateDoc*>((*groups)());
			
			// for each doc  add the wheigth of all the words of the current doc
			// in the current container
			for(group->Start();!group->End();group->Next())
			{
				Doc=ses->GetDoc(group->Current());
				GProfDocCursor ProfDoc=Doc->GetProfDocCursor();
				if(ProfDoc.End())
				{
					DocV=static_cast<GDocVector*>(Doc);
					GIWordWeightCursor WordCusor=DocV->GetWordWeightCursor();
			 		for(WordCusor.Start();!WordCusor.End();WordCusor.Next())
					{
						GIWordWeight* tmp = QueryWord->GetPtr(WordCusor());
						if(!tmp) QueryWord->InsertPtr(new GIWordWeight(WordCusor()));
						else
						{
							tmp->AddWeight(WordCusor()->GetWeight());
						}
					}
				}
			}
	
			QueryWord->Transform(otDocs,groups->GetLang());

			if(Order) delete[] Order;
			int MaxOrderSize=static_cast<unsigned int>((QueryWord->NbPtr+1)*1.1);
			Order=new GIWordWeight*[MaxOrderSize];
			memcpy(Order,QueryWord->Tab,QueryWord->NbPtr*sizeof(GIWordWeight*));
			qsort(static_cast<void*>(Order),QueryWord->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
			Order[QueryWord->NbPtr]=0;
			for(int i=0;i<10;i++)
			{
				QueryWord1->Clear();
				QueryWord1->InsertPtr(new GIWordWeight(Order[i]));
				GQuery qq(QueryWord1,ses,groups->GetLang(),0.000,true);
				double comptfind=0.0;
				for(qq.ResStart();!qq.ResEnd();qq.ResNext())
				{
					comptfind++;
					GGroup* temp=qq.GetCurrentGroup();
					double sim=qq.GetCurrentSim();
					if(temp->GetId()==group->GetId())
					{
						comptgood+=(1/comptfind);
						simintra+=sim;
						comptintra++;
					}
					else
					{
						siminter+=sim;
						comptinter++;
					}
					
				}
				compttot++;
			}
			QueryWord->Clear();
			cout<<"nbgood "<<comptgood/comptintra<<" simintra "<<simintra/comptintra<<" siminter "<<siminter/comptinter<<endl;
		}
	}
	cout<<" the   nbgood "<<comptgood/comptintra<<" simintra "<<simintra/comptintra<<" siminter "<<siminter/comptinter<<endl;
	delete(idealgroup);
	if(Order) delete[] Order;
}


//-----------------------------------------------------------------------------
GALILEI::GQueryDocsGroup::~GQueryDocsGroup(void)
{
}
