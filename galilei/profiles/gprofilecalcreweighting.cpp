/*

	GALILEI Research Project

	GProfileCalcReWeighting.h

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
#include <profiles/gprofilecalcreweighting.h>
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
//  GProfileCalcReWeighting
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcReWeighting::GProfileCalcReWeighting(GSession* session) throw(bad_alloc)
	: GProfileCalc("Direct Reweighting",session), OK(Session->GetNbLangs()), KO(Session->GetNbLangs()),
	  MaxNonZero(60)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		OK.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
		KO.InsertPtr(new GIWordsWeights(Langs(),Session->GetDic(Langs())->GetMaxId()));
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcReWeighting::GetSettings(void)
{
	return(itoa(MaxNonZero));
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::SetSettings(const char* s)
{
	if(!(*s)) return;
	MaxNonZero=strtoul(s,0,10);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::ComputeOKKO(GProfile* profile) throw(bad_alloc)
{
	GProfDocCursor Docs;

	// Clear
	for(OK.Start();!OK.End();OK.Next())
			OK()->Clear();
	for(KO.Start();!KO.End();KO.Next())
			KO()->Clear();

	// Go through all documents judged
	Docs=profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the document hasn't a language, don't treat for the profiles' computing.
		if(!Docs()->GetDoc()->GetLang())
			continue;

		// Verify Feedback
		switch(Docs()->GetFdbk())
		{
			case 'O':
			case 'N':
//				OK.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
			
			case 'K':
//				KO.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight* ptr;
	GIWordsWeights* Vector=s->GetVector();
	GIWordsWeights* MOK=OK.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	Vector->Clear();

	// Choose the elements to stay.
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
void GALILEI::GProfileCalcReWeighting::Compute(GProfile* profile)
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
GALILEI::GProfileCalcReWeighting::~GProfileCalcReWeighting(void)
{
}
