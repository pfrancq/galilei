/*

	GALILEI Research Project

	GEvaluateGroupingVariance.h

	Compare a ideal groupement with a computed one - Header.

	(C) 2002 by David Wartel

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
#ifndef GEvaluateGroupingVarianceH
#define GEvaluateGroupingVarianceH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/gevaluategrouping.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGroupingVariance provides a way to evaluate a clustering by
* providing the variance.
* @author David Wartel
* @short Clustering Evaluation x.
*/
class GEvaluateGroupingVariance : public GEvaluateGrouping
{


public:

	/**
	* Constructor.
	* @param s              Session.
	* @param groups         Ideal groupement.
	*/
	GEvaluateGroupingVariance(GSession* s,RContainer<GGroups,unsigned int,true,true>* groups) throw(bad_alloc);

protected:



public:

    /**
	* runs the evaluation of the clustering..
	*/
	virtual double Run(void);

	/**
	* calculates the clainsky index ( criterion to determine 
	* the number of clusters).
	*/
	double CalcVariance(void);

	/**
	* returns ths most relevant subprofile from a group
	*/
	GSubProfile* RelevantSubProfile(GGroup* grp);

	/**
	* retuns the sum of similarities between a subprofile and
	* all the other subprofiles of its group. 
	*/
	double GroupSumSimilarity(GSubProfile * s, GGroup *grp) ;


	/**
	* Destructor.
	*/
	virtual ~GEvaluateGroupingVariance(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
