
#ifdef GProfDocH
#define GProfDocH
//---------------------------------------------------------------------------
//include file for GALILEI
#include <gdocs/gdoc.h>
using namespace GALILEI;

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GProfDoc
class GProfDoc
{
public:
  GDoc *Doc;
  char Fdbk;

  GProfDoc(GDoc *doc,char fdbk);
  int Compare(const GProfDoc &profdoc);
  int Compare(const GProfDoc *profdoc);
};
