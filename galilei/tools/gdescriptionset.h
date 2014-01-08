/*

	GALILEI Research Project

	GDescriptionSet.h

	Description Set - Header.

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
#ifndef GDescriptionSet_H
#define GDescriptionSet_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>
#include <rvectorbool.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescription.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDescriptionSet provides a representation for a set of descriptions. It
 * is useful to compute the inverse object for a given subset.
 * @author Pascal Francq
 * @short Description Set
 */
class GDescriptionSet
{
   /**
    * The session.
    */
   GSession* Session;

   /**
    * The set of description.
    */
   R::RContainer<const GDescription,false,false> Set;

   /**
    * The inverse frequency factors.
    */
   R::RNumContainer<double,false> IF;

   /**
    * The number of references of the different type.
    */
   R::RNumContainer<size_t,false> Refs;

   /**
    * Must the factors be recomputed.
    */
   bool Dirty;

   /**
    * Temporary vector.
    */
   R::RVectorBool tmpTypes;

   /**
    * Temporary vector.
    */
   R::RVectorBool tmpConcepts;

public:

    /**
     * Constructor of the description set.
     * @param session       Session.
     */
    GDescriptionSet(GSession* session);

    /**
     * @return the number of descriptions.
     */
    size_t GetNb(void) const{return(Set.GetNb());}

    /**
     * Clear the description set.
     */
    void Clear(void);

    /**
     * Add a description to the set.
     * @param desc          Description to add.
     */
    void InsertDescription(const GDescription* desc);

    /**
     * Compute the inverse frequency factors for the set of description. In
     * practice, it is only recomputed when the set changed.
     * @param conceptid     Identifier of the concept.
     * @return the inverse frequency factor for a given concept.
     */
    double GetIF(size_t conceptid);
};

}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
