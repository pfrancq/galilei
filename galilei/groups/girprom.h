/*

	GALILEI Research Project

	GIRProm.h

	PROMETHEE Kernel for Grouping - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GIRPromH
#define GIRPromH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GPlugin class provides a representation for a basic "plugin" system for
* the GALILEI sessions.
* @author Pascal Francq
* @short Generic Plugin.
*/
class GIRProm : public R::RPromKernel
{
	/**
	* Parameters.
	*/
	GIRParams* Params;

	/**
	* Criteria representing the average similarity of the profiles with the
	* ones of the same group.
	*/
	R::RPromCriterion* CritSim;

	/**
	* Criteria representing the Similarity criterion "AvgSim".
	*/
	R::RPromCriterion* CritSimAvgSim;

	/**
	* Criteria representing the Similarity criterion "J".
	*/
	R::RPromCriterion* CritSimJ;

	/**
	* Criteria representing the Similarity criterion "AvgRatio".
	*/
	R::RPromCriterion* CritSimAvgRatio;

	/**
	* Criteria representing the Similarity criterion "AvgMinRatio".
	*/
	R::RPromCriterion* CritSimMinRatio;

	/**
	* Value of the Similarity criterion "AvgRatio".
	*/
	R::RPromCriterion* CritSimRatio;

	/**
	* Value of the Similarity criterion "AvgWOverB".
	*/
	R::RPromCriterion* CritSimWOverB;

	/**
	* Value of the Similarity criterion "SimWB".
	*/
	R::RPromCriterion* CritSimSimWB;

	/**
	* Criteria representing number of groups.
	*/
	R::RPromCriterion* CritInfo;

	/**
	* Criteria representing the entropy of a solution.
	*/
	R::RPromCriterion* CritEntropy;

	/**
	* Criteria representing the likelihood of a solution.
	*/
	R::RPromCriterion* CritLikelihood;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common OK documents.
	*/
	R::RPromCriterion* CritSameFeedbacks;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common documents but with opposite judgement.
	*/
	R::RPromCriterion* CritDiffFeedbacks;

	/**
	* Criteria representing the factor depending on the subprofiles that are
	* social.
	*/
	R::RPromCriterion* CritSocial;

public:

	/**
	* Constructor.
	* @param p              Parameters.
	*/
	GIRProm(GIRParams* p) throw(bad_alloc);

	/**
	* Assign chromosome as a solution.
	* @param s              Solution.
	* @param c              Chromosome.
	*/
	void Assign(R::RPromSol* s,GChromoIR* c);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
