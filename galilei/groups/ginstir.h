/*

	GALILEI Research Project

	GInstIR.h

	Instance for an IR Problem - Header

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef GInstIRH
#define GInstIRH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rinstg.h>
#include <rpromethee/rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>
#include <rgga/rinstg.h>


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
	* Temporary array of Objects.
	*/
	GObjIR** tmpObjs1;

	/**
	* Temporary array of Objects.
	*/
	GObjIR** tmpObjs2;

	/**
	* Test Chromosomes.
	*/
	GChromoIR** Tests;

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GThreadDataIR(GInstIR* owner);

	/**
	* Initialise thje data.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Destruct the data.
	*/
	virtual ~GThreadDataIR(void);
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
	* Couples of subprofiles having same feedbacks on common documents.
	*/
	RStd::RContainer<GSubProfilesSameDocs,unsigned int,true,true> SameFeedbacks;

	/**
	* Couples of subprofiles having different feedbacks on common documents.
	*/
	RStd::RContainer<GSubProfilesSameDocs,unsigned int,true,true> DiffFeedbacks;

	/**
	* Parameter of the GA.
	*/
	GIRParams* Params;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* The maximum number of generations.
	*/
	unsigned long MaxGen;

	/**
	* Maximal Number of KMeans.
	*/
	unsigned int MaxKMeans;

	/**
	* Type of measure used for the similarity criterion.
	*/
	SimType SimMeasure;

	/**
	* Criteria representing the average similarity of the profiles with the
	* ones of the same group.
	*/
	RPromethee::RPromCriterion* CritSim;

	/**
	* Criteria representing the average number of profiles per groups.
	*/
	RPromethee::RPromCriterion* CritInfo;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common OK documents.
	*/
	RPromethee::RPromCriterion* CritSameFeedbacks;

	/**
	* Criteria representing the factor depending on the subprofiles having
	* common documents but with opposite judgement.
	*/
	RPromethee::RPromCriterion* CritDiffFeedbacks;

	/**
	* Criteria representing the factor depending on the subprofiles that are
	* social.
	*/
	RPromethee::RPromCriterion* CritSocial;

	/**
	* Solutions corresponding to the chromosome.
	*/
	RPromethee::RPromSol** Sols;

	/**
	* Global Similarities.
	*/
	bool GlobalSim;

	/**
	* Pointer to the current clustering in the session.
	*/
	GGroups* CurrentGroups;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Language actually grouped.
	*/
	GLang* Lang;

	/**
	* Minimum number of common OK documents needed to force two profiles to be
	* in the same group.
	*/
	double MinCommonSame;

	/**
	* Minimum number of common documents judged differently needed to force two
	* profiles to be in two different groups.
	*/
	double MinCommonDiff;
	
	/**
	* Social Profiles.
	*/
	RStd::RContainer<GObjIR,unsigned int,false,true> NoSocialSubProfiles;

#ifdef RGADEBUG

	/**
	* Ideal Groups.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true>* IdealGroups;

#endif

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param l              Language.
	* @param m              Minimal similarity in a group.
	* @param max            Maximal number of generations.
	* @param popsize        The size of the population.
	* @param grps           Pointer to the current solutions.
	* @param objs           The objects to group.
	* @param s              Similarities between the subprofiles.
	* @param h              The type of heuristic to be used.
	* @param debug          Debugger.
	*/
	GInstIR(GSession* ses,GLang* l,double m,unsigned int max,unsigned int popsize,GGroups* grps,RGA::RObjs<GObjIR>* objs,bool g,GProfilesSim* s,SimType st,RGA::RDebug *debug=0) throw(bad_alloc);

	/**
	* Create a specific heuristic for the IR problem.
	*/
	virtual RGGA::RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* CreateHeuristic(void) throw(bad_alloc);

	/**
	* Get the GA object corresponding to a subprofile.
	* @param sub            SubProfile to search for.
	* @ereturns GObjIR*
	*/
	GObjIR* GetObj(const GSubProfile* sub) const;

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);

#ifdef RGADEBUG

	/**
	* Write the information of a given chromosome.
	* @param c              Chromosome.
	*/
	void WriteChromoInfo(GChromoIR* c);

	/**
	* Set the ideal groups.
	* @param ideal          Pointer to the ideal groups.
	*/
	void SetIdealGroups(RStd::RContainer<GGroups,unsigned int,true,true>* ideal) {IdealGroups=ideal;}

#endif

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void) throw(RGA::eGA);

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
	* return the Language actually grouped.
	* @return lang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Set the maximal number of generations to run.
	* @param max            Maximal number of generations.
	*/
	void SetMaxGen(unsigned int max);

	/**
	* Get the ratio of the number of documents judged differently by two
	* subprofiles over the total number of common documents judged.
	* @param sub1           First SubProfile.
	* @param sub2           Second SubProfile.
	*/
	double GetRatioDiff(GSubProfile* sub1,GSubProfile* sub2) const;

	/**
	* Get the ratio of the number of documents judged identically by two
	* subprofiles over the total number of common documents judged.
	* @param sub1           First SubProfile.
	* @param sub2           Second SubProfile.
	*/
	double GetRatioSame(GSubProfile* sub1,GSubProfile* sub2) const;

	/**
	* Set the minimal ratios used by the GA.
	*/
	void SetMinRatios(double same,double diff);

	/**
	* Get the minimum ratio of common OK documents needed to force two
	* subprofiles to be in the same group.
	* @return double.
	*/
	double GetMinCommonSame(void) const {return(MinCommonSame);}

	/**
	* Get the minimum ratio of common documents judged differently needed to
	* force two subprofiles to be in two different groups.
	* @return double.
	*/
	double GetMinCommonDiff(void) const {return(MinCommonDiff);}

	/**
	* Set the maximal number of K-Means iterations.
	* @param i              Maximal number of iterations.
	*/
	void SetMaxKMeans(unsigned int i) {MaxKMeans=i;}

	/**
	* Get the maximal number of K-Means iterations.
	* @returns unsigned int representing the maxiaml number of iterations.
	*/
	unsigned int GetMaxKMeans(void) const {return(MaxKMeans);}

	/**
	* Destruct the instance.
	*/
	virtual ~GInstIR(void);

	// friend classes
	friend class GChromoIR;
	friend class GGroupIR;
	friend class GThreadDataIR;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

