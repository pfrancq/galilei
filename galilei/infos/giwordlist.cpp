/*

	GALILEI Research Project

	GIWordList.cpp

	Represents a list of words' references - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/giwordlist.h>
#include <infos/giword.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GIWordList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordList::GIWordList(void) throw(bad_alloc)
	: RContainer<GIWord,unsigned,true,true>(30,10), Id(cNoRef)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordList::ClassName(void) const
{
	return("GWordList");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordList::InfoType(void) const
{
	return(infoWordList);
}
