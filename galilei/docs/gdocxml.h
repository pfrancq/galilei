/*

	GALILEI Research Project

	GFilterEMail.h

	Filter's document for an email - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocXMLH
#define GDocXMLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxml/rxmlstruct.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDocXML class provides a XML representation for the content of a given
* document.
*/
class GDocXML : public RXML::RXMLStruct
{
	/**
	* url of the document.
	*/
	RString URL;

public:

	/**
	* Construct a XML document.
	* @param url            URL of the document.
	*/
	GDocXML(const RString& url) throw(bad_alloc);

	/**
	* Destruct the document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
