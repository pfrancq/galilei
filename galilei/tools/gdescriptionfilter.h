/*

	GALILEI Research Project

	GDescriptionFilter.h

	Description Filter - Header.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GDescriptionFilter_H
#define GDescriptionFilter_H


//-----------------------------------------------------------------------------
// include files for the R Project
#include <rnumcontainer.h>
#include <rvectorbool.h>


//-----------------------------------------------------------------------------
// include files for the GALILEI Project
#include <gdescription.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDescriptionFilter provides a representation for description filter: it
 * copies the most and the weighted concepts from each vector from one
 * description to another.
 *
 * In practice, each class that need such as a tool, can inherit from it (this
 * avoid the allocation and deallocation of multiple blocks on the heap)
 * @author Pascal Francq
 * @short Description Filter
 */
class GDescriptionFilter
{
  	/**
	* Ordered vector for current computed profile.
	*/
	const GConceptRef** Order;

	/**
	* Size of the ordered vector.
	*/
	size_t MaxOrderSize;

public:

    /**
     * Constructor.
     */
    GDescriptionFilter(void);

    /**
     * Copy a description in another one by filtering some elements.
     * @param src           Source description.
     * @param dest          Destination description.
     * @param lmax          Maximum number of most weighted concepts to copy for
     *                      each vector (if null, all the concepts are copied).
     * @param lmin          Maximum number of irrelevant concepts (negative
     *                      weights)to copy for each vector.
     */
    void CopyFilter(const GDescription& src,GDescription& dest,size_t lmax,size_t lmin);

   /**
	 * Write the content of the 'Order' vector in a file called 'X-Y'
	 * where X is the identifier id and Y the identifier of the vector.
    * @param dir            Directory where to write.
    * @param id             Identifier.
    * @param nb             Number of elements of order to write.
    * @param concept        Concept associated to a vector.
	 */
	void WriteFile(const R::RString& dir,size_t id,size_t nb,GConcept* concept);

    /**
     * Destructor.
     */
    ~GDescriptionFilter(void);
};

}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
