/*

	GALILEI Research Project

	GProfileCalcVector.h

	Vector Computing Method  - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <profiles/gprofilecalcvector.h>
#include <docs/gdocvector.h>
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
//  GStatisticalParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GStatisticalParams::GStatisticalParams(void)
	: GCalcParams("Statistical")
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GStatisticalParams::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(IdfFactor) c='1'; else c='0';
	sprintf(tmp,"%u %c",MaxNonZero,c);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GStatisticalParams::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %c",&MaxNonZero,&c);
	if(c=='1') IdfFactor=true; else IdfFactor=false;
}



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
// InternVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfileCalcVector::InternVector : public GIWordsWeights
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
//  GProfileCalcVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::GProfileCalcVector(GSession* session, GStatisticalParams* p) throw(bad_alloc)
	: GProfileCalc("Statistical",session), Params(p), Vector(Session->GetNbLangs()),
	  NbDocsWords(Session->GetNbLangs()), NbDocsLangs(Session->GetNbLangs()), Order(0), MaxOrderSize(5000)
{
	GLangCursor Langs;


	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Vector.InsertPtr(new InternVector(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsWords.InsertPtr(new InternVector(Langs(),Session->GetDic(Langs())->GetMaxId()));
		NbDocsLangs.InsertPtr(new GNbDocsLangs(Langs()));
	}
	Order=new GIWordWeight*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
const char* GALILEI::GProfileCalcVector::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(Params->IdfFactor) c='1'; else c='0';
	sprintf(tmp,"%u %c",Params->MaxNonZero,c);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%u %c",&Params->MaxNonZero,&c);
	if(c=='1') Params->IdfFactor=true; else Params->IdfFactor=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeGlobal(GSubProfile* subprofile) throw(bad_alloc)
{
	GIWordWeightCursor Words;
	GProfDocCursor Docs;
	GIWordsWeights* Weights;
	GIWordsWeights* NbDocs;
	GLang* CurLang;
	GDocVector* CurDoc;
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

	// Determine the lists corresponding to the language of the document
	CurLang=subprofile->GetLang();
	NbDocs=NbDocsWords.GetPtr<GLang*>(CurLang);
	Weights=Vector.GetPtr<GLang*>(CurLang);

	// Construct one big "document" from all the documents judged as "OK" or "N"
	Docs=subprofile->GetProfDocCursor();

	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// If the assesment of the document is not relevant
		// or fuzzy relevant -> don't treat for the profiles computing
		CurDoc=dynamic_cast<GDocVector*>(Docs()->GetDoc());
		Fdbk=Docs()->GetFdbk();
		if((!(Fdbk & djOK))&&(!(Fdbk & djNav))) continue;

		// Add total number of document judged for the current language
		NbDocsLangs.GetPtr<GLang*>(CurLang)->Inc();

		// Add all the index terms of the document to the big one and update
		// number of documents where there appear.
		Words=CurDoc->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			w=Weights->GetInsertPtr<unsigned int>(Words()->GetId());
			if(Words()->InfoType()==infoWordList) w->SetInfoType(infoWordList);
			w->AddWeight(Words()->GetWeight());
			w=NbDocs->GetInsertPtr<unsigned int>(Words()->GetId());
			if(Words()->InfoType()==infoWordList) w->SetInfoType(infoWordList);
			w->AddWeight(1.0);
		}
	}


	// Compute the weight for the big document
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		NbDocs=NbDocsWords.GetPtr<GLang*>(Vector()->Lang);
		MaxFreq=Vector()->GetMaxWeight();
		NbDocsJudged=NbDocsLangs.GetPtr<GLang*>(Vector()->Lang)->GetNb();
		for(i=Vector()->NbPtr+1,v=Vector()->Tab,d=NbDocs->Tab;--i;v++,d++)
		{
			if(Params->IdfFactor)
				(*v)->SetWeight(((*v)->GetWeight()/MaxFreq)*log(NbDocsJudged/(*d)->GetWeight()));
			else
				(*v)->SetWeight((*v)->GetWeight()/MaxFreq);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc)
{
	GIWordWeight** ptr;
	unsigned int i;
	GIWordsWeights* Desc=s->GetVector();
	GIWordsWeights* LangVector=Vector.GetPtr<GLang*>(s->GetLang());

	// Clear the Vector.
	s->RemoveRefs();
	Desc->Clear();

	// Verify that there was something computed
	if(LangVector->IsEmpty()) return;

	// Put in Order an ordered version of LangVector
	if(LangVector->NbPtr+1>MaxOrderSize)
	{
		if(Order) delete[] Order;
		MaxOrderSize=static_cast<unsigned int>((LangVector->NbPtr+1)*1.1);
		Order=new GIWordWeight*[MaxOrderSize];
	}
	memcpy(Order,LangVector->Tab,LangVector->NbPtr*sizeof(GIWordWeight*));
	qsort(static_cast<void*>(Order),LangVector->NbPtr,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
	Order[LangVector->NbPtr]=0;

	//If MaxNonZero is null -> take all the words.
	if(Params->MaxNonZero)
	{
		for(i=Params->MaxNonZero+1,ptr=Order;(--i)&&(*ptr);ptr++)
		{
			if((*ptr)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight(*ptr));
		}
	}
	else
	{
		for(ptr=Order;(*ptr);ptr++)
		{
			if((*ptr)->GetWeight()>0)
				Desc->InsertPtr(new GIWordWeight(*ptr));
		}
	}

	// Update the references of the vector.
	s->UpdateRefs();

	//Tell the subprofile that the udpate is finished.
	s->UpdateFinished();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcVector::Compute(GSubProfile* subprofile)
{
	// Compute the global Vector lists.
	ComputeGlobal(subprofile);

	// Compute the vector for the subprofile
	ComputeSubProfile((GSubProfileVector*)subprofile);

}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcVector::~GProfileCalcVector(void)
{
	if(Order) delete[] Order;
}
