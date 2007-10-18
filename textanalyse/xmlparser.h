/*

	GALILEI Research Project

	XMLParser.h

	A XML Parser to extract information entities - Header.

	Copyright 2007 by the Université libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GXMLParserH
#define GXMLParserH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gtextanalyse.h>


//-----------------------------------------------------------------------------
/**
 */
class XMLParser : public RXMLFile
{
	/**
	 * Pointer to filter.
	 */
	GTextAnalyse* Filter;
	
	/**
	 * Is the current tag a title?
	 */
	bool IsTitle;
	
	/**
	 * Contents.
	 */
	RStack<RString,true,true,true> Contents;
	
	/**
	 * Remember current content.
	 */
	RString Content;
		
public:
	
	/**
	 * Constructor of the parser.
	 * @param filter         Filter calling the parser.
	 * @param uri            URi of the file to parse.
	 */
	XMLParser(GTextAnalyse* filter,const RURI& uri);
	
	// Overload default function
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name,RContainer<RXMLAttr,true,true>& attrs);
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void Text(const RString& text);
	virtual RChar CodeToChar(RString& code);
	
	/**
	 * Look if a structure element must be added.
	 */
	void AddStructElement(const RString& element,size_t depth);

	/**
	 * Look if an index element must be added.
	 */
	void AddIndex(const RString& element,const RString& content,size_t depth);	
};


//-----------------------------------------------------------------------------
#endif

