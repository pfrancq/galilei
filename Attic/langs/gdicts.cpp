/*

	GALILEI Research Project

	GDicts.cpp

	List of dictionnaries - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <langs/gdicts.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GDicts
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GDicts::GDicts(unsigned nb) throw(bad_alloc)
  : RContainer<GDict,unsigned,true,true>(nb,nb/2)
{
}


//---------------------------------------------------------------------------
void GALILEI::GDicts::Clear(void)
{
  unsigned i;
  GDict **ptr;

  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
    (*ptr)->Clear();
}


//---------------------------------------------------------------------------
GALILEI::GDicts::~GDicts(void)
{
}
