/*

	GALILEI Research Project

	GEvaluateGroupingCalinski.h

	Compare a ideal groupement with a computed one - Header.

	(C) 2002 by David Wartel and Lamoral Julien

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
#ifndef GEvaluateGroupingCalinskiH
#define GEvaluateGroupingCalinskiH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/gevaluategrouping.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGroupingCalinski provides a way to evaluate a clustering by
* providing the Calinski Index.
* @author David Wartel Lamoral Julien
* @short Calinski Evaluation x.
*/
class GEvaluateGroupingCalinski : public GEvaluateGrouping
{


public:

	/**
	* Constructor.
	* @param s              Session.
	* @param groups         Computed or ideal groupement.
	*/
	GEvaluateGroupingCalinski(GSession* s,RContainer<GGroupsEvaluate,unsigned int,false,false>* groups) throw(bad_alloc);


public:

	/**
	* runs the evaluation of the clustering..
	*/
	virtual double Run(void);


	/**
	* calculates the clainsky index ( criterion to determine 
	* the number of clusters).
	*/
	double CalcCalinski(void);

	/**
	* returns the number of groups in the clustering.
	*/
	int GetNbGroups(void);


	/**
	* Destructor.
	*/
	virtual ~GEvaluateGroupingCalinski(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
