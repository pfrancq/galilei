/*

	GALILEI Research Project

	GChromoIR.h

	Chromosome for an IR Problem - Header

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GChromoIRH
#define GChromoIRH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rchromog.h>
#include <rpromethee/rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>
#include <groups/girprom.h>


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
class GChromoIR : public RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>
{
private:
	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSim* Sims;

	/**
	* Value of the "Similarity" criterion.
	*/
	double CritSim;

	/**
	* Value of the Similarity criterion "AvgSim".
	*/
	double CritSimAvgSim;

	/**
	* Value of the Similarity criterion "J".
	*/
	double CritSimJ;

	/**
	* Value of the Similarity criterion "AvgRatio".
	*/
	double CritSimAvgRatio;

	/**
	* Value of the Similarity criterion "AvgMinRatio".
	*/
	double CritSimMinRatio;

	/**
	* Value of the Similarity criterion "AvgRatio".
	*/
	double CritSimRatio;

	/**
	* Value of the Similarity criterion "AvgWOverB".
	*/
	double CritSimWOverB;

	/**
	* Value of the Similarity criterion "SimWB".
	*/
	double CritSimSimWB;

	/**
	* Value of the "Information" criterion.
	*/
	double CritInfo;

	/**
	* Value of the "Entropy" criterion.
	*/
	double CritEntropy;

	/**
	* Value of the "Same Feedbacks" criterion.
	*/
	double CritSameFeedbacks;

	/**
	* Value of the "Diff Feedbacks" criterion.
	*/
	double CritDiffFeedbacks;

	/**
	* Value of the "Social" criterion.
	*/
	double CritSocial;

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
	RContainer<GObjIR,tId,false,true> Protos;

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
	RPromethee::RPromSol** thSols;

	/**
	* Container of document.
	*/
	RStd::RContainer<GDocSim,unsigned,true,false> Docs;

#ifdef RGADEBUG

	/**
	* Global Factor	
	*/
	double Global;

	/**
	* Global Factor	
	*/
	double Precision;

	/**
	* Global Factor	
	*/
	double Recall;

#endif

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identificator of the chromosome.
	*/
	GChromoIR(GInstIR* inst,unsigned int id) throw(bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data.
	*/
	virtual void Init(GThreadDataIR* thData) throw(bad_alloc);

	/**
	* Method needed by RStd::RContainer.
	*/
	int Compare(const GChromoIR* c) const;

#ifdef RGADEBUG

	/**
	* Get the global value for the chromosome.
	*/
	double GetGlobal(void) const {return(Global);}

	/**
	* Get the precision value for the chromosome.
	*/
	double GetPrecision(void) const {return(Precision);}

	/**
	* Get the global value for the chromosome.
	*/
	double GetRecall(void) const {return(Recall);}

	/**
	* Make a comparision with the ideal groupement and compute the recall,
	* precision and global measure for the current chromosome.
	* @param s              Session.
	* param ideal           Ideal grouprement.
	*/
	void CompareIdeal(GSession* s,RStd::RContainer<GGroups,unsigned int,true,true>* ideal);

#endif

	/**
	* Compute the sum of the similarities of a given subprofile to all the
	* others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GObjIR* obj);

	/**
	* Compute the most relevant subprofile of the chromosome, i.a. the
	* subprofile which is the most similar to all the others subprofiles.
	* @return Pointer to the most relevant one.
	*/
	GObjIR* ComputeGlobalRelevant(void);

	/**
	* Construct the chromosome to be the same as grps.
	*/
	void ConstructChromo(GGroups* grps) throw(RGA::eGA);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual void RandomConstruct(void) throw(RGA::eGA);

	/**
	* Evaluate the similarity of the solution using the "AvgSim" measure.
	*/
	double EvaluateAvgSim(void);

	/**
	* Evaluate the similarity using the "J" measure.
	*/
	double EvaluateJ(void);

	/**
	* Evaluate the similarity of the solution using the "AvgRatio" measure.
	*/
	double EvaluateAvgRatio(void);

	/**
	* Evaluate the similarity of the solution using the "MinRatio" measure.
	*/
	double EvaluateMinRatio(void);

	/**
	* Evaluate the similarity of the solution using the "Ratio" measure.
	*/
	double EvaluateRatio(void);

	/**
	* Evaluate the similarity of the solution using the "WOverB" measure.
	*/
	double EvaluateWOverB(void);

	/**
	* Evaluate the similarity of the solution using the "SimWB" measure.
	*/
	double EvaluateSimWB(void);

	/**
	* Compute a similarity criterion.
	* @param s              Similarity Measure.
	*/
	void EvaluateSim(GSimMeasure* s);

	/**
	* Evaluate the 'Similarity' criterion of the chromosome.
	*/
	void EvaluateSim(void);

	/**
	* Evaluate the 'Info' criterion of the chromosome.
	*/
	void EvaluateInfo(void);

	/**
	* Evaluate the 'Entropy' criterion of the chromosome.
	*/
	void EvaluateEntropy(void);

	/**
	* Evaluate the 'Same Feebacks' criterion of the chromosome.
	*/
	void EvaluateSameFeedbacks(void);

	/**
	* Evaluate the 'Diff Feebacks' criterion.
	*/
	void EvaluateDiffFeedbacks(void);

	/**
	* Evaluate the 'Social' criterion of the chromosome.
	*/
	void EvaluateSocial(void);

	/**
	* Evaluation of the chromosome.
	*/
	virtual void Evaluate(void) throw(RGA::eGA);

	/**
	*  Reallocate the objects to the groups based on the different prototypes.
	*/
	void ReAllocate(void) throw(RGA::eGA);

	/**
	* Calc the number of new prototypes until the last K-Means iteration.
	*/
	unsigned int CalcNewProtosNb(void) throw(RGA::eGA);

	/**
	* Perform a K-Means on the chromosome.
	*/
	void DoKMeans(void) throw(RGA::eGA);

	/**
	* Divide the group containing the two most dissimilar subprofiles.
	*/
	void DivideWorstSubProfiles(void) throw(RGA::eGA);

	/**
	* Merge the two groups containing the two most similar subprofiles.
	*/
	void MergeBestSubProfiles(void) throw(RGA::eGA);

	/**
	* Perform an optimisation.
	*/
	virtual void Optimisation(void) throw(RGA::eGA);

	/**
	* Do the standard mutation of the GGA.
	*/
	virtual void Modify(void) throw(RGA::eGA);

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
	* Get the value of the similarity criterion for the chromosome.
	* @returns double.
	*/
	double GetSimCriterion(void) const {return(CritSim);}

	/**
	* Get the value of the information criterion for the chromosome.
	* @returns double.
	*/
	double GetInfoCriterion(void) const {return(CritInfo);}

	/**
	* Get the value of the same criterion for the chromosome.
	* @returns double.
	*/
	double GetSameFeedbacksCriterion(void) const {return(CritSameFeedbacks);}

	/**
	* Get the value of the diff criterion for the chromosome.
	* @returns double.
	*/
	double GetDiffFeedbacksCriterion(void) const {return(CritDiffFeedbacks);}

	/**
	* Get the value of the social criterion for the chromosome.
	* @returns double.
	*/
	double GetSocialCriterion(void) const {return(CritSocial);}

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
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
