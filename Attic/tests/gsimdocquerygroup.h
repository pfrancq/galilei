/*

	GALILEI Research Project

	GSimDocQueryGroup.h

	Query from Doc Sim xhith groups- Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GSimDocQueryGroupH
#define GSimDocQueryGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/random.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSimDocQueryGroup provides a way to calc similarity with words in Docs and the
* ideal corresponding group.
* @author Lamoral Julien
* @short Doc Query Evaluate.
*/
class GSimDocQueryGroup
{
	/**
	* Language corresponding to the set of groups.
	*/
	GLang* Lang;

	/**
	* Random number generator
	*/
	R::RRandom* Random;

public:

	/**
	* Constructor.
	* @param ses            The Galilei session.
	*/
	GSimDocQueryGroup(GSession* ses) throw(bad_alloc);

	/**
	* Destructor.
	*/
	virtual ~GSimDocQueryGroup(void);

};




}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
