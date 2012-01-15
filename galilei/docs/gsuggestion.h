/*

	GALILEI Research Project

	GSuggestion.h

	Suggestion - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GSuggestionH
#define GSuggestionH


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDocRanking class provides a pair (document,ranking) to represent the
 * ranking of a document in a given situation (suggestion,score for a
 * community, etc.).
 * @author Pascal Francq
 * @short Document Ranking
 */
class GDocRanking
{
protected:

	/**
	 * Identifier of the document suggested.
	 */
	size_t DocId;

	/**
	 * Ranking of the document.
	 */
	double Ranking;

public:

	/**
	 * Create a document ranking.
	 * @param docid          Identifier of the document.
	 * @param ranking        Ranking of the document.
	 */
	GDocRanking(size_t docid,double ranking=0.0);

	/**
	 * @return the identifier of the document.
	 */
	size_t GetDocId(void) const {return(DocId);}

	/**
	 * @return the ranking of the document.
	 */
	double GetRanking(void) {return(Ranking);}

	/**
	 * Set the ranking of a document.
	 * @param ranking        Ranking.
	 */
	void SetRanking(double ranking) {Ranking=ranking;}

	/**
	 * Compare two rankings by using the identifiers of the corresponding
	 * documents, i.e. two rankings are supposed to be the same they are both
	 * related to the same document.
	 * @param ranking        Ranking to compare with.
	 * @return -1,0 or +1 depending of the comparison.
	 */
	int Compare(const GDocRanking& ranking) const;

	/**
	 * Compare a ranking with the identifiers of a document.
	 * @param docid          Identifier of the document.
	 * @return -1,0 or +1 depending of the comparison.
	 */
	int Compare(size_t docid) const;

	/**
	* Static function used to order the suggestions by ranking (the highest
	* first). The function can be used with the RContainer::ReOrder method.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int SortOrderRanking(const void* a,const void* b);
};


//-----------------------------------------------------------------------------
/**
 * The GSuggestion provides a representation for a suggestion of a given
 * document with a given rank.
 *
 * It is used with the GSugs class to suggest a given set of ranked documents
 * for a given profile or a given community.
 * @author Pascal Francq
 * @short Suggestion.
 */
class GSuggestion : public GDocRanking
{
	/**
	 * Date where the suggestion was proposed.
	 */
	R::RDate Proposed;

	/**
	 * An information about the suggestion.
	 */
	R::RString Info;

public:

	/**
	 * Construct a suggestion.
	 * @param docid          Identifier of the document.
	 * @param ranking        Ranking of the document.
	 * @param info           Information about the suggestion.
	 * @param proposed       Date of the suggestion.
	 * @return
	 */
	GSuggestion(size_t docid,double ranking,const R::RDate& proposed,const R::RString& info=R::RString::Null);

	/**
	 * @return the information associated with a suggestion.
	 */
	R::RString GetInfo(void) const {return(Info);}

	/**
	 * @return the date of the suggestion.
	 */
	R::RDate GetProposed(void) const {return(Proposed);}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
