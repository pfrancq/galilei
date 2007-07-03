/*

	Genetic Community Algorithm

	GGCA.h

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
#ifndef GGCAH
#define GGCAH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rpromcriterion.h>
#include <rfitness.h>
#include <rgga.h>
#include <rparam.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Forward class declaration
class GGCAGroup;
class GGCAGroupData;
class GGCAThreadData;
class GGCAChromo;
class GGCAInst;
class GGCAObj;
class GGCAHeuristic;
class GGCAParams;


//-----------------------------------------------------------------------------
/**
* The RFitnessIR provides a representation for a fitness function for the
* information retrieval problem.
* @author Pascal Francq
* @short IR Fitness.
*/
class GGCAFitness : public R::RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used for the Bin Packing.
	*/
	GGCAFitness(void) : R::RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	GGCAFitness& operator=(const GGCAFitness &f)
	{
		R::RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GGCAFitness& operator=(const double val)
	{
		R::RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/**
* The GGCAParam represents all the parameter used in the GGCA module.
* @short GGCA Parameters.
*/
class GGCAParams
{
public:

	/**
	* Size of the population of the GA.
	*/
	unsigned int PopSize;

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
	unsigned int MaxKMeans;

	/**
	* Convergence ratio between two k-Means iterations.
	*/
	double Convergence;

	/**
	* Number of derived chromosomes must be constructed for the optimization.
	*/
	unsigned int NbDivChromo;

	/**
	* Parameter for the criterion "Similariry".
	*/
	R::RPromCriterionParams ParamsSim;

	/**
	* Parameter for the criterion "Agreement".
	*/
	R::RPromCriterionParams ParamsAgreement;

	/**
	* Parameter for the criterion "Disagreement".
	*/
	R::RPromCriterionParams ParamsDisagreement;

	/**
	* Constructor.
	*/
	GGCAParams(void);
};


class GGCAMaxRatio
{
public:
	unsigned int ObjId;
	double Ratio;

	GGCAMaxRatio(unsigned int objid,double ratio) : ObjId(objid), Ratio(ratio) {}
	
	int Compare(const GGCAMaxRatio& ratio) const {return(static_cast<int>(Ratio-ratio.Ratio));}

	/**
	* Static function used to order the object by ratio.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int sortOrder(const void* a,const void* b);
};


class GGCAMaxRatios : public R::RContainer<GGCAMaxRatio,true,false>
{
public:

	unsigned int ObjId;

	GGCAMaxRatios(unsigned int objid,unsigned int max);
	int Compare(const GGCAMaxRatios& obj) const {return(ObjId-obj.ObjId);}
};


//-----------------------------------------------------------------------------
#endif
