/*

	GALILEI Research Project

	GProfileCalcList.h

	"Lists" Profile Computing Method  - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



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
GALILEI::GProfileCalcList::GProfileCalcList(GSession* session) throw(bad_alloc)
	: GProfileCalc(session), OK(Session->Langs->NbPtr), KO(Session->Langs->NbPtr)
{
	GLang* Lang;

	// Init Part
	for(Session->Langs->Start();!Session->Langs->End();Session->Langs->Next())
	{
		Lang=(*Session->Langs)();
		OK.InsertPtr(new GIWordCalcs(Lang,Session->GetDic(Lang)->GetMaxId()));
		KO.InsertPtr(new GIWordCalcs(Lang,Session->GetDic(Lang)->GetMaxId()));
	}
}

//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcList::Compute(GProfile* profile)
{
	GProfDoc *d;

	for(profile->DocsStart();!profile->DocsEnd();profile->DocsNext())
	{
		d=profile->GetCurDocs();
		switch(d->GetFdbk())
		{
			case 'O':
			case 'N':
				OK.GetPtr<GLang*>(d->GetDoc()->GetLang());
				break;
			
			case 'K':
				KO.GetPtr<GLang*>(d->GetDoc()->GetLang());
				break;
		}
	}

}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::~GProfileCalcList(void)
{
}
