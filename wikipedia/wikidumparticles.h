/*

	Wikipedia Plug-Ins

	WikiDumpArticles.h

	Parse a Wikipedia Dump Article File - Header.

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
#ifndef WikiDumpArticles_H
#define WikiDumpArticles_H


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rxmlparser.h>
#include <gslot.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
class Wikipedia;


//-----------------------------------------------------------------------------
class WikiDumpArticles : RXMLParser
{
	/**
	 * Is a <page> tag currently treated.
	 */
	bool InPage;

	/**
	 * Is a <text> tag currently treated.
	 */
	bool InText;

	/**
	 * Is a <title> tag currently treated.
	 */
	bool InTitle;

	/**
	 * Is a <revision> tag currently treated.
	 */
	bool InRevision;

	/**
	 * Title of the current page.
    */
	RString Title;

	/**
	 * Content of the current page.
    */
	RString Content;

	/**
	 * Is the article an original or a redirection.
	 */
	bool Original;

	/**
	 * Identifier of the article.
	 */
	size_t ArticleId;

	/**
	 * Number of analyzed articles.
	 */
	size_t NbAnalyzedArticles;

	/**
	 * Date of the update.
	 */
	RDate Updated;

	/**
	 * Plug-In.
    */
	Wikipedia* PlugIn;

	/**
	 * Pointer to the English language.
	 */
	GLang* LangEn;

public:

	/**
	 * Open a dump file.
	 * @param plugin         Plug-In.
    * @param file           Name of the dump.
    */
	WikiDumpArticles(Wikipedia* plugin,const RString& file);

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
};


//-----------------------------------------------------------------------------
#endif

