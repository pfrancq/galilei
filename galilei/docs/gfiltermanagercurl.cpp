/*

	GALILEI Research Project

	GURLManagerCURL.cpp

	Manager to handle URL file using the CURL library - Implementation.

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
// include files for ANSI C/C++
#include <stdio.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gurlmanagercurl.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// Internal function and structure
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct DwnFile
{
	char filename[L_tmpnam+1];
	FILE *stream;
};


//-----------------------------------------------------------------------------
int WriteTmpFile(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct DwnFile* out=(struct DwnFile *)stream;

	if(out && !out->stream)
	{
		/* open file for writing */
		out->stream=fopen(out->filename, "wb");
		if(!out->stream)
		return(-1);
	}
	return(fwrite(buffer, size, nmemb, out->stream));
}



//-----------------------------------------------------------------------------
//
// class GURLManagerCURL
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GURLManagerCURL::GURLManagerCURL(void)
	: GURLManager()
{
	Lib = curl_easy_init();
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManagerCURL::Download(const char* URL,RString& tmpFile) throw(GException)
{
	struct DwnFile tmpfile;
	int err;

	// Fill structure -> find a local temporary file
	tmpfile.stream=0;
	tmpnam(tmpfile.filename);
	tmpFile=tmpfile.filename;

	// Download the file
	curl_easy_setopt(Lib, CURLOPT_URL, URL);
	curl_easy_setopt(Lib, CURLOPT_WRITEFUNCTION,WriteTmpFile);
	curl_easy_setopt(Lib, CURLOPT_FILE, &tmpfile);
	err=curl_easy_perform(Lib);
	if(tmpfile.stream)
		fclose(tmpfile.stream);
	if(err)
		throw GException(RString("Can't download url")+URL);
}


//-----------------------------------------------------------------------------
GMIMEFilter* GALILEI::GURLManagerCURL::DetermineMIMEType(const char*)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManagerCURL::Delete(RString& tmpFile) throw(GException)
{
	remove(tmpFile());
}


//-----------------------------------------------------------------------------
GALILEI::GURLManagerCURL::~GURLManagerCURL(void)
{
	curl_easy_cleanup(Lib);
}
