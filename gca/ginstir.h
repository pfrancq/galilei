/*

	GALILEI Research Project

	GInstIR.h

	Instance for an IR Problem - Header

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
#ifndef GInstIRH
#define GInstIRH


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gir.h>
#include <girprom.h>
#include <rinstg.h>


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
class GThreadDataIR : public R::RThreadDataG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>
{
public:
	R::RContainer<GGroupIR,false,false> ToDel;

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
	* PROMETHE  Kernel used by the chromosome.
	*/
	GIRProm Prom;

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
	GThreadDataIR(GInstIR* owner);

	/**
	* Initialise thje data.
	*/
	virtual void Init(void);

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
class GInstIR : public R::RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>, public GIRProm
{
	/**
	* Parameter of the GA.
	*/
	GIRParams* Params;

	/**
	* Solutions corresponding to the chromosome.
	*/
	R::RPromSol** Sols;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Language actually grouped.
	*/
	GLang* Lang;

	/**
	* Social Profiles.
	*/
	R::RContainer<GObjIR,false,true> NoSocialSubProfiles;

	/**
	* Max Ratios for each objects.
	*/
	R::RContainer<GIRMaxRatios,true,false> Ratios;

#if BESTSOLSVERIFICATION
	/**
	* Container holding the best chromosomes.
	*/
	R::RContainer<GChromoIR,true,false> BestSols;
#endif

	GMeasure* SubProfilesSims;

	GMeasure* ProfilesAgree;

	GMeasure* ProfilesDisagree;

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param l              Language.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	*/
	GInstIR(GSession* ses,GLang* l,R::RObjs<GObjIR>* objs,GIRParams* p,R::RDebug *debug);

	/**
	* Initialisation of the instance.
	* @param gdata          The Data to use for the construction of the groups.
	*/
	virtual void Init(GGroupDataIR* gdata);

	/**
	* Create a specific heuristic for the IR problem.
	*/
	virtual R::RGroupingHeuristic<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>* CreateHeuristic(void);

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

	/**
	* Write the information of a given chromosome.
	* @param c              Chromosome.
	*/
	void WriteChromoInfo(GChromoIR* c);

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void);

	/**
	* return the Language actually grouped.
	* @return lang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the disagreement ratio between two subprofiles.
	* @param sub1           First SubProfile.
	* @param sub2           Second SubProfile.
	*/
	double GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const;

	/**
	* Get the disagreement ratio between two subprofiles.
	* @param sub1           First SubProfile.
	* @param sub2           Second SubProfile.
	*/
	double GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const;

	/**
	* Compute the similarity between two subprofiles.
	* @param sub1               Subprofile n1.
	* @param sub2               Subprofile n2.
	* @return double
	*/
	double GetSim(const GSubProfile* sub1,const GSubProfile* sub2) const;

	/**
	* Compute the similarity between two objects.
	* @param obj1               Object n1.
	* @param obj2               Object n2.
	* @return double
	*/
	double GetSim(const GObjIR* obj1,const GObjIR* obj2) const;

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
	virtual ~GInstIR(void);

	// friend classes
	friend class GChromoIR;
	friend class GGroupIR;
	friend class GThreadDataIR;
	friend class GIRHeuristic;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

