/*

	GALILEI Research Project

	GToken.h

	Document Token - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GTokenH
#define GTokenH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represents an occurrence of a given token. It is only used by
* GDocAnalyze.
* @short Token Occurrence
*/
class GTokenOccur
{
	/**
	* The corresponding token.
	*/
	GToken* Token;

	/**
	* The corresponding meta-vector (meta-concept).
	*/
	GVector* Vector;

	/**
	* Children of the occurrence.
	*/
	R::RContainer<GTokenOccur,false,false> Children;

	/**
	* Position of the occurrence in the document.
	*/
	size_t Pos;

	/**
	* Depth of the occurrence in the document.
	*/
	size_t Depth;

	/**
	* Syntactic position of the occurrence in the document.
	*/
	size_t SyntacticPos;

	/**
	* Index of the occurrence in the main container.
	*/
	size_t Index;

	/**
	 * Weight of the occurrence.
    */
	double Weight;

public:

	/**
	* Occurrence of a null token.
	* @param nb       Number of possible children.
	*/
	GTokenOccur(size_t nb);

	/**
	* Occurrence of a token.
	* @param token    Token associated.
	* @param vector   Vector.
	* @param pos      Position.
	* @param depth    Depth.
	* @param spos     Syntactic position.
	* @param nb       Number of possible children.
	*/
	GTokenOccur(GToken* token,GVector* vector,size_t pos,size_t depth,size_t spos,size_t nb);

	/**
	* Method needed by R::RContainer. The occurrences are supposed to be
	* ordered in ascending order of their index.
	* @param occur           Occurrence to compare with.
	* @return a value reflecting of a element has a smaller index or not.
	*/
	int Compare(const GTokenOccur& occur) const {return(Index-occur.Index);}

	/**
    * @return the corresponding token.
    */
	GToken* GetToken(void) const {return(Token);}

	friend class GToken;
	friend class GDocAnalyze;
};


//------------------------------------------------------------------------------
/**
 * The GToken class represents a token extracted from a document. It
 * is used when a document is analyzed : the tokenizer parses the document
 * and create the corresponding tokens. These tokens are then treated by the
 * different analyzers in a specific order.
 *
 * It is only used by GDocAnalyze.
 * @short Document Token.
 */
class GToken
{
	/**
	 * Internal structure to search for a particular token.
	 * @short Token Search Structure.
	 */
	struct Search
	{
		/**
		* Token.
		*/
		R::RString Token;

		/**
		* Type of the token.
		*/
		tTokenType Type;

		/**
		 * Constructor.
       * @param token        Token.
       * @param type         Type.
       */
		Search(R::RString token,tTokenType type);
	};


	/**
	 * Token.
	 */
	R::RString Token;

	/**
	 * Type of the token.
	 */
	tTokenType Type;

	/**
	 * Concept associated with that token.
	 */
	GConcept* Concept;

	/**
	 * Occurrences of the token.
	 */
	R::RContainer<GTokenOccur,false,false> Occurs;

	/**
	 * Index of the token in the main container.
	 */
	size_t Index;

public:

	/**
	 * Create a token.
	 * @param token          String containing the token.
	 */
	explicit GToken(const R::RString& token=R::RString::Null);

	/**
	 * Compare two tokens.
	 * @param token          Token to compare with.
	 * @return a number usable by R::RContainer.
	 */
	int Compare(const GToken& token) const;

	/**
	 * Compare a token with a string.
	 * @param token          Search to compare with.
	 * @return a number usable by R::RContainer.
	 */
	int Compare(const Search& token) const;

	/**
	 * Compute the hash index of a given token.
	 * @param idx            Index of the hash table.
	 * @return a number usable by R::RDblHasContainer.
	 */
	size_t HashIndex(size_t idx) const;

	/**
	 * @return a R::RString representing the token.
	 */
	R::RString GetToken(void) const {return(Token);}

	/**
    * @return the type of the token.
    */
	tTokenType GetType(void) const{return(Type);}

	/**
	 * Get the concept associated with the token (if any).
	 * @return a pointer to a GConcept (may be null).
	 */
	GConcept* GetConcept(void) const {return(Concept);}

	/**
	 * Associate a concept to the token.
	 * @param concept        Concept to assign.
	 */
	void SetConcept(GConcept* concept) {Concept=concept;};

	/**
	 * Add an occurrence of the token.
	 * @param vector         Vector associated to the occurrence.
	 * @param pos            Position of the occurrence.
	 * @param depth          Depth of the occurrence.
	 * @return a pointer to the occurrence added.
	 */
	GTokenOccur* AddOccur(GVector* vector,size_t pos,size_t depth);

	/**
	 * Look if the token contains alphabetic characters only.
    * @return true if it is the case.
    */
	bool IsAlpha(void) const;

	/**
	 * Look if the token is associated to a particular meta-concept.
	 * @param metaconcept    Meta-concept to search for.
	 */
	bool IsUsed(GConcept* metaconcept) const;

	/**
	 * Look if the token is associated to a meta-concept of a particular
	 * category.
	 * @param cat            Category to search for.
	 */
	bool IsUsed(tConceptCat cat) const;

	/**
	 * Get the number of occurrences of the token.
	 */
	size_t GetNbOccurs(void) const {return(Occurs.GetNb());}

	/**
	 * Destructor.
    */
	virtual ~GToken(void);

	friend class GDocAnalyze;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
