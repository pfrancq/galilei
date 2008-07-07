/*

	GALILEI Research Project

	GFilterPDF.h

	A PDF filter - Header.

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
#ifndef GFilterPDFH
#define GFilterPDFH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>



//-----------------------------------------------------------------------------
// forward declaration
class Dict;
class UnicodeMap;


//-----------------------------------------------------------------------------
/**
* The GFilterPDF class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterPDF: public GALILEI::GFilter
{
	/**
	* Pointer to the current position in the buffer.
	*/
	R::RChar* Pos;

	/**
	* Pointer to begin of a block.
	*/
	R::RChar* Begin;

public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param fac            Factory.
	*/
	GFilterPDF(GALILEI::GFactoryFilter* fac);

	/**
	* Analyze a document with a given URI that was downloaded in a local
	* temporary file and for which a DocXML must be created.
	* @param uri             URI of the file to analyze.
	* @param file            Local file to analyze.
	* @param docxml          Local file that will containing the DocXML.
	*/
	virtual void Analyze(const R::RURI& uri,const R::RString& file,const R::RString& docxml);

protected:

	/**
	* Get a metadata from the PDF file.
	* @param infoDict        Dictionnary of metadata of the PDF file.
	* @param key             Name of the metadata to get.
	* @return R::RString.
	*/
	R::RString CreateString(Dict* infoDict,char* key);

	/**
	* Get a meta data representing a date from the PDF file.
	* @param infoDict        Dictionnary of metadata of the PDF file.
	* @param key             Name of the metadata to get.
	* @return R::RString.
	*/
	R::RString CreateDate(Dict* infoDict,char* key);

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while(Pos&&(!Pos->IsNull())&&(Pos->IsSpace())) Pos++;}

public:

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GFilterPDF(void);
};


//-----------------------------------------------------------------------------
#endif
