/*

  gwordsclustering.cpp

  Description - Implementation.

  (c) 2002 by N. Kumps (nkumps@ulb.ac.be).                            

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

//---------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>
//---------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gwordsclustering.h>
#include <docs/gdocvector.h>
#include <groups/gsubject.h>
#include <infos/giwordlist.h>
#include <infos/giwordweight.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <langs/gwordlist.h>
using namespace GALILEI;
using namespace RStd;

//-----------------------------------------------------------------------------
//
// class GWordDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::GWordDocs()
{
	Docs=new RContainer<GDoc,unsigned,false,true>(10);
}
//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::GWordDocs(unsigned id)
	: Id(id)
{
	Docs=new RContainer<GDoc,unsigned,false,true>(10);
}
//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const GWordDocs* n) const
{
	return(Id-(n->GetId()));
}
//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const GWordDocs& n) const
{
	return(Id-(n.GetId()));
}
//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const unsigned id) const
{
	return(Id-id);
}
//-----------------------------------------------------------------------------
unsigned GALILEI::GWordsClustering::GWordDocs::GetId() const
{
	return(Id);
}
//-----------------------------------------------------------------------------
double GALILEI::GWordsClustering::GWordDocs::GetIdf() const
{
	return(Idf);
}
//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::GWordDocs::SetIdf(double idf) 
{
	Idf=idf;
}
//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::sortOrder(const void *a,const void *b)
{
	double af=(*((GWordDocs**)(a)))->GetIdf();
	double bf=(*((GWordDocs**)(b)))->GetIdf();

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}
//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::GWordDocs::AddDoc(GDoc* doc)
{
	Docs->InsertPtr(doc);
}
//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::~GWordDocs()
{
	delete(Docs);
}

//-----------------------------------------------------------------------------
//
// class GWordsClustering
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordsClustering(GDict* dic,unsigned mindocs,unsigned maxdocs,unsigned minocc)
{
	Words=new RContainer<GWordDocs,unsigned,true,true>(1000);
	Doc=new RContainer<GDoc,unsigned,false,false>(100,50);
	Dic=dic;
	NbMinDocs=mindocs;
  NbMaxDocs=maxdocs;
	NbMinOcc=minocc;
}
//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::AddDoc(GDocVector* doc)
{
	GWordDocs* worddocs;
	Doc->InsertPtr(doc);
	for (doc->Start();!doc->End();doc->Next())
	{
		if(!Dic->GroupsList.IsIn((*doc)()->GetId()))
		{
			if(Words->IsIn((*doc)()->GetId()))
			{
				if((*doc)()->GetWeight()>=NbMinOcc)
				{
					if(Words->GetPtr((*doc)()->GetId())->Docs->NbPtr<NbMaxDocs)
	        {
						Words->GetPtr((*doc)()->GetId())->AddDoc(doc);
    	    }
				}
			}
			else
				if((*doc)()->GetWeight()>=NbMinOcc)
				{
					worddocs=new GWordDocs((*doc)()->GetId());
					worddocs->SetIdf(log(double((doc->GetLang()->GetRef(otDoc,tWord))/doc->GetLang()->GetRef((*doc)()->GetId(),otDoc))));
					worddocs->AddDoc(doc);
					Words->InsertPtr(worddocs);
				}
		}
	}
}
//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::Sort()
{
	unsigned int MaxtmpSize;
	MaxtmpSize=static_cast<unsigned int>((Words->NbPtr+1)*1.1);
	Order =new GWordDocs*[MaxtmpSize];
	memcpy(Order,Words->Tab,Words->NbPtr*sizeof(GWordDocs*));
	qsort(static_cast<void*>(Order),Words->NbPtr,sizeof(GWordDocs*),GWordDocs::sortOrder);
	Order[Words->NbPtr]=0;
}
//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::CleanWords()
{
	GWordDocs* del,**cur;
	cur=Words->Tab;
	while((*cur))
	{
		if(((unsigned((*cur)->Docs->NbPtr))<NbMinDocs)||((unsigned((*cur)->Docs->NbPtr))>NbMaxDocs))
		{
			del=(*cur);
			Words->DeletePtr(del);
		}
		else
			cur++;
	}

}

//---------------------------------------------------------------------------
bool GALILEI::GWordsClustering::OrdreByDocs(unsigned n)
{
	GIWordList* tocompute,*toins,*tocompute2;
	unsigned i,k,l,nb,nbmax,maxptr,id,it;
	RContainer<GDoc,unsigned,false,true>* docslist,*mem;
	bool change,end;
	nbmax=5;
	it=1;
	change=false;
	end=false;
	Sort();
	if(n==0)
	{
		Groups=new RContainer<GIWordList,unsigned,true,true>(1000);
		for(i=0;i<Words->NbPtr;i++)
		{
			toins=new GIWordList();
			toins->SetId(Order[i]->GetId());
			toins->InsertPtr(new GIWord(Order[i]->GetId()));
			Groups->InsertPtr(toins);
		}
	}
	end=true;
	maxptr=Groups->NbPtr;
	for(i=0;i<maxptr;i++)
	{
		if(Groups->GetPtrAt(i))
		{
			tocompute=Groups->GetPtrAt(i);
			change=false;
			for(l=i+1;l<maxptr;l++)
			{
				if(Groups->GetPtrAt(l))
				{
					tocompute2=Groups->GetPtrAt(l);
					nb=0;
					k=0;
					RContainer<GDoc,unsigned,false,true> *cur1,*cur2;
					docslist=new RContainer<GDoc,unsigned,false,true>(10);
					cur1=Words->GetPtr(tocompute->GetId())->Docs;
					cur2=Words->GetPtr(tocompute2->GetId())->Docs;
					cur2->Start();
					for(cur1->Start();!cur1->End();cur1->Next())
						{
							while((!cur2->End())&&((*cur2)()->GetId()<(*cur1)()->GetId()))
								cur2->Next();
							if((!cur2->End())&&((*cur2)()->GetId()==(*cur1)()->GetId()))
							{
								docslist->InsertPtr((*cur1)());
							}
						}
						if(docslist->NbPtr>nbmax+1)
						{
							change=true;
							nbmax=docslist->NbPtr;
							id=tocompute2->GetId();
							mem=new RContainer<GDoc,unsigned,false,true>(10);
							for(docslist->Start();!docslist->End();docslist->Next())
								mem->InsertPtr((*docslist)());
						}
					delete(docslist);
				}
			}
			if(change)
			{
				toins=Groups->GetPtr(id);
				for(toins->Start();!toins->End();toins->Next())
					tocompute->InsertPtr(new GIWord((*toins)()->GetId()));
				delete(Words->GetPtr(tocompute->GetId())->Docs);
				Words->GetPtr(tocompute->GetId())->Docs=new RContainer<GDoc,unsigned,false,true>(10);
				for(mem->Start();!mem->End();mem->Next())
					Words->GetPtr(tocompute->GetId())->Docs->InsertPtr((*mem)());
				end=false;
				change=false;
				Groups->DeletePtr(toins);
				nbmax=5;
			}
		}
	}
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		if((*Groups)()->NbPtr>1)
		{
		for(((*Groups)())->Start();!((*Groups)())->End();((*Groups)())->Next())
			cout<<Dic->GetWord(Words->GetPtr((*((*Groups)()))()->GetId())->GetId())<<" ";
		nb=unsigned(Words->GetPtr((*Groups)()->GetId())->Docs->NbPtr);
		cout<<" / Nombre de docs = "<<nb<<endl;
		}
	}
	return(end);
}
//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::View()
{
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		cout<<(Dic->GroupsList)()->GetId()<<" = ";
		for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			cout<<(*(Dic->GroupsList)()->List)()->GetId()<<" / ";
		cout<<endl;
	}
}
//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::UpdateDoc(GDocVector* doc)
{
	unsigned mult;
	double nbocc;
	bool isin;
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		if(!doc->IsIn((Dic->GroupsList)()->GetId()))
		{
//			nbocc=0.0;
			isin=true;
			mult=0;
			for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			{
				mult++;
				if(doc->IsIn((*(Dic->GroupsList)()->List)()->GetId()))
					nbocc=doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight();
				else isin=false;
			}
			if(isin) doc->AddWordList((Dic->GroupsList)()->GetId(),nbocc*mult);
		}
	}
}

//---------------------------------------------------------------------------
void GALILEI::GWordsClustering::SaveAssociations(unsigned n,bool save)
{
	double nb;
	GWordList *tmp;
	GDocVector* doc;
//	bool tosave;
	unsigned mult;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
//		tosave=true;
		if(/*(tosave)&&*/((*Groups)()->NbPtr>1))
		{
			mult=0;
			tmp=new GWordList(cNoRef,"grouplist"+itoa(Dic->GetNbGroupsList()));
			((*Groups)())->Start();
			while(!((*Groups)())->End())
			{
				tmp->InsertWord(new GWord((*((*Groups)()))()->GetId(),Dic->GetWord((*((*Groups)()))()->GetId())));
				mult++;
				((*Groups)())->Next();
			}
			Dic->InsertNewWordList((*tmp),save);
			nb=1000;
			for(Words->GetPtr((*Groups)()->GetId())->Docs->Start();!Words->GetPtr((*Groups)()->GetId())->Docs->End();Words->GetPtr((*Groups)()->GetId())->Docs->Next())
			{
				doc=dynamic_cast<GDocVector*>((*Words->GetPtr((*Groups)()->GetId())->Docs)());
				for(((*Groups)())->Start();!((*Groups)())->End();((*Groups)())->Next())
					if(nb>doc->GetPtr(Words->GetPtr((*((*Groups)()))()->GetId())->GetId())->GetWeight()) nb=doc->GetPtr(Words->GetPtr((*((*Groups)()))()->GetId())->GetId())->GetWeight();
				if(!doc->IsIn(tmp->GetId()))doc->AddWordList(tmp->GetId(),nb*mult);//mult=facteur multiplicatif
				doc->SetInfos(doc->GetLang(),doc->GetN(),doc->GetNdiff(),doc->GetV(),doc->GetVdiff());
			}
		}
	}
}
//---------------------------------------------------------------------------
void GWordsClustering::Clear()
{
	if(Doc)
	{
		Doc->Clear();
	}
	if(Words)
	{
		Words->Clear();
	}
}
//---------------------------------------------------------------------------
GWordsClustering::~GWordsClustering()
{
	delete(Order);
	delete(Doc);
	delete(Groups);
	delete(Words);
}

