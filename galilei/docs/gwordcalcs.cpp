
#include <stdlib.h>

//include files for GALILEI
#include<gwordcalcs.h>

using namespace GALILEI;


/---------------------------------------------------------------------------
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

	NbWordsDocs+=doc->NbWords;			
	for(doc->Words->Start();!doc->Words->End();doc->Words->Next())
	{
		w=GetInsertPtr<unsigned int>((*doc->Words)()->Id);
		w->Av+=(*doc->Words)()->Occur;
	}
}


//--------------------------------------------------------------------------
int GWordCalcs::sortOrder(const void *a,const void *b)
{
  double af=(*((HWordCalc**)(a)))->Av;
  double bf=(*((HWordCalc**)(b)))->Av;

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
	return((*(CurOrder++))->Id);
}


//---------------------------------------------------------------------------
GWordCalcs::~GWordCalcs(void)
{
	if(Order) delete[] Order;
}



