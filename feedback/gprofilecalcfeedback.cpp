/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method  - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <math.h>



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofilecalcfeedback.h>
#include <docs/gdocvector.h>
#include <profiles/gprofdoc.h>
#include <profiles/gsubprofilevector.h>
#include <infos/gweightinfo.h>
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfileCalcFeedback::GProfileCalcFeedback(GFactoryProfileCalc* fac) throw(bad_alloc)
	: GProfileCalc(fac), MaxNonZero(60), RelFactor(1.0), FuzzyFactor(0.25),
	  IrrelFactor(0.75), Positive(false), isf(true), Vectors(5000),
	  NbDocsWords(5000), NbDocs(0), MaxOrderSize(5000)
{
	Order=new GWeightInfo*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ApplyConfig(void)
{
	MaxNonZero=Factory->GetUInt("MaxSize");
	RelFactor=Factory->GetDouble("RelFactor");
	FuzzyFactor=Factory->GetDouble("FuzzyFactor");
	IrrelFactor=Factory->GetDouble("IrrelFactor");
	Positive=Factory->GetBool("Positive");
	isf=Factory->GetBool("isf");
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Connect(GSession* session)
{
	GProfileCalc::Connect(session);
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Disconnect(GSession* session)
{
	GProfileCalc::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ComputeGlobal(GSubProfile* subprofile) throw(bad_alloc,GException)
{
	GWeightInfoCursor Words;
	GProfDocCursor Docs;
	GDocVector* CurDoc;
	GWeightInfo* w;
	bool Add;
	tDocAssessment Fdbk;
	double MaxFreq;
	double Factor;
	double Freq;

	// Clear all containers before computing
	Vectors.Clear();
	NbDocsWords.Clear();
	NbDocs=0;

	// Go through all documents, to compute the number of documents "OK", "KO"
	// and "N" for each language and the number of documents for each index term.
	Docs=subprofile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the assessment of the document  is not relevant
		// -> don't treat for the profiles computing
		CurDoc=dynamic_cast<GDocVector*>(Docs()->GetDoc());
		Fdbk=Docs()->GetFdbk();
		if((IrrelFactor==0.0)&&(Fdbk & djOutScope)) continue;

		// Add total number of document judged for the current language
		NbDocs++;

		// Update number of documents where appear each index term.
		Words=CurDoc->GetWeightInfoCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=NbDocsWords.GetInsertPtr<GWeightInfo*>(Words());
			(*w)+=1.0;
		}
	}

	// Go through all documents, add the frequences of the words of "OK"
	// documents and substract the frequences of the words of "KO" documents.
	Docs=subprofile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		CurDoc=dynamic_cast<GDocVector*>(Docs()->GetDoc());

		// Find list in function of the feedback
		Fdbk=Docs()->GetFdbk();
		if((IrrelFactor==0.0)&&(Fdbk & djOutScope)) continue;
		switch(Fdbk & djMaskJudg )
		{
			case djOK:
				Add=true;
				Factor=RelFactor;
				break;

			case djKO:
				Add=Positive;
				Factor=FuzzyFactor;
				break;

			default:
				Add=false;
				Factor=IrrelFactor;
				break;
		}

		// Add total number of words and the occurences of each word of the current document.
		Words=CurDoc->GetWeightInfoCursor();
		MaxFreq=CurDoc->GetMaxWeight();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Vectors.GetInsertPtr<GWeightInfo*>(Words());
			Freq=Words()->GetWeight()/MaxFreq;
			if((isf)&&(NbDocs>1))
				Freq*=log(NbDocs/NbDocsWords.GetPtr<unsigned int>(Words()->GetId())->GetWeight());
			if(Add)
				(*w)+=Factor*Freq;
			else
				(*w)-=Factor*Freq;
		}
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc,GException)
{
	GWeightInfo** ptr;
	unsigned int i,nb;
	GWeightInfos* Vector=s->GetVector();

	// Clear the Vector.
	s->RemoveRefs();
	Vector->Clear();

	// Choose the elements to stay.
	if(Vectors.IsEmpty())
	{
		s->UpdateFinished();
		return;
	}

	// Put in Order an ordered version of Global
	if(Vectors.NbPtr+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((Vectors.NbPtr+1)*1.1);
		Order=new GWeightInfo*[MaxOrderSize];
	}
	memcpy(Order,Vectors.Tab,Vectors.NbPtr*sizeof(GWeightInfo*));
	if(Vectors.NbPtr)
		qsort(static_cast<void*>(Order),Vectors.NbPtr,sizeof(GWeightInfo*),GWeightInfos::sortOrder);
	Order[Vectors.NbPtr]=0;

	//If MaxNonZero is null -> take all the words.
	if(MaxNonZero)
		nb=MaxNonZero;
	else
		nb=Vectors.NbPtr;
	for(i=nb+1,ptr=Order;(--i)&&(*ptr);ptr++)
		Vector->InsertPtr(new GWeightInfo(*ptr));

	// Update the references of the vector.
	s->UpdateRefs();

	//Tell the subprofile that the udpate is finished.
	s->UpdateFinished();
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Compute(GSubProfile* subprofile) throw(bad_alloc,GException)
{
	// Compute the global vectors.
	ComputeGlobal(subprofile);

	// Compute the vector for each subprofile
	ComputeSubProfile((GSubProfileVector*)subprofile);
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamUInt("MaxSize",60));
	params->InsertPtr(new GParamDouble("RelFactor",1.0));
	params->InsertPtr(new GParamDouble("FuzzyFactor",0.25));
	params->InsertPtr(new GParamDouble("IrrelFactor",0.75));
	params->InsertPtr(new GParamBool("Positive",false));
	params->InsertPtr(new GParamBool("isf",true));
}


//-----------------------------------------------------------------------------
GProfileCalcFeedback::~GProfileCalcFeedback(void)
{
	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_PROFILECALC_FACTORY("Optimist and Pessimist Feedback Method",GProfileCalcFeedback)
