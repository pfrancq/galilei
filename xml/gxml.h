/*

	GALILEI Research Project

	GXML.h

	Analyze a document - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef GXMLH
#define GXMLH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rxmlparser.h>
#include <gfilter.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
/**
 * The GXML class provides a method to analyze a document. The plug-in
 * maintains several lists of elements through the analysis of a XML documents:
 * - Words: The list of each word appearing in the document.
 * - StructTokens: The list of structural tokens appearing in the document.
 * - Depths: The different information associated to each depth of the
 *   document.
 * @author Pascal Francq
 * @short XML Documents Analyze.
 */
class GXML : public GFilter, public RXMLParser
{
	/**
	 * Specify if the current document must be full index. It is the case if
	 * full indexing is asked, and the document is a native XML.
	 */
	bool MustFullIndex;

	/**
	 * Extract structure elements?
	 */
	bool ExtractStruct;

	/**
	 * Should the structure be considered as normal content (stems)?
	 */
	bool StructIsContent;

	/**
	 * Extract pairs (declarative tags,content)?
	 */
	bool ExtractIndex;

	/**
	 * Maximum number of terms allowed for a declarative tag's content.
	 */
	size_t MaxTerms;

	/**
	 * Maximum depth of a declarative tag
	 */
	char MaxDepth;

	/**
	 * Maximal number of occurrences of a tag to be considered as index.
	 */
	size_t MaxOccurs;

	/**
	 * May declarative tags have child tags?
	 */
	bool ChildTags;

	/**
	 * Should the meta-tag be detected or must they be defined.
	 */
	bool DetectMetaTag;

	/**
	 * Name of the file containing the meta-tags.
	 */
	RString MetaTagFile;

	/**
	 * Meta tags.
	 */
	RContainer<RString,true,true> MetaTags;

	/**
	 * Weight of each tag and parameter name when they are considered as stems.
	 */
	double WeightStruct;

	/**
	 * Must the attributes values be considered as stems?
	 */
	bool ExtractValues;

	/**
	 * Weight of each parameter value when they are considered as stems.
	 */
	double WeightValues;

	/**
	 * Must a default namespace be used if tags are found with no namespace.
	 */
	bool UseDefaultNamespace;

	/**
	 * Default namespace to use.
	 */
	RString DefaultNamespace;

	/**
	 * Is the current tag a title?
	 */
	bool IsTitle;

	/**
	 * Is the current tag an identifier ?
	 */
	bool IsIdentifier;

	/**
	 * Temporary String;
	 */
	RString tmpStr;

	/**
	 * Native XML file.
	 */
	bool Native;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GXML(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Initialize the plug-in.
	*/
	virtual void Init(void);

	/**
	* Do some cleaning operations before a analyze.
	*/
	void Clear(void);

	// Overload default function
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void ResolveNamespace(const RString& namespaceURI);
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
	* Analyze a document with a given URI for which a DocXML must be created.
	* This method must be re-implemented by all filters.
	* @param doc             Document to analyze.
	* @param uri             URI of the file to analyze.
	* @param parser          Current parser of the XML stream.
	* @param rec             Receiver for the signals.
	*/
	virtual void Analyze(GDoc* doc,const R::RURI& uri,R::RXMLParser* parser,GSlot* rec);

	/**
	 * @return true if the analyze of the current tag must be stopped.
	 */
	inline bool StopAnalyseTag(void) const
	{
		return((!Native) ||  ((!MustFullIndex)&&(   (!ExtractIndex)    || (    ExtractIndex&&(DetectMetaTag&&(GetCurrentDepth()>MaxDepth)) ) )) );
	}

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destruct.
	*/
	virtual ~GXML(void);
};


//-----------------------------------------------------------------------------
#endif
