/*

	GALILEI Research Project

	GProfDoc.cpp

	Profile's Judgement of a document - Implementation.

	(C) 2001-2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <profiles/gprofile.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GProfDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GProfDoc::GProfDoc(GDoc* doc,GProfile* prof,char fdbk,const char* date)
  : Doc(doc), Profile(prof), Fdbk(fdbk), Updated(date)
{
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc& profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc.Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc.Profile->GetId());
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc* profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc->Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc->Profile->GetId());
}
