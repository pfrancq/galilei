/*

	GALILEI Research Project

	GResNodes.h

	Resulting Node Container - Header.

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
#ifndef GResNodesH
#define GResNodesH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gresnode.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
/**
 * The GResNodes provides a representation for a container of resulting nodes
 * associated with a given document.
 *
 * The nodes are ordered by depths and syntactic positions.
 * @author Concept Node Container.
 */
class GResNodes : RContainer<GResNode,true,true>
{
	/**
	* Document identifier.
	 */
	size_t DocId;

public:

	/**
	 * Construct a resulting node container.
	 * @param docid          Document identifier.
	 */
	GResNodes(size_t docid);

	/**
	 * Construct a resulting node container.
	 * @param docid          Document identifier.
	 * @param max            Initial size of the container.
	 */
	GResNodes(size_t conceptid,size_t max);

	/**
	 * Copy constructor.
	 * @param nodes          Container to copy.
	 */
	GResNodes(const GResNodes& nodes);

	/**
	 * Compare two node containers regarding their concept.
    * @param nodes          Nodes to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const GResNodes& nodes) const;

	/**
	 * Compare a node container with a document.
    * @param doctid         Document to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const size_t docid) const;

	/**
	 * Look if a node has a sibling node in the container.
	 * @param node           Node to analyze.
	 * @return true if it is the case.
	 */
	bool FindSibling(GResNode* node) const;

	/**
	 * Look if a node has a child node in the container.
	 * @param node           Node to analyze.
	 * @return true if it is the case.
	 */
	bool FindChild(GResNode* node) const;

	/**
    * @return a cursor over the nodes.
    */
	RCursor<GResNode> GetNodes(void) const {return(RCursor<GResNode>(*this));}

	/**
    * @return the number of nodes.
    */
	size_t GetNbNodes(void) const {return(RContainer<GResNode,true,true>::GetNb());}

	/**
	 * Get the index of a given element corresponding to a given criteria.
	 * @tparam T             Type used for the criteria.
    * @param tag            Criteria.
    * @param find           Used by the method to specify if the element is
	 *                       already in the container or not.
    * @return the index of the element in the container.
    */
	template<class T> size_t GetIndex(const T tag,bool& find) {return(RContainer<GResNode,true,true>::GetIndex(tag,find));}

	/**
	 * Look if a given element corresponding to a given criteria is in the
	 * container.
	 * @tparam T             Type used for the criteria.
    * @param tag            Criteria.
    * @return true if the element is in the container.
    */
	template<class T> bool IsIn(const T tag) {return(RContainer<GResNode,true,true>::IsIn(tag));}

	/**
	 * Insert a copy of a given node in the container.
    * @param node           Node which copy must be inserted.
    * @param idx            Index of the insertion.
    */
	void InsertPtrAt(const GResNode* node,size_t idx);

	/**
	 * Insert a concept node.
	 * @param parent         Parent node to insert.
    * @param node           Concept node to insert.
	 * @return the fragment newly created.
    */
	GResNode* InsertPtr(GConceptNode* parent,GConceptNode* node);

	/**
	 * Insert a node.
    * @param node           Concept node to insert.
	 * @param all            Insert all its child nodes?
	 * @return the fragment newly created.
    */
	GResNode* InsertPtr(GConceptNode* node,bool all=true);

	/**
	 * @return the document identifier.
	 */
	size_t GetDocId(void) const {return(DocId);}

	/**
	 * Reduce the node set.
	 * @param force          Force to merge the fragment sharing the same node.
	 *                       It should be true in the case of an AND operator,
	 *                       and false in the case of an OR operator.
    */
	void Reduce(bool force);

	/**
	 * Print the information of each fragment.
	 */
	void Print(void);
};


//------------------------------------------------------------------------------
#endif
