/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
using namespace std;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rdir.h>
using namespace R;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <feedback.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gweightinfo.h>
#include <glang.h>
#include <gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfileCalcFeedback::GProfileCalcFeedback(GFactoryProfileCalc* fac)
	: GProfileCalc(fac), Infos(5000,2500), MaxNonZero(60), NegNonZero(0), RelFactor(1.0),
	  FuzzyFactor(0.25), IrrelFactor(0.75),
	  Vectors(5000), VectorsIrrel(5000), VectorsFuzzy(5000),
	  NbDocs(0), MaxOrderSize(5000), IncrementalMode(false)
{
	Order=new GWeightInfo*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ApplyConfig(void)
{
	MaxNonZero=Factory->GetUInt("MaxSize");
	NegNonZero=Factory->GetUInt("NegSize");
	if(NegNonZero>MaxNonZero)
		MaxNonZero=NegNonZero;
	RelFactor=Factory->GetDouble("RelFactor");
	FuzzyFactor=Factory->GetDouble("FuzzyFactor");
	IrrelFactor=Factory->GetDouble("IrrelFactor");
	IncrementalMode=Factory->GetBool("IncrementalMode");
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
void GProfileCalcFeedback::ComputeGlobal(void)
{
	RCursor<GWeightInfo> Words;
	RCursor<GFdbk> Docs;
	double MaxFreq;
	double TotalRef;
	GConceptType* type(0);
	GWeightInfos* Cur;

	// Clear all containers before computing
	Vectors.Clear();
	VectorsIrrel.Clear();
	VectorsFuzzy.Clear();
	NbDocsRel=NbDocsFuzzy=NbDocsIrrel=NbDocs=0;

	// Go through all documents, add the frequences of the words of "OK"
	// documents and substract the frequences of the words of "KO" documents.
	Docs=Profile->GetFdbks();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the assessment of the document is not relevant
		// -> don't treat for the profiles computing
		if((IrrelFactor==0.0)&&(Docs()->GetFdbk() & djOutScope)) continue;

		// If incremental mode and document has no change -> continue
		if(IncrementalMode&&(!Docs()->MustUse(Profile))) continue;

		// Get the document : if it exists and is defined -> add it
		GDoc* doc=Session->GetDoc(Docs()->GetDocId());
		if((!doc)||(!doc->IsDefined())) continue;

		// Find list in function of the feedback
		switch(Docs()->GetFdbk() & djMaskJudg )
		{
			case djOK:
				NbDocsRel++;
				Cur=&Vectors;
				break;

			case djKO:
				NbDocsFuzzy++;
				Cur=&VectorsFuzzy;
				break;

			default:
				NbDocsIrrel++;
				Cur=&VectorsIrrel;
				break;
		}

		// Add total number of words and the occurences of each word of the current document.
		Words=doc->GetInfos();
		type=0; // No current type
		for(Words.Start();!Words.End();Words.Next())
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(Words()->GetConcept()->GetType()!=type)
			{
				// Yes -> Get the total number of document analyzed.
				type=Words()->GetConcept()->GetType();
				TotalRef=type->GetRef(otDoc);
				MaxFreq=doc->GetMaxWeight(type);
			}

			if(Words()->GetId()>32868&&Words()->GetType()->GetId()==26)
				cout<<"Problem when aggreging documents"<<endl;

			// Compute and add the frequence
			(*Cur->GetInfo(Words()))+=Words()->GetWeight()/MaxFreq;
		}
	}

	// Compute the vectors
	Words=Vectors.GetInfos();
	for(Words.Start();!Words.End();Words.Next())  // Divide the
		(*Words())*=RelFactor/NbDocsRel;
	Words=VectorsFuzzy.GetInfos();
	for(Words.Start();!Words.End();Words.Next())
		(*Vectors.GetInfo(Words()))+=Words()->GetWeight()*FuzzyFactor/NbDocsFuzzy;
	Words=VectorsIrrel.GetInfos();
	for(Words.Start();!Words.End();Words.Next())
		(*Vectors.GetInfo(Words()))+=Words()->GetWeight()*IrrelFactor/NbDocsIrrel;

	// Multiply by the idf factor and remove null weighted information (in VectorsIrrel).
	Words=Vectors.GetInfos();
	VectorsIrrel.Clear();
	type=0; // No current type
	for(Words.Start();!Words.End();Words.Next())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(Words()->GetConcept()->GetType()!=type)
			type=Words()->GetConcept()->GetType();
		(*Words())*=log10(TotalRef/static_cast<double>(type->GetRef(Words()->GetId(),otDoc)));
		if(fabs(Words()->GetWeight())<0.00001)
			VectorsIrrel.GetInfo(Words());
	}
	Words=VectorsIrrel.GetInfos();
	for(Words.Start();!Words.End();Words.Next())
		Vectors.DeletePtr(Words()->GetConcept());
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ComputeProfile(void)
{
	GWeightInfo** ptr;
	size_t i,nb,nb2;

	// Choose the elements to stay.
	if(Vectors.IsEmpty())
		return;

	// Put in Order an ordered version of Global
	if(Vectors.GetNb()+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<size_t>((Vectors.GetNb()+1)*1.1);
		Order=new GWeightInfo*[MaxOrderSize];
	}
	Vectors.GetTab(Order);
	if(Vectors.GetNb())
		qsort(static_cast<void*>(Order),Vectors.GetNb(),sizeof(GWeightInfo*),GWeightInfos::sortOrder);

	//If MaxNonZero is null -> take all the words.
	if(MaxNonZero)
	{
		nb=MaxNonZero-NegNonZero;
		if(nb>Vectors.GetNb())
			nb=Vectors.GetNb();
		nb2=NegNonZero;
		if(nb2>Vectors.GetNb()-nb)
			nb2=Vectors.GetNb()-nb;
	}
	else
	{
		nb=Vectors.GetNb();
		nb2=0;
	}

	// Copy the relevant entities
	for(i=nb+1,ptr=Order;--i;ptr++)
		Infos.InsertPtr(new GWeightInfo(**ptr));

	// Copy the irrelevant entities
	for(i=nb2+1,ptr=&Order[Vectors.GetNb()-1];--i;ptr--)
		Infos.InsertPtr(new GWeightInfo(**ptr));
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Compute(GProfile* profile)
{
	Profile=profile;
	GWeightInfo* ptr;
	GConceptType* type(0);

	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	// If incremental mode -> copy information of the profile in 'Vectors'.
	if(IncrementalMode&&profile->IsDefined())
	{
		size_t i,TotalRef;
		Cur=profile->GetInfos();
		for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(Cur()->GetConcept()->GetType()!=type)
			{
				// Yes -> Get the total number of document analyzed.
				type=Cur()->GetConcept()->GetType();
				TotalRef=type->GetRef(otDoc);
			}

			Infos.InsertPtrAt(ptr=new GWeightInfo(*Cur()),i);
			if(ptr)
			{
				(*ptr)/=log10(TotalRef/static_cast<double>(type->GetRef(Cur()->GetId(),otDoc)));
			}
		}
	}

	// Compute the global vectors.
	ComputeGlobal();

	// Compute the vector for each subprofile
	ComputeProfile();

//	WriteFile("/home/pfrancq/tmp");

	// Set the Variable of the subprofile
	profile->Update(Infos,true);
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::WriteFile(const RString& dir)
{
	RString name("profile"+RString::Number(Profile->GetId()));
	RDir::CreateDirIfNecessary(dir);
	RTextFile Out(dir+RFile::GetDirSeparator()+name);
	Out.Open(RIO::Create);
	GWeightInfo** ptr;
	size_t i;
	for(i=Vectors.GetNb()+1,ptr=Order;--i;ptr++)
		Out<<(*ptr)->GetId()<<(*ptr)->GetType()<<(*ptr)->GetWeight()<<endl;
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("MaxSize",60));
	params->InsertParam(new RParamValue("NegSize",0));
	params->InsertParam(new RParamValue("RelFactor",1.0));
	params->InsertParam(new RParamValue("FuzzyFactor",0.25));
	params->InsertParam(new RParamValue("IrrelFactor",0.75));
	params->InsertParam(new RParamValue("IncrementalMode",false));
}


//-----------------------------------------------------------------------------
GProfileCalcFeedback::~GProfileCalcFeedback(void)
{
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_PROFILECALC_FACTORY("Optimist and Pessimist Feedback Method",GProfileCalcFeedback)
