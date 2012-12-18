/*

	GALILEI Research Project

	GComputeSimLink.h

	Link Similarity Measure - Header.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).

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
#include <gsimplugin.h>


//------------------------------------------------------------------------------
/**
 * The ComputeSimLink class proposes a way to compute the similarity between two
 * vectors containing links.
 */
class GComputeSimLink : public GComputeSim
{
	/**
	* Represent a node in the graph of a document.
	*/
	class Node;

	/**
	 * Container of nodes for the first vector.
	 */
	RContainer<Node,true,true> Vector1;

	/**
	 * Container of nodes for the second vector.
	 */
	RContainer<Node,true,true> Vector2;

	/**
	 * Numerator.
	 */
	double Num;

	/**
	 * Denominator.
	 */
	double Den;

public:

	/**
	 * Constructor.
	 */
	GComputeSimLink(GSimPlugIn* plugin);

	/**
	 * Add to a given container all the links of a vector.
    * @param cont            Container containing the links.
    * @param vec             Vector to treat.
	 * @param concept         Concept corresponding to the other object (can be
	 *                        null).
	 * @param weigth          Weight of the vector.
    * @param hop             Number of hops to propagate the computation.
    */
	void Fill(RContainer<Node,true,true>& cont,const GVector* vec,GConcept* concept,double weight,size_t hop);

	/**
	 * Compute the ratio of links of the first container that are also in the
	 * second one. This comparison is weighted by the number of times a link
	 * appears and its IF.
    * @param cont1           First container.
    * @param cont2           Second container.
    */
	void AddLinks(RContainer<Node,true,true>& cont1,RContainer<Node,true,true>& cont2);

	/**
	 * Compute the similarity.
    * @param vec1            First vector.
    * @param vec2            Second vector.
    * @param sim             Similarity computed.
    * @param nb              Number of comparisons done.
    */
	virtual void Compute(GVector* vec1,GVector* vec2,double& sim,size_t& nb);
};


//------------------------------------------------------------------------------
#endif

