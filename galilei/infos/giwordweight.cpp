/*

	GALILEI Research Project

	GIWordWeight.cpp

	Weight of a word - Implementation.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include<infos/giwordweight.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GIWordWeight
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::GIWordWeight(unsigned id) throw(bad_alloc)
  : GIWord(id), Weight(0.0)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordWeight::ClassName(void) const
{
	return("GIWordWeight");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordWeight::InfoType(void) const
{
	return(infoWordCalc);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordWeight::Compare(const GIWordWeight &calc) const
{
  return(Id-calc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordWeight::Compare(const GIWordWeight *calc) const
{
  return(Id-calc->Id);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::~GIWordWeight(void)
{
}
