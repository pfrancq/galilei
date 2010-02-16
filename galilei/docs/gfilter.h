/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GFilterH
#define GFilterH


//------------------------------------------------------------------------------
// include files for R
#include <rxmlparser.h>
#include <rtextfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a generic filter that transform
* a document of a given type into a DocXML document. Each filter is associated
* with a given set of MIME types. This association is used to determine which
* filter should analyze a document.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter : public GPlugIn
{
	class MetaData;

	R::RContainer<MetaData,true,false> MetaDatas;

public:

	/**
	 * Determine how the metadata should be analyzed.
	 */
	enum MetaDataType
	{
		RawText,             /** Raw text.*/
		Sentence,            /** Sentences.*/
		Keywords             /** Keywords.*/
	};

	/**
	* Construct the filter.
	* @param fac            Factory.
	*/
	GFilter(GPlugInFactory* fac);

protected:

	/**
	* Add a specific MIME-Type for the filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(const char* mime);

	/**
	* Add a specific MIME-Type for the filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(R::RString mime);

	/**
	* Test if a given character represent the end of a sentence.
	* @param c               8-bit character to test.
	* @return true if it is a end sentence character.
	*/
	static inline bool IsEndSentence(char c)
	{
		return((c=='.')||(c=='!')||(c=='?')||(c==';')||(c==':'));
	}

	/**
	* Test if a given character represent the end of a sentence.
	* @param c               Unicode character to test.
	* @return true if it is a end sentence character.
	*/
	static inline bool IsEndSentence(R::RChar c)
	{
		return((c.Latin1()=='.')||(c.Latin1()=='!')||(c.Latin1()=='?')||(c.Latin1()==';')||(c.Latin1()==':'));
	}

public:

	/**
	* Analyze a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param parser         Parser of the XML stream.
	*/
	void AnalyzeBlock(const R::RString& block,R::RXMLParser* parser);

	/**
	* Analyze a list of keywords separating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param parser         Parser of the XML stream.
	*/
	void AnalyzeKeywords(const R::RString& list,R::RChar sep,R::RXMLParser* parser);

	/**
	 * Clear local information.
	 */
	void Clear(R::RXMLParser* parser);

	//-----------------------------------------------------
	/** @name DocXML Methods
	*/
	// @{

	/**
	 * Start a DocXML stream.
	 * @param parser         Parser of the XML stream.
	 */
	void StartStream(R::RXMLParser* parser);

	/**
	 * Add a given metadata defined by the Dublin core.
	 * @code
	 * parser->AddDublinCoreMetaData("title","This is the title of the document");
	 * @endcode
	 * @param name           Name of the metadata (without namespace and
	 *                       prefix).
	 * @param value          Value.
	 * @param type           Type of the metadata.
	 * @param sep            Separator (only for keywords)
	 */
	void AddDublinCoreMetaData(const R::RString& name,const R::RString& value,MetaDataType type=RawText,R::RChar sep=',');

	/**
	 * Start a DocXML stream.
	 * @param parser         Parser of the XML stream.
	 */
	void WriteMetaDataStream(R::RXMLParser* parser);

	/**
	 * Start a paragraph.
	 * @param parser         Parser of the XML stream.
	 */
	void StartParagraph(R::RXMLParser* parser);

	/**
	 * End a paragraph.
	 * @param parser         Parser of the XML stream.
	 */
	void EndParagraph(R::RXMLParser* parser);

	/**
	 * Extract textual content from a file.
	 * @param file           File.
	 * @param parser         Parser of the XML stream.
	 * @param text           Text to add.
	 */
	void ExtractTextualContent(R::RTextFile& file,R::RXMLParser* parser,const R::RString& text=R::RString::Null);

	/**
	 * End a DocXML stream.
	 * @param parser         Parser of the XML stream.
	 */
	void EndStream(R::RXMLParser* parser);

	//@} DocXML Methods

	/**
	* Analyze a document with a given URI for which a DocXML must be created.
	* This method must be re-implemented by all filters.
	* @param doc             Document to analyze.
	* @param uri             URI of the file to analyze.
	* @param parser          Current parser of the XML stream.
	* @param rec             Receiver for the signals.
	*/
	virtual void Analyze(GDoc* doc,const R::RURI& uri,R::RXMLParser* parser,GSlot* rec)=0;

	/**
	* Destruct the filter.
	*/
	virtual ~GFilter(void);
};


//-------------------------------------------------------------------------------
#define CREATE_FILTER_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GFilter,plugin,"Filter",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
