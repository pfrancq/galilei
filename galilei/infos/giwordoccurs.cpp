/*

	GALILEI Research Project

	giwordoccurs.cpp
	
	Container of word occurencies - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for Galilei
#include <ginfos/giwordoccurs.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GIWordOccurs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordOccurs::GIWordOccurs (unsigned nb) throw(bad_alloc)
	: RContainer<GIWordOccur,unsigned,true,true>(nb+nb/2,nb/2)

{
}


//-----------------------------------------------------------------------------
GIWordOccur* GALILEI::GIWordOccurs::GetPtr(unsigned id) throw(bad_alloc)
{
	return(GetInsertPtr<unsigned>(id));
}
