/*

	GALILEI Research Project

	GSearchToken.h

	Search Token - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GSearchTokenH
#define GSearchTokenH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GSearchToken provides a representation for a search token (such as a
 * keyword). In practice, a search token can correspond to multiple concepts in
 * different languages (typically the stems in the case of a keyword).
 * @short Search Token.
 */
class GSearchToken
{
public:

	/**
	 * This enumeration represents the different type of search tokens
	 * recognised.
	 * @short Search Token Type.
	 */
	enum tType
	{
		tTerm,                /** The token is a term (for example:
									     "connections"). The term is lowered. */
		tStem,                /** The token is the stem of a term (for example
									     "connect"). The stem is lowered. */
		tRawConcept           /** The token is a raw concept of a given concept
									     type separated by a double point. If the concept
									     type contains a double point, quotes must be
									     used (for example:
									     "http://purl.org/dc/elements/1.1":author).*/
	};

private:

	/**
	 * Concept associated to the search token.
	 */
	GConcept* Concept;

	/**
	 * Type of the token.
	 */
	tType Type;

public:

	/**
	 * Create a search token.
    * @param concept        Concept.
    * @param type           Type of the token.
    */
	GSearchToken(GConcept* concept,tType type);

	/**
	 * Create a search token based on a string. The corresponding may be null if
	 * the token is not defined in the session.
    * @param session        Session for which the token must be created.
    * @param token          Token.
    * @param type           Type of the token.
    */
	GSearchToken(GSession* session,const R::RString& token,tType type);

	/**
	 * Compare two tokens by their strings.
    * @param token          Token to compare with.
    * @return -1,0 or +1 depending of the comparison of the strings.
    */
	int Compare(const GSearchToken& token) const;

	/**
	 * Compare a token by a string.
    * @param token          String to compare with.
    * @return -1,0 or +1 depending of the comparison of the strings.
    */
	int Compare(const R::RString& token) const;

	/**
	 * Get the concept corresponding to the token.
    * @return a pointer to a concept.
    */
	GConcept* GetConcept(void) const {return(Concept);}

	/**
	 * Get the type of the token.
    * @return the token type.
    */
	tType GetType(void) const {return(Type);}

	friend class GSearchQuery;
	friend class GSearchQueryNode;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
