/*

	Genetic Community Algorithm

	GGCAHeuristic.h

	Heuristic - Header

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GGCAHeuristicH
#define GGCAHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupingheuristic.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggca.h>


//-----------------------------------------------------------------------------
/**
* The GGCAHeuristic class provides a specific heuristic for the IR problem.
* @author Pascal Francq
* @short IR Heuristic.
*/
class GGCAHeuristic : public R::RGroupingHeuristic<GGCAGroup,GGCAObj,GGCAChromo>
{
	R::RContainer<GGCAGroup,false,false> ToDel;
	R::RContainer<GGCAMaxRatios,true,false>& Ratios;

public:

	/**
	* Construct the IR heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	 GGCAHeuristic(R::RRandom* r,R::RCursor<GGCAObj> objs,R::RContainer<GGCAMaxRatios,true,false>& ratios,R::RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	virtual void Init(GGCAChromo* groups);

	/**
	* Find a group for the next object.
	*/
	virtual GGCAGroup* FindGroup(void);

	/**
	* Verify that no social profiles is alone.
	*/
	virtual void PostRun(void);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~GGCAHeuristic(void);
};


//-----------------------------------------------------------------------------
#endif
