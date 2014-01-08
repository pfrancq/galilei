/*

	GALILEI Research Project

	GComputeSim.h

	Generic Similarity Measure - Header.

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
#ifndef GComputeSim_H
#define GComputeSim_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmatrixmeasure.h>
#include <gconcepttype.h>
#include <gconceptref.h>
#include <gvector.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// forward declaration
class GSimPlugIn;


//------------------------------------------------------------------------------
/**
 * The GComputeSim class proposes a way to compute a similarity between two
 * vectors. It is an abstract class.
 */
class GComputeSim
{
protected:

	/**
	 * Pointer to the plug-in.
    */
	GSimPlugIn* PlugIn;

public:

	/**
	 * Constructor.
	 */
	GComputeSim(GSimPlugIn* plugin) : PlugIn(plugin) {}

	/**
	 * Compute the similarity.
    * @param vec1            First vector.
    * @param vec2            Second vector.
    * @param sim             Similarity computed.
    * @param nb              Number of comparisons done.
    */
	virtual void Compute(const GVector* vec1,const GVector* vec2,double& sim,size_t& nb)=0;

	/**
	 * Destructor.
    */
	virtual ~GComputeSim(void) {}
};


//------------------------------------------------------------------------------
/**
 * The GSimPlugIn class proposes some generic methods that are useful. It is
 * an abstract class.
 */
class GSimPlugIn : public GMatrixMeasure
{
protected:

	/**
	 * Number of hops
	 */
	size_t NbHops;

	/**
	 * Concept of the first object as URL.
	 */
	GConcept* Concept1;

	/**
	 * Concept of the second object as URL.
	 */
	GConcept* Concept2;

	/**
	 * Similarity computing methods.
	 */
	GComputeSim** Sims;

public:

	/**
	* Constructor of the similarities between two elements of the same type.
	* @param fac             Factory of the plug-in.
	* @param lines           Type of the elements in the lines.
	* @param cols            Type of the elements in the columns.
	*/
	GSimPlugIn(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols);

	/**
	 * @return the inverse factor of a given concept. If the elements in the
	 * line and in the columns are different, the sum of both types are
	 * computed.
	 * @param concept        Concept.
	 */
	double GetIF(GConcept* concept) const;

	/**
    * @return the number of hops
    */
	double GetNbHops(void) const {return(NbHops);}

	/**
    * @return the identifier of the first identifier.
    */
	GConcept* GetConcept1(void) const {return(Concept1);}

	/**
    * @return the identifier of the second identifier.
    */
	GConcept* GetConcept2(void) const {return(Concept2);}

	/**
	 * Destructor.
    */
	virtual ~GSimPlugIn(void);
};


//------------------------------------------------------------------------------
#endif
