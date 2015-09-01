/*

	Wikipedia Plug-Ins

	WikiDumpHistory.h

	Parse a Wikipedia Dump History File - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef WikiDumpHistory_H
#define WikiDumpHistory_H


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rxmlparser.h>
#include <gslot.h>
#include <gprofile.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
class Wikipedia;
class WikiToken;


//-----------------------------------------------------------------------------
class cRevision
{
public:
	GProfile* Profile;
	RDate Date;

	cRevision(void) : Profile(0), Date(RDate::Null) {}
	int Compare(const cRevision& rev) const
	{
		return(Profile->Compare(*rev.Profile));
	}
	int Compare(const GProfile* profile) const
	{
		return(Profile->Compare(*profile));
	}
};


//-----------------------------------------------------------------------------
class WikiDumpHistory : RXMLParser
{
	/**
	 * Plug-In.
    */
	Wikipedia* PlugIn;

	/**
	 * Is the article an original or a redirection.
	 */
	bool Original;

	/**
	 * Number of analyzed articles.
	 */
	size_t NbAnalyzedArticles;

	/**
	 * Number of revisions analyzed.
	 */
	size_t NbAnalyzedRevisions;

	/**
	 * In a revision tag?
	 */
	bool InRevision;

	/**
	 * In a contributor tag?
	 */
	bool InContributor;

	/**
	 * In a redirect?
	 */
	bool InRedirect;

	/**
	 * In a tag where content must be treated?
    */
	bool InContent;

	/**
	 * Attribute must be read?
	 */
	bool InAttribute;

	/**
	 * Content.
    */
	RString Content;

	/**
	 * Is the user anonymous ?
	 */
	bool Anonymous;

	/**
	 * Identifier of the user.
	 */
	size_t UserId;

	/**
	 * Name of the user.
	 */
	RString UserName;

	/**
	 * Date of the revision.
	 */
	RDate RevisionDate;

	/**
	 * URI of the current page.
	 */
	RString URI;

	/**
	 * Page currently treated.
	 */
	GDoc* Page;

	/**
	 * Stock of revisions.
	 */
	RContainer<cRevision,true,false> Stock;

	/**
	 * Actual revision for a given page.
	 */
	RContainer<cRevision,false,true> Revisions;

public:

	/**
	 * Open a dump file.
	 * @param plugin         Plug-In.
    * @param file           Name of the dump.
    */
	WikiDumpHistory(Wikipedia* plugin,const RString& file);

	/**
	* Open the file.
	* @param mode           The open mode for the file.
	*/
	virtual void Open(void);

private:

	/**
	* Method called each time a beginning tag is parsed (after the parsing of
	* the attributes).
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time a beginning tag is parsed (after the parsing of
	* the attributes).
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTagParsed(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time a tag was treated when reading a XML file.
	* @param namespaceURI  Namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	*/
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time some text elements (words or spaces) are parsed
	* when reading a XML file. Actually, the text is added as content to the
	* current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const RString& text);

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
};


//-----------------------------------------------------------------------------
#endif
