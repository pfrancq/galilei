/*

	GALILEI Research Project

	GDocFragmentRanks.h

	Document Fragment's Rankings - Header.

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
#ifndef GDocFragmentRanksH
#define GDocFragmentRanksH


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDocFragmentRanks class provides a list of ranking for a given document
 * fragment. Each ranking is identified by an information.
 * @short Document Fragments Rankings
 */
class GDocFragmentRanks : R::RContainer<GDocFragmentRank,true,true>
{
	/**
	 * Document Fragment.
	 */
	GDocFragment* Fragment;

	/**
	 * Global ranking of the fragment.
	 */
	double Ranking;

public:

	/**
	 * Constructor.
    * @param fragment       Document fragment.
    */
	GDocFragmentRanks(GDocFragment* fragment);

	/**
	* Method to compare document fragments.
	* @param fragment       Document fragment.
	*/
	int Compare(const GDocFragmentRanks& fragment) const;

	/**
	* Method to compare document fragments.
	* @param fragment       Document fragment.
	*/
	int Compare(const GDocFragment& fragment) const;

	/**
	 * Get the document fragment.
	 * @return a pointer to a GDocFragment.
	 */
	GDocFragment* GetFragment(void) const {return(Fragment);}

	/**
	* Add new ranking for the current document fragment and a given information.
	* If a ranking with the same information already exists, it is replaced with
	* the ranking.
	* @param ranking         Ranking
	* @param engine          Engine
	* @return the ranking (replaced or created).
	*/
	GDocFragmentRank* AddRanking(double ranking,const R::RString info);

	/**
	* @return a cursor on the rankings of the document fragment.
	*/
	R::RCursor<GDocFragmentRank> GetRankings(void) const;

	/**
	 * @return the global ranking of the document fragment.
	 */
	double GetRanking(void) {return(Ranking);}

	/**
	 * Set the global ranking for the document fragment.
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
