/*

	GALILEI Research Project

	GConceptNode.h

	Concept Node - Header.

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
#ifndef GConceptNodeH
#define GConceptNodeH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gconceptrecord.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * The GConceptNode provides a representation for an occurrence of a concept in
 * a document tree, i.e. a concept node.
 * @short Concept Node.
 */
class GConceptNode : R::RNode<GConceptTree,GConceptNode,false>, public GConceptRecord
{
	/**
	 * Tree of the node.
	 */
	GConceptTree* Tree;

public:

	/**
	 * Construct a concept node.
	 * @param tree           Tree of the node.
	 * @param type           Type of the token.
	 * @param conceptid      Identifier of the concept.
	 * @param synpos         Syntactic position.
	 * @param pos            Position in the file.
	 * @param syndepth       Syntactic depth of the record.
	 */
	GConceptNode(GConceptTree* tree,tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t depth);

	/**
	 * Clear the node.
	 */
	virtual void Clear(void);

	/**
    * @return the tree of the node.
    */
	GConceptTree* GetTree(void) const {return(Tree);}

	/**
	* @return the parent of the node. If null, the node is a top node.
	*/
	GConceptNode* GetParent(void) const;

	/**
	* @return the first child node. If null, the node has no children.
	*/
	GConceptNode* GetFirst(void) const;

	/**
	* @return the next node of the same parent. If null, the node is the last
	* one.
	*/
	GConceptNode* GetNext(void) const;

	/**
	* @return the number of child nodes.
	*/
	size_t GetNbNodes(void) const;

	/**
    * @return the total number of child nodes (including all possible children).
    */
	size_t GetNbTotalNodes(void) const;

	/**
	 * Simply print the node information on the screen;
	 */
	void Print(void) const;

	friend class GConceptTree;
	friend class R::RTree<GConceptTree,GConceptNode,false>;
	friend class R::RNode<GConceptTree,GConceptNode,false>;
	friend class R::RNodeCursor<GConceptTree,GConceptNode>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
