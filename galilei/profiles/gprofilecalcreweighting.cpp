/*

	GALILEI Research Project

	GProfileCalcReWeighting.h

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


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <profiles/gprofilecalcreweighting.h>
#include <docs/gdocvector.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <infos/giword.h>
#include <infos/giwordlist.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GReWeightingParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GReWeightingParams::GReWeightingParams(void)
	: GCalcParams("ReWeighting")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GReWeightingParams::GetSettings(void)
{
	return(itoa(MaxNonZero));
}


//-----------------------------------------------------------------------------
void GALILEI::GReWeightingParams::SetSettings(const char* s)
{
	if(!(*s)) return;
	MaxNonZero=strtoul(s,0,10);
}



//-----------------------------------------------------------------------------
//
// class GProfileCalcReWeighting::GNbDocsLangs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfileCalcReWeighting::GNbDocsLangs
{
public:
	GLang* Lang;
	unsigned int NbDocs;

	GNbDocsLangs(GLang* l) : Lang(l), NbDocs(0) {}
	void Clear(void) {NbDocs=0;}
	int Compare(const GNbDocsLangs* p) const {return(Lang->Compare(p->Lang));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	void Inc(void) {NbDocs++;}
	unsigned int GetNb(void) const {return(NbDocs);}
};



//-----------------------------------------------------------------------------
//
// InternVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfileCalcReWeighting::InternVector : public GIWordsWeights
{
public:
	GLang* Lang;

	InternVector(GLang* lang,unsigned int n) : GIWordsWeights(n), Lang(lang)  {}
	int Compare(const InternVector& c) const {return(Lang->Compare(c.Lang));}
	int Compare(const InternVector* c) const {return(Lang->Compare(c->Lang));}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
};



//-----------------------------------------------------------------------------
//
//  GProfileCalcReWeighting
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcReWeighting::GProfileCalcReWeighting(GSession* session, GReWeightingParams* p) throw(bad_alloc)
	: GProfileCalc("Direct Reweighting",session), Params(p), Vectors(Session->GetNbLangs()),
	  Order(0), MaxOrderSize(5000)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Vectors.InsertPtr(new InternVector(Langs(),Session->GetDic(Langs())->GetMaxId()));
	}
	Order=new GIWordWeight*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcReWeighting::GetSettings(void)
{
	return(itoa(Params->MaxNonZero));
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::SetSettings(const char* s)
{
	if(!(*s)) return;
	Params->MaxNonZero=strtoul(s,0,10);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::ComputeGlobal(GProfile* profile) throw(bad_alloc)
{
	GProfDocCursor Docs;
	GLang* CurLang;
	GDocVector* CurDoc;
	GIWordWeight* w;
	GIWordsWeights* Vec;
	tDocJudgement Fdbk;

	// Clear all containers before computing
	for(Vectors.Start();!Vectors.End();Vectors.Next())
			Vectors()->Clear();

	// Go through all documents, to compute for each documents "OK", "KO" and "N"
	// for each language as index term.
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language or its judgement is not relevant
		// -> don't treat for the profiles computing
		CurDoc=dynamic_cast<GDocVector*>(Docs()->GetDoc());
		CurLang=CurDoc->GetLang();
		if(!CurLang) continue;
		Fdbk=Docs()->GetFdbk();

		// Determine the lists corresponding to the language of the document
		// and insert a word corresponding to the documents id.
		switch(Fdbk)
		{
			case djOK:
			case djNav:
				Vec=Vectors.GetPtr<GLang*>(CurLang);
				w=Vec->GetInsertPtr<unsigned int>(Docs()->GetDoc()->GetId());
				w->AddWeight(1.0);
				break;

			case djOutScope:
				Vec=Vectors.GetPtr<GLang*>(CurLang);
				w=Vec->GetInsertPtr<unsigned int>(Docs()->GetDoc()->GetId());
				w->AddWeight(-1.0);
				break;

			default: break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight** ptr;
	unsigned int i;
	GIWordsWeights* Vector=s->GetVector();
	GIWordsWeights* Global=Vectors.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	s->RemoveRefs();
	Vector->Clear();

	// Choose the elements to stay.
	if(Global->IsEmpty()) return;

	// Put in Order an ordered version of Global
	if(Global->NbPtr+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((Global->NbPtr+1)*1.1);
		Order=new GIWordWeight*[MaxOrderSize];
	}
	memcpy(Order,Global->Tab,Global->NbPtr*sizeof(GIWordWeight*));
	qsort(static_cast<void*>(Order),Global->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
	Order[Global->NbPtr]=0;

	//If MaxNonZero is null -> take all the words.
	if(Params->MaxNonZero)
	{
		for(i=Params->MaxNonZero+1,ptr=Order;(--i)&&(*ptr);ptr++)
		{
			if((*ptr)->GetWeight()>0)
				Vector->InsertPtr(new GIWordWeight(*ptr));
		}
	}
	else
	{
		for(ptr=Order;(*ptr);ptr++)
		{
			if((*ptr)->GetWeight()>0)
				Vector->InsertPtr(new GIWordWeight(*ptr));
		}
	}

	// Update the references of the vector.
	s->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::Compute(GProfile* profile)
{
	// Compute the OK and KO lists.
	ComputeGlobal(profile);

	// Compute the vector for each subprofile
	for(profile->Start();!profile->End();profile->Next())
		ComputeSubProfile((GSubProfileVector*)((*profile)()));

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcReWeighting::~GProfileCalcReWeighting(void)
{
	if(Order) delete[] Order;
}
