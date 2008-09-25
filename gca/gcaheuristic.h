/*

	Genetic Community Algorithm

	GCAHeuristic.h

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
#ifndef GCAHeuristicH
#define GCAHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupingheuristic.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
* The GCAHeuristic class provides a specific heuristic for the IR problem.
* @author Pascal Francq
* @short IR Heuristic.
*/
class GCAHeuristic : public R::RGroupingHeuristic<GCAGroup,GCAObj,GCAChromo>
{
	R::RContainer<GCAGroup,false,false> ToDel;
	R::RContainer<GCAMaxRatios,true,false>& Ratios;

public:

	/**
	* Construct the IR heuristic.
	* @param r              The random generator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	 GCAHeuristic(R::RRandom* r,R::RCursor<GCAObj> objs,R::RContainer<GCAMaxRatios,true,false>& ratios,R::RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	virtual void Init(GCAChromo* groups);

	/**
	* Find a group for the next object.
	* @param obj             Object.
	*/
	virtual GCAGroup* FindGroup(GCAObj* obj);

	/**
	* Verify that no social profiles is alone.
	*/
	virtual void PostRun(void);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~GCAHeuristic(void);
};


//-----------------------------------------------------------------------------
#endif
