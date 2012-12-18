/*

	GALILEI Research Project

	GComputeSimCos.h

	Cosine Similarity Measure - Header.

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
#ifndef GComputeSimCos_H
#define GComputeSimCos_H


//------------------------------------------------------------------------------
// include files for the plug-in
#include <gsimplugin.h>


//------------------------------------------------------------------------------
/**
 * The ComputeSimCos class proposes a way to compute the cosine similarity
 * between two vectors.
 */
class GComputeSimCos : public GComputeSim
{
public:

	/**
	 * Constructor.
	 */
	GComputeSimCos(GSimPlugIn* plugin) : GComputeSim(plugin) {}

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

