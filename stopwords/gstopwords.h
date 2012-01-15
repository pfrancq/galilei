/*

	GALILEI Research Project

	GStopWords.h

	Stopwords Treatment - Header.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GStopWordsH
#define GStopWordsH


//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <ganalyzer.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * The GStopWords class provides a filter for the stopwords.
 * @short Stopwords Filter
 */
class GStopWords : public GAnalyzer
{
	/**
	* Cursor on the different languages defined in the system.
	*/
	RCastCursor<GPlugIn,GLang> Langs;

	/**
	 * Stopwords to delete.
	 */
	R::RContainer<GTextToken,false,false> ToDelete;

	/**
	* Number of different stopwords for the different languages in the current
	* document.
	*/
	R::RNumContainer<size_t,false> NbDiffStops;

	/**
	 * Define if the stopwords must be removed from the document descriptions.
	 */
	bool RemoveStopwords;

	/**
	* Define if the language are static.
	*/
	bool StaticLang;

	/**
	* Minimum percentage of words of a document which must be in the stop list
	* of a language to make the language the current one.
	*/
	double MinStopWords;

public:

	/**
	* Construct the analyzer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GStopWords(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Look if the tokens contain stopwords in the different languages. There
	 * are removed from the list.
	 *
	 * If the parameter StaticLang is false, the analyzer tries to determine
	 * the language of the document based on the number of different stopwords
	 * for each languages. In this case, all the tokens must be treated before
	 * another analyzer gets the hand.
	 * @param analyzer       Analyzer.
	 * @return false if the analyzer waits for some tokens to finish the
	 *         treatment.
	 */
	virtual void TreatTokens(GDocAnalyze* analyzer);
};




//------------------------------------------------------------------------------
#endif
