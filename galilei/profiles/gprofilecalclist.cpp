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
	unsigned int NbKO;
	unsigned int NbOK;
	GIWordCalcs* MOK;
	GIWordCalcs* MKO;
	
	// Clear
	for(OK.Start();!OK.End();OK.Next())
			OK()->Clear();
	for(KO.Start();!KO.End();KO.Next())
			KO()->Clear();

	// Go through all documents judged
	for(profile->DocsStart();!profile->DocsEnd();profile->DocsNext())
	{
		d=profile->GetCurDocs();
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
		s=(*profile)();
		MOK=OK.GetPtr<GLang*>(s->GetLang());
		MKO=KO.GetPtr<GLang*>(s->GetLang());
		NbOK=0;
		NbKO=0;
		while(((NbOK<Size)&&(NbOK<OK.NbPtr))||((NbKO<Size)&&(NbOK<KO.NbPtr)))
		{
			if((NbOK<Size)&&(NbOK<OK.NbPtr))
			{
				kwdid=MOK->NextWord();
				ref=s->GetKO()->GetPtr<unsigned int>(kwdid);
				if(ref)
				{
					s->GetKO()->DeletePtr(ref);
					NbKO--;
					s->GetCommon()->InsertPtr(new GIWord(kwdid));
				}
				else
				{
					s->GetOK()->InsertPtr(new GIWord(kwdid));
					NbOK++;
				}
			}
			if((NbKO<Size)&&(NbOK<KO.NbPtr))
			{
				kwdid=MKO->NextWord();
				ref=s->GetOK()->GetPtr<unsigned int>(kwdid);
				if(ref)
				{
					s->GetOK()->DeletePtr(ref);
					NbOK--;
					s->GetCommon()->InsertPtr(new GIWord(kwdid));
				}
				else
				{
					s->GetKO()->InsertPtr(new GIWord(kwdid));
					NbKO++;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::~GProfileCalcList(void)
{
}
