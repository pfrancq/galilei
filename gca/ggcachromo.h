/*

	Genetic Community Algorithm

	GGCAChromo.h

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
#ifndef GGCAChromoH
#define GGCAChromoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rchromog.h>
#include <rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggca.h>
#include <ggcaprom.h>


//-----------------------------------------------------------------------------
/**
* The GGCAChromo class provides a representation for a chromosome for the IR
* Problem.
* @author Pascal Francq
* @short IR Chromosome.
*/
class GGCAChromo : public R::RChromoG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>
{
private:

	R::RContainer<GGCAGroup,false,false>* ToDel;

	/**
	* Value of the Similarity criterion "J".
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
	GGCAObj** thObjs1;

	/**
	* Number of objects in thObjs1.
	*/
	unsigned int NbObjs1;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GGCAObj** thObjs2;

	/**
	* Number of objects in thObjs2.
	*/
	unsigned int NbObjs2;

	/**
	* Prototypes used for the KMeans.
	*/
	R::RContainer<GGCAObj,false,false> Protos;

	/**
	* Test Chromosome (Thread dependent data).
	*/
	GGCAChromo** thTests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	GGCAProm* thProm;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	R::RPromSol** thSols;

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identificator of the chromosome.
	*/
	GGCAChromo(GGCAInst* inst,unsigned int id);


	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data.
	*/
	virtual void Init(GGCAThreadData* thData);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GGCAChromo* c) const;

	/**
	* Construct the chromosome to be the same as grps.
	*/
	void ConstructChromo(GSession* grps);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
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
	* Calc the number of new prototypes until the last K-Means iteration.
	*/
	unsigned int CalcNewProtosNb(void);

	/**
	* Perform a K-Means on the chromosome.
	*/
	void DoKMeans(void);

	/**
	* Divide the group containing the two most dissimilar subprofiles.
	*/
	void DivideWorstSubProfiles(void);

	/**
	* Merge the two groups containing the two most similar subprofiles.
	*/
	void MergeBestSubProfiles(void);

	/**
	* Treat the social subprofiles, i.e. if these subprofiles are alone in a
	* group, put them in another group.
	* @param rel                If true, the group is chosen as the one having the
	*                           closest relevant subprofile. If false, the average
	*                           similarity is used.
	*/
	void TreatSocialSubProfiles(bool rel);

	/**
	* Perform an optimisation.
	*/
	virtual void Optimisation(void);

	/**
	* Do the standard mutation of the GGA.
	*/
	virtual void Modify(void);

	/**
	* Look if two subprofiles are in the same group or not.
	* @param obj1           Identificator of the first subprofile.
	* @param obj2           Identificator of the second subprofile.
	* @return true if they are in the same group, else false.
	*/
	bool SameGroup(unsigned int obj1,unsigned int obj2) const;

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	GGCAChromo& operator=(const GGCAChromo& chromo);

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
	* Destructor.
	*/
	virtual ~GGCAChromo(void);

	// friend classes
	friend class GGCAGroup;
	friend class GGCAInst;
	friend class GGCAProm;
	friend class GGCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif
