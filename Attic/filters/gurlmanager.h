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
#include <gdocs/gdoc.h>
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
	*/
	GURLManager(void);

protected:

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

public:

	/**
	* Transform a file in a GDocXML. Try to find the type of the document by
	* analysing the extension of it.
	* @param doc            Document to analyze
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(const GDoc* doc);

	/**
	* Add a mime type and a filter.
	* @param mime           Name of the mimetype.
	* @param f              Filter.
	*/
	void AddMIME(const char* mime,GFilter* f);

	/**
	* @param mime           Name of the mimetype.
	*/
	GMIMEFilter* GetMIMEType(const char* mime) const;

	/**
	* Destructor of URL manager.
	*/
	virtual ~GURLManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
