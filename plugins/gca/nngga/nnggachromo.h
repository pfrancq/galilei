	/*

	GALILEI Research Project

	NNGGAChromo.h

	Chromosome - Header

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef NNGGAChromoH
#define NNGGAChromoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rchromonn.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <gca_nngga.h>
#include <nnggagroup.h>
#include <nnggainst.h>


//-----------------------------------------------------------------------------
/**
* The NNGGAChromoDoc class provides a representation for a chromosome for the
* clustering the documents.
* @author Pascal Francq
* @short Documents Chromosome.
*/
class NNGGAChromoDoc : public RChromoNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identifier of the chromosome.
	*/
	NNGGAChromoDoc(NNGGAInstDoc* inst,size_t id);

	/**
	* Construct the chromosome based on existing groups.
	* @param groups          Cursor over the groups.
	*/
	void ConstructChromo(R::RCursor<GTopic> groups);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void);

	// friend classes
	friend class NNGGAGroupDoc;
	friend class NNGGAInstDoc;
	friend class NNGGAHeuristicDoc;
};


//-----------------------------------------------------------------------------
/**
* The NNGGAChromoProfile class provides a representation for a chromosome for the
* clustering the profiles.
* @author Pascal Francq
* @short Profiles Chromosome.
*/
class NNGGAChromoProfile : public RChromoNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identifier of the chromosome.
	*/
	NNGGAChromoProfile(NNGGAInstProfile* inst,size_t id);

	/**
	* Construct the chromosome based on existing groups.
	* @param groups          Cursor over the groups.
	*/
	void ConstructChromo(R::RCursor<GCommunity> groups);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void);

	// friend classes
	friend class NNGGAGroupProfile;
	friend class NNGGAInstProfile;
	friend class NNGGAHeuristicProfile;
};


//-----------------------------------------------------------------------------
#endif
