/*
	GALILEI Research Project

	GLinkCalcTresh.cpp

	Link Calculation for Treshold Kleinberg Algorithm ( based on Kleinberg works ) - Implementation.

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
#include <docs/glinkcalctresh.h>
#include <sessions/gsession.h>
#include <docs/glinks.h>
#include <docs/glink.h>
#include <sessions/gslot.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>


using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GLinkCalcTreshParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcTreshParams::GLinkCalcTreshParams(void)
	: GLinkCalcParams("Treshold Kleinberg Algorithm"),NbIteration(5),
	  NbResultsHub(5), NbResultsAuto(5), Kvalue(60), HTresh(true), ATresh(false),
	  FTresh(false), AKTresh(false), FKTresh(false)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcTreshParams::GetSettings(void)
{
	static char tmp[300];
	char c1,c2,c3,c4,c5;

	if(HTresh) c1='1'; else c1='0';
	if(ATresh) c2='1'; else c2='0';
	if(FTresh) c3='1'; else c3='0';
	if(AKTresh) c4='1'; else c4='0';
	if(FKTresh) c5='1'; else c5='0';
	sprintf(tmp,"%u %u %u %c %c %c %c %c %u",NbIteration,NbResultsHub,NbResultsAuto,c1,c2,c3,c4,c5,Kvalue );
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTreshParams::SetSettings(const char* s)
{
	char c1,c2,c3,c4,c5;

	if(!(*s)) return;
	sscanf(s,"%u %u %u %c %c %c %c %c %u",&NbIteration,&NbResultsHub,&NbResultsAuto,&c1,&c2,&c3,&c4,&c5,&Kvalue);
	if(c1=='1') HTresh=true; else HTresh=false;
	if(c2=='1') ATresh=true; else ATresh=false;
	if(c3=='1') FTresh=true; else FTresh=false;
	if(c4=='1') AKTresh=true; else AKTresh=false;
	if(c5=='1') FKTresh=true; else FKTresh=false;
}



//-----------------------------------------------------------------------------
//
// GLinkCalcTresh
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcTresh::GLinkCalcTresh(GSession* session,GLinkCalcTreshParams* p) throw(bad_alloc)
	: GLinkCalc("Treshold Kleinberg Algorithm",session), Params(p)
{
	// Variables de test
	nbEtapes=0;
	MeanAddAuto=MeanAddHub=0;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTresh::Operation_I(RContainer<GBalancedLinks,unsigned int,true,true>* Xa,R::RContainer<GBalancedLinks,unsigned int,true,true>* Xh)
{
	float sum,mean,tmp;
	GLinkCursor cur;
	GBalancedLinksCursor lcur;


	// Init Part
	lcur.Set(Xa);
	Norme_Out=0;

	// compute for Hub Treshold
	// compute mean, if element greater than mean then use it during computation
	if( (Params->HTresh == true) || (Params->FTresh == true) || (Params->FKTresh == true) )
	{
		//cout<< "compute for HTesh"<<endl;
		for(lcur.Start();!lcur.End();lcur.Next())
		{

			mean=tmp=0;
			cur= Xh->GetPtr( lcur()->GetId())->GetLinkCursor();

			// compute mean
			for(cur.Start();!cur.End();cur.Next())
			{
				mean+= Xh->GetPtr( cur()->GetId() )->GetWeight();
			}
			mean = mean/cur.GetNb();
			mean = mean - mean/100;

			sum=0;
			for(cur.Start();!cur.End();cur.Next())
			{
				tmp= Xh->GetPtr( cur()->GetId() )->GetWeight();
				if (tmp>= mean)
				{
					sum+=tmp;
				}
			}
			lcur()->SetWeight(sum);
			Norme_Out += sum*sum;
		}
	}
	// compute normaly (according to Klienberg Algo)
	else
	{
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			cur= Xh->GetPtr( lcur()->GetId())->GetLinkCursor();

			for(cur.Start();!cur.End();cur.Next())
			{
				sum+= Xh->GetPtr( cur()->GetId() )->GetWeight();

			}
			lcur()->SetWeight(sum);
			Norme_Out += sum*sum;
		}
	}

	// compute square root fo the norm.
	Norme_Out=sqrt(Norme_Out);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTresh::Operation_O(RContainer<GBalancedLinks,unsigned int,true,true>* Xa,RContainer<GBalancedLinks,unsigned int,true,true>* Xh)
{
	float sum, mean,tmp;
	GLinkCursor cur;
	GBalancedLinksCursor lcur,balLinksCur;

	unsigned int K;

	// Init Part
	lcur.Set(Xh);
	Norme_In=0;

	// compute for Authority Treshold.
	//  compute mean, if element greater than mean then use it during computation
	if ((Params->ATresh == true) || (Params->FTresh == true) )
	{
		//cout<<"compute for ATresh"<<endl;
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			mean=tmp=0;
			cur= Xa->GetPtr( lcur()->GetId() )->GetLinkCursor();

			// compute mean
			for(cur.Start();!cur.End();cur.Next())
			{
				mean+= Xa->GetPtr( cur()->GetId() )->GetWeight() ;
			}
			mean = mean/cur.GetNb();
			mean = mean - mean/100;

			//add value if greater than the mean
			for(cur.Start();!cur.End();cur.Next())
			{
				tmp= Xa->GetPtr( cur()->GetId() )->GetWeight() ;
				if (tmp >= mean)
				{
					sum+=tmp;
				}
			}
			lcur()->SetWeight(sum);
			Norme_In += sum*sum;
		}
	}
	// compute for Authority K Treshold.
	//  given a treshold K (in percent), compute algo with only K greater values of the authorities.
	else if ((Params->AKTresh == true) || (Params->FKTresh == true))
	{
		//cout<<"compute for AKTresh"<<endl;
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			cur= Xa->GetPtr( lcur()->GetId() )->GetLinkCursor();

 			// K elements added
			RContainer<GBalancedLinks,unsigned int,false,true>* SortDoc= new RContainer<GBalancedLinks,unsigned int,false,true>(cur.GetNb() +1 ,10);
			for(cur.Start();!cur.End();cur.Next())
			{
				SortDoc->InsertPtr(Xa->GetPtr(cur()->GetId()));
			}

			// calcul de la valeur de K %->nb de docs
			tmp = (float) Params->Kvalue*SortDoc->NbPtr/100 ;
			K = (int)ceil(tmp );

			RContainer<GBalancedLinks,unsigned int,true,false>* best_res = new RContainer<GBalancedLinks,unsigned int,true,false>(K,5);

			//find best results for Xa
			balLinksCur.Set(SortDoc);
			FindBestValues(K,balLinksCur,best_res);

			//add value considered as best values.
			for(balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
			{
				tmp = balLinksCur()->GetWeight();
				sum+=tmp;
			}
			lcur()->SetWeight(sum);
			Norme_In += sum*sum;

			delete SortDoc;
			delete best_res;
		}
	}
	// compute normaly (according to Kleinberg Algo)
	else
	{
		//cout<< "none"<<endl;
		for(lcur.Start();!lcur.End();lcur.Next())
		{
			sum=0;
			cur= Xa->GetPtr( lcur()->GetId() )->GetLinkCursor();

			for(cur.Start();!cur.End();cur.Next())
			{
				sum+= Xa->GetPtr( cur()->GetId() )->GetWeight() ;
			}
			lcur()->SetWeight(sum);
			Norme_In += sum*sum;
		}
	}
	// compute square root of the norm.
	Norme_In=sqrt(Norme_In);
}



//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTresh::Normalization(R::RContainer<GBalancedLinks,unsigned int,true,true>* Links, float Norme)
{
	GBalancedLinksCursor lcur;
	//float test=0;

	lcur.Set(Links);
	for(lcur.Start();!lcur.End();lcur.Next())
	{
		lcur()->SetWeight( lcur()->GetWeight() / Norme );
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTresh::FindBestValues(unsigned int nbVal,GBalancedLinksCursor lcur,RContainer<GBalancedLinks,unsigned int,true,false>* Bv)
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
	delete[] res;
}


//-----------------------------------------------------------------------------
struct GLinkCalcTresh::BestValue GALILEI::GLinkCalcTresh::findMin(struct BestValue* in, unsigned int nbVal)
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
void GALILEI::GLinkCalcTresh::Compute(GSubProfile* Subprof)
{

	RContainer<GDoc,unsigned int, false,true>* BaseSet;
	R::RContainer<GBalancedLinks,unsigned int,true,true>* Xa;
	R::RContainer<GBalancedLinks,unsigned int,true,true>* Xh  ;
	RContainer<GBalancedLinks,unsigned int,true,false>* best_Xh;
	RContainer<GBalancedLinks,unsigned int,true,false>* best_Xa;



	GLink* tmpLink;
	GDoc* tmpDoc;
	R::RDate date;
	tDocJudgement Fdbk;
	GProfDocCursor subprofCur;
	GDocCursor docCur ;
	GLinkCursor linkCur;
	GLinksCursor linksCur;
	GBalancedLinksCursor balLinksCur;

	unsigned int currentDocId , k;
	//test var
	unsigned int nbAdd=0;

	//test
//	cout<< "Params"<<endl;
//	cout  << "NbIt "<<Params->NbIteration<<endl;
//	cout<< "Nb Hubs : "<< Params->NbResultsHub<<endl;
//	cout << "Nb Auto ; "<< Params->NbResultsAuto<<endl;
// 	cout<< "HTresh "<<Params->HTresh<<endl;
// 	cout<< "ATresh "<<Params->ATresh<<endl;
// 	cout<< "FTresh "<<Params->FTresh<<endl;
// 	cout<< "AKTresh "<<Params->AKTresh<<endl;
// 	cout<< "FKTresh "<<Params->FKTresh<<endl;



	// -1- Init Part
	BaseSet = new RContainer<GDoc,unsigned int, false,true>(100,50);
	subprofCur = Subprof->GetProfDocCursor();
	currentDocId=k=0;

	//---------------------------------------------------------------
	// Init Root Set( all the docs juged OK by the SubProfile
	// Store these docs in the BaseSet.
	for (subprofCur.Start(); !subprofCur.End(); subprofCur.Next() )
	{
		Fdbk = subprofCur()->GetFdbk();
		// if current doc was juged OK by the SubProfile Insert it
		if (Fdbk & djOK)
		{
			currentDocId = subprofCur()->GetDoc()->GetId();
			BaseSet->InsertPtr( subprofCur()->GetDoc() );

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
	// -2- Creation of Vector Needed for links computation
	 // Init Part
	Xa=new R::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);
	Xh = new R::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);

	docCur.Set(BaseSet);
	for (docCur.Start(); ! docCur.End() ; docCur.Next())
	{
		Xa->InsertPtr(new GBalancedLinks( docCur() ) );
		Xh->InsertPtr(new GBalancedLinks(docCur() ) );
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
					Xa->GetPtr( currentDocId )->InsertPtr( linkCur() );
					Xh->GetPtr( linkCur()->GetDoc()->GetId() )->InsertPtr( tmpLink =new GLink(docCur() ) )  ;
					tmpLink->SetOccurs( linkCur()->GetOccurs());
				}
			}
		}
	}


// 	Xa=new R::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);
// 	Xh = new R::RContainer<GBalancedLinks,unsigned int,true,true>(50,10);
// 
// 	 for (int i=1;i<6;i++)
// 	{
// 		Xa->InsertPtr(new GBalancedLinks( Session->GetDoc(i) ) );
// 		Xh->InsertPtr(new GBalancedLinks(Session->GetDoc(i) ) );
// 	}
// 	Xa->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(2)));
// 	Xa->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(5)));
// 	Xa->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(3)));
// 	Xa->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
// 	Xa->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(1)));
// 	Xa->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(5)));
// 	Xa->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(1)));
// 	Xa->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
// 	Xa->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(3)));
// 	Xa->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(2)));
// 
// 	Xh->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(3)));
// 	Xh->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(4)));
// 	Xh->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(1)));
// 	Xh->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
// 	Xh->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(5)));
// 	Xh->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(2)));
// 	Xh->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(4)));
// 	Xh->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
// 	Xh->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(1)));
// 	Xh->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(3)));



	//---------------------------------------------------------------
	// -3- Compute Hubs and Autorities.

	// compute operation I first for ATresh
	if (Params->ATresh == true)
	{
		Operation_I(Xa, Xh);
	}
	k=0;

	while (k< Params->NbIteration)
	{


		Operation_O(Xa,Xh);

		Operation_I(Xa, Xh);

		Normalization(Xa,Norme_Out);

		Normalization(Xh,Norme_In);

		k++;
	}


	cout <<"resultats Xh"<<endl;
	balLinksCur.Set(Xh);
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		linkCur = balLinksCur()->GetLinkCursor();
		cout<< "le document parent :" << balLinksCur()->GetId()<<" et le poid calcule : "<<balLinksCur()->GetWeight()<<endl;
	}
	cout<<endl<<"--------------------------------------------"<<endl;

	cout <<"resultats Xa"<<endl;
	balLinksCur.Set(Xa);
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		linkCur = balLinksCur()->GetLinkCursor();
		cout<< "le document parent :" << balLinksCur()->GetId()<<" et le poid calcule : "<<balLinksCur()->GetWeight()<<endl;
	}
	cout<<endl<<"--------------------------------------------"<<endl;


	//---------------------------------------------------------------
	// -4- Sort Results and Find Best Values for the Hubs and Authorities.
	// Init Part
	best_Xh = new RContainer<GBalancedLinks,unsigned int,true,false>(Params->NbResultsHub,5);
	best_Xa = new RContainer<GBalancedLinks,unsigned int,true,false>(Params->NbResultsAuto,5);

	// find best results for Xa
	balLinksCur.Set(Xa);
	FindBestValues(Params->NbResultsHub,balLinksCur,best_Xh);

	// Find best results for Xh
	balLinksCur.Set(Xh);
	FindBestValues(Params->NbResultsAuto,balLinksCur,best_Xa);

	// Add Best Results to current SubProfile.
	//----------------------------------------------
	// Add documents considered as Best Hubs
	balLinksCur.Set(best_Xh);

	nbAdd=0; //test
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		// If the SubProfile hasn't judged the doc -> Insert a judgement for the doc.
		tmpDoc=balLinksCur()->GetDoc();
		if (! Subprof->GetFeedback( tmpDoc))
		{
			Session->InsertFdbk(Subprof->GetProfile(), balLinksCur()->GetDoc(),tDocJudgement( djNav | djHub ), date);
		}
	}
// 	nbEtapes++;    //test
// 	MeanAddHub = (( MeanAddHub*(nbEtapes-1) + (double)nbAdd/Params->NbResultsHub)/ nbEtapes);  //moyenne incrementale    //test
// 	cout <<" pourcent d'ajout Hubs :" << (float)nbAdd/Params->NbResultsHub <<" et nouvelle moyenne : "<<MeanAddHub<<endl;
// 	nbAdd=0;

	// Add documents considered as Best Autorities
	balLinksCur.Set(best_Xa);
	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
	{
		// If the SubProfile hasn't judged the doc -> Insert a judgement for the doc.
		tmpDoc=balLinksCur()->GetDoc();
		if (! Subprof->GetFeedback( tmpDoc) )
		{
			Session->InsertFdbk(Subprof->GetProfile(),balLinksCur()->GetDoc(),tDocJudgement( djOK | djAutority ), date);
		}
	}
// 	MeanAddAuto = (( MeanAddAuto*(nbEtapes-1) + (double)nbAdd/Params->NbResultsAuto)/ nbEtapes);  //moyenne incrementale    //test
// 	out <<" pourcent d'ajout Auto :" << (float)nbAdd/Params->NbResultsAuto <<" et nouvelle moyenne : "<<MeanAddAuto<<endl;

	//---------------------------------------------
	// -5- delete all the container no more in use.
	delete(BaseSet);
	delete(Xa);
	delete(Xh);
	delete(best_Xh);
	delete(best_Xa);

}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcTresh::GetSettings(void)
{
	static char tmp[400];
	char c1,c2,c3,c4,c5;

	if(Params->HTresh) c1='1'; else c1='0';
	if(Params->ATresh) c2='1'; else c2='0';
	if(Params->FTresh) c3='1'; else c3='0';
	if(Params->AKTresh) c4='1'; else c4='0';
	if(Params->FKTresh) c5='1'; else c5='0';
	sprintf(tmp,"%u %u %u %c %c %c %c %c %u",Params->NbIteration,Params->NbResultsHub,Params->NbResultsAuto,c1,c2,c3,c4,c5, Params->Kvalue);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcTresh::SetSettings(const char* s)
{
	char c1,c2,c3,c4,c5;

	if(!(*s)) return;
	sscanf(s,"%u %u %u %c %c %c %c %c %u",&Params->NbIteration,&Params->NbResultsHub,&Params->NbResultsAuto,&c1,&c2,&c3,&c4,&c5,&Params->Kvalue);
	if(c1=='1') Params->HTresh=true; else Params->HTresh=false;
	if(c2=='1') Params->ATresh=true; else Params->ATresh=false;
	if(c3=='1') Params->FTresh=true; else Params->FTresh=false;
	if(c4=='1') Params->AKTresh=true; else Params->AKTresh=false;
	if(c5=='1') Params->FKTresh=true; else Params->FKTresh=false;

}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcTresh::~GLinkCalcTresh(void)
{
}


