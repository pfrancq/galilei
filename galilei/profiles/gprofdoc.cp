

#include <stdlib.h>

//include files for GALILEI
#include<gprofdoc.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// class GProfDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GProfDoc::GProfDoc(GDoc  *doc,char fdbk)
  : Doc(doc), Fdbk(fdbk)
{
}


//---------------------------------------------------------------------------
int GProfDoc::Compare(const GProfDoc &profdoc)
{
  return(Doc->Id-profdoc.Doc->Id);
}


//---------------------------------------------------------------------------
int GProfDoc::Compare(const GProfDoc *profdoc)
{
  return(Doc->Id-profdoc->Doc->Id);
}



