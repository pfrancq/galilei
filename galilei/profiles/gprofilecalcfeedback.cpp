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
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcFeedback::GProfileCalcFeedback(GSession* session) throw(bad_alloc)
	: GProfileCalc("User Feedback",session), Vectors(Session->GetNbLangs()),
	  MaxNonZero(60), Beta(1.0), Gamma(1.0), AddKO(false)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Vectors.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcFeedback::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(AddKO) c='1'; else c='0';
	sprintf(tmp,"%u %lf %lf %c",MaxNonZero,Beta,Gamma,c);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcFeedback::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %lf %lf %c",&MaxNonZero,&Beta,&Gamma,&c);
	if(c=='1') AddKO=true; else AddKO=false;
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
	bool Cont;
	double BNbWords;
	double GNbWords;

	// Init Part
	for(Vectors.Start();!Vectors.End();Vectors.Next())
			Vectors()->Clear();

	// Go through all documents, add the frequences of the words of "OK"
	// documents and substract the frequences of the words of "KO" documents.
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language, don't treat for the profiles' computing.
		CurDoc=Docs()->GetDoc();
		CurLang=CurDoc->GetLang();
		if(!CurLang)
			continue;
		Cont=true;
		Vector=Vectors.GetPtr<const GLang*>(CurLang);

		// Find list in function of the feedback
		switch(Docs()->GetFdbk())
		{
			case 'O':
			case 'N':
				Add=true;
				break;
			
			case 'K':
				Add=false;
				break;

			default:
				Cont=false;
		}
		if(!Cont) continue;

 		// Add total number of words and the occurences of each word of the current document.
		Words=CurDoc->GetWordWeightCursor();
		BNbWords=Beta/CurDoc->GetV();
		GNbWords=Gamma/CurDoc->GetV();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Vector->GetInsertPtr<unsigned int>(Words()->GetId());
			if(Add||AddKO)
				w->AddWeight(BNbWords*Words()->GetWeight());
			else
				w->SubstractWeight(GNbWords*Words()->GetWeight());
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
	for(unsigned int Nb=MaxNonZero+1;--Nb;)
	{
		if(!Global->IsNextWord()) break;
		ptr=Global->NextWord();
		Vector->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
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
