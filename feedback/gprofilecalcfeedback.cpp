/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method  - Implementation.

	(C) 2001-2002 by P. Francq.

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <profiles/gprofilecalcfeedback.h>
#include <docs/gdoc.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <infos/giword.h>
#include <infos/giwordlist.h>
#include <infos/giwordsweights.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GProfileCalcVector::GNbDocsLangs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfileCalcFeedback::GNbDocsLangs
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
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcFeedback::GProfileCalcFeedback(GSession* session) throw(bad_alloc)
	: GProfileCalc("User Feedback",session), Vectors(Session->GetNbLangs()),
	  NbDocsWords(Session->GetNbLangs()), NbDocsLangs(Session->GetNbLangs()),
	  MaxNonZero(60), RelFactor(1.0), FuzzyFactor(1.0), AddFuzzy(false), IdfFactor(true)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Vectors.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsWords.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsLangs.InsertPtr(new GNbDocsLangs(Langs()));
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcFeedback::GetSettings(void)
{
	static char tmp[100];
	char c,c2;

	if(AddFuzzy) c='1'; else c='0';
	if(IdfFactor) c2='1'; else c2='0';
	sprintf(tmp,"%u %lf %lf %lf %c %c",MaxNonZero,RelFactor,FuzzyFactor,NoRelFactor,c,c2);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcFeedback::SetSettings(const char* s)
{
	char c,c2;

	if(!(*s)) return;
	sscanf(s,"%u %lf %lf %lf %c %c",&MaxNonZero,&RelFactor,&FuzzyFactor,&NoRelFactor,&c,&c2);
	if(c=='1') AddFuzzy=true; else AddFuzzy=false;
 	if(c2=='1') IdfFactor=true; else IdfFactor=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcFeedback::ComputeGlobal(GProfile* profile) throw(bad_alloc)
{
	GIWordWeightCursor Words;
	GProfDocCursor Docs;
	GIWordsWeights* Vector;
	GLang* CurLang;
	GDoc* CurDoc;
	GIWordWeight* w;
	bool Add;
	GIWordsWeights* NbDocs;
	tDocJudgement Fdbk;
	unsigned int NbDocsJudged;
	double MaxFreq;
	double Factor;
	double Freq;

	// Clear all containers before computing
	for(Vectors.Start();!Vectors.End();Vectors.Next())
			Vectors()->Clear();
	for(NbDocsWords.Start();!NbDocsWords.End();NbDocsWords.Next())
			NbDocsWords()->Clear();
	for(NbDocsLangs.Start();!NbDocsLangs.End();NbDocsLangs.Next())
			NbDocsLangs()->Clear();

	// Go through all documents, to compute the number of documents "OK", "KO"
	// and "N" for each language and the number of documents for each index term.
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language or its judgement is not relevant
		// -> don't treat for the profiles computing
		CurDoc=Docs()->GetDoc();
		CurLang=CurDoc->GetLang();
		if(!CurLang) continue;
		Fdbk=Docs()->GetFdbk();
		if((NoRelFactor==0.0)&&(Fdbk!=djOK)&&(Fdbk!=djNav)&&(Fdbk!=djKO)) continue;

		// Determine the lists corresponding to the language of the document
		NbDocs=NbDocsWords.GetPtr<GLang*>(CurLang);

		// Add total number of document judged for the current language
		NbDocsLangs.GetPtr<GLang*>(CurLang)->Inc();

		// Update number of documents where appear each index term.
		Words=CurDoc->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=NbDocs->GetInsertPtr<unsigned int>(Words()->GetId());
			w->AddWeight(1.0);
		}
	}

	// Go through all documents, add the frequences of the words of "OK"
	// documents and substract the frequences of the words of "KO" documents.
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language, don't treat for the profiles' computing.
		CurDoc=Docs()->GetDoc();
		CurLang=CurDoc->GetLang();
		if(!CurLang) continue;
		Vector=Vectors.GetPtr<const GLang*>(CurLang);

		// Find list in function of the feedback
		if((NoRelFactor==0.0)&&(Fdbk!=djOK)&&(Fdbk!=djNav)&&(Fdbk!=djKO)) continue;
		switch(Docs()->GetFdbk())
		{
			case djOK:
			case djNav:
				Add=true;
				Factor=RelFactor;
				break;
			
			case djKO:
				Add=false||AddFuzzy;
				Factor=FuzzyFactor;
				break;

			default:
				Add=false;
				Factor=NoRelFactor;
				break;
		}

		// Determine the lists and the general parameter corresponding to the
		// document
		NbDocs=NbDocsWords.GetPtr<GLang*>(CurLang);
		NbDocsJudged=NbDocsLangs.GetPtr<GLang*>(CurLang)->GetNb();
		Words=CurDoc->GetWordWeightCursor();
		MaxFreq=CurDoc->GetMaxWeight();

		// Add total number of words and the occurences of each word of the current document.	
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Vector->GetInsertPtr<unsigned int>(Words()->GetId());
			if(IdfFactor)
				Freq=(Words()->GetWeight()/MaxFreq)*log(NbDocsJudged/NbDocs->GetPtr<unsigned int>(Words()->GetId())->GetWeight());
			else
				Freq=Words()->GetWeight()/MaxFreq;
			if(Add)
				w->AddWeight(Factor*Freq);
			else
				w->SubstractWeight(Factor*Freq);
		}
	}

	// Sort Part
	for(Vectors.Start();!Vectors.End();Vectors.Next())
			Vectors()->Sort();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcFeedback::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight* ptr;
	GIWordsWeights* Vector=s->GetVector();
	GIWordsWeights* Global=Vectors.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	Vector->Clear();

	// Choose the elements to stay.
	if(Global->IsEmpty()) return;

	//If MaxNonZero is null -> take all the words.
	if(MaxNonZero)
	{
		for(unsigned int Nb=MaxNonZero+1;--Nb;)
		{
			if(!Global->IsNextWord()) break;
			ptr=Global->NextWord();
			Vector->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
		}
	}
	else
	{
		while(Global->IsNextWord())
		{
			ptr=Global->NextWord();
			Vector->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
		}
	}

	// Sort the vector.
	Vector->Sort();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcFeedback::Compute(GProfile* profile)
{
	// Compute the global vectors.
	ComputeGlobal(profile);

	// Compute the vector for each subprofile
	for(profile->Start();!profile->End();profile->Next())
		ComputeSubProfile((GSubProfileVector*)((*profile)()));

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcFeedback::~GProfileCalcFeedback(void)
{
}
