	/*

	GALILEI Research Project

	GCAChromo.h

	Chromosome - Header

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GCAChromoH
#define GCAChromoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rchromosc.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>
#include <gcainst.h>
#include <gcagroup.h>


//-----------------------------------------------------------------------------
/**
* The GCAChromo class provides a representation for a chromosome for the IR
* Problem.
* @author Pascal Francq
* @short IR Chromosome.
*/
class GCAChromo : public RChromoSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identifier of the chromosome.
	*/
	GCAChromo(GCAInst* inst,size_t id);

	/**
	* Construct the chromosome based on existing groups.
	* @param groups          Cursor over the groups.
	*/
	template<class cObj,class cGroup> void ConstructChromo(R::RCursor<cGroup> groups);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void);

	/**
	* Destruct the chromosome.
	*/
	virtual ~GCAChromo(void);

	// friend classes
	friend class GCAGroup;
	friend class GCAInst;
	friend class GCAProm;
	friend class GCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif
