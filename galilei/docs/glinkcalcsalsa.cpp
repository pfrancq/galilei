/*
	GALILEI Research Project

	GLinkCalcSALSA.cpp

	Link Calculation for SALSA Algorithm - Implementation.

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
#include <docs/glinkcalcsalsa.h>
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
//  GLinkCalcSALSAParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcSALSAParams::GLinkCalcSALSAParams(void)
	: GLinkCalcParams("SALSA Algorithm")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcSALSAParams::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(LimitLink) c='1'; else c='0';
	sprintf(tmp,"%u %u %c %u",NbResultsHub,NbResultsAuto,c,NbLinks);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSAParams::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %c %u",&NbResultsHub,&NbResultsAuto,&c,&NbLinks);
	if(c=='1') LimitLink=true; else LimitLink=false;

}


//-----------------------------------------------------------------------------
//
// GLinkCalcSALSA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLinkCalcSALSA::GLinkCalcSALSA(GSession* session,GLinkCalcSALSAParams* p) throw(bad_alloc)
	: GLinkCalc("SALSA Algorithm",session), Params(p)
{
	// Variables de test
	nbEtapes=0;
	MeanAddAuto=MeanAddHub=0;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSA::Operation_H(RGenericSparseMatrix<RGenericCell<double>,true>* Qhubs,RGenericSparseVector<RGenericCell<double>,true>* Xhubs)
{
	double sum;

	 //Init Part
	RGenericCellCursorDouble cellCur;
	RGenericVectorCursorDouble vectCur;

	//TEST
//	cout << "before compute"<<endl;
//	cellCur.Set(Xhubs);
//	for (cellCur.Start();!cellCur.End(); cellCur.Next())
//	{
//		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
//	}
//	cout<< endl;


	Norme_H=0;
	vectCur.Set(Qhubs) ;
	// Compute Part
	for (vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cellCur.Set(vectCur());
		sum=0;
		for (cellCur.Start();! cellCur.End();cellCur.Next())
		{
			sum+= cellCur()->GetValue() ;
		}

		Xhubs->GetInsertPtr(vectCur()->GetId())->SetValue(sum);
		Norme_H += sum*sum;
	}

	Norme_H = sqrt(Norme_H);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSA::Operation_A(RGenericSparseMatrix<RGenericCell<double>,true>* Qautos,RGenericSparseVector<RGenericCell<double>,true>* Xauto)
{
	double sum;

	//Init Part
	RGenericCellCursorDouble cellCur;
	RGenericVectorCursorDouble vectCur;

	Norme_A=0;
	vectCur.Set(Qautos) ;
	// Compute Part
	for (vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cellCur.Set(vectCur());
		sum=0;
		for (cellCur.Start();! cellCur.End();cellCur.Next())
		{
			sum+= cellCur()->GetValue() ;// * Xauto->GetPtr(cellCur()->GetId())->GetValue();
		}
		Xauto->GetInsertPtr(vectCur()->GetId())->SetValue(sum);
		Norme_A += sum*sum;
	}

	Norme_A = sqrt(Norme_A);

}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSA::Normalization(RGenericSparseVector<RGenericCell<double>,true>* X, double Norme)
{
	RGenericCellCursorDouble cellCur;
	float test=0;

	//normalization
	cellCur.Set(X);
	for(cellCur.Start();!cellCur.End();cellCur.Next())
	{
		cellCur()->SetValue( cellCur()->GetValue() / Norme );
		test += cellCur()->GetValue() *cellCur()->GetValue();
	}

}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSA::FindBestValues(unsigned int nbVal,RGenericSparseVector<RGenericCell<double>,true>*  X,RGenericSparseVector<RGenericCell<int>,true>* Bv)
{
	BestValue* res;
	BestValue min;
	RGenericCellCursorDouble cellCur;
	unsigned int i;

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

	cellCur.Set(X);
	for(cellCur.Start();!cellCur.End();cellCur.Next())
	{
		if (cellCur()->GetValue()>min.Value)
		{
			res[min.Id].Id=cellCur()->GetId();
			res[min.Id].Value=cellCur()->GetValue();
			min = findMin(res,nbVal);
		}
	}

	// insert values in the congtainer Bv.
	for(unsigned int j=0;j<nbVal;j++)
	{
		if (res[j].Id==0) continue;
	Bv->GetInsertPtr( j)->SetValue(res[j].Id);
	}

	delete res;
}

//-----------------------------------------------------------------------------
struct GLinkCalcSALSA::BestValue GALILEI::GLinkCalcSALSA::findMin(struct BestValue* in, unsigned int nbVal)
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
void GALILEI::GLinkCalcSALSA::Compute(GSubProfile* Subprof)
{

	RContainer<GDoc,unsigned int, false,true>* BaseSet;
	R::RContainer<GLinks,unsigned int,true,true>* out_Links;
	R::RContainer<GLinks,unsigned int,true,true>* in_Links  ;
	RGenericSparseVector<RGenericCell<int>,true>* best_Links_H;
	RGenericSparseVector<RGenericCell<int>,true>* best_Links_A;

	GLink* tmpLink;
	GDoc* tmpDoc;
	R::RDate date;
	tDocJudgement Fdbk;
	GProfDocCursor subprofCur;
	GDocCursor docCur ;
	GLinkCursor linkCur;
	GLinksCursor linksCur;
	GLinksCursor linksCur2;

	unsigned int currentDocId , k;
	//test var
	unsigned int nbAdd=0;

	//test
//	cout<< "Params"<<endl;
//	cout  << "NbIt "<<Params->NbIteration<<endl;
//	cout<< "Nb Hubs : "<< Params->NbResultsHub<<endl;
//	cout << "Nb Auto ; "<< Params->NbResultsAuto<<endl;
//	cout << "Use Multiple Links "<<Params->UseMultipleLink<<endl;
//	cout <<" Limitation Liens "<<Params->LimitLink<<endl;
//	cout<< "Nb Links : "<<Params->NbLinks<<endl;


	// Init Part
	BaseSet = new RContainer<GDoc,unsigned int, false,true>(100,50);
	subprofCur = Subprof->GetProfDocCursor();
	currentDocId=k=0;

	//---------------------------------------------------------------
	// Init Root Set( all the docs juged OK by the profile
	// Store these docs in the BaseSet.
	for (subprofCur.Start(); !subprofCur.End(); subprofCur.Next() )
	{
		Fdbk = subprofCur()->GetFdbk();
		// if current doc was juged OK by the profile Insert it
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
	// Creation of Vector Needed for links computation
	 // Init Part
	out_Links=new R::RContainer<GLinks,unsigned int,true,true>(50,10);
	in_Links = new R::RContainer<GLinks,unsigned int,true,true>(50,10);

	docCur.Set(BaseSet);
	for (docCur.Start(); ! docCur.End() ; docCur.Next())
	{
		out_Links->InsertPtr(new GLinks( docCur() ) );
		in_Links->InsertPtr(new GLinks(docCur() ) );
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

//	 for (int i=1;i<6;i++)
//	{
//		out_Links->InsertPtr(new GLinks( Session->GetDoc(i) ) );
//		in_Links->InsertPtr(new GLinks(Session->GetDoc(i) ) );
//	}
//	out_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(2)));
//	out_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(5)));
//	out_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(3)));
//	out_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
//	out_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(1)));
//	out_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(5)));
//	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(1)));
//	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
//	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(3)));
//	out_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(2)));
//
//	in_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(3)));
//	in_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(4)));
//	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(1)));
//	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
//	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(5)));
//	in_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(2)));
//	in_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(4)));
//	in_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
//	in_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(1)));
//	in_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(3)));


	//---------------------------------------------------------------
	// Compute marginal density and associated matrix
	// Dr marginal density for row
	// Dc marginal density for column
	RGenericSparseVector<RGenericCell<double>,true>* Dr = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	RGenericSparseVector<RGenericCell<double>,true>* Dc = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	RGenericCell<double>* tmpCell;

	linksCur.Set( out_Links );
	for (linksCur.Start(); !linksCur.End(); linksCur.Next())
	{
		Dr->GetInsertPtr( linksCur()->GetId())->SetValue(linksCur()->NbPtr);
		linkCur= linksCur()->GetLinkCursor();
		for (linkCur.Start(); !linkCur.End(); linkCur.Next())
		{
			tmpCell=Dc->GetInsertPtr( linkCur()->GetId());
			tmpCell->SetValue(tmpCell->GetValue() +1);
		}
	}

	//---------------------------------------------------------------
	// creation of the matix for Hub and Autorities : H'= Wr*Wc' and A'=Wc'*Wr
	RGenericSparseMatrix<RGenericCell<double>,true>* H = new RGenericSparseMatrix<RGenericCell<double>,true>(100,50);
	RGenericSparseMatrix<RGenericCell<double>,true>* A = new RGenericSparseMatrix<RGenericCell<double>,true>(100,50);
	double sum=0;

	//------------------------------------------------------------------
	// creation of H(i,j) = W(i,k)/Dc(k) * W(j,k)/Dr(j)
	linksCur.Set(out_Links);
	linksCur2.Set(out_Links);
	// parse  matrix for indice 'i'
	for (linksCur.Start(); !linksCur.End(); linksCur.Next())
	{
		linkCur.Set(linksCur());
		//parse for indice 'j'
		for (linksCur2.Start(); !linksCur2.End(); linksCur2.Next())
		{
			sum=0;
			// parse elements of line 'i' : element 'k'
			for(linkCur.Start(); !linkCur.End(); linkCur.Next())
			{
				// verify if the element is also contained in line 'j'
				if ( linksCur2()->IsIn(linkCur()->GetId()) )
				{
					sum+= ( (1/Dc->GetPtr(linkCur()->GetId())->GetValue() )* (1/Dr->GetPtr(linksCur2()->GetId())->GetValue()) );
				}
			}
			if(sum)
			{
				H->GetInsertPtr(linksCur()->GetId())->GetInsertPtr(linksCur2()->GetId())->SetValue(sum);
			}
		}
	}

	//------------------------------------------------------------------
	// creation of A(i,j) = W(k,i)/Dr(k) * W(k,j)/Dc(j) = U(i,k)/Dr(k) * U(j,k)/Dc(j)  where U=W'
	linksCur.Set(in_Links);
	linksCur2.Set(in_Links);
	// parse  matrix for indice 'i'
	for (linksCur.Start(); !linksCur.End(); linksCur.Next())
	{
		linkCur.Set(linksCur());
		//parse for indice 'j'
		for (linksCur2.Start(); !linksCur2.End(); linksCur2.Next())
		{
			sum=0;
			// parse elements of line 'i' : element 'k'
			for(linkCur.Start(); !linkCur.End(); linkCur.Next())
			{
				// verify if the element is also contained in line 'j'
				if ( linksCur2()->IsIn(linkCur()->GetId()) )
				{
					sum+= ( (1/Dr->GetPtr(linkCur()->GetId())->GetValue() )* (1/Dc->GetPtr(linksCur2()->GetId())->GetValue()) );
				}
			}
			if(sum)
			{
				A->GetInsertPtr(linksCur()->GetId())->GetInsertPtr(linksCur2()->GetId())->SetValue(sum);
			}
		}
	}

	// Creation of the eigen vectors -> vectors containing results
	RGenericSparseVector<RGenericCell<double>,true>* Xh = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	RGenericSparseVector<RGenericCell<double>,true>* Xa = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);

	// Compute Hubs and Autorities.
	Operation_H(H,Xh);
	Operation_A(A,Xa);
	Normalization(Xh,Norme_H);
	Normalization(Xa,Norme_A);


	//---------------------------------------------------------------
	// Sort Results and Find Best Values for the Hubs and Authorities.
	// Init Part
	best_Links_H= new RGenericSparseVector<RGenericCell<int>,true>(1,Params->NbResultsHub,5);
	best_Links_A = new RGenericSparseVector<RGenericCell<int>,true>(1,Params->NbResultsAuto,5);

	// Find best results for out_Links
	FindBestValues(Params->NbResultsHub,Xh,best_Links_H);

	// Find best results for in_Links
	FindBestValues(Params->NbResultsAuto,Xa,best_Links_A);

	// Add Best Results to current Profile.

	// Add documents considered as Best Hubs

	RGenericCellCursorInt cellCur;
	cellCur.Set(best_Links_H);

	nbAdd=0; //test
	for (cellCur.Start();!cellCur.End();cellCur.Next())
	{
		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
		tmpDoc=Session->GetDoc(cellCur()->GetValue());
		if (! Subprof->GetFeedback( tmpDoc) )
		{
			Session->InsertFdbk(Subprof->GetProfile(),tmpDoc,tDocJudgement( djNav | djHub ), date);
		}
	}
	nbEtapes++;    //test
	MeanAddHub = (( MeanAddHub*(nbEtapes-1) + (double)nbAdd/Params->NbResultsHub)/ nbEtapes);  //moyenne incrementale    //test
	cout <<" pourcent d'ajout Hubs :" << (float)nbAdd/Params->NbResultsHub <<" et nouvelle moyenne : "<<MeanAddHub<<endl;
	nbAdd=0;

	// Add documents considered as Best Autorities
	cellCur.Set(best_Links_A);
	for (cellCur.Start();!cellCur.End();cellCur.Next())
	{
		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
		tmpDoc=Session->GetDoc(cellCur()->GetValue());
		if (! Subprof->GetFeedback(tmpDoc))
		{
			Session->InsertFdbk(Subprof->GetProfile(),tmpDoc,tDocJudgement( djOK | djAutority ), date);
		}
	}
	MeanAddAuto = (( MeanAddAuto*(nbEtapes-1) + (double)nbAdd/Params->NbResultsAuto)/ nbEtapes);  //moyenne incrementale    //test
	cout <<" pourcent d'ajout Auto :" << (float)nbAdd/Params->NbResultsAuto <<" et nouvelle moyenne : "<<MeanAddAuto<<endl;

	//test
//	balLinksCur.Set(out_Links);
//
//	nbAdd=0; //test
//	unsigned int nbTotal=balLinksCur.GetNb();
//	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
//	{
//		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
//		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
//		{
//			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djNav | djHub ), date)) ;
//			nbAdd++;
//			//cout << "Document Hub add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
//		}
//	}
//	if(! nbTotal)
//	{
//		cout << "zero doc added on zero docs hubs."<<endl;
//	}
//	else
//	{
//		nbEtapes++;    //test
//		cout << "nbr doc added :" <<nbAdd<<endl;
//		MeanAddHub = (( MeanAddHub*(nbEtapes-1) + (double)nbAdd/nbTotal )/ nbEtapes);  //moyenne incrementale    //test
//		cout <<" pourcent d'ajout Hubs :" << (float)nbAdd/nbTotal <<" et nouvelle moyenne : "<<MeanAddHub<<endl;
//		nbAdd=0;
//	}

	// Add documents considered as Best Autorities
//	balLinksCur.Set(in_Links);
//	unsigned int  nbTotal=balLinksCur.GetNb();
////	nbEtapes++;
////	nbAdd=0;
//	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
//	{
//		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
//		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
//		{
//			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djOK | djAutority ), date )) ;
//			nbAdd++;
//			//cout << "Document Autorte add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
//		}
//	}
//    	if(! nbTotal)
//	{
//		cout << "zero doc added on zero docs auto."<<endl;
//	}
//	else
//	{
//	cout << "nbr doc added :" <<nbAdd<<endl;
//	MeanAddAuto = (( MeanAddAuto*(nbEtapes-1) + (double)nbAdd/nbTotal)/ nbEtapes);  //moyenne incrementale    //test
//	cout <<" pourcent d'ajout Auto :" << (float)nbAdd/nbTotal <<" et nouvelle moyenne : "<<MeanAddAuto<<endl;
//	}


	//---------------------------------------------------------------
	// test all results and print it.
//	cout<<endl<< "----------------------------------------------"<<endl;
//	cout<< "----------------------------------------------"<<endl;
//	cout<<"trie resultats"<<endl;
//	cout << "les meilleur resultats OUT :"<<endl;
//	RCursor<GLinks,unsigned int> Cur(best_Links_Out);
//	for (Cur.Start();!Cur.End();Cur.Next())
//	{
//		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
//	}
//	cout<<endl<< "----------------------------------------------"<<endl<<endl;
//	cout << "les meilleur resultats :"<<endl;
//	Cur.Set(best_Links_In);
//	for (Cur.Start();!Cur.End();Cur.Next())
//	{
//		cout<< Cur()->GetWeight()<< "pour "<<Cur()->GetDoc()->GetId()<<" dont url : "<<Cur()->GetDoc()->GetURL()<<endl;
//	}



	// delete all the container no more used.
	delete(BaseSet);
	delete(out_Links);
	delete(in_Links);
	delete(Dr);
	delete(Dc);
	delete(H);
	delete(A);
	delete(Xh);
	delete(Xa);
	delete(best_Links_H);
	delete(best_Links_A);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLinkCalcSALSA::GetSettings(void)
{
	static char tmp[300];
	char c;

	if(Params->LimitLink) c='1'; else c='0';
	sprintf(tmp,"%u %u %c %u",Params->NbResultsHub,Params->NbResultsAuto,c,Params->NbLinks);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcSALSA::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u",&Params->NbResultsHub,&Params->NbResultsAuto);

	char c;

	if(!(*s)) return;
	sscanf(s,"%u %u %c %u",&Params->NbResultsHub,&Params->NbResultsAuto,&c,&Params->NbLinks);
	if(c=='1') Params->LimitLink=true; else Params->LimitLink=false;
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalcSALSA::~GLinkCalcSALSA(void)
{
}
