

#include<stdlib.h>


//include files for GALILEI
#include<gwordcalc.h>

using namespace GALILEI;



//---------------------------------------------------------------------------
//
//  GWordCalc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordCalc::GWordCalc(unsigned id) throw(bad_alloc)
  : GWordRef(id),Av(0.0)
{
}


//---------------------------------------------------------------------------
int GWordCalc::Compare(const GWordCalc &calc)
{
  return(Id-calc.Id);
}


//---------------------------------------------------------------------------
int GWordCalc::Compare(const GWordCalc *calc)
{
  return(Id-calc->Id);
}


//---------------------------------------------------------------------------
GWordCalc::~GWordCalc(void)
{
}


