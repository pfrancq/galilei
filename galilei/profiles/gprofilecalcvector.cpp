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
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfileCalcVector::GNbWordsDocs
{
public:
	GLang* Lang;
	double NbWords;

	GNbWordsDocs(GLang* l) : Lang(l), NbWords(0.0) {}
	void Clear(void) {NbWords=0.0;}
	int Compare(const GNbWordsDocs* p) const {return(Lang->Compare(p->Lang));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	void AddNb(unsigned int n) {NbWords+=n;}
	double GetNb(void) const {return(NbWords);}
};


//-----------------------------------------------------------------------------
//
//  GProfileCalcVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::GProfileCalcVector(GSession* session) throw(bad_alloc)
	: GProfileCalc("Statistical",session), OK(Session->GetNbLangs()), KO(Session->GetNbLangs()),
	  NbWords(0), MaxNonZero(60)
{
	GLangCursor Langs;

	NbWords=new RStd::RContainer<GNbWordsDocs,unsigned int,true,true>(Session->GetNbLangs());
	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		OK.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		KO.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbWords->InsertPtr(new GNbWordsDocs(Langs()));
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcVector::GetSettings(void)
{
	return(itoa(MaxNonZero));
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::SetSettings(const char* s)
{
	if(!(*s)) return;
	MaxNonZero=strtoul(s,0,10);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeOKKO(GProfile* profile) throw(bad_alloc)
{
	GIWordOccurCursor Words;
	GProfDocCursor Docs;
	GIWordsWeights* Weights;
	GNbWordsDocs* NbW;
	GLang* CurLang;
	GDoc* CurDoc;
	GIWordWeight* w;
	GIWordWeight **c;
	unsigned int i;

	// Init Part
	for(OK.Start();!OK.End();OK.Next())
			OK()->Clear();
	for(KO.Start();!KO.End();KO.Next())
			KO()->Clear();
	for(NbWords->Start();!NbWords->End();NbWords->Next())
		(*NbWords)()->Clear();

	// Construct two big "documents": one with all the documents judged as "OK"
	// and one with all the documents judged as "KO".
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language, don't treat for the profiles' computing.
		CurDoc=Docs()->GetDoc();
		CurLang=CurDoc->GetLang();
		if(!CurLang)
			continue;
		NbW=NbWords->GetPtr<GLang*>(CurLang);

		// Find list in function of the feedback
		switch(Docs()->GetFdbk())
		{
			case 'O':
			case 'N':
				Weights=OK.GetPtr<GLang*>(CurLang);
				break;
			
			case 'K':
				Weights=KO.GetPtr<GLang*>(CurLang);
				break;

			default:
				Weights=0;
		}
		if(!Weights) continue;

 		// Add total number of words and the occurences of each word of the current document.
		Words=CurDoc->GetWordOccurCursor();
		NbW->AddNb(CurDoc->GetV());
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Weights->GetInsertPtr<unsigned int>(Words()->GetId());
			w->AddWeight(Words()->GetNbOccurs());
		}
	}

	// Calculate Frequences
	for(OK.Start();!OK.End();OK.Next())
	{
		NbW=NbWords->GetPtr<GLang*>(OK()->GetLang());
		for(i=OK()->NbPtr+1,c=OK()->Tab;--i;c++)
			(*c)->SetWeight((*c)->GetWeight()/NbW->GetNb());
		OK()->Sort();
	}
	for(KO.Start();!KO.End();KO.Next())
	{
		NbW=NbWords->GetPtr<GLang*>(KO()->GetLang());
		for(i=KO()->NbPtr+1,c=KO()->Tab;--i;c++)
			(*c)->SetWeight((*c)->GetWeight()/NbW->GetNb());
		KO()->Sort();
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight* ptr;
	GIWordsWeights* Vector=s->GetVector();
	GIWordsWeights* MOK=OK.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	Vector->Clear();

	// Choose the elements to stay.
	if(MOK->IsEmpty()) return;
	for(unsigned int Nb=MaxNonZero+1;--Nb;)
	{
		if(!MOK->IsNextWord()) break;
		ptr=MOK->NextWord();
		Vector->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
	}

	// Sort the vector.
	Vector->Sort();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::Compute(GProfile* profile)
{
	// Compute the OK and KO lists.
	ComputeOKKO(profile);

	// Compute the vector for each subprofile
	for(profile->Start();!profile->End();profile->Next())
		ComputeSubProfile((GSubProfileVector*)((*profile)()));

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::~GProfileCalcVector(void)
{
	if(NbWords)
		delete NbWords;
}
