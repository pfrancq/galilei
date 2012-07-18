/*

	GALILEI Research Project

	GStringIndexer.h

	String Indexer - Header.

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
#ifndef GStringIndexerH
#define GStringIndexerH


//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <ganalyzer.h>
#include <rtextfile.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * The GStringIndexer class provides a way to limit the tokens in a document
 * and to associate a concept to it.
 * @short String Indexer
 */
class GStringIndexer : public GAnalyzer
{
	/**
	 * Type of the concepts representing terms (words, etc.).
	 */
	GConceptType* TermsSpace;

	/**
	 * Category of metadata concepts.
	 */
	GConceptCat* Metadata;

	/**
	* Minimum number of characters to have a valid token.
	*/
	size_t MinSize;

	/**
	 * Minimum number of occurrences of the token to be valid.
	 */
	size_t MinOccurs;

	/**
	 * Apply the rules to the metadata ?
	 */
	bool ApplyRulesMetadata;

public:

	/**
	* Construct the analyzer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GStringIndexer(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Extract concepts from current tokens. It is the responsibility of the
	 * analyzer to eventually filter some tokens.
	 * @param analyzer       Analyzer.
	 */
	virtual void TreatTokens(GDocAnalyze* analyzer);
};




//------------------------------------------------------------------------------
#endif
