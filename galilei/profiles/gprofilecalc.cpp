/*

	GALILEI Research Project

	GProfileCalc.cpp

	Generic Profile' Computing Method - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gprofilecalc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalc::GProfileCalc(GSession* session) throw(bad_alloc)
	: Session(session)
{
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalc::~GProfileCalc(void)
{
}
