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
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Value of the "Similarity" criterion.
	*/
	double CritSim;

	/**
	* Value of the "Information" criterion.
	*/
	double CritInfo;

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
	* Value of the "Similarity" criterion with local changes.
	*/
	double LocalCritSim;

	/**
	* Value of the "Information" criterion with local changes.
	*/
	double LocalCritInfo;

	/**
	* Value of the "Same Feedbacks" criterion with local changes.
	*/
	double LocalCritSameFeedbacks;

	/**
	* Value of the "Diff Feedbacks" criterion with local changes.
	*/
	double LocalCritDiffFeedbacks;

	/**
	* Value of the "Social" criterion with local changes.
	*/
	double LocalCritSocial;

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
	bool IsInGroup(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the sum of the similarities of a given subprofile to all the
	* others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the sum of the similarities of a given subprofile to all the
	* others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GObjIR* obj);

	/**
	* Compute the sum of the distances of a given subprofile to all the others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumDist(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the maximum similarity of a given subprofile to all the others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeMaxSim(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the minimum similarity of a given subprofile to all the others.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeMinSim(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the minimum similarity inside a list of subprofiles.
	* @param obj            subprofile used as reference.
	* @returns result.
	*/
	double ComputeMinSim(GObjIR** grp,unsigned int nb);

	/**
	* Compute the maximum similarity between subprofiles of two groups.
	* @param rgrp           The reference list of subprofiles.
	* @param rnb            Number of reference subprofiles.
	* @param grp            The group used for the comparison.
	* @returns result.
	*/
	double ComputeMaxSim(GObjIR** rgrp,unsigned int rnb,GGroupIR* grp);

	/**
	* Compute the maximum similarity between subprofiles of a group and a list
	* of subprofiles.
	* @param rgrp           The reference list of subprofiles.
	* @param rnb            Number of reference subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @returns result.
	*/
	double ComputeMaxSim(GObjIR** rgrp,unsigned int rnb,GObjIR** grp,unsigned int nb);

	/**
	* Compute the relevant subprofile of the group, i.a. the subprofile which
	* is the most similar to all the others subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @rel                  Relevant subprofile computed.
	* @returns Sum of similarities to the relevant subprofile.
	*/
	double ComputeRelevantSum(GObjIR** grp,unsigned int nb,GObjIR*  &rel);

	/**
	* Compute the relevant subprofile of the group, i.a. the subprofile which
	* is the most similar to all the others subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @rel                  Relevant subprofile computed.
	* @returns Sum of distances to the relevant subprofile.
	*/
	double ComputeRelevantSumDist(GObjIR** grp,unsigned int nb,GObjIR*  &rel);

	/**
	* Compute the relevant subprofile of the group, i.a. the subprofile which
	* is the most similar to all the others subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @rel                  Relevant subprofile computed.
	*/
	void ComputeRelevant(GObjIR** grp,unsigned int nb,GObjIR*  &rel);

	/**
	* Compute the most relevant subprofile of the chromosome, i.a. the
	* subprofile which is the most similar to all the others subprofiles.
	* @return Pointer to the most relevant one.
	*/
	GObjIR* ComputeGlobalRelevant(void);

	/**
	* Compute the relevant subprofile of the group, i.a. the subprofile which
	* is the most similar to all the others subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @rel                  Relevant subprofile computed.
	* @returns Minimum of similarities to the relevant subprofile.
	*/
	double ComputeRelevantMin(GObjIR** grp,unsigned int nb,GObjIR*  &rel);

	/**
	* Compute the relevant subprofile of the group, i.a. the subprofile which
	* is the most similar to all the others profiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @rel                  Relevant subprofile computed.
	* @returns Maximum similarity to the relevant subprofile.
	*/
	double ComputeRelevantMax(GObjIR** grp,unsigned int nb,GObjIR*  &rel);

	/**
	* Evaluation of the chromosome and the changed one (OldNew).
	* @param nbcrit         Minimal number of criteria to be ameliorated. If
	*                       null, use PROMETHEE to evaluate.
	* @return true if the new one seems to be the best.
	*/
	bool EvaluateOldNew(unsigned int nbcrit);

	/**
	* Look if two groups were merged together.
	* @param grp1           First group.
	* @param grp2           Second group.
	* @param nbcrit         Minimal number of criteria to be ameliorated. If
	*                       null, use PROMETHEE to evaluate.
	* @return bool.
	*/
	bool MergeGroups(GGroupIR* grp1,GGroupIR* grp2,unsigned int nbcrit);

	/**
	* Look if a group was be divided.
	* @param grp            Group to analyse.
	* @param nbcrit         Minimal number of criteria to be ameliorated. If
	*                       null, use PROMETHEE to evaluate.
	* @return bool.
	*/
	bool DivideGroup(GGroupIR* grp,unsigned int nbcrit);

	/**
	* Construct the chromosome to be the same as grps.
	*/
	void ConstructChromo(GGroups* grps);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "AvgSim" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateAvgSim(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "J" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateJ(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "AvgRatio" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateAvgRatio(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "MinRatio" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateMinRatio(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "Ratio" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateRatio(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "WOverB" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateWOverB(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the similarity of the solution where eventually grp1 must be
	* devided, or grp1 and grp2 must be merged using the "SimWB" measure.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateSimWB(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the 'Similarity' criterion of the chromosome where eventually
	* grp1 must be devided, or grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateSim(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the 'Info' criterion of the chromosome where eventually where
	* must be devided, or grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateInfo(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the 'Same Feebacks' criterion of the chromosome where eventually
	* grp1 must be devided, or grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateSameFeedbacks(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the 'Diff Feebacks' criterion of the chromosome where eventually
	* grp1 must be devided, or grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateDiffFeedbacks(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluate the 'Social' criterion of the chromosome where eventually grp1
	* must be devided, or grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void EvaluateSocial(GGroupIR* grp1=0,GGroupIR* grp2=0);

	/**
	* Evaluation of the chromosome where eventually grp1 must be devided, or
	* grp1 and grp2 must be merged.
	* @param grp1           First group.
	* @param grp2           Second group.
	*/
	void Evaluate(GGroupIR* grp1,GGroupIR* grp2);

	/**
	* Evaluation of the chromosome.
	*/
	virtual void Evaluate(void);

	/**
	* Try some divisions.
	* @param nbcrit         Minimal number of criteria to be ameliorated. If
	*                       null, use PROMETHEE to evaluate.
	* @param true if at least a division was done.
	*/
	bool TryDivisions(unsigned int nbcrit=0);

	/**
	* Try some merges.
	* @param nbcrit         Minimal number of criteria to be ameliorated. If
	*                       null, use PROMETHEE to evaluate.
	* @param true if at least a merge was done.
	*/
	bool TryMerges(unsigned int nbcrit=0);

	/**
	* Perform an optimisation.
	*/
	virtual void Optimisation(void);

	/**
	* Do the standard crossover of the GGA and do a reorganisation after.
	*/
	virtual bool Crossover(GChromoIR* parent1,GChromoIR* parent2);

	/**
	* Do the standard mutation of the GGA and do a reorganisation after.
	*/
	virtual bool Mutation(void);

	/**
	* Do the standard mutation of the GGA.
	*/
	virtual bool Modify(void);

	/**
	* Does a reorganisation of the chromosome.
	*/
	void ReOrganisation(void);

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
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
