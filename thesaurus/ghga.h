/*

	GALILEI Project

	GHGA.h

	Hierarchical Genetic Algorithms for GALILEI - Header.

	Copyright 1998-2009 by Pascal Francq (pfrancq@ulb.ac.be).
	Copyright 1998-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GHGA_H
#define GHGA_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rfitness.h>
#include <robjh.h>
#include <rhga.h>
#include <rnodega.h>
#include <rnodesga.h>


//------------------------------------------------------------------------------
// Forward class declaration
class GNodeInfosData;
class GNodeInfos;
class GThreadDataH;
class GChromoH;
class GInstH;


//------------------------------------------------------------------------------
/**
* This class represent the fitness for the GALILEI HGA.
* @author Pascal Francq
* @short GALILEI HGA Fitness.
*/
class GFitnessH : public R::RFitness<double,false>
{
public:

	/**
	* Construct the fitness.
	*/
	GFitnessH(void) : R::RFitness<double,false>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	GFitnessH& operator=(const GFitnessH &f)
	{
		R::RFitness<double,false>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GFitnessH& operator=(const double val)
	{
		R::RFitness<double,false>::operator=(val);
		return(*this);
	}
};


//------------------------------------------------------------------------------
#endif
