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
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rinstg.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>
#include <groups/girprom.h>
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
	* PROMETHE  Kernel used by the chromosome.
	*/
	GIRProm Prom;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	RPromethee::RPromSol** Sols;

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
class GInstIR : public RGGA::RInstG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>, public GIRProm
{
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
	* Solutions corresponding to the chromosome.
	*/
	RPromethee::RPromSol** Sols;

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
	* Social Profiles.
	*/
	RStd::RContainer<GObjIR,unsigned int,false,true> NoSocialSubProfiles;

#if GALILEITEST
	/**
	* Ideal Groups.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true>* IdealGroups;
#endif

#if BESTSOLSVERIFICATION
	/**
	* Container holding the best chromosomes.
	*/
	RStd::RContainer<GChromoIR,unsigned int,true,false> BestSols;
#endif

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param l              Language.
	* @param grps           Pointer to the current solutions.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	*/
	GInstIR(GSession* ses,GLang* l,GGroups* grps,RGA::RObjs<GObjIR>* objs,GIRParams* p,RGA::RDebug *debug) throw(bad_alloc);

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param l              Language.
	* @param grps           Pointer to the current solutions.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param pop            Population size.
	* @param debug          Debugger.
	*/
	GInstIR(GSession* ses,GLang* l,GGroups* grps,RGA::RObjs<GObjIR>* objs,GIRParams* p,unsigned int pop,RGA::RDebug *debug) throw(bad_alloc);

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

#ifdef GALILEITEST

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
	* return the Language actually grouped.
	* @return lang.
	*/
	GLang* GetLang(void) const {return(Lang);}

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
	* Compute the similarity between two subprofiles.
	* @param sub1               Subprofile n°1.
	* @param sub2               Subprofile n°2.
	* @return double
	*/
	double GetSim(GSubProfile* sub1,GSubProfile* sub2);

	/**
	* Compute the similarity between two objects.
	* @param obj1               Object n°1.
	* @param obj2               Object n°2.
	* @return double
	*/
	double GetSim(GObjIR* obj1,GObjIR* obj2);

	/**
	* This function can be used to do a traitement after the GA stops.
	*/
	virtual void PostRun(void);

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

