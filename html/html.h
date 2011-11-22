/*

	GALILEI Research Project

	HTML.h

	A HTML filter - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2003 by Valery Vandaele.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GFilterHTMLH
#define GFilterHTMLH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gfilter.h>
#include <rxmlparser.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
* The GFilterHTML class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterHTML : public GFilter, public RXMLParser
{
    /**
     * The current analyzer.
     */
    GDocAnalyze* Analyzer;

    /**
     * Is the current tag 'META'
     */
    bool MetaTag;

    /**
     * Is the current tag 'TITLE'
     */
    bool TitleTag;

    /**
     * Has the Tag 'BODY' be parsed.
     */
    bool BodyTag;

    /**
     * Content attribute ?
     */
    bool Content;

    /**
     * Valid meta tag.
     */
    RString Meta;

public:

	/**
	* Construct the HTML filter.
	* @param session         Session.
	* @param fac            Factory.
	*/
	GFilterHTML(GSession* session,GPlugInFactory* fac);

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

	/**
	* Function called each time a tag will be treated when reading a XML file.
	* Actually, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void BeginTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Method called each time an attribute will be treated when reading a XML
	* file.
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the attribute.
	* @param name            Complete name of the attribute.
	 */
	virtual void AddAttribute(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time some attribute value elements (words or spaces)
	* are parsed when reading a XML file.
	* @param value           Value processed.
	 */
	virtual void Value(const RString& value);

	/**
	* Function called each time a tag was treated when reading a XML file.
	* Actually, the current tag is set its parent.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void EndTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Function called each time a text is processed when reading a XML file.
	* Actually, the text is added as content to the current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const RString& text);

   /**
	* Create the parameters.
	* @param fac             Factory.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	*The destructor
	*/
	virtual ~GFilterHTML(void);
};


//-----------------------------------------------------------------------------
#endif
