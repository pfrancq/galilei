/*

	GALILEI Research Project

	gwordcalcs.cpp

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

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
#include <stdlib.h>

//include files for GALILEI
#include <gdocs/gwordcalcs.h>
#include <gsessions/gsession.h>
using namespace GALILEI;


//---------------------------------------------------------------------------
//
//  GWordCalcs
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordCalcs::GWordCalcs(GLang *lang,GSession *session) throw(bad_alloc)
	: RContainer<GWordCalc,unsigned,true,true>(session->GetDic(lang)->MaxId,50),
	  NbWordsDocs(0.0), Order(0)
{
}


//---------------------------------------------------------------------------
void GWordCalcs::Analyse(GDoc *doc)
{
	GWordCalc *w;

	NbWordsDocs+=doc->GetNbWords();
	for(doc->Words.Start();!doc->Words.End();doc->Words.Next())
	{
		w=GetInsertPtr<unsigned int>(doc->Words()->GetId());
		w->Av+=doc->Words()->GetNbOccurs();
	}
}


//--------------------------------------------------------------------------
int GWordCalcs::sortOrder(const void *a,const void *b)
{
  double af=(*((GWordCalc**)(a)))->Av;
  double bf=(*((GWordCalc**)(b)))->Av;

  if(af==bf) return(0);
  if(af>bf)
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
void GWordCalcs::EndCalc(void)
{
	GWordCalc **c;
   unsigned int i;

	// Calculate Frequences
	for(i=NbPtr+1,c=Tab;--i;c++)
		(*c)->Av/=NbWordsDocs;

	// ReOrder by Frequence
	if(Order) delete[] Order;
	Order=new GWordCalc*[NbPtr];
	memcpy(Order,Tab,NbPtr*sizeof(GWordCalc*));
	qsort(static_cast<void*>(Order),NbPtr,sizeof(GWordCalc*),sortOrder);
	CurOrder=Order;
}


//---------------------------------------------------------------------------
unsigned int GWordCalcs::NextWord(void)
{
	return((*(CurOrder++))->GetId());
}


//---------------------------------------------------------------------------
GWordCalcs::~GWordCalcs(void)
{
	if(Order) delete[] Order;
}



