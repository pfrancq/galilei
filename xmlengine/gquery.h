/*

	GALILEI Research Project

	GQuery.h

	Search Query - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#ifndef GQueryH
#define GQueryH


//------------------------------------------------------------------------------
// include files for ANSI C++
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rstring.h>
#include <rtree.h>
#include <rcursor.h>
#include <rxmlfile.h>
#include <glang.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for current project
#include <gquerynode.h>
#include <gqueryres.h>


//------------------------------------------------------------------------------
/**
 * The GQuery class provides a representation for a structured query, that is a
 * tree of query nodes, where each node is either an operator or a word.
 * @short Structured Query.
 */
class GQuery : public RTree<GQuery,GQueryNode,true>
{
	/**
	 * Pointer to the engine.
	 */
	GEngineXML* Engine;

	/**
	 * Container of concepts.
	 */
	RContainer<GConcept,false,false> Concepts;

	/**
	 * Temporary container of document.
    */
	RNumContainer<size_t,true> DocIds;

	/**
	 * FIFO stack of results.
	 */
	RStack<GQueryRes,false,true,true> Results;

public:

	/**
	 * Constructor of the query.
	 * @param engine         Engine launching the query.
	 * @param query          Query submitted.
    */
	GQuery(GEngineXML* engine,const RString& query);

	/**
	 * Set the query. It is analyzes the query in order to create the tree. In
	 * practice, the query is parsed from left to right.
    * @param query          Query submitted.
    */
	void Set(const RString& query);

	/**
	 * Create the nodes appearing in a given string. The method is called
	 * recursively to obtain the whole query.
    * @param parent         Parent node (if null, it is the top node).
    * @param str            String to analyze.
    * @return true if a node was created for the string passed.
    */
	bool CreateToken(GQueryNode* parent,const RString& str);

private:

	/**
	 * Create a new node corresponding to a string.
    * @param str            String
	 * @param define         String contains a type.
    * @return a pointer that will be inserted in the query tree.
    */
	virtual GQueryNode* NewNode(const RString& str,bool type);

	/**
	 * Create a new node corresponding to an operator.
    * @param op             Operator.
    * @return a pointer that will be inserted in the query tree.
    */
	virtual GQueryNode* NewNode(tQueryOperator op);

	/**
	 * Clear a string. In practice, the leading and ending spaces are removed. If
	 * the string is an expression, the corresponding parentheses are removed.
    * @param str            String to clear.
    * @return the string cleared.
    */
	RString ClearString(const RString& str);

	/**
	 * Push a list of all occurrences of a given concept. In practice, it
	 * searches for the highest semantic concept in the document tree :
	 * - If the concept is a text one, its parent node is used.
	 * - If the concept is a semantic one (for example a XML tag), it is taken :
    * @param concept        Concept to search for.
    */
	void FindOccurrences(GConcept* concept);

	/**
	 * Apply the operator on two list and fill the results with it.
	 * @param op             Operator to apply.
	 * @param left           Left operand.
	 * @param right          Right operand.
	 * @param res            Result.
    */
	void ApplyOperator(tQueryOperator op,GDocRef* left,GDocRef* right,GDocRef* res);

	/**
	 * Apply a given operator. In practice, it takes the two most results on the
	 * stack as respectively the right and the left operands.
    * @param op             Operator to apply.
    */
	void ApplyOperator(tQueryOperator op);

public:

	/**
	 * Adapt the list of results with a current node. in practice, if the node
	 * corresponds to a concept, a list is filled with all its occurrences and
	 * pushed if not empty. If it corresponds to an operator, two lists are
	 * popped and a result is pushed.
	 * @param node           Node to analyze. If null, the whole query is
	 *                       performed.
    */
	void Perform(GQueryNode* node);

	/**
	 * Print a node to the console. A number of spaces corresponding to its
	 * depth in the tree is inserted before. After that, the method is called for
	 * its children.
    * @param node           Node to print. If null, the top node is taken.
    */
	void Print(GQueryNode* node);

	/**
	 * @return the result on the top of the stack (may be null if no result).
	 */
	const GQueryRes* GetResult(void) const;

	/**
    * @return the number of concepts.
    */
	size_t GetNbConcepts(void) const {return(Concepts.GetNb());}

	/**
	 * @return a container on the concepts that must be present in the documents.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SZE_MAX, iterate until the end of the container.
	 */
	RCursor<GConcept> GetConcepts(size_t min=0,size_t max=SIZE_MAX) const;

	/**
    * @return a cursor over the identifier of the documents.
    */
	RNumCursor<size_t> GetDocIds(void) const;

	/**
	 * @return true if a given concept is used in the query.
	 */
	bool IsIn(const GConcept& concept) const;

	/**
	 * @return true if a given concept is used in the query.
	 */
	bool IsIn(size_t conceptid) const;

	/**
	 * Destructor.
	 */
	~GQuery(void);
};


//------------------------------------------------------------------------------
#endif
