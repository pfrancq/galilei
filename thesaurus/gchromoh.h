/*

	GALILEI Project

	GChromoH.h

	Chromosomes representing a tree of vectors - Header

	Copyright 1998-2012 by Pascal Francq (pfrancq@ulb.ac.be).
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
#ifndef GChromoH_H
#define GChromoH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rhga.h>
#include <rchromoh.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ghga.h>

//------------------------------------------------------------------------------
class GChromoH : public R::RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeInfos,R::RObjH>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance.
	* @param id             Identificator of the chromosome.
	*/
	GChromoH(GInstH* inst,unsigned int id);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the
	*                       chromosome.
	*/
	virtual void Init(GThreadDataH* thData);

	/**
	* Evaluate the quality of the solution
	*/
	virtual void Evaluate(void);

	/**
	* Destruct the chromosome.
	*/
	virtual ~GChromoH(void);
};


//------------------------------------------------------------------------------
#endif
