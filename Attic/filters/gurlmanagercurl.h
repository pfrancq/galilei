/*

	GALILEI Research Project

	GURLManagerCURL.h

	Manager to handle URL file using the CURL library - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GURLManagerCURLH
#define GURLManagerCURLH


//-----------------------------------------------------------------------------
// include files for CURL
#include <curl/curl.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gurlmanager.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GURLManagerKDE class provides a representation for a manager using the
* CURL library.
* @author Pascal Francq
* @short CURL URL Manager.
*/
class GURLManagerCURL : public GURLManager
{
	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

public:

	/**
	* Construct a URL manager.
	* @param path            Path to find the plugins.
	*/
	GURLManagerCURL(const char* path) throw(GException);

protected:

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile) throw(GException);

	/**
	* Delete the file locally.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile) throw(GException);

public:

	/**
	* Destructor of URL manager.
	*/
	virtual ~GURLManagerCURL(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
