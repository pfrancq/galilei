/*

	GALILEI Research Project

	GDocFragmentRank.h

	Document Fragment's Ranking - Header.

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
#ifndef GDocFragmentRankH
#define GDocFragmentRankH


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDocFragmentRank class provides a triplet (document fragment,ranking,info) that
 * represents the ranking of a document in a given situation (search,
 * suggestion, etc.).
  * @short Document Fragment's Ranking
 */
class GDocFragmentRank
{
protected:

	/**
	 * Document fragment.
	 */
	GDocFragment* Fragment;

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
	 * @param fragment       Document fragment.
	 * @param ranking        Ranking of the document.
	 * @param info           Information.
	 */
	GDocFragmentRank(GDocFragment* fragment,double ranking=0.0,const R::RString info=R::RString::Null);

	/**
	 * Compare two rankings by using first the fragment and then the information.
	 * @param ranking        Ranking to compare with.
	 * @return -1,0 or +1 depending of the comparison.
	 */
	int Compare(const GDocFragmentRank& ranking) const;

	/**
	 * Compare a ranking with an information.
	 * @param info           Information to compare with.
	 * @return -1,0 or +1 depending of the comparison.
	 * @warning This method works only to search for a ranking in a container
	 * that contains only ranking related to the same document fragment.
	 */
	int Compare(const R::RString& info) const;

	/**
	 * Get the document fragment.
	 * @return a pointer to a GDocFragment.
	 */
	GDocFragment* GetFragment(void) const {return(Fragment);}

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
	* Static function used to order the ranking (the highest first). This
	* function can be used with the RContainer::ReOrder method.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int SortOrderRanking(const void* a,const void* b);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
