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
#include <infos/giwordoccurs.h>
#include <infos/giwordoccur.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GIWordOccurs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordOccurs::GIWordOccurs (unsigned nb) throw(bad_alloc)
	: GInfo(),RContainer<GIWordOccur,unsigned,true,true>(nb+nb/2,nb/2)

{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordOccurs::ClassName(void) const
{
	return("GIWordOccurs");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordOccurs::InfoType(void) const
{
	return(infoWordOccurs);
}


//-----------------------------------------------------------------------------
GIWordOccur* GALILEI::GIWordOccurs::GetPtr(unsigned id) throw(bad_alloc)
{
	return(GetInsertPtr<unsigned>(id));
}
