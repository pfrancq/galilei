/*

	GALILEI Research Project

	GProfileCalcList.h

	"Lists" Profile Computing Method  - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gprofilecalclist.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::GProfileCalcList(GSession* session) throw(bad_alloc)
	: GProfileCalc(session), OK(0), KO(0)
{
}

//-----------------------------------------------------------------------------
void GALILEI::GProfileCalcList::Compute(GProfile* profile)
{
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalcList::~GProfileCalcList(void)
{
}
