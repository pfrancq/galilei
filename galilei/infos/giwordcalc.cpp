/*

	GALILEI Research Project

	GIWordCalc.cpp

	Frequence of a word in a set of documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include<ginfos/giwordcalc.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GIWordCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordCalc::GIWordCalc(unsigned id) throw(bad_alloc)
  : GIWord(id), Freq(0.0)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordCalc::ClassName(void) const
{
	return("GIWordCalc");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordCalc::InfoType(void) const
{
	return(infoWordCalc);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordCalc::Compare(const GIWordCalc &calc) const
{
  return(Id-calc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordCalc::Compare(const GIWordCalc *calc) const
{
  return(Id-calc->Id);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordCalc::~GIWordCalc(void)
{
}
