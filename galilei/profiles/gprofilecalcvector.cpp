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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <profiles/gprofilecalcvector.h>
#include <docs/gdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gprofoptions.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledescvector.h>
#include <profiles/gprofdoc.h>
#include <infos/giword.h>
#include <infos/giwordlist.h>
#include <infos/giwordsweights.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::GProfileCalcVector(GSession* session) throw(bad_alloc)
	: GProfileCalc(session), OK(Session->GetNbLangs()), KO(Session->GetNbLangs())
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Session->GetLangs());

	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		OK.InsertPtr(new GIWordsWeights(CurLang(),Session->GetDic(CurLang())->GetMaxId()));
		KO.InsertPtr(new GIWordsWeights(CurLang(),Session->GetDic(CurLang())->GetMaxId()));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::Compute(GProfile* profile)
{
	GSubProfile* s;
	GIWordsWeights* MOK;               // OK Docs Stats for a given language.
	GIWordsWeights* MKO;               // KO Docs Stats for a given language.
	GIWordWeight* ptr;
	GProfDocCursor Docs;
	GIWordsWeights* Vector;

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
				OK.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
			
			case 'K':
				KO.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
		}
	}

	// Compute the frequences
	for(OK.Start();!OK.End();OK.Next())
			OK()->EndCalc();
	for(KO.Start();!KO.End();KO.Next())
			KO()->EndCalc();
	
	// Compute list 'OK', 'KO', 'Common' for each subprofile
	for(profile->Start();!profile->End();profile->Next())
	{
		// Init Local variables
		s=(*profile)();
		Vector=((GSubProfileDescVector*)s->GetPtr<const tSubProfileDesc>(sdVector))->GetVector();
		MOK=OK.GetPtr<GLang*>(s->GetLang());
		MKO=KO.GetPtr<GLang*>(s->GetLang());

		// Construct the vector
		Vector->Clear();
		for(unsigned int Nb=Session->GetProfOptions()->ListSize+1;--Nb;)
		{
			if(!MOK->IsNextWord()) break;
			ptr=MOK->NextWord();
			Vector->InsertPtr(new GIWordWeight(ptr->GetId(),ptr->GetWeight()));
		}
		Vector->Sort();
	}

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::~GProfileCalcVector(void)
{
}
