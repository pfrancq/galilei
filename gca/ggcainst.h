/*

	Genetic Community Algorithm

	GGCAInst.h

	Instance - Header

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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
#ifndef GGCAInstH
#define GGCAInstH


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggca.h>
#include <ggcaprom.h>


//-----------------------------------------------------------------------------
/**
* The GGCAThreadData class provides a representation for "thread-dependent" data
* for the IR GA.
* @author Pascal Francq
* @short IR "thread-dependent" Data.
*/
class GGCAThreadData : public R::RThreadDataG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>
{
public:
	
	R::RContainer<GGCAGroup,false,false> ToDel;

	/**
	* Temporary array of Objects.
	*/
	GGCAObj** tmpObjs1;

	/**
	* Temporary array of Objects.
	*/
	GGCAObj** tmpObjs2;

	/**
	* Test Chromosomes.
	*/
	GGCAChromo** Tests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	GGCAProm Prom;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	R::RPromSol** Sols;

	/**
	*/
	unsigned int NbSols;

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GGCAThreadData(GGCAInst* owner);

	/**
	* Initialise thje data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~GGCAThreadData(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the IR problem.
* @author Pascal Francq
* @short IR Instance.
*/
class GGCAInst : public R::RInstG<GGCAInst,GGCAChromo,GGCAFitness,GGCAThreadData,GGCAGroup,GGCAObj>, public GGCAProm
{
	/**
	* Parameter of the GA.
	*/
	GGCAParams* Params;

	/**
	* Solutions corresponding to the chromosome.
	*/
	R::RPromSol** Sols;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Social Profiles.
	*/
	R::RContainer<GGCAObj,false,true> NoSocialProfiles;

	/**
	* Max Ratios for each objects.
	*/
	R::RContainer<GGCAMaxRatios,true,false> Ratios;

#if BESTSOLSVERIFICATION
	/**
	* Container holding the best chromosomes.
	*/
	R::RContainer<GGCAChromo,true,false> BestSols;
#endif

	GMeasure* ProfilesSims;

	GMeasure* ProfilesAgree;

	GMeasure* ProfilesDisagree;

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	*/
	GGCAInst(GSession* ses,RCursor<GGCAObj> objs,GGCAParams* p,R::RDebug *debug);

	/**
	* Initialisation of the instance.
	* @param gdata          The Data to use for the construction of the groups.
	*/
	virtual void Init(void);

	/**
	* Create a specific heuristic for the IR problem.
	*/
	virtual R::RGroupingHeuristic<GGCAGroup,GGCAObj,GGCAChromo>* CreateHeuristic(void);

	/**
	* Get the GA object corresponding to a profile.
	* @param prof            Profile to search for.
	* @ereturns GGCAObj*
	*/
	GGCAObj* GetObj(const GProfile* prof) const;

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);

	/**
	* Write the information of a given chromosome.
	* @param c              Chromosome.
	*/
	void WriteChromoInfo(GGCAChromo* c);

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void);

	/**
	* Get the disagreement ratio between two profiles.
	* @param prof1           First Profile.
	* @param prof2           Second Profile.
	*/
	double GetDisagreementRatio(const GProfile* prof1,const GProfile* prof2) const;

	/**
	* Get the disagreement ratio between two profiles.
	* @param prof1           First Profile.
	* @param prof2           Second Profile.
	*/
	double GetAgreementRatio(const GProfile* prof1,const GProfile* prof2) const;

	/**
	* Compute the similarity between two profiles.
	* @param prof1           Profile n1.
	* @param prof2           Profile n2.
	* @return double
	*/
	double GetSim(const GProfile* prof1,const GProfile* prof2) const;

	/**
	* Compute the similarity between two objects.
	* @param obj1               Object n1.
	* @param obj2               Object n2.
	* @return double
	*/
	double GetSim(const GGCAObj* obj1,const GGCAObj* obj2) const;

	/**
	* This function can be used to do a traitement after the GA stops.
	*/
	virtual void PostRun(void);

	/**
	 *  Called when no handler was found for a given notification.
	 */ 
	virtual void HandlerNotFound(const R::RNotification& notification);
	
	/**
	* Destruct the instance.
	*/
	virtual ~GGCAInst(void);

	// friend classes
	friend class GGCAChromo;
	friend class GGCAGroup;
	friend class GGCAThreadData;
	friend class GGCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif

