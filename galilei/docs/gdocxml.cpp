/*

	GALILEI Research Project

	GDocXML.cpp

	A XML representation of a document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdocxml.h>
using namespace GALILEI;
using namespace RXML;


//-----------------------------------------------------------------------------
//
// class GDocXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocXML::GDocXML(const RString& url) throw(bad_alloc)
	: RXMLStruct(), URL(url)
{
}


//---------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}




