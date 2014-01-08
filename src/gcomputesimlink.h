/*

	GALILEI Research Project

	GComputeSimLink.h

	Link Similarity Measure - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GComputeSimLink_H
#define GComputeSimLink_H


//------------------------------------------------------------------------------
// include files for the plug-in
#include <gcomputesimmeta.h>



//------------------------------------------------------------------------------
/**
 * The ComputeSimLink class proposes a way to compute the similarity between two
 * vectors containing links. In practice, it computes the ratio of common links
 * regarding the total number of different links between both vectors weighted
 * by their idf.
 *
 * To take negative concept weights into account, two vectors sharing no
 * concepts have a similarity of 0.5.
 */
class GComputeSimLink : public GComputeSimMeta
{
	class Node;

	/**
	 * Neighbors of the first object.
	 */
	RContainer<Node,true,true> Neighbors1;

	/**
	 * Neighbors of the second object.
	 */
	RContainer<Node,true,true> Neighbors2;

public:

	/**
	 * Constructor.
	 */
	GComputeSimLink(GSimPlugIn* plugin);

	/**
	 * Add to a given container all the links of a vector.
    * @param cont            Container containing the links.
    * @param vec             Vector to treat.
	 * @param weight          Weight of the parent link.
    * @param hop             Number of hops to propagate the computation.
    */
	void Fill(RContainer<Node,true,true>& cont,const GVector* vec,double weight,size_t hop);

	/**
	 * Compute the similarity.
    * @param vec1            First vector.
    * @param vec2            Second vector.
    * @param sim             Similarity computed.
    * @param nb              Number of comparisons done.
    */
	virtual void Compute(const GVector* vec1,const GVector* vec2,double& sim,size_t& nb);
};


//------------------------------------------------------------------------------
#endif

