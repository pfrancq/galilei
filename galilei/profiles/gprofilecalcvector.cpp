/*

	GALILEI Research Project

	GProfileCalcVector.h

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
#include <profiles/gprofilecalcvector.h>
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
class GALILEI::GProfileCalcVector::GNbDocsLangs
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
//  GProfileCalcVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::GProfileCalcVector(GSession* session) throw(bad_alloc)
	: GProfileCalc("Statistical",session), Vector(Session->GetNbLangs()),
	  NbDocsWords(Session->GetNbLangs()), NbDocsLangs(Session->GetNbLangs()),
	  MaxNonZero(60), IdfFactor(true)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Vector.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsWords.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsLangs.InsertPtr(new GNbDocsLangs(Langs()));
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcVector::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(IdfFactor) c='1'; else c='0';
	sprintf(tmp,"%u %c",MaxNonZero,c);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %c",&MaxNonZero,&c);
	if(c=='1') IdfFactor=true; else IdfFactor=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeGlobal(GProfile* profile) throw(bad_alloc)
{
	GIWordWeightCursor Words;
	GProfDocCursor Docs;
	GIWordsWeights* Weights;
	GIWordsWeights* NbDocs;
	GLang* CurLang;
	GDoc* CurDoc;
	GIWordWeight* w;
	GIWordWeight** v;
	GIWordWeight** d;
	unsigned int i,NbDocsJudged;
	double MaxFreq;
	tDocJudgement Fdbk;

	// Clear all containers before computing
	for(Vector.Start();!Vector.End();Vector.Next())
			Vector()->Clear();
	for(NbDocsWords.Start();!NbDocsWords.End();NbDocsWords.Next())
			NbDocsWords()->Clear();
	for(NbDocsLangs.Start();!NbDocsLangs.End();NbDocsLangs.Next())
			NbDocsLangs()->Clear();

	// Construct one big "document" from all the documents judged as "OK" or "N"
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language or its judgement is not relevant
		// or fuzzy relevant -> don't treat for the profiles computing
		CurDoc=Docs()->GetDoc();
		CurLang=CurDoc->GetLang();
		if(!CurLang) continue;
		Fdbk=Docs()->GetFdbk();
		if((Fdbk!=djOK)&&(Fdbk!=djNav)) continue;

		// Determine the lists corresponding to the language of the document
		NbDocs=NbDocsWords.GetPtr<GLang*>(CurLang);
		Weights=Vector.GetPtr<GLang*>(CurLang);

		// Add total number of document judged for the current language
		NbDocsLangs.GetPtr<GLang*>(CurLang)->Inc();

		// Add all the index terms of the document to the big one and update
		// number of documents where there appear.
		Words=CurDoc->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Weights->GetInsertPtr<unsigned int>(Words()->GetId());
			w->AddWeight(Words()->GetWeight());
			w=NbDocs->GetInsertPtr<unsigned int>(Words()->GetId());
			w->AddWeight(1.0);
		}
	}

	// Compute the weight for the big document
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		NbDocs=NbDocsWords.GetPtr<GLang*>(Vector()->GetLang());
		MaxFreq=Vector()->GetMaxWeight();
		NbDocsJudged=NbDocsLangs.GetPtr<GLang*>(Vector()->GetLang())->GetNb();
		for(i=Vector()->NbPtr+1,v=Vector()->Tab,d=NbDocs->Tab;--i;v++,d++)
		{
			if(IdfFactor)
				(*v)->SetWeight(((*v)->GetWeight()/MaxFreq)*log(NbDocsJudged/(*d)->GetWeight()));
			else
				(*v)->SetWeight((*v)->GetWeight()/MaxFreq);
		}
		Vector()->Sort();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight* ptr;
	GIWordsWeights* Desc=s->GetVector();
	GIWordsWeights* LangVector=Vector.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	s->RemoveRefs();
	Desc->Clear();

	// Verify that there was something computed
	if(LangVector->IsEmpty()) return;

	//If MaxNonZero is null -> take all the words.
	if(MaxNonZero)
	{
		for(unsigned int Nb=MaxNonZero+1;--Nb;)
		{
			if(!LangVector->IsNextWord()) break;
			ptr=LangVector->NextWord();
			Desc->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
		}
	}
	else
	{
		while(LangVector->IsNextWord())
		{
			ptr=LangVector->NextWord();
			Desc->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
		}
	}

	// Sort the vector.
	Desc->Sort();
	s->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::Compute(GProfile* profile)
{
	// Compute the Vector lists.
	ComputeGlobal(profile);

	// Compute the vector for each subprofile
	for(profile->Start();!profile->End();profile->Next())
		ComputeSubProfile((GSubProfileVector*)((*profile)()));

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::~GProfileCalcVector(void)
{
}
