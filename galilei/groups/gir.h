/*

	GALILEI Research Project

	GIR.h

	GA for the Information Retrieval Problem - Header.

	(C) 2002 by Pascal Francq

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
		RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GFitnessIR& operator=(const double val)
	{
		RFitness<double,true>::operator=(val);
		return(*this);
	}
};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
