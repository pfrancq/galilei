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
* @author Pascal Francq
*/
class GDocXML : public RXML::RXMLStruct
{
	/**
	* url of the document.
	*/
	RStd::RString URL;

	/**
	* file containg the document (must be local).
	*/
	RStd::RString FileName;

public:

	/**
	* Construct a XML document.
	* @param url            URL of the document.
	* @param filename       File of the document.
	*/
	GDocXML(const char* url,const char* filename) throw(bad_alloc);

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	const char* GetURL(void) const {return(URL());}

	/**
	* Get the File Name.
	* @returns Pointer to a C string.
	*/
	const char* GetFile(void) const {return(FileName());}

	/**
	* @returns A pointer to the tag containing the content of the corresponding
	*          document.
	*/
	RXMLTag* GetContent(void) {return(GetTag("Content"));}

	/**
	* Destruct the document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
