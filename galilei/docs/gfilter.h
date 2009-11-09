/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <rdownload.h>


//------------------------------------------------------------------------------
// include files for GALILEI
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
* filter should analyse a document.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter : public GPlugIn
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
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(char* block,R::RXMLTag* attach);

	/**
	* Analyze a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(R::RChar* block,R::RXMLTag* attach);

	/**
	* Analyze a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeBlock(const R::RString& block,R::RXMLTag* attach);

	/**
	* Analyze a list of keywords separating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeKeywords(char* list,char sep,R::RXMLTag* attach);

	/**
	* Analyze a list of keywords separating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	R::RXMLTag* AnalyzeKeywords(R::RChar* list,R::RChar sep,R::RXMLTag* attach);

	/**
	* Analyze a list of keywords separating by a single character.
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
	virtual void Analyze(const R::RURI& uri,const R::RURI& file,const R::RURI& docxml)=0;

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
