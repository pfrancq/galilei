/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GFilterH
#define GFilterH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdocxml.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a document to filter in a XML
* structure.
* @author Pascal Francq
*/
class GFilter
{	
protected:
	
	/**
	* URL of the document to filter.
	*/
	RString URL;

public:

	/**
	* Construct the filter for a specific document.
	* @param url            URL of the document.
	*/
	GFilter(const RString& url);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content. This method must be reimplemented in the child
	* classes.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc)=0;

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
