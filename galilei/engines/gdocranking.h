/*

	GALILEI Research Project

	GSuggestion.h

	Suggestion - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GDocRankingH
#define GDocRankingH


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDocRanking class provides a triplet (document,ranking,info) that
 * represents the ranking of a document in a given situation (search,
 * suggestion, etc.).
 * @author Pascal Francq
 * @short Document Ranking
 */
class GDocRanking
{
protected:

	/**
	 * Identifier of the document.
	 */
	size_t DocId;

	/**
	 * Ranking of the document.
	 */
	double Ranking;

	/**
	 * An information about the ranking.
	 */
	R::RString Info;

public:

	/**
	 * Create a document ranking.
	 * @param docid          Identifier of the document.
	 * @param ranking        Ranking of the document.
	 * @param info           Information.
	 */
	GDocRanking(size_t docid,double ranking=0.0,const R::RString info=R::RString::Null);

	/**
	 * @return the identifier of the document.
	 */
	size_t GetDocId(void) const {return(DocId);}

	/**
	 * Get the URI of the document by using its identifier.
    * @param session        Session.
    * @return the document URI.
    */
	R::RURI GetURI(GSession* session) const;

	/**
	 * Get the name of the document by using its identifier.
    * @param session        Session.
    * @return the document name.
    */
	R::RString GetName(GSession* session) const;

	/**
	 * @return the ranking of the document.
	 */
	double GetRanking(void) {return(Ranking);}

	/**
	 * @return the information associated with a suggestion.
	 */
	R::RString GetInfo(void) const {return(Info);}

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


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
