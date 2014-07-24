/*

	GALILEI Research Project

	GConceptNode.h

	Concept Node - Header.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GConceptNode provides a representation for an occurrence of a concept in
 * a document tree, i.e. a concept node.
 * @author Concept Node.
 */
class GConceptNode : R::RNode<GConceptTree,GConceptNode,false>
{
	/**
	 * Type of the token corresponding to the node.
	 */
	tTokenType Type;

	/**
	* Concept Identifier.
	 */
	size_t ConceptId;

	/**
	 * Syntactic position in the document.
	 */
	size_t SyntacticPos;

	/**
	 * Position in the document (in byte).
	 */
	size_t Pos;

	/**
	 * Syntactic depth of the node.
	 */
	size_t SyntacticDepth;

	/**
	 * Index of the node in the main container.
	 */
	size_t Index;

public:

	/**
	 * Construct a concept node.
	 * @param type           Type of the token.
	 * @param conceptid      Identifier of the concept.
	 * @param synpos         Syntactic position.
	 * @param pos            Position in the file.
	 * @param depth          Syntactic depth of the record.
	 */
	GConceptNode(tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t depth);

	/**
	 * Compare two nodes regarding their depths and positions
    * @param node           Node to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const GConceptNode& node) const;

	/**
	 * Clear the node.
	 */
	virtual void Clear(void);

	/**
    * @return the type of the token corresponding to the node.
    */
	tTokenType GetType(void) const {return(Type);}

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
	 * @return the concept identifier.
	 */
	size_t GetConceptId(void) const {return(ConceptId);}

	/**
	 * @return the syntactic position in the file.
	 */
	size_t GetSyntacticPos(void) const {return(SyntacticPos);}

	/**
	 * @return the position in the file.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * @return the depth of the record.
	 */
	size_t GetSyntacticDepth(void) const {return(SyntacticDepth);}

	/**
	 * @return the index of the node.
	 */
	size_t GetIndex(void) const {return(Index);}

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
