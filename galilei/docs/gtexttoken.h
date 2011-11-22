/*

	GALILEI Research Project

	GTextToken.h

	Document Token - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).

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
#ifndef GTextTokenH
#define GTextTokenH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GTextToken class represents a token extracted from a text document. It
 * is used when a document is analyzed : the tokenizer parses the text received
 * and create the corresponding tokens. These tokens are then treated by the
 * different analyzers in a specific order.
 * @short Document Token.
 */
class GTextToken
{
public:

		/**
		 * This class represents an occurrence of a token.
		 */
		class Occurrence
		{
			/**
			 * The corresponding token.
			 */
			GTextToken* Token;

			/**
			 * The corresponding vector.
			 */
			GVector* Vector;

			/**
			 * Position of the occurrence in the document.
			 */
			size_t Pos;

			/**
			 * Depth of the occurrence in the document.
			 */
			size_t Depth;

			/**
			 * Index of the occurrence in the main container.
			 */
			size_t Index;

		public:

			/**
			 * Occurrence of a null token.
			 */
			Occurrence(void);

			/**
			 * Occurrence of a token.
			 * @param token  Token associated.
			 * @param vector Vector.
			 * @param pos    Position.
			 * @param depth  Depth.
			 */
			Occurrence(GTextToken* token,GVector* vector,size_t pos,size_t depth);

			/**
			 * Method needed by R::RContainer. The occurrences are supposed to be
          * ordered in ascending order of their index.
          * @param occur  Occurrence to compare with.
			 * @return a value reflecting of a element has a smaller index or not.
			 */
			int Compare(const Occurrence& occur) const {return(Index-occur.Index);}

			/**
			 * Get the token corresponding to this occurrence.
			 * @return a pointer to GTextToken.
			 */
			GTextToken* GetToken(void) const {return(Token);}

			friend class GTextToken;
			friend class GDocAnalyze;
		};

private:

	/**
	 * Token.
	 */
	R::RString Token;

	/**
	 * Concept associated with that token.
	 */
	GConcept* Concept;

	/**
	 * Occurrences of the token.
	 */
	R::RContainer<Occurrence,false,false> Occurs;

	/**
	 * Index of the token in the main container.
	 */
	size_t Index;

public:

	/**
	 * Create a token.
	 * @param token          String containing the token.
	 */
	GTextToken(const R::RString& token=R::RString::Null);

	/**
	 * Compare two tokens.
	 * @param token          Token to compare with.
	 * @return a number usable by R::RContainer.
	 */
	int Compare(const GTextToken& token) const;

	/**
	 * Compare a token with a string.
	 * @param token          String to compare with.
	 * @return a number usable by R::RContainer.
	 */
	int Compare(const R::RString& token) const;

	/**
	 * Compute the hash index of a given token.
	 * @param idx            Index of the hash table.
	 * @return a number usable by R::RDblHasContainer.
	 */
	size_t HashIndex(size_t idx) const;

	/**
	 * Get the token.
	 * @return a R::RString.
	 */
	R::RString GetToken(void) const {return(Token);}

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
	Occurrence* AddOccur(GVector* vector,size_t pos,size_t depth);

	/**
	 * Get the number of occurrences of the token.
	 */
	size_t GetNbOccurs(void) const {return(Occurs.GetNb());}

	friend class GDocAnalyze;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
