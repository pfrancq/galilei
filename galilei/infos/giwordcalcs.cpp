/*

	GALILEI Research Project

	GIWordcalcs.cpp

	Frequences of words appearing in a set a documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giwordcalcs.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GIWordCalcs
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GIWordCalcs::GIWordCalcs(GLang *lang,unsigned int nb) throw(bad_alloc)
	: RContainer<GIWordCalc,unsigned,true,true>(nb,50), NbWordsDocs(0.0), Order(0)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordCalcs::ClassName(void) const
{
	return("GIWordCalcs");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordCalcs::InfoType(void) const
{
	return(infoWordCalcs);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordCalcs::Analyse(GDoc *doc)
{
	GIWordCalc* w;

	NbWordsDocs+=doc->GetNbWords();
	for(doc->WordsStart();!doc->WordsEnd();doc->WordsNext())
	{
		w=GetInsertPtr<unsigned int>(doc->GetCurWords()->GetId());
		w->Freq+=doc->GetCurWords()->GetNbOccurs();
	}
}


//--------------------------------------------------------------------------
int GALILEI::GIWordCalcs::sortOrder(const void *a,const void *b)
{
  double af=(*((GIWordCalc**)(a)))->Freq;
  double bf=(*((GIWordCalc**)(b)))->Freq;

  if(af==bf) return(0);
  if(af>bf)
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
void GALILEI::GIWordCalcs::EndCalc(void)
{
	GIWordCalc **c;
	unsigned int i;

	// Calculate Frequences
	for(i=NbPtr+1,c=Tab;--i;c++)
		(*c)->Freq/=NbWordsDocs;

	// ReOrder by Frequence
	if(Order) delete[] Order;
	Order=new GIWordCalc*[NbPtr+1];
	memcpy(Order,Tab,NbPtr*sizeof(GIWordCalc*));
	Order[NbPtr]=0;
	qsort(static_cast<void*>(Order),NbPtr,sizeof(GIWordCalc*),sortOrder);
	CurOrder=Order;
}


//---------------------------------------------------------------------------
unsigned int GALILEI::GIWordCalcs::NextWord(void)
{
	return((*(CurOrder++))->GetId());
}


//---------------------------------------------------------------------------
GALILEI::GIWordCalcs::~GIWordCalcs(void)
{
	if(Order) delete[] Order;
}



