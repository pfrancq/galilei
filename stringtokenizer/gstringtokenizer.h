/*

	GALILEI Research Project

	GStringTokenizer.h

	Classic String Tokenizer - Header.

	Copyright 2011 by Pascal Francq (pascal@francq.info).

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
#ifndef GStringTokenizerH
#define GStringTokenizerH


//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <gtokenizer.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * The GTokenizer class provides some methods that break a set of characters
 * into tokens.
 *
 * It is used in the analyze of a document to determine how to extract the
 * basic elements (words, abbreviation, e-mails, etc.).
 * @short String Tokenizer
 */
class GStringTokenizer : public GTokenizer
{
	/**
	 * State of the machine (start a new token, parsing a normal character,
    * separation character or end of a token).
	 */
	enum {Leading,Normal,Sep,End} State;

	/**
	 * Position of the first separation character to skip.
	 */
	size_t SkipSep;

	/**
	 * First valid position (if cNoRef, it is undefined).
	 */
	size_t FirstPos;

public:

	/**
	* Construct the tokenizer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GStringTokenizer(GSession* session,GPlugInFactory* fac);

	/**
	 * Method call each time the tokenizer is started to analyze some text.
	 */
	virtual void Start(void);

	/**
	 * This method is called each time the analyzer treat a character.
	 *
	 * The method should called the AddToken method the analyzer to add valid
	 * tokens.
	 * @param analyzer       Analyzer.
	 * @param analyzer       Analyzer.
	 * @param car            Character treated.
	 */
	virtual void TreatChar(GDocAnalyze* analyzer,const R::RChar& car);
};




//------------------------------------------------------------------------------
#endif
