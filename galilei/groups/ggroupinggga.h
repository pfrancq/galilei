/*

	GALILEI Research Project

	GGroupingGGA.h

	Heuristic using a GGA - Header.

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
#ifndef GGroupingGGAH
#define GGroupingGGAH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromcriterion.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupingGGA provides a representation for a method to group some
* subprofiles using a grouping genetic algorithm.
* @author Pascal Francq
* @short GGA Method.
*/
class GGroupingGGA : public GGrouping
{
protected:

	/**
	* Size of the population of the GA.
	*/
	unsigned int PopSize;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Minimum number of common OK documents needed to force two profiles to be
	* in the same group.
	*/
	double MinCommonOK;

	/**
	* Minimum number of common documents judged differently needed to force two
	* profiles to be in two different groups.
	*/
	double MinCommonDiff;

	/**
	* Maximal number of generation to run.
	*/
	unsigned int MaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool Step;

	/**
	* Number of generation for each step.
	*/
	unsigned int StepGen;

	/**
	* Type of measure used for the similarity criterion.
	*/
	SimType SimMeasure;

	/**
	* Parameter for the criterion "Similariry".
	*/
	RPromethee::RPromCriterionParams ParamsSim;

	/**
	* Parameter for the criterion "NB Profiles".
	*/
	RPromethee::RPromCriterionParams ParamsNb;

	/**
	* Parameter for the criterion "OK Factor".
	*/
	RPromethee::RPromCriterionParams ParamsOK;

	/**
	* Parameter for the criterion "Diff Factor".
	*/
	RPromethee::RPromCriterionParams ParamsDiff;

	/**
	* Parameter for the criterion "Social Factor".
	*/
	RPromethee::RPromCriterionParams ParamsSocial;

	/**
	* Global similarities used.
	*/
	bool GlobalSim;

	/**
	* GA Objects.
	*/
	RGA::RObjs<GObjIR>* Objs;

	/**
	* Instance of the GA.
	*/
	GInstIR* Instance;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingGGA(GSession* s) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* "Global PopSize MaxGen StepGen Step MinSimLevel MinCommonOK MinCommonDiff"
	* " ParamsSim.P ParamsSim.Q ParamsSim.Weight ParamsNb.P ParamsNb.Q ParamsNb.Weight "
	* " ParamsOK.P ParamsOK.Q ParamsOK.Weight ParamsDiff.P ParamsDiff.Q ParamsDiff.Weight"
	* @param s*             C string coding the settings.
	*/
	virtual void SetSettings(const char* s);

	/**
	* Set the size of the population.
	* @param s              Size.
	*/
	void SetPopSize(unsigned int s) {PopSize=s;}

	/**
	* Get the size of the population.
	* @returns Size.
	*/
	unsigned int GetPopSize(void) const {return(PopSize);}

	/**
	* Set the type of similarity used.
	* @param t              Type to used.
	*/
	void SetSimMeasure(unsigned int t) {SimMeasure=static_cast<SimType>(t);}

	/**
	* Get the type of similarity used.
	* @returns SimType.
	*/
	SimType GetSimMeasure(void) const {return(SimMeasure);}

	/**
	* Set the minimum level of similarity for the groups.
	* @param m              Minimum level.
	*/
	void SetMinSimLevel(double m) {MinSimLevel=m;}

	/**
	* Get the minimum level of similarity for the groups.
	* @returns double representing the minimum level.
	*/
	double GetMinSimLevel(void) const {return(MinSimLevel);}

	/**
	* Set the minimum number of common OK documents needed to force two
	* profiles to be in the same group.
	* @param m              Minimum level.
	*/
	void SetMinCommonOK(double m) {MinCommonOK=m;}

	/**
	* Get the minimum number of common OK documents needed to force two
	* profiles to be in the same group.
	* @returns double representing the minimum level.
	*/
	double GetMinCommonOK(void) const {return(MinCommonOK);}

	/**
	* Set the minimum number of common documents judged differently needed to
	* force two profiles to be in two different groups.
	* @param m              Minimum level.
	*/
	void SetMinCommonDiff(double m) {MinCommonDiff=m;}

	/**
	* Get the minimum number of common documents judged differently needed to
	* force two profiles to be in two different groups.
	* @returns double representing the minimum level.
	*/
	double GetMinCommonDiff(void) const {return(MinCommonDiff);}

	/**
	* Set the Maximal number of generation.
	* @param max            Maximal number.
	*/
	void SetMaxGen(unsigned int max) {MaxGen=max;}

	/**
	* Get the Maximal number of generation.
	* @returns Maximal number.
	*/
	unsigned int GetMaxGen(void) const {return(MaxGen);}

	/**
	* Set the status of the step mode.
	* @param s              Status.
	*/
	void SetStep(bool s) {Step=s;}

	/**
	* Get the status of the step mode.
	* @returns bool value representing the status.
	*/
	bool GetStep(void) const {return(Step);}

	/**
	* Set the number of generations for each step.
	* @param n              Number of generations.
	*/
	void SetStepGen(unsigned int n) {StepGen=n;}

	/**
	* Get the number of generations for each step.
	* @returns unsigned int representing the number of generations.
	*/
	unsigned int GetStepGen(void) const {return(StepGen);}

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	void SetCriterionParam(const char* crit,double p,double q,double w);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	void GetCriterionParam(const char* crit,double& p,double& q,double& w);

	/**
	* Get the status of the global similarity.
	* @return bool value.
	*/
	bool GetGlobalSim(void) const {return(GlobalSim);}

	/**
	* Set the status of the global similarity.
	* @param s              Global similarity?
	*/
	void SetGlobalSim(bool s) {GlobalSim=s;}

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Construct the groups of the session based on a chromosome.
	*/
	void ConstructGroupsFromChromo(GChromoIR* chromo) throw(bad_alloc);

protected:

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* subprofiles of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp) const;

	/**
	* Test if a group is still coherent if a subprofile is added to it.
	* Actually, this means verify that all the subprofiles of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param sub            SubProfile eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp,const GSubProfile* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp);

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) throw(GException);

public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingGGA(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
