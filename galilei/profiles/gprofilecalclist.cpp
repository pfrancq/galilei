/*

	GALILEI Research Project

	GProfileCalcList.h

	"Lists" Profile Computing Method  - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gprofilecalclist.h>
#include <gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::GProfileCalcList(GSession* session,unsigned int size) throw(bad_alloc)
	: GProfileCalc(session), OK(Session->GetNbLangs()), KO(Session->GetNbLangs()),
	  Size(size)
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Session->GetLangs());

	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		OK.InsertPtr(new GIWordCalcs(CurLang(),Session->GetDic(CurLang())->GetMaxId()));
		KO.InsertPtr(new GIWordCalcs(CurLang(),Session->GetDic(CurLang())->GetMaxId()));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcList::Compute(GProfile* profile)
{
	GProfDoc *d;
	GSubProfile* s;
	unsigned int kwdid;
	GIWord *ref;
	GIWordCalcs* MOK;               // OK Docs' Stats for a given language.
	GIWordCalcs* MKO;               // KO Docs' Stats for a given language.
	GIWordList* SubOK;              // OK List of a given subprofile.
	GIWordList* SubKO;              // KO List of a given subprofile.
	GIWordList* SubCommon;          // Common List of a given subprofile.

	// Clear
	for(OK.Start();!OK.End();OK.Next())
			OK()->Clear();
	for(KO.Start();!KO.End();KO.Next())
			KO()->Clear();

	// Go through all documents judged
	for(profile->DocsStart();!profile->DocsEnd();profile->DocsNext())
	{
		d=profile->GetCurDocs();

		// If the document hasn't a language, don't treat for the profiles' computing.
		if(!d->GetDoc()->GetLang())
			continue;

		// Verify Feedback
		switch(d->GetFdbk())
		{
			case 'O':
			case 'N':
				OK.GetPtr<GLang*>(d->GetDoc()->GetLang())->Analyse(d->GetDoc());
				break;
			
			case 'K':
				KO.GetPtr<GLang*>(d->GetDoc()->GetLang())->Analyse(d->GetDoc());
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
		SubOK=s->GetOK();
		SubKO=s->GetKO();
		SubCommon=s->GetCommon();
		MOK=OK.GetPtr<GLang*>(s->GetLang());
		MKO=KO.GetPtr<GLang*>(s->GetLang());

		// Clear the lists 'OK','KO' and 'Common' of the current subprofile.
		SubOK->Clear();
		SubKO->Clear();
		SubCommon->Clear();

		// Construct the lists while:
		//  1°) The MOK and MKO lists are not both empty (IsNextWord() methods).
		//  2°) The length of both lists OK and KO are not equal to the maximal
		//      size (SubOK->NbPtr and SubKO->NbPtr values).
		while(((MOK->IsNextWord())&&(SubOK->NbPtr<Size))||((MKO->IsNextWord())&&(SubKO->NbPtr<Size)))
		{
			// Look if the OK list needs some words. If yes, look if the next
			// word to add is already in the KO list. If so, delete it from KO
			// and insert it in Common, else insert it in the OK list.
			if((MOK->IsNextWord())&&(SubOK->NbPtr<Size))
			{
				kwdid=MOK->NextWord();
				ref=s->GetKO()->GetPtr<unsigned int>(kwdid);
				if(ref)
				{
					s->GetKO()->DeletePtr(ref);
					s->GetCommon()->InsertPtr(new GIWord(kwdid));
				}
				else
					s->GetOK()->InsertPtr(new GIWord(kwdid));
			}

			// Look if the KO list needs some words. If yes, look if the next
			// word to add is already in the OK list. If so, delete it from OK
			// and insert it in Common, else insert it in the KO list.
			if((MKO->IsNextWord())&&(SubKO->NbPtr<Size))
			{
				kwdid=MKO->NextWord();
				ref=s->GetOK()->GetPtr<unsigned int>(kwdid);
				if(ref)
				{
					s->GetOK()->DeletePtr(ref);
					s->GetCommon()->InsertPtr(new GIWord(kwdid));
				}
				else
					s->GetKO()->InsertPtr(new GIWord(kwdid));
			}
		}
	}

	// Tell the profile that the udpate is finished.
	profile->UpdateFinished();
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::~GProfileCalcList(void)
{
}
