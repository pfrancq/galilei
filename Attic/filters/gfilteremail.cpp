/*

	GALILEI Research Project

	GDocXML.h

	A XML representation of a document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilteremail.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(const RString& url)
	: GFilter(url)
{
}


//-----------------------------------------------------------------------------
bool GFilterEMail::Analyze(GDocXML* /*doc*/)
{
	return(true);
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
}
