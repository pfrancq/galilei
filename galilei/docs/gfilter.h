/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GFilterH
#define GFilterH


//------------------------------------------------------------------------------
// include files for R
#include <rdownload.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_FILTER_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a generic filter that transform
* a document of a given type into a DocXML document. Each filter is associated
* with a given set of MIME types. This association is used to determine which
* filter should analyse a document.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter : public GPlugin<GFactoryFilter>
{
protected:

	/**
	* DocXML document that will contain the result of the document analysis.
	*/
	GDocXML* Doc;

public:

	/**
	* Construct the filter.
	* @param fac            Factory.
	*/
	GFilter(GFactoryFilter* fac);

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
	* Analyse a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(char* block,R::RXMLTag* attach);

	/**
	* Analyse a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(R::RChar* block,R::RXMLTag* attach);

	/**
	* Analyse a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(const R::RString& block,R::RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeKeywords(char* list,char sep,R::RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeKeywords(R::RChar* list,R::RChar sep,R::RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeKeywords(const R::RString& list,R::RChar sep,R::RXMLTag* attach);

	/**
	* Analyze a document with a given URI that was downloaded in a local
	* temporary file and for which a DocXML must be created. This method must
	* be reimplemented by all filters.
	* @param uri             URI of the file to analyze.
	* @param file            Local file to analyze.
	* @param docxml          Local file that will containing the DocXML.
	*/
	virtual void Analyze(const R::RURI& uri,const R::RString& file,const R::RString& docxml)=0;

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryDocFilter represent a factory for a given filter.
* @author Pascal Francq
* @short Generic Filter Factory.
*/
class GFactoryFilter : public GFactoryPlugin<GFactoryFilter,GFilter,GFilterManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryFilter(GFilterManager* mng,R::RString n,R::RString f)
		: GFactoryPlugin<GFactoryFilter,GFilter,GFilterManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GFilterManager class provides a representation for a generic manager that
* handles :
* - Plugins of filters.
* - A list of association between a MIME type and a filter.
* - URL, by downloading the file and making a local copy available (this copy
*   has to be remove after the analysis).
* @author Pascal Francq
* @short Generic URL Manager.
*/
class GFilterManager : public GPluginManager<GFilterManager,GFactoryFilter,GFilter>, public R::RDownload
{
protected:

	class GMIMEFilter;
	class GMIMEExt;

	/**
	* List of all pairs (MIME type,filter) available.
	*/
	R::RContainer<GMIMEFilter,true,true> MIMES;

	/**
	* List of all pairs (extension, MIME type) available.
	*/
	R::RContainer<GMIMEExt,true,true> Exts;

	/**
	 * Filter that should analyze the file.
	 */
	GFilter* Filter;

	/**
	 * Document to analyze.
	 */
	GDoc* Doc;

public:

	/**
	* Construct the filter manager.
	*/
	GFilterManager(void);

protected:

	/**
	 * Find the MIME type of a file based on its extension (Ex. ".html" gives
	 * "text/html").
	 */
	void FindMIMEType(void);

	/**
	 * If the protocol is HTTP and the server returns a content type for the
	 * downloaded file. This function verifies that a filter exist for the
	 * document to download.
	 * @param MIME           MIME type send by the server.
	 * @return true if the file should be downloaded.
	 */
	virtual bool IsValidContent(const R::RString& MIME);

public:

	/**
	* Return the URI of the file to analyze. If the file is not a XML file, the
	* method tries to determine the MIME type and to find a corresponding
	* filter.
	* @param doc             Document to analyze.
	* @param docxml          Temporary file containing the DocXML if necessary.
	* @return The result depends:
	* #- The original URI is given back if it is a XML file.
	* #- A local temporary file containing a XML version of the file if a
	*    filter was used (the temporary file must be deleted by the caller).
	* @exception A GException is generated if the document could not be
	* analysed (no MIME type, no filter, ...).
	*/
	R::RURI WhatAnalyze(GDoc* doc,R::RIO::RSmartTempFile& docxml);

	/**
	* Add a pair (MIME type,filter).
	* @param mime           Name of the MIME type.
	* @param f              Pointer to the filter.
	*/
	void AddMIME(const char* mime,GFilter* f);

	/**
	* Delete all the MIME type associated with a filter.
	* @param f              Pointer to the filter.
	*/
	void DelMIMES(GFilter* f);

	/**
	* Destructor of filter manager.
	*/
	virtual ~GFilterManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_FILTER_FACTORY(name,plugin)\
	CREATE_FACTORY(GFilterManager,GFactoryFilter,GFilter,plugin,"Filter",API_FILTER_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
