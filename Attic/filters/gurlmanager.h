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
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class GSession;


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
	/**
	* Corresponding Session.
	*/
	GSession* Session;

	/**
	* List of all mime types avalaible.
	*/
	RContainer<GMIMEFilter,unsigned int,true,true> MIMES;

	/**
	* List of all filters avalaible.
	*/
	RContainer<GFilter,unsigned int,true,false> Filters;

public:

	/**
	* Construct a URL manager.
	* @param session        Session.
	*/
	GURLManager(GSession* session);

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
	* Transform a file in a GDocXML. Try to find the type of the document by
	* analysing the extension of it.
	* @param URL            URL of the document.
	* @param mime           MIME-Type of the document.
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(const char* URL,const char* mime);

	/**
	* Add a mime type and a filter.
	* @param ext            Extension.
	* @param f              Filter.
	*/
	void AddMIME(const RString& ext,GFilter* f);

	/**
	* Destructor of URL manager.
	*/
	virtual ~GURLManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
