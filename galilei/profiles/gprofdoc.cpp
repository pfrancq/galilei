/*

	GALILEI Research Project

	GProfDoc.cpp

	Profile's Judgement of a document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gprofdoc.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GProfDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GProfDoc::GProfDoc(GDoc* doc,char fdbk)
  : Doc(doc), Fdbk(fdbk)
{
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc& profdoc)
{
  return(Doc->GetId()-profdoc.Doc->GetId());
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc* profdoc)
{
  return(Doc->GetId()-profdoc->Doc->GetId());
}



