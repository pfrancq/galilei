/*

	GALILEI Research Project

	GIWordsWeights.cpp

	Weights of a list of words - Implementation.

	(C) 2002 by P. Francq.

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
	GIWordOccurCursor Words=doc->GetWordOccurCursor();

	NbWordsDocs+=doc->GetV();
	for(Words.Start();!Words.End();Words.Next())
	{
		w=GetInsertPtr<unsigned int>(Words()->GetId());
		w->Weight+=Words()->GetNbOccurs();
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
void GALILEI::GIWordsWeights::Sort(void)
{
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
void GALILEI::GIWordsWeights::EndCalc(void)
{
	GIWordWeight **c;
	unsigned int i;

	// Calculate Frequences
	for(i=NbPtr+1,c=Tab;--i;c++)
		(*c)->Weight/=NbWordsDocs;

	Sort();
}


//---------------------------------------------------------------------------
void GALILEI::GIWordsWeights::InitWord(void)
{
	CurOrder=Order;
}


//---------------------------------------------------------------------------
GIWordWeight* GALILEI::GIWordsWeights::NextWord(void)
{
	if(*CurOrder)
		return(*(CurOrder++));
	else
		return(0);
}


//---------------------------------------------------------------------------
GALILEI::GIWordsWeights::~GIWordsWeights(void)
{
	if(Order) delete[] Order;
}



