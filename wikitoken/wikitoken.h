/*

	Wikipedia Plug-Ins

	WikiToken.h

	Wikipedia Content Token - Header.

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
#ifndef WikiToken_H
#define WikiToken_H



//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <galilei.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
/**
 * The WikiToken provides a class for a token extracted from a Wikipedia notice.
 * @short Wikipedia Token.
 */
class WikiToken
{
public:
	/**
	 * Type of the token.
	 */
	enum tType {Text,Section,Link,Title};

private:

	/**
	 * Type of the token.
	 */
	tType Type;

	/**
	 * Content of the token.
    */
	RString Content;

	/**
	 * Depth.
    */
	int Depth;

	/**
	 * Position in Wikipedia.
	 */
	size_t Pos;

	/**
	 * Syntactic position.
	 */
	size_t SyntacticPos;

public:

	/**
	 * Constructor.
	 */
	WikiToken(void);

	/**
	 * Compare two Wikipedia tokens. In practice, it does nothing.
	 * @return always -1.
	 */
	int Compare(const WikiToken&) const {return(-1);}

	/**
	 * Clear the information related to the token (to reuse it).
    */
	void Clear(void);

	/**
	 * Transform a title in a valid Wikipedia title (superscript for the first
	 * letter and _ used as spaces).
    */
	static RString GetWikiTitle(const RString& title);

	friend class Wikipedia;
	friend class WParser;
};


//------------------------------------------------------------------------------
/**
 * The WikiTokens manages a set of token extracted from Wikipedia. In practice,
 * the tokens are reused from an analyze to the another one.
 * @short Wikipedia Tokens.
 */
class WikiTokens
{
	/**
	 * All tokens created in memory.
	 */
	RContainer<WikiToken,true,false> MemTokens;

	/**
	 * All tokens extracted.
	 */
	RContainer<WikiToken,false,false> Tokens;

	/**
	 * Current document.
    */
	const GDoc* Doc;

public:

	/**
	 * Constructor.
    */
	WikiTokens(void);

	/**
	 * Set the document associated to current tokens extracted.
    * @param doc
    */
	void SetDoc(const GDoc* doc) {Doc=doc;}

	/**
	 * Verify that the document to analyze is the same than the one extracted
	 * from Wikipedia.
	 * @param doc            Document to analyze.
    * @return
    */
	bool SameDoc(const GDoc* doc) const {return(Doc==doc);}

	/**
	 * Create a token.
	 * @return a pointer to a token.
	 */
	WikiToken* CreateToken(void);

	/**
    * @return a cursor over all the tokens extacted.
    */
	RCursor<WikiToken> GetTokens(void) const;

	/**
	 * Clear the container of extracted tokens (the tokens are not deallocated).
    * @return
    */
	void Clear(void);

	/**
	 * Delete the last token inserted.
    */
	void DeleteLastToken(void);

	/**
	 * This static method returns a pointer to the shared instance of tokens. It
	 * is the communication interface between the Wikipedia tool and the
	 * Wikipedia parser.
	 * @return pointer to the shared instance.
	 */
	static WikiTokens* Get(void);
};


//-----------------------------------------------------------------------------
#endif
