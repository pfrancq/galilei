/*

	GALILEI Research Project

	GInstIR.h

	Instance for an IR Problem - Header

	(C) 2002 by P. Francq.

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
#ifndef GInstIRH
#define GInstIRH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rinstg.h>
#include <rpromethee/rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GThreadDataIR class provides a representation for "thread-dependent" data
* for the IR GA.
* @author Pascal Francq
* @short IR "thread-dependent" Data.
*/
class GThreadDataIR : public RGGA::RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>
{
public:

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GThreadDataIR(GInstIR* owner);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfileSamesGroupIR class provides a representation of a couple of
* subprofiles having a number of OK documents in common.
*/
class GSubProfilesSameGroupIR
{
public:

	/**
	* Identificator of the first subprofile.
	*/
	unsigned int Id1;

	/**
	* Identificator of the Second subprofile.
	*/
	unsigned int Id2;

	/**
	* Number of judgement documents in common.
	*/
	unsigned int NbDocs;

	/**
	* Constructor.
	* @param i1             Identificator of the first subprofile.
	* @param i2             Identificator of the second subprofile.
	* @param nb             Number of common documents judged.
	*/
	GSubProfilesSameGroupIR(unsigned int i1,unsigned int i2,unsigned int nb)
		: Id1(i1), Id2(i2), NbDocs(nb) {}

	/**
	* Compare method by RStd::RContainer.
	*/
	int Compare(const GSubProfilesSameGroupIR*) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
* The instance of the IR problem.
* @author Pascal Francq
* @short IR Instance.
*/
class GInstIR : public RGGA::RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>, public RPromethee::RPromKernel
{
	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSim* Sims;

	/**
	* Couples of subprofiles having common OK documents.
	*/
	RStd::RContainer<GSubProfilesSameGroupIR,unsigned int,true,false> SameGroups;

	/**
	* Couples of subprofiles having common documents with different judgement.
	*/
	RStd::RContainer<GSubProfilesSameGroupIR,unsigned int,true,false> DiffGroups;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* The maximum number of generations.
	*/
	unsigned long MaxGen;

	/**
	* Criteria representing the average similarity of the profiles with the
	* ones of the same group.
	*/
	RPromethee::RPromCriterion* CritSim;

	/**
	* Criteria representing the average number of profiles per groups.
	*/
	RPromethee::RPromCriterion* CritNb;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common OK documents.
	*/
	RPromethee::RPromCriterion* CritOKDocs;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common documents but with opposite judgement.
	*/
	RPromethee::RPromCriterion* CritDiffDocs;

	/**
	* Solutions corresponding to the chromosome.
	*/
	RPromSol** Sols;

	/**
	* Global Similarities.
	*/
	bool GlobalSim;

public:

	/**
	* Construct the instance.
	* @param m              Minimal similarity in a group.
	* @param max            Maximal number of generations.
	* @param popsize        The size of the population.
	* @param prob           The problem.
	* @param h              The type of heuristic to be used.
	* @param debug          Debugger.
	*/
	GInstIR(double m,unsigned int max,unsigned int popsize,RGA::RObjs<GObjIR>* objs,bool g,GProfilesSim* s,RGGA::HeuristicType h,RDebug *debug=0) throw(bad_alloc);

	/**
	* Initialisation of the instance.
	* @param gdata          The Data to use for the construction of the groups.
	* @param grps           Pointer to the current solutions.
	*/
	virtual void Init(GGroupDataIR* gdata,GGroups* grps=0) throw(bad_alloc);

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void) throw(eGA);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	void SetCriterionParam(const char* crit,double p,double q,double w);

	/**
	* Look if the global similarities are supposed to be global.
	* @return bool.
	*/
	bool IsGlobalSim(void) const {return(GlobalSim);}

	/**
	* Destruct the instance.
	*/
	virtual ~GInstIR(void);

	// friend classes
	friend class GChromoIR;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
