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
#ifndef RIRH
#define RIRH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rga/rfitness.h>
#include <rgga/rgga.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


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
* Define the type of measure used for the corresponding similarity criterion.
*/
enum SimType {
	stAvgSim                /** Average Similarity.*/,
	stJ                     /** sum_proto(intra)/min_proto(inter).*/,
	stAvgRatio              /** Average min(intra)/max(inter).*/,
	stMinRatio              /** Min min(intra)/max(inter).*/,
	stRatio                 /** Min(intra)/Max(inter).*/,
	stWOverB                /** Sb/Sw.*/,
	stSimWB                 /** Simb/Simw.*/};


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


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
