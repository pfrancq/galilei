/*

	GALILEI Research Project

	GDocs.cpp

	List of documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/




//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdocs.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocs::GDocs(unsigned int nb,GSession* session) throw(bad_alloc)
	: RContainer<GDoc,unsigned,true,false>(nb+(nb/2),nb/2),Session(session)
{
}


//-----------------------------------------------------------------------------
GALILEI::GDocs::~GDocs(void)
{
}
