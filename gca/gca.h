/*

	Genetic Community Algorithm

	GCA.h

	Main - Header.

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
#ifndef GCAH
#define GCAH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rpromcriterion.h>
#include <rfitness.h>
#include <rgga.h>
#include <rparam.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Forward class declaration
class GCAGroup;
class GCAThreadData;
class GCAChromo;
class GCAInst;
class GCAObj;
class GCAHeuristic;
class GCAParams;


//-----------------------------------------------------------------------------
/**
* The GCAFitness provides a representation for a fitness function for the
* information retrieval problem.
* @author Pascal Francq
* @short IR Fitness.
*/
class GCAFitness : public R::RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used for the Bin Packing.
	*/
	GCAFitness(void) : R::RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	GCAFitness& operator=(const GCAFitness &f)
	{
		R::RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GCAFitness& operator=(const double val)
	{
		R::RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/**
* The GCAParam represents all the parameter used in the GCA module.
* @short GCA Parameters.
*/
class GCAParams
{
public:

	/**
	* Size of the population of the GA.
	*/
	size_t PopSize;

	/**
	* Maximal number of generation to run.
	*/
	size_t MaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool Step;

	/**
	* Number of generation for each step.
	*/
	size_t StepGen;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Minimum threshold for the agreement ratio.
	*/
	double MinAgreement;

	/**
	* Minimum threshold for the disagreement ratio.
	*/
	double MinDisagreement;

	/**
	* Maximal Number of k-Means.
	*/
	size_t MaxKMeans;

	/**
	* Convergence ratio between two k-Means iterations.
	*/
	double Convergence;

	/**
	* Number of derived chromosomes must be constructed for the optimization.
	*/
	size_t NbDivChromo;

	/**
	* Parameter for the criterion "Similariry".
	*/
	R::RParam* ParamsSim;

	/**
	* Parameter for the criterion "Agreement".
	*/
	R::RParam* ParamsAgreement;

	/**
	* Parameter for the criterion "Disagreement".
	*/
	R::RParam* ParamsDisagreement;

	/**
	* Constructor.
	*/
	GCAParams(void);
};


class GCAMaxRatio
{
public:
	size_t ObjId;
	double Ratio;

	GCAMaxRatio(size_t objid,double ratio) : ObjId(objid), Ratio(ratio) {}

	int Compare(const GCAMaxRatio& ratio) const {return(static_cast<int>(Ratio-ratio.Ratio));}

	/**
	* Static function used to order the object by ratio.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int sortOrder(const void* a,const void* b);
};


class GCAMaxRatios : public R::RContainer<GCAMaxRatio,true,false>
{
public:

	size_t ObjId;

	GCAMaxRatios(size_t objid,size_t max);
	int Compare(const GCAMaxRatios& obj) const {return(ObjId-obj.ObjId);}
};


//-----------------------------------------------------------------------------
#endif
