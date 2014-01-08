/*

	GALILEI Research Project

	GStringTokenizer.h

	Classic String Tokenizer - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
	* Determine if the extracted tokens may contain other things than letters.
	*/
	bool ExtractNonLetter;

	/**
	 * Extract URI as links.
	 */
	bool ExtractURI;

	/**
	* Filter the token extracted?
	*/
	bool Filtering;

	/**
	* Maximum consecutive occurrences of a same character in a token.
	*/
	size_t MaxConsecutiveOccurs;

	/**
	 * Maximum number of non-letter characters in a token.
	 */
	size_t MaxNonLetter;

	/**
	* Value of the ratio of normal letters on the total number of letters in a
	* token.
	*/
	double NormalRatio;

	/**
	 * Number of non-letters actually treated.
	 */
	size_t NbNonLetters;

	/**
	 * Number of consecutive occurrences of the current character.
	 */
	size_t NbCurOccurs;

	/**
	 * Last character treated.
	 */
	RChar LastCar;

	/**
	 * Is the current token a valid one ?
	 */
	bool ValidToken;

	/**
	 * Position of the first double point.
	 */
	size_t PosDoublePoint;

	/**
	 * Position of the first at sign.
	 */
	size_t Pos1At;

	/**
	 * Position of the second at sign.
	 */
	size_t Pos2At;

	/**
	 * State of the finite-state machine (start a new token, parsing a normal
	 * character, parsing a separation character or end the token).
	 */
	enum {StartToken,Normal,Sep,EndToken} State;

	/**
	 * Position of the first separation character to skip.
	 */
	size_t SkipSep;

	/**
	 * First valid position (if cNoRef, it is undefined).
	 */
	size_t FirstPos;

	/**
	 * Accepted schemes.
    */
	RContainer<RString,true,true> Schemes;
	
	/**
	 * Type of the concepts representing URI.
	 */
	GConceptType* URISpace;

public:

	/**
	* Construct the tokenizer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GStringTokenizer(GSession* session,GPlugInFactory* fac);

	/**
	 * Initialize the schemes container.
    */
	void InitSchemes(void);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Method call each time the tokenizer is started to analyze some text.
	 */
	virtual void Start(void);

	/**
	 * Look if a string is an URI that must be added as an link token.
    * @param str            String to analyze (can be modified).
    * @return true if it was treated as URI.
    */
	bool IsURI(R::RString& str);

	/**
	 * This method is called each time the analyzer treat a character.
	 *
	 * The method should called the AddToken method the analyzer to add valid
	 * tokens.
	 * @param analyzer       Analyzer.
	 * @param analyzer       Analyzer.
	 * @param car            Character treated.
	 * @return true if the character starts a token.
	 */
	virtual bool TreatChar(GDocAnalyze* analyzer,const R::RChar& car);
};




//------------------------------------------------------------------------------
#endif
