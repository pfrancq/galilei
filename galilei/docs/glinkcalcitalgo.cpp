/*
	GALILEI Research Project

	GLinkCalcItAlgo.cpp

	Link Calculation for Iterative Algorithm - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
//include file for GALILEI
#include <docs/glinkcalcitalgo.h>
#include <sessions/gsession.h>
#include <docs/glinks.h>
#include <docs/glink.h>
#include <sessions/gslot.h>

using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GLinkCalcItAlgoParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcItAlgoParams::GLinkCalcItAlgoParams(void)
	: GLinkCalcParams("Iterative Algorithm")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcItAlgoParams::GetSettings(void)
{
	static char tmp[300];

	sprintf(tmp,"%u %u",NbIteration,NbResults);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgoParams::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u",&NbIteration,&NbResults);
}



//-----------------------------------------------------------------------------
//
// GLinkCalcItAlgo
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcItAlgo::GLinkCalcItAlgo(GSession* session,GLinkCalcItAlgoParams* p) throw(bad_alloc)
	: GLinkCalc("Iterative Algorithm",session), Params(p)
{
	Links_Out = new RContainer<GLinks,unsigned int,true,true> (5,2);
	Links_In = new RContainer<GLinks,unsigned int,true,true> (5,2);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::InitAlgo(void)
{
	GDocCursor cur = Session->GetDocsCursor();
	GLinkCursor lcur;
	GLinks* links_out=0;
	GLinks* links_in=0;


	for (cur.Start();!cur.End();cur.Next())
	{
		//create a container for outgoing links for Curent document
		Links_Out->InsertPtr( new GLinks( cur() ));
		Links_In->InsertPtr( new GLinks( cur() ));
	}

	for (cur.Start();!cur.End();cur.Next())
	{
		if (!cur()->GetNbLinks()) continue;
		links_out=Links_Out->GetPtr(cur()->GetId());
		lcur = cur()->GetLinkCursor();
		
		for (lcur.Start();!lcur.End();lcur.Next())
		{
			links_out->InsertPtr(lcur());

			links_in = Links_In->GetPtr(lcur()->GetDoc()->GetId());
			links_in->InsertPtr(new GLink(cur() ));
		}
	}

	// test les resultats de l'initialisation.
//	GLinksCursor llcur = GetLinksOutCursor();
//
//	for (llcur.Start();!llcur.End();llcur.Next())
//	{
//		lcur = llcur()->GetLinkCursor();
//		cout<< "le document parent :" << llcur()->GetDoc()->GetId()<<endl;
//		for (lcur.Start();!lcur.End();lcur.Next())
//		{
//			cout<< "le doc pointe a l'id : "<< lcur()->GetDoc()->GetId() <<endl;
//		}
//	}
//
//  cout<< "container Links_In -------------------------------------------"<<endl;
//
//	 llcur = GetLinksInCursor();
//
//	for (llcur.Start();!llcur.End();llcur.Next())
//	{
//		lcur = llcur()->GetLinkCursor();
//		cout<< "le document parent :" << llcur()->GetDoc()->GetId()<<endl;
//		for (lcur.Start();!lcur.End();lcur.Next())
//		{
//			cout<< "le doc qui pointe a l'id : "<< lcur()->GetDoc()->GetId() <<endl;
//		}
//	}
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::Operation_I(void)
{
	float sum;
	GLinkCursor cur;

	GLinksCursor lcur=GetLinksOutCursor();
	Norme_Out=0;
	
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		sum=0;
		cur= Links_In->GetPtr( new GLinks(lcur()->GetDoc()))->GetLinkCursor();

		for(cur.Start();!cur.End();cur.Next())
		{
			sum+= Links_In->GetPtr( cur()->GetDoc()->GetId() )->GetWeight();
		}
		lcur()->SetWeight(sum);
		Norme_Out += sum*sum;
	}
	Norme_Out=sqrt(Norme_Out);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::Operation_O(void)
{
	float sum;
	GLinkCursor cur;

	GLinksCursor lcur=GetLinksInCursor();
	Norme_In=0;
	
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		sum=0;
		cur= Links_Out->GetPtr( new GLinks(lcur()->GetDoc()))->GetLinkCursor();

		for(cur.Start();!cur.End();cur.Next())
		{
			sum+= Links_Out->GetPtr( cur()->GetDoc()->GetId() )->GetWeight();
		}
		lcur()->SetWeight(sum);
		Norme_In += sum*sum;
	}
	Norme_In=sqrt(Norme_In);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::Normalization_I()
{
	GLinksCursor lcur=GetLinksOutCursor();
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		lcur()->SetWeight( lcur()->GetWeight()/Norme_Out );
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::Normalization_O()
{
	GLinksCursor lcur=GetLinksInCursor();
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		lcur()->SetWeight( lcur()->GetWeight()/Norme_In );
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::FindBestValues(unsigned int nbVal,GLinksCursor lcur,RContainer<GLinks,unsigned int,true,false>* Bv)
{
	BestValue* res;
	BestValue min;
	GLinks* tmpL;
	unsigned int i,j;

	if (!nbVal) return ;
	res = new BestValue[nbVal];
	
	// init to zero
	i=0;
	while (i<nbVal)
	{
		res[i].Id=0;
		res[i].Value=0.0;
		i++;
	}

	min.Id=0;
	min.Value=0;
	
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		if (lcur()->GetWeight()>min.Value)
		{
			res[min.Id].Id=lcur()->GetDoc()->GetId();
			res[min.Id].Value=lcur()->GetWeight();
			min = findMin(res,nbVal);
		}
	}
	//sort values contained in res.
	struct BestValue tmp;
	for(j=nbVal-1;j>1;j--)
	{
		for(i=0;i<j;i++)
		{
			if (res[i].Value < res[i+1].Value)
			{
				tmp=res[i];
				res[i]=res[i+1];
				res[i+1]=tmp;
			}
		}
	}

	// insert values in the congtainer Bv.
	for(unsigned int j=0;j<nbVal;j++)
	{
		if (res[j].Id==0) continue;
		Bv->InsertPtr(tmpL =new GLinks(Links_Out->GetPtr(res[j].Id )->GetDoc()));
		tmpL->SetWeight(res[j].Value);
	}
}


//-----------------------------------------------------------------------------
struct GLinkCalcItAlgo::BestValue GALILEI::GLinkCalcItAlgo::findMin(struct BestValue* in, unsigned int nbVal)
{
	unsigned int i=0;
	struct BestValue res;

	res.Id=0;
	res.Value=1000;

	while (i<nbVal)
	{
		if (in[i].Value <res.Value)
		{
			res.Id=i;
			res.Value=in[i].Value;
		}
		i++;
	}
	return res;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::Compute(GSlot* rec)
{
	unsigned int k=0;

	InitAlgo();
  
	rec->receiveNextMethod(k);
	while (k<Params->NbIteration)
	{
		Operation_I();
		Operation_O();
		Normalization_I();
		Normalization_O();
		k++;

		rec->receiveNextMethod(k);
	}

	// sort all the results
	RContainer<GLinks,unsigned int,true,false>* Best_Links_Out = new RContainer<GLinks,unsigned int,true,false>(5,2);
	GLinksCursor lcur= GetLinksOutCursor();
	FindBestValues(Params->NbResults,lcur,Best_Links_Out);

	RContainer<GLinks,unsigned int,true,false>* Best_Links_In = new RContainer<GLinks,unsigned int,true,false>(5,2);
	lcur= GetLinksInCursor();
	FindBestValues(Params->NbResults,lcur,Best_Links_In);

	
	// test all results and print it.
	cout<<"trie resultats"<<endl;
	cout << "les meilleur resultats OUT :"<<endl;
	RCursor<GLinks,unsigned int> Cur(Best_Links_Out);
	for (Cur.Start();!Cur.End();Cur.Next())
	{
		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
	}
	cout<<endl<< "----------------------------------------------"<<endl<<endl;
	cout << "les meilleur resultats :"<<endl;
	Cur.Set(Best_Links_In);
	for (Cur.Start();!Cur.End();Cur.Next())
	{
		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
	}


	//---------
	// test les resultats.
	// test les resultats de l'initialisation.
//	GLinksCursor llcur = GetLinksOutCursor();
//	GLinkCursor lcur;
//
//	for (llcur.Start();!llcur.End();llcur.Next())
//	{
//		lcur = llcur()->GetLinkCursor();
//		cout<< "le document parent :" << llcur()->GetDoc()->GetId()<<" et le poid calcule : "<<llcur()->GetWeight()<<endl;
//		for (lcur.Start();!lcur.End();lcur.Next())
//		{
//			cout<< "le doc pointe a l'id : "<< lcur()->GetDoc()->GetId() <<endl;
//		}
//	}
//
//	cout<< "--------------------------------------------------------"<<endl;
//	llcur = GetLinksInCursor();
//
//	for (llcur.Start();!llcur.End();llcur.Next())
//	{
//		lcur = llcur()->GetLinkCursor();
//		cout<< "le document parent :" << llcur()->GetDoc()->GetId()<<" et le poid calcule : "<<llcur()->GetWeight()<<endl;
//		for (lcur.Start();!lcur.End();lcur.Next())
//		{
//			cout<< "le doc qui pointe a l'id : "<< lcur()->GetDoc()->GetId() <<endl;
//		}
//	}
}


//-----------------------------------------------------------------------------
GLinksCursor& GALILEI::GLinkCalcItAlgo::GetLinksOutCursor(void)
{
	GLinksCursor *cur=GLinksCursor::GetTmpCursor();
	cur->Set(Links_Out);
	return(*cur);
}


//-----------------------------------------------------------------------------
GLinksCursor& GALILEI::GLinkCalcItAlgo::GetLinksInCursor(void)
{
	GLinksCursor *cur=GLinksCursor::GetTmpCursor();
	cur->Set(Links_In);
	return(*cur);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcItAlgo::GetSettings(void)
{
	static char tmp[300];

	sprintf(tmp,"%u %u",Params->NbIteration,Params->NbResults);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcItAlgo::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u",&Params->NbIteration,&Params->NbResults);
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcItAlgo::~GLinkCalcItAlgo(void)
{
	GLinksCursor lcur=GetLinksOutCursor();
	for (lcur.Start();!lcur.End();lcur.Next())
	{
		delete(lcur());
	}
	delete(Links_Out);

	lcur=GetLinksInCursor();	
	for (lcur.Start();!lcur.End();lcur.Next())
	{
		delete(lcur());
	}
	delete(Links_In);
}


