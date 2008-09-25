	/*

	Genetic Community Algorithm

	GCAChromo.h

	Chromosome - Header

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#ifndef GCAChromoH
#define GCAChromoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rchromog.h>
#include <rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>
#include <gcaprom.h>


//-----------------------------------------------------------------------------
/**
* The GCAChromo class provides a representation for a chromosome for the IR
* Problem.
* @author Pascal Francq
* @short IR Chromosome.
*/
class GCAChromo : public R::RChromoG<GCAInst,GCAChromo,GCAFitness,GCAThreadData,GCAGroup,GCAObj>
{
private:

	R::RContainer<GCAGroup,false,false>* ToDel;

	/**
	* Value of the Similarity criterion "J" :
	* (Average Similarity inside each group)/(Maximal similarity between centroids+2) .
	*/
	double CritSimJ;

	/**
	* Value of the "Agreement" criterion.
	*/
	double CritAgreement;

	/**
	* Value of the "Disagreement" criterion.
	*/
	double CritDisagreement;

	/**
	* Fi minus value.
	*/
	double FiMinus;

	/**
	* Fi minus value.
	*/
	double FiPlus;

	/**
	* Fi minus value.
	*/
	double Fi;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GCAObj** thObjs1;

	/**
	* Number of objects in thObjs1.
	*/
	size_t NbObjs1;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GCAObj** thObjs2;

	/**
	* Number of objects in thObjs2.
	*/
	size_t NbObjs2;

	/**
	* Prototypes used for the KMeans.
	*/
	R::RContainer<GCAObj,false,false> Protos;

	/**
	* Test Chromosome (Thread dependent data).
	*/
	GCAChromo** thTests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	GCAProm* thProm;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	R::RPromSol** thSols;

	/**
	 * Identifier of the first of the two most similar centroid.
	 */
	size_t MostSimilarGroup1;

	/**
	 * Identifier of the second of the two most similar centroid.
	 */
	size_t MostSimilarGroup2;

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identifier of the chromosome.
	*/
	GCAChromo(GCAInst* inst,size_t id);


	/**
	* Initialization of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data.
	*/
	virtual void Init(GCAThreadData* thData);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GCAChromo* c) const;

	/**
	* Construct the chromosome based on existing groups.
	* @param groups          Cursor over the groups.
	*/
	template<class cObj,class cGroup> void ConstructChromo(R::RCursor<cGroup> groups);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void);

	/**
	* Evaluation of the chromosome.
	*/
	virtual void Evaluate(void);

	/**
	*  Reallocate the objects to the groups based on the different prototypes.
	*/
	void ReAllocate(void);

	/**
	* Compute the number of new prototypes until the last K-Means iteration.
	*/
	size_t CalcNewProtosNb(void);

	/**
	* Perform a K-Means on the chromosome.
	*/
	void DoKMeans(void);

	/**
	* Divide the group containing the two most dissimilar objects.
	*/
	void DivideWorstObjects(void);

	/**
	* Merge the two groups containing the two most similar centroids.
	*/
	void MergeBestGroups(void);

	/**
	* Treat the social objects, i.e. if these objects are alone in a group, put
	* them in another group.
	*/
	void TreatSocialObjects(void);

	/**
	 * kMeans-based Optimization.
	 */
	void kMeansOptimisation(void);

	/**
	* Perform an optimization.
	*/
	virtual void Optimisation(void);

	/**
	* Do a mutation of the chromosome. Since the GCA seems to over-group, the
	* mutation merge the two groups containing the most similar centroids.
	*/
//	virtual void Mutation(void);

	/**
	* Look if two objects are in the same group or not.
	* @param obj1            Identifier of the first object.
	* @param obj2            Identifier of the second object.
	* @return true if they are in the same group, else false.
	*/
	bool SameGroup(size_t obj1,size_t obj2) const;

	/**
	* The assignment operator.
	* @param chromo          Chromosome used as source.
	*/
	GCAChromo& operator=(const GCAChromo& chromo);

	/**
	* Get the value of the 'J' measure.
	* @returns double.
	*/
	double GetSimJ(void) {return(CritSimJ);}

	/**
	* Get the value of the agreement criterion for the chromosome.
	* @returns double.
	*/
	double GetAgreementCriterion(void) const {return(CritAgreement);}

	/**
	* Get the value of the disagreement criterion for the chromosome.
	* @returns double.
	*/
	double GetDisagreementCriterion(void) const {return(CritDisagreement);}

	/**
	* @return Fi of the solution.
	*/
	double GetFi(void) const {return(Fi);}

	/**
	* @return Fi+ of the solution.
	*/
	double GetFiPlus(void) const {return(FiPlus);}

	/**
	* @return Fi- of the solution.
	*/
	double GetFiMinus(void) const {return(FiMinus);}

	/**
	* Destruct the chromosome.
	*/
	virtual ~GCAChromo(void);

	// friend classes
	friend class GCAGroup;
	friend class GCAInst;
	friend class GCAProm;
	friend class GCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif
