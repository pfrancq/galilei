/*

	GALILEI Research Project

	GIR.h

	GA for the Information Retrieval Problem - Header.

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
#ifndef GIRH
#define GIRH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rpromethee/rpromcriterion.h>
#include <rga/rfitness.h>
#include <rgga/rgga.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroupingparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward class declaration
class GSubProfile;
class GGroupIR;
class GGroupDataIR;
class GThreadDataIR;
class GChromoIR;
class GInstIR;
class GObjIR;
class GIRHeuristic;


//-----------------------------------------------------------------------------
/**
* Define the type of similarity measures used.
*/
enum SimCritType
{
	sctCorl                 /** Correlation function used as one criterion.*/,
	sctCrits                /** Multiple measures are used as criteria.*/
};


//-----------------------------------------------------------------------------
/**
* Define the type of measure used for the corresponding similarity criterion.
*/
enum SimType {
	stNone                  /** No Similarity Measure.*/,
	stAvgSim                /** Average Similarity.*/,
	stJ                     /** sum_proto(intra)/min_proto(inter).*/,
	stAvgRatio              /** Average min(intra)/max(inter).*/,
	stMinRatio              /** Min min(intra)/max(inter).*/,
	stRatio                 /** Min(intra)/Max(inter).*/,
	stWOverB                /** Sb/Sw.*/,
	stSimWB                 /** Simb/Simw.*/
};


//-----------------------------------------------------------------------------
/**
* The GSimMeasure class provides a representation for a similarity measure that
* may be used for the evaluation of the chromosome.
* @author Pascal Francq
* @short Measure of Similarity.
*/
class GSimMeasure
{
public:
	/**
	* Name of the measure.
	*/
	RStd::RString Name;

	/**
	* Type fo the measure.
	*/
	SimType Type;

	/**
	* Is this emasure used as criteria?
	*/
	bool Use;

	/**
	* If similarity is evaluated using a correlation, what is the weight of
	* this measure.
	*/
	double Weight;

	/**
	* Constructor.
	* @param n              Name.
	* @param t              Type.
	* @param u              Used as criteria?
	* @param w              Weight if correlation used.
	*/
	GSimMeasure(const char* n,SimType t,bool u=false,double w=0.0);

	/**
	* Compare method used by the RStd::RContainer.
	*/
	int Compare(const GSimMeasure* s) const;

	/**
	* Compare method used by the RStd::RContainer.
	*/
	int Compare(const char* s) const;

	/**
	* Assigment operator.
	* @param src            Object used as source.
	*/
	GSimMeasure& operator=(const GSimMeasure& src);
};


//-----------------------------------------------------------------------------
/**
* The RFitnessIR provides a representation for a fitness function for the
* information retrieval problem.
* @author Pascal Francq
* @short IR Fitness.
*/
class GFitnessIR : public RGA::RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used for the Bin Packing.
	*/
	GFitnessIR(void) : RGA::RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	GFitnessIR& operator=(const GFitnessIR &f)
	{
		RGA::RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GFitnessIR& operator=(const double val)
	{
		RGA::RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/**
* The GIRParam represents all the parameter used in the GIR module.
* @short GIR Parameters.
*/
class GIRParams : public GGroupingParams
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
	* Maximal Number of KMeans.
	*/
	unsigned int MaxKMeans;

	/**
	* Type of measure used for the similarity criterion.
	*/
	SimCritType SimMeasures;

	/**
	* Measures.
	*/
	RStd::RContainer<GSimMeasure,unsigned int,true,true> Measures;

	/**
	* Parameter for the criterion "Similariry".
	*/
	RPromethee::RPromCriterionParams ParamsSim;

	/**
	* Parameter for the criterion "Information".
	*/
	RPromethee::RPromCriterionParams ParamsInfo;

	/**
	* Parameter for the criterion "Entropy".
	*/
	RPromethee::RPromCriterionParams ParamsEntropy;

	/**
	* Parameter for the criterion "Likelihood".
	*/
	RPromethee::RPromCriterionParams ParamsLikelihood;

	/**
	* Parameter for the criterion "Same Feedbacks".
	*/
	RPromethee::RPromCriterionParams ParamsSameFeedbacks;

	/**
	* Parameter for the criterion "Diff Feedbacks".
	*/
	RPromethee::RPromCriterionParams ParamsDiffFeedbacks;

	/**
	* Parameter for the criterion "Social".
	*/
	RPromethee::RPromCriterionParams ParamsSocial;

	/**
	* Global similarities used.
	*/
	bool GlobalSim;

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*);

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GIRParams& operator=(const GIRParams& src);

	/**
	* Constructor.
	*/
	GIRParams(void);
};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
