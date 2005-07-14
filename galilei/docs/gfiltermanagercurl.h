/*

	GALILEI Research Project

	GFilterManagerCURL.h

	Filter manager handling URL using the CURL library - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#include <gfilter.h>


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
	*/
	GFilterManagerCURL(void);

protected:

	/**
	* Parameter function gived to curl_easy_setopt
	*/
	static int WriteTmpFile(void* buffer, size_t size, size_t nmemb, void* stream);

public:

	/**
	* Try to guess the MIME types of a temporary file. This method looks for the
	* information using the libCurl library.
	* @param tmpfile        Temporary file created.
	* @return Name fo of the MIME type.
	*/
	virtual const char* DetermineMIMEType(const char* tmpfile);

protected:

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile);

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile);

public:

	/**
	* Destructor of the URL manager.
	*/
	virtual ~GFilterManagerCURL(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
