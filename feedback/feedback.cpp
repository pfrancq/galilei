/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Feedback-based Profile Computing Method - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#include <gsession.h>
#include <gfdbk.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfileCalcFeedback::GProfileCalcFeedback(GSession* session,GPlugInFactory* fac)
	: GProfileCalc(session,fac), MaxNonZero(60), NegNonZero(0),
	  RelFactor(1.0), FuzzyFactor(0.25), IrrelFactor(0.75),
	  VectorsRel(20), VectorsIrrel(20), VectorsFuzzy(20), Internal(20),
	  NbDocs(0), MaxOrderSize(5000), IncrementalMode(false)
{
	Order=new const GConceptRef*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ApplyConfig(void)
{
	MaxNonZero=FindParam<RParamValue>("Max Size")->GetUInt();
	NegNonZero=FindParam<RParamValue>("Negative Size")->GetUInt();
	if(NegNonZero>MaxNonZero)
		MaxNonZero=NegNonZero;
	RelFactor=FindParam<RParamValue>("Relevant Factor")->GetDouble();
	FuzzyFactor=FindParam<RParamValue>("Fuzzy Factor")->GetDouble();
	IrrelFactor=FindParam<RParamValue>("Irrelevant Factor")->GetDouble();
	IncrementalMode=FindParam<RParamValue>("Incremental Mode")->GetBool();
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::AddVectors(R::RContainer<GVector,true,true>& vectors,double factor,double nbdocs)
{
	RCursor<GVector> Vector(vectors);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		GVector* Ins(Internal.GetInsertPtr(Vector()->GetConcept()));
		RCursor<GConceptRef> Cur(Vector()->GetRefs());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Compute and add the frequency
			GConceptRef* Ref(Ins->GetRef(Cur()));
			(*Ref)+=Cur()->GetWeight()*factor/nbdocs;
		}
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ComputeGlobal(void)
{
	RContainer<GVector,true,true>* Cur;

	// Clear all containers before computing
	VectorsRel.Clear();
	VectorsIrrel.Clear();
	VectorsFuzzy.Clear();
	Internal.Clear();
	NbDocsRel=NbDocsFuzzy=NbDocsIrrel=NbDocs=0;

	// Go through all documents, add the frequencies of the words of "OK"
	// documents and subtract the frequencies of the words of "KO" documents.
	RCursor<GFdbk> Fdbk(Profile->GetFdbks());
	for(Fdbk.Start();!Fdbk.End();Fdbk.Next())
	{
		// If the assessment of the document is not relevant
		// -> don't treat for the profiles computing
		if((IrrelFactor==0.0)&&(Fdbk()->GetFdbk() & ftIrrelevant)) continue;

		// If incremental mode and document has no change -> continue
		if(IncrementalMode&&(!Fdbk()->MustUse(Profile))) continue;

		// Get the document : if it exists and is defined -> add it
		GDoc* Doc(Session->GetObj(pDoc,Fdbk()->GetDocId()));
		if((!Doc)||(!Doc->IsDefined())) continue;

		// Find list in function of the feedback
		switch(Fdbk()->GetFdbk())
		{
			case ftRelevant:
				NbDocsRel++;
				Cur=&VectorsRel;
				break;

			case ftFuzzyRelevant:
				NbDocsFuzzy++;
				Cur=&VectorsFuzzy;
				break;

			default:
				NbDocsIrrel++;
				Cur=&VectorsIrrel;
				break;
		}

		// Add total number of concepts and the occurrences of each concept of the current document for each vector.
		RCursor<GVector> Vector(Doc->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			GVector* Ins(Cur->GetInsertPtr(Vector()->GetConcept()));

			// Go trough the concepts of the current vector
			double MaxFreq(Vector()->GetMaxAbsWeight());
			RCursor<GConceptRef> Cur(Vector()->GetRefs());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				// Compute and add the frequency
				GConceptRef* Ref(Ins->GetRef(Cur()));
				(*Ref)+=Cur()->GetWeight()/MaxFreq;
			}
		}


	// Add the vectors in Internal
	AddVectors(VectorsRel,RelFactor,NbDocsRel);
	AddVectors(VectorsFuzzy,FuzzyFactor,NbDocsFuzzy);
	AddVectors(VectorsIrrel,IrrelFactor,NbDocsIrrel);

	// Multiply by the idf factor
	Vector.Set(Internal);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		// Go trough the concepts of the current vector
		RCursor<GConceptRef> Cur(Vector()->GetRefs());
		for(Cur.Start();!Cur.End();Cur.Next())
			(*Cur())*=Cur()->GetConcept()->GetIF(otDoc);
		}
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ComputeProfile(void)
{

	// Copy the information of the relevant profile to the community.
	RCursor<GVector> Vector(Internal);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		if(!Vector()->GetNb())
			continue;

		// Reallocate Order if necessary, copy the elements and order it
		if(Vector()->GetNb()+1>MaxOrderSize)
		{
			if(Order) delete[] Order;
			MaxOrderSize=static_cast<size_t>((static_cast<double>(Vector()->GetNb())+1)*1.1);
			Order=new const GConceptRef*[MaxOrderSize];
		}
		Vector()->GetTab(Order);
		if(Vector()->GetNb())
			qsort(static_cast<void*>(Order),Vector()->GetNb(),sizeof(GConceptRef*),GVector::SortOrder);
		Order[Vector()->GetNb()]=0;
                //WriteFile("/home/pfrancq/tmp/profile",Vector()->GetNb(),Vector()->GetConcept());
		GVector* Ins(Vectors.GetInsertPtr(Vector()->GetConcept()));

		//If MaxNonZero is null -> take all the words.
		size_t i,nb,nb2;
		const GConceptRef** ptr;
		if(MaxNonZero)
		{
			nb=MaxNonZero-NegNonZero;
			if(nb>Vector()->GetNb())
				nb=Vector()->GetNb();
			nb2=NegNonZero;
			if(nb2>Vector()->GetNb()-nb)
				nb2=Vector()->GetNb()-nb;
		}
		else
		{
			nb=Vector()->GetNb();
			nb2=0;
		}

		// Copy the most relevant concepts
		for(i=nb+1,ptr=Order;--i;ptr++)
		{
			GConceptRef* Ref(Ins->GetRef((*ptr)->GetConcept()));
			(*Ref)+=(*ptr)->GetWeight();
		}

		// Copy the most irrelevant concepts
		for(i=nb2+1,ptr=&Order[Internal.GetNb()-1];--i;ptr--)
		{
			GConceptRef* Ref(Ins->GetRef((*ptr)->GetConcept()));
			(*Ref)+=(*ptr)->GetWeight();
		}
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Compute(const GProfile* profile)
{
	Profile=profile;

	// Clear Infos
	Vectors.Clear();

	// If incremental mode -> copy information of the profile in 'VectorsRel'.
	if(IncrementalMode&&profile->IsDefined())
	{
		RCursor<GVector> Vector(profile->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			GVector* Ins(Vectors.GetInsertPtr(Vector()->GetConcept()));
			RCursor<GConceptRef> Cur(Vector()->GetRefs());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				// Compute and add the frequency
				GConceptRef* Ref(Ins->GetRef(Cur()));
				(*Ref)+=Cur()->GetWeight();
			}
		}
	}

	// Compute the global vectors.
	ComputeGlobal();

	// Compute the vector for each profile
	ComputeProfile();
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::WriteFile(const RString& dir,size_t nb,GConcept* concept)
{
	RString name("profile"+RString::Number(Profile->GetId())+"-"+RString::Number(concept->GetId())+".txt");
	RDir::CreateDirIfNecessary(dir);
	RTextFile Out(dir+RFile::GetDirSeparator()+name);
	Out.Open(RIO::Create);
	const GConceptRef** ptr;
	size_t i;
	for(i=nb+1,ptr=Order;--i;ptr++)
		Out<<(*ptr)->GetId()<<(*ptr)->GetType()<<(*ptr)->GetWeight()<<endl;
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::CreateConfig(void)
{
	InsertParam(new RParamValue("Max Size",60));
	InsertParam(new RParamValue("Negative Size",0));
	InsertParam(new RParamValue("Relevant Factor",1.0));
	InsertParam(new RParamValue("Fuzzy Factor",0.25));
	InsertParam(new RParamValue("Irrelevant Factor",0.75));
	InsertParam(new RParamValue("Incremental Mode",false));
}


//-----------------------------------------------------------------------------
GProfileCalcFeedback::~GProfileCalcFeedback(void)
{
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/deallocation
	//      -> parse it to prevent memory leaks
	if(Order) delete[] Order;
}


//------------------------------------------------------------------------------
CREATE_PROFILECALC_FACTORY("Optimist and Pessimist Feedback Method","Optimist and Pessimist Feedback Method",GProfileCalcFeedback)
