/*

	GALILEI Research Project

	GConceptNodes.h

	Concept Node Container - Header.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GConceptNodesH
#define GConceptNodesH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gconceptnode.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GConceptNodes provides a representation for a container of concept nodes
 * associated with a given concept.
 *
 * The nodes are ordered by depths and syntactic positions.
 * @author Concept Node Container.
 */
class GConceptNodes : public R::RContainer<GConceptNode,false,true>
{
	/**
	* Concept identifier.
	 */
	size_t ConceptId;

public:

	/**
	 * Construct a concept node container.
	 * @param conceptid      Concept identifier.
	 */
	GConceptNodes(size_t conceptid);

	/**
	 * Construct a concept node container.
	 * @param conceptid      Concept identifier.
	 * @param max            Initial size of the container.
	 */
	GConceptNodes(size_t conceptid,size_t max);

	/**
	 * Copy constructor.
	 * @param nodes          Container to copy.
	 */
	GConceptNodes(const GConceptNodes& nodes);

	/**
	 * Compare two node containers regarding their concept.
    * @param nodes          Nodes to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const GConceptNodes& nodes) const;

	/**
	 * Compare a node container with an concept.
    * @param conceptid      Concept to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const size_t conceptid) const;

	/**
	 * @return the concept identifier.
	 */
	size_t GetConceptId(void) const {return(ConceptId);}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
