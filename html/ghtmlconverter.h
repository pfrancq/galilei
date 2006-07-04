/*

	GALILEI Research Project

	GHTMLConverter.h

	HTML file Converter to DocXML- Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef GHTMLConverter_H
#define GHTMLConverter_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>

//------------------------------------------------------------------------------
// include files for GALILEI Project
#include <gdocxml.h>
#include <gfilter.h>
//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a HTML file. The file is loaded in a XML structure that
* should be compatible with XHTML.
* @short HTML File converter.
* @author Pascal Francq,Valery Vandaele.
*/
class GHTMLConverter : public R::RXMLFile
{
	class Tag;

	/**
	* Pointer to the calling filter
	*/
	GFilter* Filter;

	/**
	* Pointer to the gdocxml structure
	*/
	GDocXML* Doc;

	/**
	* HTML Tags.
	*/
	R::RContainer<Tag,true,true> Tags;

	/**
	* Is the </html> found?
	*/
	bool FoundClosingHTML;

	/**
	* Base url for the links contained in the document
	*/
	R::RString Base;

	/**
	* Maintain the current Paragraph tag (used when text parent is docxml:content)
	*/
	R::RXMLTag* ParTag;

	/**
	* Is the current DocXML file correctly created?
	*/
	bool DocXMLCreated;

public:

	/**
	* Construct a HTML file to read.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	GHTMLConverter(GFilter* filter,RString name,GDocXML* xmlstruct,const R::RString& encoding="Latin1");

	/**
	* Construct a HTML file to read.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	GHTMLConverter(GFilter* filter,RString name,GDocXML& xmlstruct,const R::RString& encoding="Latin1");

	/**
	* Set the doctype of the XML document. The doctype is transform in lowercase
	* and XHTML correspond to the open tag 'html'. If html is not the result, an
	* exception occurs.
	* @param docType        Name of the encoding.
	*/
	virtual void SetDocType(const R::RString& docType);

protected:

	/**
	* This methode creates all the tags valid for the HTML version supported.
	*/
	void InitValidTags(void);

	/**
	* Test if a given character represent the end of a sentence.
	* @param c               8-bit character to test.
	* @return true if it is a end sentence character.
	*/
	static inline bool IsEndSentence(R::RChar c)
	{
		return((c=='.')||(c=='!')||(c=='?')||(c==';')||(c==':'));
	}

public:

	/**
	* Function called each time a tag will be treated when reading a XML file.
	* Acutally, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @param attrs         Attributes associated to the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void BeginTag(const R::RString& namespaceURI, const R::RString& lName, const R::RString& name,R::RContainer<R::RXMLAttr,true,true>& attrs);

	/**
	* Function called each time a tag was treated when reading a XML file.
	* Acutally, the current tag is set its parent.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void EndTag(const R::RString& namespaceURI, const R::RString& lName, const R::RString& name);

	/**
	* Function called each time a text is processed when reading a XML file.
	* Actually, the text is added as content to the current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const R::RString& text);

	/**
	* Function to find the end of the tag and to skip all the content
	* @param tag           The name of the tag to close
	*/
	virtual void SkipTagContent(const R::RString& tag);

	/**
	* Insert A link in the docxml structure
	* @param attrs         Attributes associated to the tag(link).
	*/
	void InsertLink(R::RContainer<R::RXMLAttr,true,true>& attrs);

	/**
	* Function who reconstruct the partial url of a link
	* @param urlG             The url to modify. The urlG will be empty if u only contains an anchor
	* @param u                the new URL
	*/
	void ConstructURL(R::RString& urlG,R::RString u);

	/**
	* Function called when a <base> tag is met
	* This function store the base if exists, to be
	* able to recreate links URL
	* @param attrs         Attributes associated to the tag(base).
	*/
	void StoreBase(R::RContainer<R::RXMLAttr,true,true>& attrs);

	/**
	* Function called when a <meta> tag is met
	* This function Insert tags into docxml structure to treat metaDatas
	* @param attrs         Attributes associated to the tag(base).
	*/
	void InsertMetaData(R::RContainer<R::RXMLAttr,true,true>& attrs);

	/**
	* This function transform a given string that is supposed to represent a
	* HTML code (ex: "Ucirc") into the corresponding character (ex: �.
	* @param code           HTML Code.
	* @returns A RChar corresponding to the code or 0 if the code is not
	* identified.
	*/
	virtual R::RChar CodeToChar(R::RString& code);

	/**
	* Function that specify if only quotes are allowed to delimit a parameter in
	* a tag. This function return false since quote can be omitted in HTML.
	*/
	virtual bool OnlyQuote(void);

	/**
	* Function that specify if invalid XML codes (sequences beginning with a '&'
	* are accepted. This function return true since it is allowed in HTML.
	*/
	virtual bool InvalidXMLCodeAccept(void);

	/**
	* Destruct the XML file.
	*/
	~GHTMLConverter(void);

	friend class GFilter;
};


}  //-------- End of namespace GALILEI -----------------------------------------------


//------------------------------------------------------------------------------
#endif
