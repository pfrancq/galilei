/*

	GALILEI Research Project

	GRanking.h

	Class to represent the ranking of results from a Search engine - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GRankingH
#define GRankingH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdexcept>


//------------------------------------------------------------------------------
// include files for R library
#include <rstd.h>
#include <rstring.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GRanking class provides a representation for the ranking
* of a page extracted from a search engine
* @author Valery Vandaele
* @short ranking for search engine results
*/
class GRanking
{
private:

	/**
	* The ranking of this result
	*/
	int Rank;

	/**
	* The search engine which has obtained this ranking
	*/
	R::RString Engine;

public:

	/**
	* Construct a ranking.
	* @param rank            Given ranking.
	* @param engine          Name of the enfine.
	*/
	GRanking(int rank,R::RString engine);

	/**
	* Get the name of the engine the ranking is associated with.
	* @return Name of the search engine.
	*/
	R::RString GetEngine(void) const {return Engine;};

	/**
	* Get the rank of the current element.
	* @return Value of the ranking.
	*/
	int GetRank(void) const {return Rank;};

	/**
	* Set a new ranking value
	* @param rank            New ranking value.
	*/
	void SetRank(int rank) {Rank=rank;};

	/**
	* Compare method (not used)
	*/
	int Compare(const GRanking& /*r*/) const {return -1;};

	/**
	* Compare method (not used)
	*/
	int Compare(const GRanking* /*r*/) const {return -1;};

	/**
	* Destructor of the ranking.
	*/
	virtual ~GRanking(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif

