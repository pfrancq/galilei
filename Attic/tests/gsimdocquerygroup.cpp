/*

	GALILEI Research Project

	GSimDocQueryGroup.cpp

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
#include <tests/gsimdocquerygroup.h>
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
#include <groups/ggroupvector.h>
#include <groups/ggroupcalcgravitation.h>
using namespace GALILEI;




//-----------------------------------------------------------------------------
//
// class GSimDocQueryGroup
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSimDocQueryGroup::GSimDocQueryGroup(GSession* ses) throw(bad_alloc)
{
	unsigned int NbWordPerGroup=2;
	int MinOccurs=100;
	int NbTest=10;
//	ses->DocsFilter(20,20);
	const char* name;
	RContainer<GGroupsEvaluate,unsigned int,false,false>* idealgroup;
	GGroupsEvaluate* groups;
	GGroupEvaluateDoc* group;
	GDoc* Doc;
	GDocVector* DocV;
	idealgroup=ses->GetIdealDoc();

	GIWordsWeights* QueryWord = new GIWordsWeights(5000);
	GIWordsWeights* FilteredQueryWord = new GIWordsWeights(100);

	// for each languages
	for(int j=0;j<NbTest;j++)
	{
		int NbtotTrue=0;
		int NbtotFalse=0;
		int nbtot=0;

		for(idealgroup->Start();!idealgroup->End();idealgroup->Next())
		{
			groups=(*idealgroup)();

			// For each group in this language
			for(groups->Start();!groups->End();groups->Next())
			{
				group=static_cast<GGroupEvaluateDoc*>((*groups)());
   	
				FilteredQueryWord->Clear();
				QueryWord->Clear();

				// for each doc  add the weigth of all the words of the current doc
				// in the current container
				for(group->Start();!group->End();group->Next())
				{
					Doc=ses->GetDoc(group->Current());
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

				//filtrer the container and get the sum of the restant weight;
				double TotWeight=0;
				int ComptWord=0;
				for(QueryWord->Start();!QueryWord->End();QueryWord->Next())
				{
					GIWordWeight* tmp=(*QueryWord)();
					if(tmp->GetWeight()<MinOccurs) tmp->SetWeight(0);
					else
					{
						TotWeight+=tmp->GetWeight();
						ComptWord++;
					}
				}
				cout<<" Nbre de Mots restant "<<ComptWord<<" ";

				// We make a new container of giwordsweight with NbWordPerGroup word
				// randomly taked whit a maximum probability for the max weigth
				for(unsigned int i=0;i<NbWordPerGroup;i++)
				{
					int curentsum=0;
					int max=int(Random->Value(int(TotWeight)));
					bool find=false;
					for(QueryWord->Start();(!QueryWord->End())&&(!find);QueryWord->Next())
					{
						GIWordWeight* tmp=(*QueryWord)();
						curentsum+=int(tmp->GetWeight());
						if(max<curentsum)
						{
							if(!FilteredQueryWord->GetPtr(tmp)) FilteredQueryWord->InsertPtr(new GIWordWeight(tmp)) ;
							find=true;
							cout<<" mot garde: "<<groups->GetLang()->GetDict()->GetWord(tmp->GetId())<<" ";
						}
					}
				}

				// Compare the GIwordweights container to all the groups...
				GGroupsCursor idealgroups=ses->GetGroupsCursor();
				double MaxSim=0.0;
				bool SameGroup=false;
				for(idealgroups.Start();!idealgroups.End();idealgroups.Next())
				{
					GGroupCursor Group;
					Group=idealgroups()->GetGroupCursor();
					//for each group
					for(Group.Start();!Group.End();Group.Next())
					{
						GGroup* Grp=Group();
						GGroupVector* GrpV=static_cast<GGroupVector*>(Group());
						if(FilteredQueryWord->Similarity(GrpV->GetVector())>MaxSim)
						{
							MaxSim=FilteredQueryWord->Similarity(GrpV->GetVector());
							if(group->GetId()==Grp->GetId()) SameGroup=true;
								else SameGroup=false;
							bool end=true;
							for(Grp->Start();end;Grp->Next())
							{
								name=(*Grp)()->GetProfile()->GetName();
								end=false;
							}
						}

					}
				}
				if(SameGroup)
				{
					NbtotTrue++;
					cout<<" Bon groupe "<<group->GetId()<<" max sim avec "<<name<<endl;
				}
				else
				{
					NbtotFalse++;
					cout<<" Mauvais Groupe "<<group->GetId()<<" max sim avec "<<name<<endl;
				}
				nbtot++;
			}
		}
		cout<<NbtotTrue<<" "<<NbtotFalse<<" "<<nbtot<<endl;
	}
	delete(QueryWord);
	delete(FilteredQueryWord);
}


//-----------------------------------------------------------------------------
GALILEI::GSimDocQueryGroup::~GSimDocQueryGroup(void)
{
}
