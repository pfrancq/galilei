/*

	GALILEI Research Project

	GXML.h

	Analyze a document - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#include <gfilter.h>
#include <rstack.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
// include files for current project
#include <../shared/gxmlparser.h>


//-----------------------------------------------------------------------------
// forward declaration
class XMLSchema;


//-----------------------------------------------------------------------------
/**
 * The XMLTest class provides a representation for a sequence of characters
 * associated to the content of tags.
 * @author Pascal Francq
 * @short XML Textual Content Unit
 */
class XMLText
{
public:

	 /**
	  * Text.
	  */
	 RString Text;

	 /**
	  * Position in the file.
	  */
	 size_t Pos;

	 /**
	  * Depth in the XML tree.
	  */
	 size_t Depth;

	 /**
	  * Constructor of the class.
	  * @param txt           Text.
	  * @param pos           Position.
	  * @param depth         Depth.
	  */
	 XMLText(const RString& txt,size_t pos,size_t depth)
		: Text(txt), Pos(pos), Depth(depth) {}

	 /**
	  * Compare method needed by R::RContainer.
	  */
	 int Compare(const XMLText&) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
 * The XML Content class holds all the text associated with the content of a
 * given tag occurrence.
 * @author Pascal Francq
 * @short XML Textual Content
 */
class XMLContent : public RContainer<XMLText,true,false>
{
public:

	 /**
	  * Constructor.
	  */
	 XMLContent(void) : RContainer<XMLText,true,false>(200) {}

	 /**
	  * Compare method needed by R::RContainer.
	  */
	 int Compare(const XMLContent&) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
 * The XMLTag provides a representation for the a XML tag.
 * @author Pascal.
 * @short XML Tag.
 */
class XMLTag
{
public:

	/**
	 * Concept associated to the tag.
	 */
	GConcept* Concept;

	/**
	 * The tag.
	 */
	RString Tag;

	/**
	 * Number of occurrences of the tag in the document analyzed (must be reset
	 * after each analyze).
	 */
	size_t NbOccurs;

	/**
	 * Corresponding XML Schema.
	 */
	XMLSchema* Schema;

	/**
	 * Type of the tag:
	 * - Undefined: No information of what kind of tag it is.
	 * - Text: The tag contains default textual content.
	 * - Metadata: The tag is a metadata.
	 * - Unknown: When the metadata tags are not fixed, the analyzed must be
	 * done before to determine if the content is a default textual one or a
	 * metadata.
	 * - Exclude: The textual content of the tag should not be analyzed.
	 */
	enum {Undefined,Text,Metadata,Unknown,Exclude} Type;

	/**
	 * Its textual contents (must be reset after each analyze).
	 */
	RContainer<XMLContent,true,false> Content;

	/**
	 * Constructor.
	 * @param atg            Tag.
	 *
	 */
	XMLTag(const RString& tag);

	/**
	 * Compare two tags using their name. This method is needed by R::RContainer.
	 * @param tag            Tag to compare with.
	 */
	int Compare(const XMLTag& tag) const;

	/**
	 * Compare the name of the tag with a given one. This method is needed by
	 * R::RContainer.
	 * @param tag            Tag name to compare with.
	 */
	int Compare(const RString& tag) const;

	/**
	 * Get the concept associated to the tag. In practice, it is create in the
	 * corresponding dictionary if necessary.
	 */
	GConcept* GetConcept(void);
};


//-----------------------------------------------------------------------------
/**
 * The XMLSchema provides a representation for a given XML Schema.
 * @author Pascal Francq
 * @short XML Schema.
 */
class XMLSchema
{
public:

	/**
	 * URI of the schema.
	 */
	RString URI;

	/**
	 * In the tensor space model, each XML schema is a concept type in the
	 * category 'Semantic'.
	 */
	GConceptType* Type;

	/**
	 * When a vector of semantic information must be created, each XML schema
	 * must also be associated with a meta-concept.
	 */
	GConcept* MetaConcept;

	/**
	 * Tags.
	 */
	RContainer<XMLTag,true,true> Tags;

	/**
	 * Constructor.
	 * @param uri            URI representing the schema.
	 */
	XMLSchema(const RString& uri);

	/**
	 * Compare two XML schemas using their URI. This method is needed by
	 * R::RContainer.
	 * @param schema         Schema to compare with.
	 */
	int Compare(const XMLSchema& schema);

	/**
	 * Compare the uri of the XML schema with a given one. This method is needed
	 * by R::RContainer.
	 * @param uri            URI to compare with.
	 */
	int Compare(const RString& uri);
};


//-----------------------------------------------------------------------------
/**
 * The XMLTagOccur provides a representation of a particular occurrence of a
 * given tag in the document analyzed.
 * @author Pascal Francq
 * @short XML Tag Occurrence
 */
class XMLTagOccur
{
public:

	 /**
	  * The tag.
	  */
	 XMLTag* Tag;

	 /**
	  * Its particular content.
	  */
	 XMLContent* Content;

	 /**
	  * The corresponding token occurrence.
	  */
	 GTokenOccur* Occur;

	 /**
	  * Constructor.
	  * @param tag           Tag.
	  * @param content       Content.
	  */
	 XMLTagOccur(XMLTag* tag,XMLContent* content)
	 	: Tag(tag), Content(content), Occur(0) {}

	 /**
	  * Compare method needed by R::RContainer.
	  */
	 int Compare(const XMLTagOccur&) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
 * The GXML class provides a method to analyze a XML document. The plug-in
 * extract:
 * - Words: The list of each word appearing in the document.
 * - Semantic information: Structure information.
 * - Metadata: The metadata (eventually detected).
 * @author Pascal Francq
 * @short XML Documents Analyze.
 * @article Document_Description
 */
class GXML : public GFilter, public GXMLParser
{
	/**
	 * Analyzer that has called the XML filter.
	 */
	GDocAnalyze* Analyzer;

	/**
	 * Extract semantic information.
	 */
	bool ExtractSemantic;

	/**
	 * Extract metadata?
	 */
	bool ExtractMetadata;

	/**
	 * Maximum number of terms allowed in a metadata tag.
	 */
	size_t MetadataMaxTerms;

	/**
	 * Maximum depth of a metadata tag.
	 */
	size_t MetadataMaxDepth;

	/**
	 * Maximal number of occurrences of a metadata tag.
	 */
	size_t MetadataMaxOccurs;

	/**
	 * May metadata tags have child tags?
	 */
	bool MetadataChildTags;

	/**
	 * Should the metadata tag be detected or must they be defined.
	 */
	bool DetectMetadataTag;

	/**
	 * Metadata tags.
	 */
	RContainer<RString,true,true> MetadataTags;

	/**
	 * Tags which content is to exclude.
	 */
	RContainer<RString,true,true> ExcludeTags;

	/**
	 * Must a default namespace be used if tags are found with no namespace.
	 */
	bool UseDefaultNamespace;

	/**
	 * Default namespace to use (if necessary).
	 */
	RString DefaultNamespace;

	/**
	 * All the XML schemas found during the analysis.
	 */
	RContainer<XMLSchema,true,true> Schemas;

	/**
	 * Last-In - Last-Out stack of tags.
	 */
	RStack<XMLTagOccur,true,true,true> Tags;

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

	/**
	 * Get the schema corresponding to an URI.
    * @param uri            Schema.
    * @return a pointer to the schema.
    */
	XMLSchema* GetSchema(const RString& uri);

	/**
	 * Get the concept corresponding to a XML schema (given by its URI). If it
	 * doesn't exist, it is created.
    * @param namespaceURI   URI of the XML schema.
    * @param lName          Name of the tag/attribute.
    * @return a pointer to concept.
    */
	XMLTag* GetXMLTag(const RString& namespaceURI,const RString& lName);

	// Overload default function
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void ResolveNamespace(const RString& namespaceURI);
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void Text(const RString& text);
	virtual void AddAttribute(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void Value(const RString& value);

	/**
	 * Transform a code into a character. It call first the valid codes for
	 * XML, if the code is not valid it is replaced by a space.
	 * @param code           String representing the code.
	 * @return Character.
	 */
	virtual RChar CodeToChar(RString& code);

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

	/**
	 * Extract a text fragment from a file with a given URI. This method should
	 * be re-implemented by filters.
	 * @param fragment       Fragment to extract.
    * @return a string containing the text fragment.
    */
	virtual R::RString GetTextFragment(GDocFragment* fragment);

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
