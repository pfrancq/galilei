/*

	GALILEI Research Project

	PDF.h

	A PDF filter - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).
	Copyright 1996-2005 by Glyph & Cog, LLC.

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
using namespace GALILEI;
using namespace R;
using namespace std;


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
class GFilterPDF : public GFilter
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
	* Construct the PDF filter.
	* @param session         Session.
	* @param fac            Factory.
	*/
	GFilterPDF(GSession* session,GPlugInFactory* fac);

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

protected:

	/**
	* Get a metadata from the PDF file.
	* @param infoDict        Dictionnary of metadata of the PDF file.
	* @param key             Name of the metadata to get.
	* @return R::RString.
	*/
	R::RString CreateString(Dict* infoDict,const char* key);

	/**
	* Get a meta data representing a date from the PDF file.
	* @param infoDict        Dictionnary of metadata of the PDF file.
	* @param key             Name of the metadata to get.
	* @return R::RString.
	*/
	R::RString CreateDate(Dict* infoDict,const char* key);

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while(Pos&&(!Pos->IsNull())&&(Pos->IsSpace())) Pos++;}

public:

	/**
	* Destructor.
	*/
	virtual ~GFilterPDF(void);
};


//-----------------------------------------------------------------------------
#endif
