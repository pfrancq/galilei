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
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <sessions/gsession.h>
using namespace GALILEI;



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
GALILEI::GProfileCalcReWeighting::GProfileCalcReWeighting(GSession* session) throw(bad_alloc)
	: GProfileCalc("Direct Reweighting",session), OK(Session->GetNbLangs()), KO(Session->GetNbLangs()),
	  Order(0), MaxOrderSize(5000), MaxNonZero(60)
{
	GLangCursor Langs;

	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		OK.InsertPtr(new InternVector(Langs(),Session->GetDic(Langs())->GetMaxId()));
		KO.InsertPtr(new InternVector(Langs(),Session->GetDic(Langs())->GetMaxId()));
	}
	Order=new GIWordWeight*[MaxOrderSize];
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
			case djOK:
			case djNav:
//				OK.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
			
			case djKO:
//				KO.GetPtr<GLang*>(Docs()->GetDoc()->GetLang())->Analyse(Docs()->GetDoc());
				break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcReWeighting::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight** ptr;
	unsigned int i;
	GIWordsWeights* Vector=s->GetVector();
	GIWordsWeights* MOK=OK.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	s->RemoveRefs();
	Vector->Clear();

	// Put in Order an ordered version of MOK
	if(MOK->NbPtr>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((MOK->NbPtr+1)*1.1);
		Order=new GIWordWeight*[MaxOrderSize];
	}
	memcpy(Order,MOK->Tab,MOK->NbPtr*sizeof(GIWordWeight*));
	qsort(static_cast<void*>(Order),MOK->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
	Order[MOK->NbPtr]=0;

	//If MaxNonZero is null -> take all the words.
	if(MaxNonZero)
	{
		for(i=MaxNonZero+1,ptr=Order;(--i)&&(*ptr);ptr++)
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
	if(Order) delete[] Order;
}
