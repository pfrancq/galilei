/*

	GALILEI Research Project

	GEvaluateGroupingMethods.h

	Compare a ideal groupement with a computed one - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GEvaluateGroupingMethodsH
#define GEvaluateGroupingMethodsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include<groups/gevaluategrouping.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGrouping provides a way to evaluate a clustering by
* providing a list of criterion.
* @author David Wartel
* @short Clustering Evaluation .
*/
class GEvaluateGroupingMethods
{


protected:

	 RContainer<GEvaluateGrouping,unsigned int,true,true>*  Methods;      



public:

	/**
	* Constructor.
	* @param s              Session.
	* @param groups         Ideal groupement.
	*/
	GEvaluateGroupingMethods(void) throw(bad_alloc);

	/**
	* registers a new method.
	*/
	void RegisterEvaluationMethod(GEvaluateGrouping* method);

	/**
	* Get a cursor to the evaluation method registered.
	* @return Return a GGoupingCursor.
	*/
	GEvaluateGroupingCursor& GetEvaluateGroupingCursor(void);


	/**
	* Destructor.
	*/
	virtual ~GEvaluateGroupingMethods(void);
};




}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
