
#ifndef GWordCalcH
#define GWordCalcH
//-------------------------------
//include files for GALILEI
#include <gdocs/gwordref.h>
using namespace GALILEI;

 namespace GALILEI{
 //-------------------------------
class GWordCalc : public GWordRef
{
public:
  double Av;

  GWordCalc(unsigned id) throw(bad_alloc);
  int Compare(const GWordCalc &calc);
  int Compare(const GWordCalc *calc);
  ~GWordCalc(void);
};
 }
 #endif


