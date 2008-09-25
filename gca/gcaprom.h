/*

	Genetic Community Algorithm

	GCAProm.h

	PROMETHEE Kernel - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#ifndef GCAPromH
#define GCAPromH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
* The GCAProm represented a PROMETHEE Kernel for the GCA.
* the GALILEI sessions.
* @author Pascal Francq
* @short GCA PROMETHEE Kernel.
*/
class GCAProm : public R::RPromKernel
{
	/**
	* Parameters.
	*/
	GCAParams* Params;

	/**
	* Criteria representing the Similarity criterion "J".
	*/
	R::RPromCriterion* CritSimJ;

	/**
	* Criteria depending of the agreement ratios.
	*/
	R::RPromCriterion* CritAgreement;

	/**
	* Criteria depending of the disagreement ratios.
	*/
	R::RPromCriterion* CritDisagreement;

public:

	/**
	* Constructor.
	* @param p              Parameters.
	*/
	GCAProm(GCAParams* p);

	/**
	* Assign chromosome as a solution.
	* @param s              Solution.
	* @param c              Chromosome.
	*/
	void Assign(R::RPromSol* s,GCAChromo* c);
};


//-----------------------------------------------------------------------------
#endif
