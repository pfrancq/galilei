/*

	GALILEI Research Project

	gdoc.cpp

	Document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoclist.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GDocList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocList::GDocList(const unsigned int nb) throw(bad_alloc)
	: RContainer<GDoc,unsigned int,false,true>(nb,nb/2)
{
}


//-----------------------------------------------------------------------------
GALILEI::GDocList::~GDocList(void)
{
}
