/*

	GALILEI Research Project

	GFilterManagerCURL.h

	Filter manager handling URL using the CURL library - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GFilterManagerCURLH
#define GFilterManagerCURLH


//------------------------------------------------------------------------------
// include files for CURL
#include <curl/curl.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfiltermanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFilterManagerCURL class provides a representation for a filter manager
* using the CURL library.
* @author Pascal Francq
* @short URL Manager using the CURL library.
*/
class GFilterManagerCURL : public GFilterManager
{
	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

public:

	/**
	* Construct the URL manager.
	* @param path            Path to find the plugins.
	* @param dlg             Should the dialog box be loaded.
	*/
	GFilterManagerCURL(const char* path,bool dlg=true) throw(std::bad_alloc,GException);

protected:

	/**
	* Parameter function gived to curl_easy_setopt
	*/
	static int WriteTmpFile(void* buffer, size_t size, size_t nmemb, void* stream);

	/**
	* Try to guess the MIME types of a temporary file. This method looks for the
	* information using the libCurl library.
	* @param tmpfile        Temporary file created.
	* @return Name fo of the MIME type.
	*/
	virtual const char* DetermineMIMEType(const char* tmpfile) throw(GException);

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile) throw(GException);

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile) throw(GException);

public:

	/**
	* Destructor of the URL manager.
	*/
	virtual ~GFilterManagerCURL(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
