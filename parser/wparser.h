/*

	Wikipedia Plug-Ins

	WParser.h

	Parse the Token extracted from Wikipedia - Header.

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
#ifndef WParserH
#define WParserH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gfilter.h>
#include <rstack.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * Parser for a Wikipedia article extracted from a dump file. If not call via
 * the Wikipedia Tool, an exception is generated.
 * @short Wikipedia Article Parser.
 */
class WParser : public GFilter
{
	/**
	 * Analyzer that has called the filter.
	 */
	GDocAnalyze* Analyzer;

	/**
	 * Extract structure.
	 */
	bool ExtractStruct;

	/**
	 * Extract metadata?
	 */
	bool ExtractMetadata;

	/**
	 * Type of the concepts representing terms (words, etc.).
	 */
	GConceptType* TermsSpace;

	/**
	 * URI concepts.
    */
	GConceptType* URI;

	/**
	 * Pointer to the English language.
	 */
	GLang* LangEn;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	WParser(GSession* session,GPlugInFactory* fac);

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
	* Analyze an article with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destruct.
	*/
	virtual ~WParser(void);
};


//-----------------------------------------------------------------------------
#endif
