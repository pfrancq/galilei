/*

	GALILEI Research Project

	GConceptTree.h

	Concepts Tree - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GConceptTreeH
#define GConceptTreeH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GConceptTree provides a tree of concepts, each concept may appear at
 * multiple depths in the tree. It means to represent the description of an
 * object. One such an object is a XML document which is, by nature, a tree of
 * tags, attributes and texts. In fact, each text document can be seen as a tree
 * where the different depths correspond to the level of the document
 * (part, chapter, section, etc.).
 *
 * In practice, the tree is represent as a container of nodes (GConceptNode):
 * - Each node represents a concept occurring at a given position and a given
 *   depth in the object represented.
 * - The container are ordered such as nodes of a same level are consecutive.
 *   These levels are managed internally through location caches.
 * - To navigate through the tree, each node remembers the position of its
 *   first child in the container, and the number of children.
 *
 * Here is a method that prints all the identifier of the concepts as their
 * appear in the tree:
 * @code
 * void Print(GConceptTree& tree)
 * {
 *
 * }
 * @endcode
 * @author Pascal Francq
 * @short Concepts Tree.
 */
class GConceptTree
{
	class GLC;

	/**
	 * Nodes of the tree.
	 */
	R::RContainer<GConceptNode,true,false> Nodes;

	/**
	 * Location caches.
	 */
	R::RContainer<GLC,true,false> LCs;

public:

	/**
	 * Default constructor.
	 */
	GConceptTree(void);

	/**
	 * Copy constructor.
	 * @param tree           Original structure.
	 */
	GConceptTree(const GConceptTree& tree);

	/**
	 * Constructor.
	 * @param vtd            Number of VTD records.
	 * @param lc             Number of location caches.
	 */
	GConceptTree(size_t vtd,size_t lc);

	/**
	 * Set the sizes of the container if know.
	 * @param vtd            Number of VTD records.
	 * @param lc             Number of location caches.
	 */
	void SetSizes(size_t vtd,size_t lc);

	/**
	 * Get the number of nodes in the tree (0 implies that the tree is empty).
	 */
	size_t GetNbNodes(void) const;

	/**
	 * Get the number of Location caches for the structure.
	 */
	size_t GetNbLCs(void) const;

	/**
	 * Get the number of entries in a given location cache.
	 * @param level          Level of the location cache.
	 */
	size_t GetNbLCEntries(size_t level) const;

	/**
	 * Set the number of entries at a given location cache.
	 * @param level          Level of the location cache.
	 * @param size           Size.
	 */
	void SetNbLCEntries(size_t level,size_t size) const;

	/**
	 * Add a node to the tree.
	 * @param concept        Concept associated with the record.
	 * @param pos            Position in the object.
	 * @param depth          Depth of the concept.
	 * @param child          Position of the first child in the next level.
	 * @param nbrecs         An idea of the number of nodes to store to the
	 *                       given level.
	 * @return Pointer to a node created.
	 */
	GConceptNode* AddNode(GConcept* concept,size_t pos,char depth,size_t child=SIZE_MAX,size_t nbrecs=0);

	/**
	 * Get a pointer to the nodes.
	 */
	R::RCursor<GConceptNode> GetNodes(void) const;

	/**
	 * Get the position of the first child in the next level.
	 * @param node           Node to search from.
	 * @return cNoRef if the tag has no child.
	 */
	size_t GetFirstChild(GConceptNode* rec) const;

	/**
	 * Clear the structure.
	 */
	void Clear(void);

	/**
	 * Destruct the structure.
	 */
	~GConceptTree(void);
};


//------------------------------------------------------------------------------
/**
 *  The GConceptNode provides a representation for a sort of VTD record to represent
 *  a XML node (Tag, attribute, value or content).
 *  @author Pascal Francq
 *  @short VTD Record
 */
class GConceptNode
{
private:

	/**
	* Concept.
	 */
	GConcept* Concept;

	/**
	 * Position in the document.
	 */
	size_t Pos;

	/**
	 * Depth and type of of the node. The first four bytes indicates the type,
	 * the rest the depth (maximal depth of 63).
	 */
	char Depth;

public:

	/**
	 * Construct a VTD Record.
	 * @param concept        Concept associated with the record.
	 * @param pos            Position in the file.
	 * @param depth          Depth of the record.
	 */
	GConceptNode(GConcept* concept,size_t pos,char depth);

	/**
	 * Compare method used by R::RContainer.
	 */
	int Compare(const GConceptNode&) const {return(-1);}

	/**
	 * Get the concept.
	 */
	GConcept* GetConcept(void) const {return(Concept);}

	/**
	 * Get the position in the file.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * Get the depth of the record.
	 */
	char GetDepth(void) const {return(Depth);}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
