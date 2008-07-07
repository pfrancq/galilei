/*

	GALILEI Research Project

	XMLParser.h

	A XML Parser to extract information entities - Header.

	Copyright 2007-2008 by the Université libre de Bruxelles.

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
#include <rxmlparser.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gtextanalyse.h>


//-----------------------------------------------------------------------------
/**
 */
class XMLParser : public RXMLParser
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

	/**
	 * Current tag.
	 */
	GDocStructNode* CurTag;

	/**
	 * Current attribute.
	 */
	GDocStructNode* CurAttr;

public:

	/**
	 * Constructor of the parser.
	 * @param filter         Filter calling the parser.
	 * @param uri            URi of the file to parse.
	 */
	XMLParser(GTextAnalyse* filter,const RURI& uri);

	// Overload default function
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void AddAttribute(const RString& namespaceURI, const RString& lName, const RString& name);
	virtual void Value(const RString& value);
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void Text(const RString& text);

	/**
	 * Transform a code into a character. It call first the valid codes for
	 * XML, if the code is not valid it is replaced by a space.
	 * @param code           String representing the code.
	 * @return Character.
	 */
	virtual RChar CodeToChar(RString& code);

	/**
	 * Add a structure element (tag or attribute) if needed.
	 * @param element        Name of the element to add.
	 * @param tag            Is it a tag (true) or an attribute (false).
	 */
	void AddStructElement(const RString& element,bool tag);

	/**
	 * Add a tag index if needed.
	 * @param element        Name of the index tag to test.
	 * @param content        Content of the tag, i.e. the whole text contained
	 *                       through the opening and closing tags.
	 */
	void AddTagIndex(const RString& element,const RString& content);
};


//-----------------------------------------------------------------------------
#endif

