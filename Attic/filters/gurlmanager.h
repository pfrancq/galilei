/*

	GALILEI Research Project

	GURLManager.h

	Generic Manager to handle URL file - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GURLManagerH
#define GURLManagerH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class GSession;
class GDoc;
class GDocXML;
class GFilter;
class GMIMEFilter;


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
protected:

	/**
	* List of all mime types avalaible.
	*/
	RStd::RContainer<GMIMEFilter,unsigned int,true,true> MIMES;

	/**
	* List of all filters avalaible.
	*/
	RStd::RContainer<GFilter,unsigned int,true,true> Filters;

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
	* @returns true if the document was download;
	*/
	virtual void Download(const char* URL,RStd::RString& tmpFile) throw(GException);

	/**
	* Delete the file locally.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(RStd::RString& tmpFile) throw(GException);

public:

	/**
	* Transform a file in a GDocXML. Try to find the type of the document by
	* analysing the extension of it.
	* @param doc            Document to analyze
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(GDoc* doc) throw(GException);

	/**
	* Add a mime type and a filter.
	* @param mime           Name of the mimetype.
	* @param f              Filter.
	*/
	void AddMIME(const char* mime,GFilter* f);

	/**
	* Get the filter for a specific mime type.
	* @param mime           Name of the mimetype.
	* @return Pointer to a GMIMEFilter.
	*/
	GMIMEFilter* GetMIMEType(const char* mime) const;

	/**
	* Get the list of filters activ.
	* Return Pointer to RContainer<GFilter,unsigned int,true,false>.
	*/
	RStd::RContainer<GFilter,unsigned int,true,true>* GetFilters(void) {return(&Filters);}

	/**
	* Destructor of URL manager.
	*/
	virtual ~GURLManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
