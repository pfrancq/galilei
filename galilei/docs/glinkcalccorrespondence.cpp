/*
	GALILEI Research Project

	GLinkCalcCorrespondence.cpp

	Link Calculation for Correspondence Algorithm  - Implementation.

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

#include <langs/glang.h>
#include <docs/glinkcalccorrespondence.h>
#include <sessions/gsession.h>
#include <docs/glinks.h>
#include <docs/glink.h>
#include <sessions/gslot.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>


using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GLinkCalcCorrespondenceParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcCorrespondenceParams::GLinkCalcCorrespondenceParams(void)
	: GLinkCalcParams("Correspondence Algorithm")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcCorrespondenceParams::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(LimitLink) c='1'; else c='0';
	sprintf(tmp,"%u %u %u %c %u",NbIteration,NbResultsHub,NbResultsAuto,c,NbLinks);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondenceParams::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %u %c %u",&NbIteration,&NbResultsHub,&NbResultsAuto,&c,&NbLinks);
	if(c=='1') LimitLink=true; else LimitLink=false;

}



//-----------------------------------------------------------------------------
//
// GLinkCalcCorrespondence
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcCorrespondence::GLinkCalcCorrespondence(GSession* session,GLinkCalcCorrespondenceParams* p) throw(bad_alloc)
	: GLinkCalc("Correspondence Algorithm",session), Params(p)
{
	Links_Out = new RContainer<GLinks,unsigned int,true,true> (10,5);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::InitAlgo(void)
{
	GDocCursor cur = Session->GetDocsCursor();
	GLinkCursor lcur;
	GLinks* links_out=0;

	// Create a container of  outgoing links for all document present in the system
	for (cur.Start();!cur.End();cur.Next())
	{
		Links_Out->InsertPtr( new GLinks( cur() ));
	}

		// For each page p set all the pages qi pointed by p
	for (cur.Start();!cur.End();cur.Next())
	{
		if (!cur()->GetNbLinks()) continue;
		links_out=Links_Out->GetPtr(cur()->GetId());
		lcur = cur()->GetLinkCursor();

		for (lcur.Start();!lcur.End();lcur.Next())
		{
			//  out_going links
			links_out->InsertPtr(lcur());
		}
	}
}

//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::AddDoc(GDoc* /* doc*/)
{}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Operation_I(RStd::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links,RStd::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links)
{
	unsigned int choice,maxLink,i;
	float sum;
	unsigned int nbPages;
	GLinkCursor cur;
	GBalancedLinksCursor lcur;

	// Init Part
	lcur.Set(out_Links);
	Norme_Out=0;

	if(Params->LimitLink)
	{
		maxLink=Params->NbLinks;
		if(Params->UseMultipleLink) choice=1;
		else choice=2;
	}
	else
	{
		if(Params->UseMultipleLink) choice=3;
		else choice=4;
	}

	switch (choice)
	{
		// Limit Link && MultipleLink
		case 1:
			for(lcur.Start(),i=maxLink;!lcur.End();lcur.Next(),i--)
			{
				sum=0;
				nbPages=0;
				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();

				for(cur.Start();!cur.End();cur.Next())
				{
					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight()* cur()->GetOccurs();
					nbPages+=cur()->GetOccurs() ;
				}
				if (nbPages)
				{
					sum =sum / nbPages;
					lcur()->SetWeight(sum);
					Norme_Out += sum*sum;
				}
				else
				{
					lcur()->SetWeight(0);
				}
			}
			break;

		// Limit Link && not MultipleLink
		case 2:
			for(lcur.Start(),i=maxLink;!lcur.End();lcur.Next(),i--)
			{
				sum=0;
				nbPages=0;
				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();

				for(cur.Start();!cur.End();cur.Next())
				{
					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight();
					nbPages++;
				}
				if (nbPages)
				{
					sum =sum /nbPages;
					lcur()->SetWeight(sum);
					Norme_Out += sum*sum;
				}
				else
				{
					lcur()->SetWeight(0);
				}
			}
			break;

		// not Limit Link && MultipleLink
		case 3:
			for(lcur.Start();!lcur.End();lcur.Next())
			{
				sum=0;
				nbPages=0;
				cur= in_Links->GetPtr( lcur()->GetId() )->GetLinkCursor();

				for(cur.Start();!cur.End();cur.Next())
				{
					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight()* cur()->GetOccurs();
					nbPages+=cur()->GetOccurs() ;
				}
				if (nbPages)
				{
					sum =sum / nbPages;
					lcur()->SetWeight(sum);
					Norme_Out += sum*sum;
				}
				else
				 {
					 lcur()->SetWeight(0);
				 }
			}
			break;

		// not Limit Link && not MultipleLink
		case 4:
			for(lcur.Start();!lcur.End();lcur.Next())
			{
				sum=0;
				nbPages=0;
				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();

				for(cur.Start();!cur.End();cur.Next())
				{
					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight();
					nbPages++;
				}
				if (nbPages)
				{
					sum =sum / nbPages;
					lcur()->SetWeight(sum);
					Norme_Out += sum*sum;
				}
			}
			break;
	}

	Norme_Out=sqrt(Norme_Out);

		cout <<"la norme  OUT"<<Norme_Out<< endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Operation_O(RStd::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links,RStd::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links)
{
	float sum;
	unsigned int nbPages;
	GLinkCursor cur;
	GBalancedLinksCursor lcur;

	// Init Part
	lcur.Set(in_Links);
	Norme_In=0;

	if (Params->UseMultipleLink)
	{
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			nbPages=0;
			cur= out_Links->GetPtr( lcur()->GetId())->GetLinkCursor();

			for(cur.Start();!cur.End();cur.Next())
			{
				sum+= out_Links->GetPtr( cur()->GetId() )->GetWeight() * cur()->GetOccurs();
				nbPages+= cur()->GetOccurs();
			}
			if (nbPages)
			{
				sum =sum / nbPages;
				lcur()->SetWeight(sum);
				Norme_In += sum*sum;
			}
			else
			{
				lcur()->SetWeight(0);
			}
			
		}
	}
	else
	{
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			nbPages=0;
			cur= out_Links->GetPtr( lcur()->GetId() )->GetLinkCursor();

			for(cur.Start();!cur.End();cur.Next())
			{
				sum+= out_Links->GetPtr( cur()->GetId() )->GetWeight() ;
				nbPages++;
			}
			if (nbPages)
			{
				sum =sum /nbPages;
				lcur()->SetWeight(sum);
				Norme_In += sum*sum;
			}
		}
	}
	
	Norme_In=sqrt(Norme_In);
	cout <<"la norme IN "<<Norme_In<< endl;
}



//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Normalization(RStd::RContainer<GBalancedLinks,unsigned int,true,true>* Links, float Norme)
{
	GBalancedLinksCursor lcur;
	float test=0;

	lcur.Set(Links);
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		lcur()->SetWeight( lcur()->GetWeight() / Norme );
		test += lcur()->GetWeight() *lcur()->GetWeight();
	}

	cout <<" apres normalisation : "<< test << endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::FindBestValues(unsigned int nbVal,GBalancedLinksCursor lcur,RContainer<GBalancedLinks,unsigned int,true,false>* Bv)
{
	BestValue* res;
	BestValue min;
	GBalancedLinks* tmpL;
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
		Bv->InsertPtr(tmpL =new GBalancedLinks(Links_Out->GetPtr(res[j].Id )->GetDoc()));
		tmpL->SetWeight(res[j].Value);
	}
}


//-----------------------------------------------------------------------------
struct GLinkCalcCorrespondence::BestValue GALILEI::GLinkCalcCorrespondence::findMin(struct BestValue* in, unsigned int nbVal)
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
void GALILEI::GLinkCalcCorrespondence::Compute(GProfile* Prof)
{

	RContainer<GDoc,unsigned int, false,true>* BaseSet;
	RStd::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links;
	RStd::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links  ;
	RContainer<GBalancedLinks,unsigned int,true,false>* Best_Links_Out;
	RContainer<GBalancedLinks,unsigned int,true,false>* Best_Links_In;

	GLink* tmpLink;
	RTimeDate::RDate date;
	tDocJudgement Fdbk;
	GProfDocCursor profCur;
	GDocCursor docCur ;
	GLinkCursor linkCur;
	GLinksCursor linksCur;
	GBalancedLinksCursor balLinksCur;
	unsigned int currentDocId , k;

	//test                                      sscanf(s,"%u %u %u %c %u",&Params->NbIteration,&Params->NbResultsHub,&Params->NbResultsAuto,&c,&Params->NbLinks);
	cout<< "Params"<<endl;
	cout  << "NbIt "<<Params->NbIteration<<endl;
	cout<< "Nb Hubs : "<< Params->NbResultsHub<<endl;
	cout << "Nb Auto ; "<< Params->NbResultsAuto<<endl;
	cout << "Use Multiple Links "<<Params->UseMultipleLink<<endl;
	cout <<" Limitation Liens "<<Params->LimitLink<<endl;
	cout<< "Nb Links : "<<Params->NbLinks<<endl;


	// Init Part
	BaseSet = new RContainer<GDoc,unsigned int, false,true>(100,50);
    profCur = Prof->GetProfDocCursor();
    currentDocId=k=0;

	//---------------------------------------------------------------
	// Init Root Set( all the docs juged OK by the profile
	// Store these docs in the BaseSet.
	for (profCur.Start(); !profCur.End(); profCur.Next() )
	{
		Fdbk = profCur()->GetFdbk();
		// if current doc was juged OK by the profile Insert it
		if (Fdbk & djOK)
		{
			currentDocId = profCur()->GetDoc()->GetId();
			BaseSet->InsertPtr( profCur()->GetDoc() );

			// Expansion of Root Set :
			// Add all Docs pointed by the current doc
			if ( Links_Out->GetPtr( currentDocId ) );
			{
				linkCur= Links_Out->GetPtr( currentDocId )->GetLinkCursor();
				for (linkCur.Start(); ! linkCur.End(); linkCur.Next() )
				{
					// If the pointed doc is not in the BaseSet yet ->insert it
					if (! BaseSet->IsIn( linkCur()->GetDoc()->GetId() ) )
					{
						BaseSet->InsertPtr( linkCur()->GetDoc() );
					}
				}
			}

			// Add all docs which point to the current doc-> If q point to current doc p then insert doc q
			linksCur.Set(Links_Out);
			for (linksCur.Start(); ! linksCur.End(); linksCur.Next() )
			{
				if ( linksCur()->IsIn( currentDocId ) )
				{
					if (! BaseSet->IsIn(linksCur()->GetDoc()->GetId() ) )
					{
						BaseSet->InsertPtr( linksCur()->GetDoc());
					}
				}
			}
		}
	}

	//---------------------------------------------------------------
	// Creation of Vector Needed for links computation
	 // Init Part
	out_Links=new RStd::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);
	in_Links = new RStd::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);

	docCur.Set(BaseSet);
	for (docCur.Start(); ! docCur.End() ; docCur.Next())
	{
		out_Links->InsertPtr(new GBalancedLinks( docCur() ) );
		in_Links->InsertPtr(new GBalancedLinks(docCur() ) );
	}

	for (docCur.Start(); ! docCur.End() ; docCur.Next())
	{
		currentDocId= docCur()->GetId();
		if (Links_Out->GetPtr( currentDocId ) )
		{
			linkCur = Links_Out->GetPtr( currentDocId )->GetLinkCursor();
			for (linkCur.Start(); ! linkCur.End() ; linkCur.Next() )
			{
				if ( BaseSet->IsIn(linkCur()->GetId() ) )
				{
					out_Links->GetPtr( currentDocId )->InsertPtr( linkCur() );
					in_Links->GetPtr( linkCur()->GetDoc()->GetId() )->InsertPtr( tmpLink =new GLink(docCur() ) )  ;
					tmpLink->SetOccurs( linkCur()->GetOccurs());
				}
			}
		}
	}

	//---------------------------------------------------------------
	// Compute Hubs and Autorities.
	while (k< Params->NbIteration)
	{
		Operation_I(out_Links, in_Links);
		Operation_O(out_Links,in_Links);
		Normalization(out_Links,Norme_Out);
		Normalization(in_Links,Norme_In);
		k++;
	}

	//---------------------------------------------------------------
	// Sort Results and Find Best Values for the Hubs and Authorities.
	// Init Part
	Best_Links_Out = new RContainer<GBalancedLinks,unsigned int,true,false>(Params->NbResultsHub,5);
	Best_Links_In = new RContainer<GBalancedLinks,unsigned int,true,false>(Params->NbResultsAuto,5);

	// find best results for out_Links
	balLinksCur.Set(out_Links);
	FindBestValues(Params->NbResultsHub,balLinksCur,Best_Links_Out);

	// Find best results for in_Links
	balLinksCur.Set(in_Links);
	FindBestValues(Params->NbResultsAuto,balLinksCur,Best_Links_In);

	// Add Best Results to current Profile.

	// Add documents considered as Best Hubs
	balLinksCur.Set(Best_Links_Out);
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
		{
			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djNav | djHub ), date)) ;
			//cout << "Document Hub add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
		}
	}

	// Add documents considered as Best Autorities
	balLinksCur.Set(Best_Links_In);
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
		{
			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djOK | djAutority ), date )) ;
			//cout << "Document Autorte add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
		}
	}

//
//	//---------------------------------------------------------------
//	// test all results and print it.
//	cout<<endl<< "----------------------------------------------"<<endl;
//	cout<< "----------------------------------------------"<<endl;
//	cout<<"trie resultats"<<endl;
//	cout << "les meilleur resultats OUT :"<<endl;
//	RCursor<GLinks,unsigned int> Cur(Best_Links_Out);
//	for (Cur.Start();!Cur.End();Cur.Next())
//	{
//		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
//	}
//	cout<<endl<< "----------------------------------------------"<<endl<<endl;
//	cout << "les meilleur resultats :"<<endl;
//	Cur.Set(Best_Links_In);
//	for (Cur.Start();!Cur.End();Cur.Next())
//	{
//		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
//	}



	// delete all the container no more used.
	delete(out_Links);
	delete(in_Links);
	delete(BaseSet);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcCorrespondence::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(Params->LimitLink) c='1'; else c='0';
	sprintf(tmp,"%u %u %u %c %u",Params->NbIteration,Params->NbResultsHub,Params->NbResultsAuto,c,Params->NbLinks);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u %u",&Params->NbIteration,&Params->NbResultsHub,&Params->NbResultsAuto);

	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %u %c %u",&Params->NbIteration,&Params->NbResultsHub,&Params->NbResultsAuto,&c,&Params->NbLinks);
	if(c=='1') Params->LimitLink=true; else Params->LimitLink=false;
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcCorrespondence::~GLinkCalcCorrespondence(void)
{
	delete(Links_Out);
}


