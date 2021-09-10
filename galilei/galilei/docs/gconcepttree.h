/*

	GALILEI Research Project

	GConceptTree.h

	Concepts Tree - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GConceptTree implements a tree of concepts, each concept may appear at
 * multiple depths in the tree. It means to represent documents which have, by
 * nature, a structured organization. A XML documents for example is a tree of
 * tags, attributes and texts.
 *
 * In practice, the internal implementation supposes that:
 * - Each node (GConceptNode) represents a concept occurrence at a given
 *   position and a given depth in the object represented.
 * - A container stores all created nodes. It is responsible for the allocation
 *   and deallocation of the nodes. The idea is that the created nodes are only
 *   deallocated in the destructor.
 * - A container where the nodes are ordered by syntactic positions.
 * - A container stores all concepts appearing in a document and a pointer to
 *   all occurrences.
 * @short Concepts Tree.
 */
class GConceptTree : R::RTree<GConceptTree,GConceptNode,false>, R::RContainer<GConceptNode,true,false>
{
	/**
	 * Concept ordered by syntactic positions.
	 */
	R::RContainer<GConceptNode,false,false> Pos;

	/**
	 * Concept references.
	 */
	R::RContainer<GConceptNodes,true,true> Refs;

	/**
	 * Document identifier.
	 */
	size_t DocId;

public:

	/**
	 * Copy constructor.
    * @param tree           Source tree.
    */
	GConceptTree(const GConceptTree& tree);

	/**
	 * Construct a tree from a given set of records.
	 * @param docid          Document identifier.
	 * @param records        Container of records.
	 * @param nbrecords      Number of records used for the document tree.
	 * @param nbrefs         Number of concepts referenced.
    */
	GConceptTree(size_t docid,const R::RContainer<GConceptRecord,false,true>& records,size_t nbtoprecords,size_t nbrefs);

	/**
	 * constructor.
	 * @param docid          Document identifier.
	 * @param max            Maximum number of initial nodes to create.
	 * @param nb             Initial size of the concept reference container.
	 */
	GConceptTree(size_t docid,size_t max,size_t nb);

	/**
	 * Verify that the container may hold a given number of nodes and concept
	 * references. The structure is also emptied.
	 * @param docid          Document identifier.
	 * @param max            Number of nodes.
	 * @param nb             Number of the concept references.
	*/
	void Verify(size_t docid,size_t max,size_t nb);

private:

	/**
	 * Treat the next elements in the cursor as possible children.
    * @param record         Cursor.
    * @param parent         Parent node.
    */
	void TreatChildNode(R::RCursor<GConceptRecord>& record,GConceptNode* parent);

	/**
	 * Copy a node from one tree to the other one.
	 * @param parent         Parent node that will contain the node copied.
	 * @param node           Node to copy.
    */
	void CopyNode(GConceptNode* parent,GConceptNode* node);

public:

	/**
	 * Insert a node in the tree.
	 * @param parent         Parent node. If Null, it is supposed to be a top
	 *                       node.
	 * @param type           Type of the token.
	 * @param conceptid      Concept identifier associated with the node.
	 * @param synpos         Syntactic position.
	 * @param pos            Position in the object.
	 * @param depth          Depth of the concept.
	 * @return Pointer to a node created.
	 */
	GConceptNode* InsertNode(GConceptNode* parent,tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t depth);

	/**
	 * Clear the structure (but the main container).
	 */
	virtual void Clear(void);

	/**
    * @return the identifier of the document.
    */
	size_t GetDocId(void) const {return(DocId);}

	/**
	* @return the total number of nodes in the tree.
	*/
	size_t GetNbNodes(void) const {return(R::RTree<GConceptTree,GConceptNode,false>::GetNbNodes());}

	/**
	* @return the total number of top nodes in the tree.
	*/
	size_t GetNbTopNodes(void) const {return(R::RTree<GConceptTree,GConceptNode,false>::GetNbTopNodes());}

	/**
	* @return the total number of concepts references.
	*/
	size_t GetNbRefs(void) const {return(Refs.GetNb());}

	/**
	 * Get all the nodes representing the occurrences of a given concept.
	 * @param concept        Concept.
	 * @return a cursor over the nodes.
    */
	R::RCursor<GConceptNode> GetNodes(GConcept* concept) const;

	/**
	 * Get all the nodes ordered by syntactic position.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SIZE_MAX, iterate until the end of the container.
	 * @return a cursor over the nodes.
    */
	R::RCursor<GConceptNode> GetNodes(size_t min=0,size_t max=SIZE_MAX) const;

	/**
	 * Get the node at a given syntactic position.
    * @param synpos         Syntactic position.
    * @return a pointer or null if the position is outside the tree.
    */
	const GConceptNode* GetNode(size_t synpos) const {return(Pos.GetPtrAt(synpos));}

	/**
	 * Get the nearest node at a given syntactic position. In practice, the
	 * method searches alternatively before and after the given position.
    * @param synpos         Syntactic position.
    * @return a pointer or null if the position is outside the tree.
    */
	const GConceptNode* GetNearestNode(size_t synpos) const;

	/**
	 * Get the nearest node at a given syntactic position.  In practice, the
	 * method searches the first node after or before the given position. If no
	 * node is found, it searches in the order direction.
    * @param synpos          Syntactic position.
	 * @param after           Search after the position (true) or before it
	 *                        (false)?
    * @return a pointer or null if the position is outside the tree.
    */
	const GConceptNode* GetNearestNode(size_t synpos,bool after) const;

	/**
	 * Find the root node (the most common highest node) of two nodes. If at
	 * least one of the node doesn't have a parent, no root node can be found.
	 * @param node1          First node.
	 * @param node2          Second node.
	 * @return the root node of 0 if both nodes are top nodes.
    */
	const GConceptNode* GetRoot(const GConceptNode* node1,const GConceptNode* node2) const;

	/**
	 * Find the minimal position centred on a given node and a number of nodes
	 * before.
    * @param node           Concept node.
    * @param nbbefore       Maximum number of concept node before.
    * @return a position.
    */
	size_t GetMinPos(const GConceptNode* node,size_t nbbefore) const;

	/**
	 * Find the maximal position centred on a given node and a number of nodes
	 * after.
    * @param node           Concept node.
    * @param nbafter        Maximum number of concept node after.
    * @return a position.
    */
	size_t GetMaxPos(const GConceptNode* node,size_t nbafter) const;

	/**
	 * Compute the maximal position occupied by a referenced node.
    * @return a size_t.
    */
	size_t GetMaxPos(void) const;

	/**
	 * Compute the maximal syntactic position occupied by a referenced node.
    * @return a size_t.
    */
	size_t GetMaxSyntacticPos(void) const;

	/**
	 * Simply print the information of all nodes on the screen;
	 */
	void Print(void) const;

	/**
	 * Destructor.
	 */
	~GConceptTree(void);

	friend class GConceptNode;
	friend class R::RNode<GConceptTree,GConceptNode,false>;
	friend class R::RTree<GConceptTree,GConceptNode,false>;
	friend class R::RNodeCursor<GConceptTree,GConceptNode>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
