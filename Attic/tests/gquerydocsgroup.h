/*                      ;

	GALILEI Research Project

	GQueryDocsGroup.h

	Query from Doc Sim xhith groups- Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
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
#ifndef GQueryDocsGroupH
#define GQueryDocsGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmath/random.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GQueryDocsGroup provides a way to calc similarity with words in Docs and the
* ideal corresponding group.
* @author Francq Pascal and Lamoral Julien
* @short Doc Query Evaluate.
*/
class GQueryDocsGroup
{
	/**
	* Current Session;
	*/
	GSession* Session;

	/**
	* Array of GIWordWeight used for ordering.
	*/
	GIWordWeight** Order;

	/**
	* Size of the array;
	*/
	unsigned int SizeOrder;

	/**
	* Similarity Query-Same Groups
	*/
	double SimQueryIntra;

	/**
	* Similarity Query-Diff Groups.
	*/
	double SimQueryInter;

	/**
	* Number of correct groups found.
	*/
	double Targets;

public:

	/**
	* Constructor.
	* @param ses            The Galilei session.
	*/
	GQueryDocsGroup(GSession* ses) throw(bad_alloc);

	/**
	* Run the query.
	*/
	void Run(void);

	/**
	* Get the similarity Query-Same Groups.
	* @returns double
	*/
	double GetSimQueryIntra(void) const {return(SimQueryIntra);}

	/**
	* Get the similarity Query-Diff Groups.
	* @returns double
	*/
	double GetSimQueryInter(void) const {return(SimQueryInter);}

	/**
	* Get the number of correct groups found.
	* @returns double
	*/
	double GetTargets(void) const {return(Targets);}

	/**
	* Destructor.
	*/
	virtual ~GQueryDocsGroup(void);

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
