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

#include <docs/gdoc.h>


//include file for R
//#include <rmath/rgenericsparsematrix.h>
//#include <rmath/rgenericsparsevector.h>
//#include <rmath/rgenericcell.h>
//#include <rmath/rgenericcellcursor.h>
//#include <rmath/rgenericvectorcursor.h>

using namespace GALILEI;
using namespace R;



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
}


////-----------------------------------------------------------------------------
//void GALILEI::GLinkCalcCorrespondence::InitAlgo(void)
//{
//	GDocCursor cur = Session->GetDocsCursor();
//	GLinkCursor lcur;
//	GLinks* links_out=0;
//
//	// Create a container of  outgoing links for all document present in the system
//	for (cur.Start();!cur.End();cur.Next())
//	{
//		Links_Out->InsertPtr( new GLinks( cur() ));
//	}
//
//		// For each page p set all the pages qi pointed by p
//	for (cur.Start();!cur.End();cur.Next())
//	{
//		if (!cur()->GetNbLinks()) continue;
//		links_out=Links_Out->GetPtr(cur()->GetId());
//		lcur = cur()->GetLinkCursor();
//
//		for (lcur.Start();!lcur.End();lcur.Next())
//		{
//			//  out_going links
//			links_out->InsertPtr(lcur());
//		}
//	}
//}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Operation_H(RGenericSparseMatrix<RGenericCell<double>,true>* Qhubs,RGenericSparseVector<RGenericCell<double>,true>* Xhubs)
{
//	unsigned int choice,maxLink,i;
	double sum;
	//unsigned int nbPages;
	//GLinkCursor cur;
	//GBalancedLinksCursor lcur;


	 //Init Part
	RGenericCellCursorDouble2 cellCur;
	RGenericVectorCursorDouble2 vectCur;
	RGenericSparseVector<RGenericCell<double>,true>* tmpX = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);

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
			//cout<< " i "<<vectCur()->GetId()<<" et j "<<cellCur()->GetId()<<endl;
			sum+= cellCur()->GetValue() * Xhubs->GetPtr(cellCur()->GetId())->GetValue();
			//cout<< "value "<<sum<<endl;
		}

		tmpX->GetInsertPtr(vectCur()->GetId())->SetValue(sum);
		Norme_H += sum*sum;
	}

	Norme_H = sqrt(Norme_H);
	//cout << "la Norme H : "<<Norme_H<<endl;

	*Xhubs=*tmpX;

//TEST
//cout<<"after compute"<<endl;
//	cellCur.Set(Xhubs);
//	for (cellCur.Start();!cellCur.End(); cellCur.Next())
//	{
//		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
//	}
//	cout<< endl;

	delete tmpX;
	
	// Init Part
//	lcur.Set(out_Links);
//	Norme_Out=0;
//
//	if(Params->LimitLink)
//	{
//		maxLink=Params->NbLinks;
//		if(Params->UseMultipleLink) choice=1;
//		else choice=2;
//	}
//	else
//	{
//		if(Params->UseMultipleLink) choice=3;
//		else choice=4;
//	}
//
//	switch (choice)
//	{
//		// Limit Link && MultipleLink
//		case 1:
//			for(lcur.Start(),i=maxLink;!lcur.End();lcur.Next(),i--)
//			{
//				sum=0;
//				nbPages=0;
//				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();
//
//				for(cur.Start();!cur.End();cur.Next())
//				{
//					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight()* cur()->GetOccurs();
//					nbPages+=cur()->GetOccurs() ;
//				}
//				if (nbPages)
//				{
//					sum =sum / nbPages;
//					lcur()->SetWeight(sum);
//					Norme_Out += sum*sum;
//				}
//				else
//				{
//					lcur()->SetWeight(0);
//				}
//			}
//			break;
//
//		// Limit Link && not MultipleLink
//		case 2:
//			for(lcur.Start(),i=maxLink;!lcur.End();lcur.Next(),i--)
//			{
//				sum=0;
//				nbPages=0;
//				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();
//
//				for(cur.Start();!cur.End();cur.Next())
//				{
//					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight();
//					nbPages++;
//				}
//				if (nbPages)
//				{
//					sum =sum /nbPages;
//					lcur()->SetWeight(sum);
//					Norme_Out += sum*sum;
//				}
//				else
//				{
//					lcur()->SetWeight(0);
//				}
//			}
//			break;
//
//		// not Limit Link && MultipleLink
//		case 3:
//			for(lcur.Start();!lcur.End();lcur.Next())
//			{
//				sum=0;
//				nbPages=0;
//				cur= in_Links->GetPtr( lcur()->GetId() )->GetLinkCursor();
//
//				for(cur.Start();!cur.End();cur.Next())
//				{
//					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight()* cur()->GetOccurs();
//					nbPages+=cur()->GetOccurs() ;
//				}
//				if (nbPages)
//				{
//					sum =sum / nbPages;
//					lcur()->SetWeight(sum);
//					Norme_Out += sum*sum;
//				}
//				else
//				 {
//					 lcur()->SetWeight(0);
//				 }
//			}
//			break;
//
//		// not Limit Link && not MultipleLink
//		case 4:
//			for(lcur.Start();!lcur.End();lcur.Next())
//			{
//				sum=0;
//				nbPages=0;
//				cur= in_Links->GetPtr( lcur()->GetId())->GetLinkCursor();
//
//				for(cur.Start();!cur.End();cur.Next())
//				{
//					sum+= in_Links->GetPtr( cur()->GetId() )->GetWeight();
//					nbPages++;
//				}
//				if (nbPages)
//				{
//					sum =sum / nbPages;
//					lcur()->SetWeight(sum);
//					Norme_Out += sum*sum;
//				}
//			}
//			break;
//	}
//
//	Norme_Out=sqrt(Norme_Out);
//
//		cout <<"la norme  OUT"<<Norme_Out<< endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Operation_A(RGenericSparseMatrix<RGenericCell<double>,true>* Qautos,RGenericSparseVector<RGenericCell<double>,true>* Xauto)
{
	double sum;
	//unsigned int nbPages;
	//GLinkCursor cur;
	//GBalancedLinksCursor lcur;

	//Init Part
	RGenericCellCursorDouble2 cellCur;
	RGenericVectorCursorDouble2 vectCur;
	RGenericSparseVector<RGenericCell<double>,true>* tmpX = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50) ;

	Norme_A=0;
	vectCur.Set(Qautos) ;
	// Compute Part
	for (vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cellCur.Set(vectCur());
		sum=0;
		for (cellCur.Start();! cellCur.End();cellCur.Next())
		{
			sum+= cellCur()->GetValue() * Xauto->GetPtr(cellCur()->GetId())->GetValue();
			//cout<< " i "<<vectCur()->GetId()<<" et j "<<cellCur()->GetId()<<endl;
			//cout<< "value "<<sum<<endl;
		}
		tmpX->GetInsertPtr(vectCur()->GetId())->SetValue(sum);
		Norme_A += sum*sum;
	}

	Norme_A = sqrt(Norme_A);
	cout << "la Norme A : "<<Norme_A<<endl;

	*Xauto = *tmpX;


	delete tmpX;


	cout<<"after compute Xa:"<<endl;
	cellCur.Set(Xauto);
	for (cellCur.Start();!cellCur.End(); cellCur.Next())
	{
		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
	}
	cout<< endl;

	
//	// Init Part
//	lcur.Set(in_Links);
//	Norme_In=0;
//
//	if (Params->UseMultipleLink)
//	{
//		for(lcur.Start();!lcur.End();lcur.Next())
//		{
//			sum=0;
//			nbPages=0;
//			cur= out_Links->GetPtr( lcur()->GetId())->GetLinkCursor();
//
//			for(cur.Start();!cur.End();cur.Next())
//			{
//				sum+= out_Links->GetPtr( cur()->GetId() )->GetWeight() * cur()->GetOccurs();
//				nbPages+= cur()->GetOccurs();
//			}
//			if (nbPages)
//			{
//				sum =sum / nbPages;
//				lcur()->SetWeight(sum);
//				Norme_In += sum*sum;
//			}
//			else
//			{
//				lcur()->SetWeight(0);
//			}
//
//		}
//	}
//	else
//	{
//		for(lcur.Start();!lcur.End();lcur.Next())
//		{
//			sum=0;
//			nbPages=0;
//			cur= out_Links->GetPtr( lcur()->GetId() )->GetLinkCursor();
//
//			for(cur.Start();!cur.End();cur.Next())
//			{
//				sum+= out_Links->GetPtr( cur()->GetId() )->GetWeight() ;
//				nbPages++;
//			}
//			if (nbPages)
//			{
//				sum =sum /nbPages;
//				lcur()->SetWeight(sum);
//				Norme_In += sum*sum;
//			}
//		}
//	}
//
//	Norme_In=sqrt(Norme_In);
//	cout <<"la norme IN "<<Norme_In<< endl;
}



//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::Normalization(RGenericSparseVector<RGenericCell<double>,true>* X, double Norme)
{
	//GBalancedLinksCursor lcur;
	RGenericCellCursorDouble2 cellCur;
	float test=0;

	//TEST
	cout << "before normalisation"<<endl;
	cellCur.Set(X);
	for (cellCur.Start();!cellCur.End(); cellCur.Next())
	{
		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
	}
	cout<< endl;

     //normalization
	cellCur.Set(X);
	for(cellCur.Start();!cellCur.End();cellCur.Next())
	{
		cellCur()->SetValue( cellCur()->GetValue() / Norme );
		test += cellCur()->GetValue() *cellCur()->GetValue();
	}

	cout <<" apres normalisation : "<< test << endl;

	//TEST
	cout << "et valeur apres norm"<<endl;
	cellCur.Set(X);
	for (cellCur.Start();!cellCur.End(); cellCur.Next())
	{
		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
	}
	cout<< endl<<endl;

}


//-----------------------------------------------------------------------------
void GALILEI::GLinkCalcCorrespondence::FindBestValues(unsigned int nbVal,RGenericSparseVector<RGenericCell<double>,true>*  X,RGenericSparseVector<RGenericCell<int>,true>* Bv)
{
	BestValue* res;
	BestValue min;
	RGenericCellCursorDouble2 cellCur;
	//RGenericCell<int>* icell;
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
}


//void GALILEI::GLinkCalcCorrespondence::FindBestValues(unsigned int nbVal,GBalancedLinksCursor lcur,RContainer<GBalancedLinks,unsigned int,true,false>* Bv)
//{
//	BestValue* res;
//	BestValue min;
//	GBalancedLinks* tmpL;
//	unsigned int i,j;
//
//	if (!nbVal) return ;
//	res = new BestValue[nbVal];
//
//	// init to zero
//	i=0;
//	while (i<nbVal)
//	{
//		res[i].Id=0;
//		res[i].Value=0.0;
//		i++;
//	}
//
//	min.Id=0;
//	min.Value=0;
//
//	for(lcur.Start();!lcur.End();lcur.Next())
//	{
//		if (lcur()->GetWeight()>min.Value)
//		{
//			res[min.Id].Id=lcur()->GetDoc()->GetId();
//			res[min.Id].Value=lcur()->GetWeight();
//			min = findMin(res,nbVal);
//		}
//	}
//	//sort values contained in res.
//	struct BestValue tmp;
//	for(j=nbVal-1;j>1;j--)
//	{
//		for(i=0;i<j;i++)
//		{
//			if (res[i].Value < res[i+1].Value)
//			{
//				tmp=res[i];
//				res[i]=res[i+1];
//				res[i+1]=tmp;
//			}
//		}
//	}
//
//	// insert values in the congtainer Bv.
//	for(unsigned int j=0;j<nbVal;j++)
//	{
//		if (res[j].Id==0) continue;
//		Bv->InsertPtr(tmpL =new GBalancedLinks(Links_Out->GetPtr(res[j].Id )->GetDoc()));
//		tmpL->SetWeight(res[j].Value);
//	}
//}


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
void GALILEI::GLinkCalcCorrespondence::Compute(GSubProfile* Subprof)
{

	RContainer<GDoc,unsigned int, false,true>* BaseSet;
	RContainer<GLinks,unsigned int,true,true>* out_Links;
	RContainer<GLinks,unsigned int,true,true>* in_Links  ;
//	RContainer<GBalancedLinks,unsigned int,true,false>* Best_Links_Out;
//	RContainer<GBalancedLinks,unsigned int,true,false>* Best_Links_In;

//	GLink* tmpLink;
	RDate date;
	tDocJudgement Fdbk;
	GProfDocCursor subprofCur;
//	GDocCursor docCur ;
	GLinkCursor linkCur;
	GLinksCursor linksCur;
//	GBalancedLinksCursor balLinksCur;
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
	subprofCur = Subprof->GetProfDocCursor();
	currentDocId=k=0;

	// -- 1 --
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

	// -- 2 --
	//---------------------------------------------------------------
	// Creation of Vector Needed for links computation
	 // Init Part
	out_Links=new RContainer<GLinks,unsigned int,true,true>(50,10);
	in_Links = new RContainer<GLinks,unsigned int,true,true>(50,10);

//	docCur.Set(BaseSet);
//	for (docCur.Start(); ! docCur.End() ; docCur.Next())
//	{
//		out_Links->InsertPtr(new GLinks( docCur() ) );
//		in_Links->InsertPtr(new GLinks(docCur() ) );
//	}
//
//	for (docCur.Start(); ! docCur.End() ; docCur.Next())
//	{
//		currentDocId= docCur()->GetId();
//		if (Links_Out->GetPtr( currentDocId ) )
//		{
//			linkCur = Links_Out->GetPtr( currentDocId )->GetLinkCursor();
//			for (linkCur.Start(); ! linkCur.End() ; linkCur.Next() )
//			{
//				if ( BaseSet->IsIn(linkCur()->GetId() ) )
//				{
//					out_Links->GetPtr( currentDocId )->InsertPtr( linkCur() );
//					in_Links->GetPtr( linkCur()->GetDoc()->GetId() )->InsertPtr( tmpLink =new GLink(docCur() ) )  ;
//					tmpLink->SetOccurs( linkCur()->GetOccurs());
//				}
//			}
//		}
//	}

	 for (int i=1;i<6;i++)
	{
		out_Links->InsertPtr(new GLinks( Session->GetDoc(i) ) );
		in_Links->InsertPtr(new GLinks(Session->GetDoc(i) ) );
	}
	out_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(2)));
	out_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(5)));
	out_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(3)));
	out_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
	out_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(1)));
	out_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(5)));
	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(1)));
	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
	out_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(3)));
	out_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(2)));

	in_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(3)));
	in_Links->GetPtr(1)->InsertPtr(new GLink(Session->GetDoc(4)));
	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(1)));
	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(4)));
	in_Links->GetPtr(2)->InsertPtr(new GLink(Session->GetDoc(5)));
	in_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(2)));
	in_Links->GetPtr(3)->InsertPtr(new GLink(Session->GetDoc(4)));
	in_Links->GetPtr(4)->InsertPtr(new GLink(Session->GetDoc(2)));
	in_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(1)));
	in_Links->GetPtr(5)->InsertPtr(new GLink(Session->GetDoc(3)));

	// -- 3 --
	// creation of the Transition Matrix for Hubs: Qh et Autorities: Qa
	//--------------------------------------------------------------------------------
	RGenericSparseMatrix<RGenericCell<double>,true>* Qh = new RGenericSparseMatrix<RGenericCell<double>,true>(100,50);
	RGenericSparseMatrix<RGenericCell<double>,true>* Qa = new RGenericSparseMatrix<RGenericCell<double>,true>(100,50);
//	RGenericSparseVector<RGenericCell<float>,true>* tmpVector;
	RGenericCell<double>* tmpCell;
	//RGenericSparseMatrix<RGenericCell<float>,true> Qh = new RGenericSparseMatrix<RGenericCell<float>,true>(100,50);

	GLinksCursor linksCurI;
	GLinksCursor linksCurJ;
	GLinkCursor linkCur_H;
	GLinkCursor linkCur_A;
	double densMargI_H,densMargJ_H; 
	double densMargI_A,densMargJ_A;
	double valueH,valueA;

	linksCurI.Set( out_Links );
	linksCurJ.Set( in_Links );
//	unsigned int id1;
	densMargI_H=densMargJ_H=0  ;
	densMargI_A=densMargJ_A=0  ;

	// Qh(i,j) = Sum_k { dh(i,i)*W(i,k)*da(k,k)*WT(k,j) }
	// Qh(i,j) = Sum_k { da(i,i)*WT(i,k)*dh(k,k)*W(k,j)
	// for indice i
	for (linksCurI.Start(); !linksCurI.End(); linksCurI.Next())
	{
		linkCur_H= linksCurI()->GetLinkCursor();
		densMargI_H=linksCurI()->NbPtr;
  
				// for indice j
		for (linksCurJ.Start(); !linksCurJ.End(); linksCurJ.Next())
		{
			densMargI_A=linksCurJ()->NbPtr;
			valueH=0;
			valueA=0;
					//indice k
			for(linkCur_H.Start(); !linkCur_H.End(); linkCur_H.Next())
			{
				if ( in_Links->GetPtr(linkCur_H()->GetId())->IsIn(linksCurJ()->GetId()))
				{
					densMargJ_H=in_Links->GetPtr(linkCur_H()->GetId() )->NbPtr ;
					valueH += ( (double)1/densMargI_H)* ((double)1/ densMargJ_H ) ;
					//cout <<"i "<<linksCurI()->GetId()<<" et j "<<linksCurJ()->GetId()<<"et k "<<linkCur_H()->GetId() <<endl;
					//cout << densMargI_H<<"  "<<densMargJ_H<<endl;
					//cout << "value "<<valueH<<endl;
				}
			}
			if (valueH)
			{
				// add element Qij with the value
				tmpCell = Qh->GetInsertPtr(linksCurI()->GetId())->GetInsertPtr(linksCurJ()->GetId());
				tmpCell->SetValue(valueH);
			}
			
			linkCur_A= linksCurJ()->GetLinkCursor();
			for (linkCur_A.Start(); !linkCur_A.End(); linkCur_A.Next())
			{
				if ( out_Links->GetPtr(linkCur_A()->GetId())->IsIn(linksCurI()->GetId()))
				{
					densMargJ_A=out_Links->GetPtr(linkCur_A()->GetId() )->NbPtr ;
					valueA += ( (double)1/densMargI_A)* ((double)1/ densMargJ_A ) ;
					//cout << densMargI_A<<"  "<<densMargJ_A<<endl;
					//cout <<"i "<<linksCurI()->GetId()<<" et j "<<linksCurJ()->GetId()<<endl;
					//cout << "value "<<valueA<<endl;
				}
			}

			if (valueA)
			{
				// add element Qij with the value
				tmpCell = Qa->GetInsertPtr(linksCurJ()->GetId() )->GetInsertPtr(linksCurI()->GetId());
				tmpCell->SetValue(valueA);
			}
		}
	}
                       //test
//	cout<< endl<< "test QH "<<endl;
	RGenericCellCursorDouble2 cellCur;
	RGenericVectorCursorDouble2 vectCur;
//	vectCur.Set(Qh);
//	for( vectCur.Start();!vectCur.End(); vectCur.Next())
//	{
//		cout<<endl<< "Ligne "<< vectCur()->GetId()<<endl;
//		cellCur.Set(vectCur());
//		for (cellCur.Start();!cellCur.End();cellCur.Next())
//		{
//			cout<< "  - "<<cellCur()->GetId()<<" - "<<cellCur()->GetValue() ;
//		}
//	}

//	cout<< endl;
//
//		cout<< "test QA "<<endl;
//	vectCur.Set(Qa);
//	for( vectCur.Start();!vectCur.End(); vectCur.Next())
//	{
//		cout <<endl<< "Ligne "<< vectCur()->GetId()<<endl;
//		cellCur.Set(vectCur());
//		for (cellCur.Start();!cellCur.End();cellCur.Next())
//		{
//			cout<< "  - "<<cellCur()->GetId()<<" - "<<cellCur()->GetValue() ;
//		}
//	}
//	cout<< endl;

	
	// -- 4 --
	//compute sum of all elements in the matrix W -> W..
	// compute vector Pih and Pia --> Wi. /W.. and W.j /W..
	//-------------------------------------------------------------------------
	double sumW=0;
	for (linksCurI.Start(); !linksCurI.End(); linksCurI.Next())
	{
		sumW+= linksCurI()->NbPtr;
	}
	cout <<"norme W " <<sumW<<endl;

	// calcul de Pih (steady-state vector)
	RGenericSparseVector<RGenericCell<double>,true>* Pih = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	RGenericSparseVector<RGenericCell<double>,true>* Pia = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	for (linksCurI.Start(); !linksCurI.End(); linksCurI.Next())
	{
		tmpCell = Pih->GetInsertPtr( linksCurI()->GetId() );
		tmpCell->SetValue(linksCurI()->NbPtr /sumW );
	}

	for (linksCurJ.Start(); !linksCurJ.End(); linksCurJ.Next())
	{
		tmpCell = Pia->GetInsertPtr( linksCurJ()->GetId() );
		tmpCell->SetValue(linksCurJ()->NbPtr /sumW );
	}


	// -- 5 --
	//Compute Transient matrix Th (Ta)  (reuse of matrix Qh(Qa))
	// Th = Qh -1*Pih  --> Th(i,j)=Qh(i,j) - Pih(j)
	// Ta = Qa -1*Pia  --> Ta(i,j)=Qa(i,j) - Pia(j)
	//-------------------------------------------------------------------------
	// -- 5.1 -- Th
	vectCur.Set(Qh);
	for( vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cellCur.Set(Pih);
		for (cellCur.Start();!cellCur.End();cellCur.Next())
		{
			tmpCell = vectCur()->GetPtr(cellCur()->GetId()) ;
			if( tmpCell )
			{
				tmpCell->SetValue( tmpCell->GetValue() - cellCur()->GetValue()) ;
			}
			else
			{
				tmpCell = vectCur()->GetInsertPtr(cellCur()->GetId());
				tmpCell->SetValue( - cellCur()->GetValue() ) ;
			}
		}
	}

	// -- 5.2 -- Ta
	vectCur.Set(Qa);
	for( vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cellCur.Set(Pia);
		for (cellCur.Start();!cellCur.End();cellCur.Next())
		{
			tmpCell = vectCur()->GetPtr(cellCur()->GetId()) ;
			if( tmpCell )
			{
				tmpCell->SetValue( tmpCell->GetValue() - cellCur()->GetValue()) ;
			}
			else
			{
				tmpCell = vectCur()->GetInsertPtr(cellCur()->GetId());
				tmpCell->SetValue( - cellCur()->GetValue() ) ;
			}
		}
	}

	//test
	cout <<endl<< "test Ta";
	vectCur.Set(Qa);
	for( vectCur.Start();!vectCur.End(); vectCur.Next())
	{
		cout <<endl<< "Ligne "<< vectCur()->GetId()<<endl;
		cellCur.Set(vectCur());
		for (cellCur.Start();!cellCur.End();cellCur.Next())
		{
			cout<< "  * "<<cellCur()->GetId()<<" * "<<cellCur()->GetValue() ;
		}
	}
	cout<<endl;
	

	// -- 6 --
	// Creation of the eigen vectors -> vectors containing results
	RGenericSparseVector<RGenericCell<double>,true>* Xh = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);
	RGenericSparseVector<RGenericCell<double>,true>* Xa = new RGenericSparseVector<RGenericCell<double>,true>(1,100,50);

	for(vectCur.Start(); ! vectCur.End(); vectCur.Next())
	{
		tmpCell = Xh->GetInsertPtr(vectCur()->GetId());
		tmpCell->SetValue(1);
		tmpCell = Xa->GetInsertPtr(vectCur()->GetId());
		tmpCell->SetValue(1);
	}

	// test
//	cellCur.Set(Xh);
//	for (cellCur.Start();!cellCur.End(); cellCur.Next())
//	{
//		cout<<" -- "<<cellCur()->GetId()<<" -- "<<cellCur()->GetValue();
//	}
//
//	cout<<"  +   "<<Xh->GetPtr(4)->GetValue() << endl;
	
	//---------------------------------------------------------------
	// Compute Hubs and Autorities.
	while (k< Params->NbIteration)
	{
		Operation_H(Qh,Xh);
		Operation_A(Qa,Xa);
		Normalization(Xh,Norme_H);
		Normalization(Xa,Norme_A);
		k++;
	}

	cout << "res Xh"<<endl;
	cellCur.Set(Xh);
	for (cellCur.Start();!cellCur.End();cellCur.Next())
	{
		cout<< "  * "<<cellCur()->GetId()<<" * "<<cellCur()->GetValue() ;
	}
	cout<<endl;

	cout << "res Xa "<<endl;
	cellCur.Set(Xa);
	for (cellCur.Start();!cellCur.End();cellCur.Next())
	{
		cout<< "  * "<<cellCur()->GetId()<<" * "<<cellCur()->GetValue() ;
	}
	cout<<endl;

	

	//---------------------------------------------------------------
	// Sort Results and Find Best Values for the Hubs and Authorities.
	// Init Part
	RGenericSparseVector<RGenericCell<int>,true>* Best_Links_H = new RGenericSparseVector<RGenericCell<int>,true>(1,Params->NbResultsHub,5);
	RGenericSparseVector<RGenericCell<int>,true>* Best_Links_A = new RGenericSparseVector<RGenericCell<int>,true>(1,Params->NbResultsAuto,5);
	

	// find best results for out_Links
	FindBestValues(Params->NbResultsHub,Xh,Best_Links_H);

	// Find best results for in_Links
	FindBestValues(Params->NbResultsAuto,Xa,Best_Links_A);

	RGenericCellCursor<int> icell;
	icell.Set(Best_Links_H);
	for (icell.Start(); ! icell.End(); icell.Next())
	{
		cout<< icell()->GetId() <<"  "<<icell()->GetValue()<<endl;
	}
	
//	// Add Best Results to current Profile.
//
//	// Add documents considered as Best Hubs
//	balLinksCur.Set(Best_Links_Out);
//	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
//	{
//		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
//		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
//		{
//			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djNav | djHub ), date)) ;
//			//cout << "Document Hub add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
//		}
//	}
//
//	// Add documents considered as Best Autorities
//	balLinksCur.Set(Best_Links_In);
//	for (balLinksCur.Start();!balLinksCur.End();balLinksCur.Next())
//	{
//		// If the profile hasn't judged the doc -> Insert a judgement for the doc.
//		if (! Prof->GetFeedback( balLinksCur()->GetDoc() ))
//		{
//			Prof->AddJudgement( new GProfDoc(balLinksCur()->GetDoc(), Prof ,tDocJudgement( djOK | djAutority ), date )) ;
//			//cout << "Document Autorte add to profile: "<< linksCur()->GetDoc()->GetURL()<<endl;
//		}
//	}

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


