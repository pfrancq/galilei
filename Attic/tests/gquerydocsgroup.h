/*                      ;

	GALILEI Research Project

	GQueryDocsGroup.h

	Query from Doc Sim xhith groups- Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
	* Recall of the queries.
	*/
	double Recall;

	/**
	* Number of times the correct group is the first retrieved.
	*/
	double First;

	/**
	* Number of times the correct groups is the 2 first retrieved.
	*/
	double Second;

public:

	/**
	* Constructor.
	* @param ses            The Galilei session.
	*/
	GQueryDocsGroup(GSession* ses) throw(bad_alloc);

	/**
	* Run the query.
	* @param first          Number of first words used.
	* @param nb             Number of words in a query.
	* @param trans          Transform query vector?
	*/
	void Run(unsigned int first,unsigned int nb,bool trans);

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
	double GetRecall(void) const {return(Recall);}

	/**
	* Get the number of times the correct group is the first retrieved.
	* @returns double
	*/
	double GetFirst(void) const {return(First);}

	/**
	* Get the number of times the correct groups is the 2 first retrieved.
	* @returns double
	*/
	double GetSecond(void) const {return(Second);}

	/**
	* Destructor.
	*/
	virtual ~GQueryDocsGroup(void);

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
