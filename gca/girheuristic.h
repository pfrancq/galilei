/*

	GALILEI Project Library

	GIRHeuristic.h

	Heuristic for an IR Problem - Header

	Copyright 2002 by the Universit�Libre de Bruxelles.

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
#ifndef GIRHeuristicH
#define GIRHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupingheuristic.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIRHeuristic class provides a specific heuristic for the IR problem.
* @author Pascal Francq
* @short IR Heuristic.
*/
class GIRHeuristic : public R::RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>
{
	R::RContainer<GGroupIR,false,false> ToDel;

	R::RContainer<GIRMaxRatios,true,false>& Ratios;

public:

	/**
	* Construct the IR heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	 GIRHeuristic(R::RRandom* r,R::RCursor<GObjIR>* objs,R::RContainer<GIRMaxRatios,true,false>& ratios,R::RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	virtual void Init(GChromoIR* groups);

	/**
	* Find a group for the next object.
	*/
	virtual GGroupIR* FindGroup(void);

	/**
	* Verify that no social profiles is alone.
	*/
	virtual void PostRun(void);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~GIRHeuristic(void);
};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
