/*

	GALILEI Research Project

	GURLManager.h

	Generic Manager to handle URL file - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GURLManagerH
#define GURLManagerH


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
* The GURLManager class provides a representation for a manager responsible to
* transform generic URL into a local file. If needed, the file has to be
* download and store locally.
* @author Pascal Francq
* @short Generic URL Manager.
*/
class GURLManager
{
public:

	/**
	* Construct a URL manager.
	*/
	GURLManager(void);

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	* @return true if the document could be downloaded.
	*/
	virtual bool Download(const char* URL,RString& tmpFile);

	/**
	* Delete the file locally.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(RString& tmpFile);

	/**
	* Transform a file of a given MIME type in a GDocXML.
	* @param URL            URL of the document.
	* @param mime           Mime type of document.
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(const char* URL,const char* mime);

	/**
	* Transform a file in a GDocXML. Try to find the type of the document by
	* analysing the extension of it.
	* @param URL            URL of the document.
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(const char* URL);

	/**
	* Destructor of URL manager.
	*/
	virtual ~GURLManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
