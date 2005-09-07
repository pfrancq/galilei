/*

	GALILEI Research Project

	GChromoIR.h

	Chromosome for an IR Problem - Header

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
#ifndef GChromoIRH
#define GChromoIRH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rchromog.h>
#include <rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gir.h>
#include <girprom.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GChromoIR class provides a representation for a chromosome for the IR
* Problem.
* @author Pascal Francq
* @short IR Chromosome.
*/
class GChromoIR : public R::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>
{
private:

	R::RContainer<GGroupIR,false,false>* ToDel;

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
	GObjIR** thObjs1;

	/**
	* Number of objects in thObjs1.
	*/
	unsigned int NbObjs1;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GObjIR** thObjs2;

	/**
	* Number of objects in thObjs2.
	*/
	unsigned int NbObjs2;

	/**
	* Prototypes used for the KMeans.
	*/
	R::RContainer<GObjIR,false,false> Protos;

	/**
	* Test Chromosome (Thread dependent data).
	*/
	GChromoIR** thTests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	GIRProm* thProm;

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
	GChromoIR(GInstIR* inst,unsigned int id) throw(std::bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data.
	*/
	virtual void Init(GThreadDataIR* thData) throw(std::bad_alloc);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GChromoIR* c) const;

	/**
	* Construct the chromosome to be the same as grps.
	*/
	void ConstructChromo(GSession* grps) throw(R::eGA);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual void RandomConstruct(void) throw(R::eGA);

	/**
	* Evaluation of the chromosome.
	*/
	virtual void Evaluate(void) throw(R::eGA);

	/**
	*  Reallocate the objects to the groups based on the different prototypes.
	*/
	void ReAllocate(void) throw(R::eGA);

	/**
	* Calc the number of new prototypes until the last K-Means iteration.
	*/
	unsigned int CalcNewProtosNb(void) throw(R::eGA);

	/**
	* Perform a K-Means on the chromosome.
	*/
	void DoKMeans(void) throw(R::eGA);

	/**
	* Divide the group containing the two most dissimilar subprofiles.
	*/
	void DivideWorstSubProfiles(void) throw(R::eGA);

	/**
	* Merge the two groups containing the two most similar subprofiles.
	*/
	void MergeBestSubProfiles(void) throw(R::eGA);

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
	virtual void Optimisation(void) throw(R::eGA);

	/**
	* Do the standard mutation of the GGA.
	*/
	virtual void Modify(void) throw(R::eGA);

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
	GChromoIR& operator=(const GChromoIR& chromo);

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
	virtual ~GChromoIR(void);

	// friend classes
	friend class GGroupIR;
	friend class GInstIR;
	friend class GIRProm;
	friend class GIRHeuristic;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
