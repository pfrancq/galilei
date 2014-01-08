/*

	GALILEI Research Project

	GResNode.h

	Resulting Node - Header.

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
#ifndef GResNodeH
#define GResNodeH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// forward declaration
class GResNodes;


//------------------------------------------------------------------------------
/**
 * The GResNode provides a representation for a node result for a given query,
 * i.e. a document fragment.
 * @author Resulting Node.
 */
class GResNode
{
	/**
	 * Parent container.
    */
	GResNodes* Parent;

	/**
	 * Resulting Node.
	 */
	GConceptNode* Node;

	/**
	 * Minimum syntactic position.
    */
	size_t MinPos;

	/**
	 * Maximum syntactic position.
    */
	size_t MaxPos;

	/**
	 * Child nodes used by the query to select the node.
    */
	RContainer<GConceptNode,false,false> Children;

public:

	/**
	 * Construct a resulting node.
	 * @param parent         Parent container.
	 * @param node           Concept node.
	 */
	GResNode(GResNodes* parent,GConceptNode* node);

	/**
	 * Compare two nodes regarding their depths and positions
    * @param node           Node to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const GResNode& node) const;

	/**
    * @return the parent container.
    */
	GResNodes* GetParent(void) const {return(Parent);}

	/**
	* @return the node.
	*/
	GConceptNode* GetNode(void) const {return(Node);}

	/**
	 * @return the minimum syntactic position.
    */
	size_t GetMinPos(void) const {return(MinPos);};

	/**
	 * @return the maximum syntactic position.
    */
	size_t GetMaxPos(void) const {return(MaxPos);};

	/**
	 * Add a child node.
    * @param child          Node to add.
    */
	void AddChild(GConceptNode* child);

	/**
    * @return the number of children.
    */
	inline size_t GetNbChildren(void) const {return(Children.GetNb());}

	/**
    * @return a cursor over the children.
    */
	RCursor<GConceptNode> GetChildren(void) const {return(RCursor<GConceptNode>(Children));}

	/**
	* Equal operator: Verify that the two nodes have the same node.
	* @param node            Node to compare with.
	*/
	inline bool operator==(const GResNode& node) const {return(Node==node.Node);}

	/**
	 * Look if two nodes overlaps, i.e. they have children of a common node that
	 * are not too much remote.
	 * @param node           Node to compare with.
	 * @return true if overlap.
	 */
	bool Overlap(const GResNode* node) const;

	/**
	 * Merge the children of a node.
    * @param node           Source node.
    */
	void Merge(const GResNode* node);
};


//------------------------------------------------------------------------------
#endif
