/*

	GALILEI Research Project

	GTokenizer.h

	Text Tokenizer - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GTokenizerH
#define GTokenizerH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GTokenizer class provides some methods that break a set of characters
 * into tokens.
 *
 * It proposes a framework for a finite-state machine with memory.
 *
 * It is used in the analyze of a document to determine how to extract the
 * basic elements (words, abbreviation, e-mails, etc.).
 * @short Text Tokenizer
 */
class GTokenizer : public GPlugIn
{
	/**
	 * Memory.
	 */
	R::RString Buffer;

	/**
	 * Current position.
	 */
	size_t CurPos;

public:

	/**
	* Construct the tokenizer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GTokenizer(GSession* session,GPlugInFactory* fac);

	/**
	 * Add a character to the memory.
	 * @param car            Character to save.
	 */
	void AddChar(const R::RChar& car);

	/**
	 * Extract a string from the memory
	 * @param begin          Beginning position.
	 * @param end            Ending position. If it is cNoRef, the end is the
	 *                       last character of the memory. Else, the ending
	 *                       position is to copied.
	 * @return
	 */
	R::RString Extract(size_t begin,size_t end);

	/**
	 * Get the position currently treated.
	 * @return position.
	 */
	size_t GetPos(void) const {return(CurPos);}

	/**
	 * Method call each time the tokenizer is started to analyze some text. It
	 * must be called by all inheriting method.
	 */
	virtual void Start(void);

	/**
	 * This method is called each time the analyzer treat a character.
	 *
	 * The method should called the AddToken method the analyzer to add valid
	 * tokens.
	 * @param analyzer       Analyzer.
	 * @param car            Character treated.
	 */
	virtual void TreatChar(GDocAnalyze* analyzer,const R::RChar& car)=0;
};


//-------------------------------------------------------------------------------
#define CREATE_TOKENIZER_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GTokenizer,plugin,"Tokenizer",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
