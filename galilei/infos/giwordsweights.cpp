/*

	GALILEI Research Project

	GIWordsWeights.cpp

	Weights of a list of words - Implementation.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/giwordsweights.h>
#include <infos/giwordoccur.h>
#include <infos/giwordweight.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GIWordsWeights
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GIWordsWeights::GIWordsWeights(GLang *lang,unsigned int nb) throw(bad_alloc)
	: RContainer<GIWordWeight,unsigned,true,true>(nb,50), NbWordsDocs(0.0),
	  Lang(lang), Order(0)
{
}


//---------------------------------------------------------------------------
int GALILEI::GIWordsWeights::Compare(const GIWordsWeights& c) const
{
	return(Lang->Compare(c.Lang));
}


//---------------------------------------------------------------------------
int GALILEI::GIWordsWeights::Compare(const GIWordsWeights* c) const
{
	return(Lang->Compare(c->Lang));
}


//---------------------------------------------------------------------------
int GALILEI::GIWordsWeights::Compare(const GLang* l) const
{
	return(Lang->Compare(l));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordsWeights::ClassName(void) const
{
	return("GIWordsWeights");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordsWeights::InfoType(void) const
{
	return(infoWordCalcs);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::Clear(void)
{
	RContainer<GIWordWeight,unsigned,true,true>::Clear();
	NbWordsDocs=0.0;
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::Analyse(GDoc* doc)
{
	GIWordWeight* w;

	NbWordsDocs+=doc->GetV();
	for(doc->WordsStart();!doc->WordsEnd();doc->WordsNext())
	{
		w=GetInsertPtr<unsigned int>(doc->GetCurWords()->GetId());
		w->Weight+=doc->GetCurWords()->GetNbOccurs();
	}
}


//--------------------------------------------------------------------------
int GALILEI::GIWordsWeights::sortOrder(const void *a,const void *b)
{
  double af=(*((GIWordWeight**)(a)))->Weight;
  double bf=(*((GIWordWeight**)(b)))->Weight;

  if(af==bf) return(0);
  if(af>bf)
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::EndCalc(void)
{
	GIWordWeight **c;
	unsigned int i;

	// Calculate Frequences
	for(i=NbPtr+1,c=Tab;--i;c++)
		(*c)->Weight/=NbWordsDocs;

	// ReOrder by Frequence
	if(Order) delete[] Order;
	Order=new GIWordWeight*[NbPtr+1];
	if(NbPtr)
	{
		memcpy(Order,Tab,NbPtr*sizeof(GIWordWeight*));
		qsort(static_cast<void*>(Order),NbPtr,sizeof(GIWordWeight*),sortOrder);
	}
	Order[NbPtr]=0;
	CurOrder=Order;
}


//---------------------------------------------------------------------------
unsigned int GALILEI::GIWordsWeights::NextWord(void)
{
	return((*(CurOrder++))->GetId());
}


//---------------------------------------------------------------------------
GALILEI::GIWordsWeights::~GIWordsWeights(void)
{
	if(Order) delete[] Order;
}



