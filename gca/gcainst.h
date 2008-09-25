/*

	Genetic Community Algorithm

	GCAInst.h

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
#ifndef GCAInstH
#define GCAInstH


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>
#include <gcaprom.h>



//-----------------------------------------------------------------------------
/**
* The GCAThreadData class provides a representation for "thread-dependent" data
* for the IR GA.
* @author Pascal Francq
* @short IR "thread-dependent" Data.
*/
class GCAThreadData : public R::RThreadDataG<GCAInst,GCAChromo,GCAFitness,GCAThreadData,GCAGroup,GCAObj>
{
public:

	R::RContainer<GCAGroup,false,false> ToDel;

	/**
	* Temporary array of Objects.
	*/
	GCAObj** tmpObjs1;

	/**
	* Temporary array of Objects.
	*/
	GCAObj** tmpObjs2;

	/**
	* Test Chromosomes.
	*/
	GCAChromo** Tests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	GCAProm Prom;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	R::RPromSol** Sols;

	/**
	*/
	size_t NbSols;

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GCAThreadData(GCAInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~GCAThreadData(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the IR problem.
* @author Pascal Francq
* @short IR Instance.
*/
class GCAInst : public R::RInstG<GCAInst,GCAChromo,GCAFitness,GCAThreadData,GCAGroup,GCAObj>, public GCAProm
{
	/**
	* Parameter of the GA.
	*/
	GCAParams* Params;

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
	R::RContainer<GCAObj,false,true> NoSocialProfiles;

	/**
	* Max Ratios for each objects.
	*/
	R::RContainer<GCAMaxRatios,true,false> Ratios;

	/*
	 * Similarity measure to used.
	 */
	GMeasure* Sims;

	/*
	 * Agreement measure to used.
	 */
	GMeasure* Agree;

	/*
	 * Disagreement measure to used.
	 */
	GMeasure* Disagree;

	/**
	 * Type of the elements to group.
	 */
	tObjType Type;

	/**
	 * Incremental mode.
	 */
	bool Incremental;

#if BESTSOLSVERIFICATION
	/**
	* Container holding the best chromosomes.
	*/
	R::RContainer<GCAChromo,true,false> BestSols;
#endif

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	* @param type           Type of the element to group.
	* @param mes            Name of the measures to used (ex: "Documents",
	*                       "Profiles", etc.).
	* @param inc            Incremental mode (The GCA constructs the
	*                       chromosomes from existing solutions).
	*/
	GCAInst(GSession* ses,R::RCursor<GCAObj> objs,GCAParams* p,R::RDebug* debug,tObjType type,const R::RString& mes,bool inc);

	/**
	* Initialisation of the instance.
	* @param gdata          The Data to use for the construction of the groups.
	*/
	virtual void Init(void);

	/**
	* Create a specific heuristic for the IR problem.
	*/
	virtual R::RGroupingHeuristic<GCAGroup,GCAObj,GCAChromo>* CreateHeuristic(void);

	/**
	* Get the GA object corresponding to an element.
	* @param id              Identifier of the element.
	* @ereturns GCAObj*
	*/
	GCAObj* GetObj(size_t id) const;

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
	void WriteChromoInfo(GCAChromo* c);

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	double GetDisagreementRatio(size_t element1,size_t element2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	double GetAgreementRatio(size_t element1,size_t element2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	double GetSim(size_t element1,size_t element2) const;

	/**
	* This function can be used to do a treatment after the GA stops.
	*/
	virtual void PostRun(void);

	/**
	 *  Called when no handler was found for a given notification.
	 */
	virtual void HandlerNotFound(const R::RNotification& notification);

	/**
	* Destruct the instance.
	*/
	virtual ~GCAInst(void);

	// friend classes
	friend class GCAChromo;
	friend class GCAGroup;
	friend class GCAThreadData;
	friend class GCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif

