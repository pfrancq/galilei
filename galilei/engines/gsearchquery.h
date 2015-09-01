/*

	GALILEI Research Project

	GSearchQuery.h

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
#ifndef GSearchQueryH
#define GSearchQueryH


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
#include <gsearchquerynode.h>
#include <gsearchtoken.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GSearchQuery class provides a representation for a structured query, that
 * is a tree of query nodes, where each node is either an operator or a
 * keyword.
 *
 * A query can be expanded using the stems as alternatives to the keywords. Let
 * us suppose the original query "connects & accepting" passed as argument. If
 * expanded with stems, it query applied will be "(connects | connect) &
 * (accepting | accept)".
 * @short Search Query.
 */
class GSearchQuery : protected R::RTree<GSearchQuery,GSearchQueryNode,true>
{
	/**
	 * Session launching the query.
	 */
	GSession* Session;

	/**
	 * Tokens given in the query.
	 */
	R::RContainer<R::RString,true,true> Tokens;

	/**
	 * Container of all concepts.
	 */
	R::RContainer<GConcept,false,true> Concepts;

	/**
	 * AND is the only operator used in the query.
	 */
	bool OnlyAND;

	/**
	 * Should query be expanded with the stems of the keywords ?
    */
	bool ExpandStems;

	/**
	 * Temporary concepts.
	 */
	R::RContainer<GConcept,false,true> Stems;

public:

	/**
	 * Constructor of the query.
	 * @param session        Session.
	 * @param expandstems    Define if the query is expanded with the stems of
	 *                       the keywords.
    */
	GSearchQuery(GSession* session,bool expandstems);

	/**
	 * Build the query based on a string. Bu default, it is analyzes the string
	 * in order to create the tree. In practice, the query is parsed from left
	 * to right.
    * @param query          Query submitted.
    */
	virtual void Build(const R::RString& query);

	/**
	 * Look if only the AND operator is used.
	 * @return true if it is the case.
	 */
	bool UseOnlyAND(void) const {return(OnlyAND);}

	/**
	 * Get the top node of a query.
    * @return a pointer to a GSearchQueryNode.
    */
	GSearchQueryNode* GetTop(void) {return(R::RTree<GSearchQuery,GSearchQueryNode,true>::GetTop());}

	/**
	 * Create the nodes appearing in a given string. The method is called
	 * recursively to obtain the whole query.
    * @param parent         Parent node (if null, it is the top node).
    * @param str            String to analyze.
    * @return true if a node was created for the string passed.
    */
	bool CreateToken(GSearchQueryNode* parent,const R::RString& str);

	/**
	 * Insert a token in the query tree.
    * @param parent         Parent token.
    * @param node           Token to add.
    */
	void InsertNode(GSearchQueryNode* parent,GSearchQueryNode* node);

	/**
	 * Create a token and insert it in the query tree. Eventually, if the
	 * keyword is a keyword, stems are used as alternatives and combine with the
	 *  OR operator.
    * @param parent         Parent token.
    * @param token          String representing the keyword.
    * @param type           Type of the token
    * @return a pointer to the node  was inserted in the query tree.
    */
	GSearchQueryNode* CreateToken(GSearchQueryNode* parent,const R::RString& token,GSearchToken::tType type);

	/**
	 * Create a new node corresponding to a string.
    * @param token          String containing the token.
	 * @param type           Type of the token.
    * @return a pointer that will be inserted in the query tree.
    */
	virtual GSearchQueryNode* NewNode(const R::RString& token,GSearchToken::tType type);

	/**
	 * Create a new node corresponding to a given concept.
    * @param concept        Concept.
	 * @param type           Type of the token.
    * @return a pointer that will be inserted in the query tree.
    */
	virtual GSearchQueryNode* NewNode(GConcept* concept,GSearchToken::tType type);

	/**
	 * Create a new node corresponding to an operator.
    * @param op             Operator.
    * @return a pointer that will be inserted in the query tree.
    */
	virtual GSearchQueryNode* NewNode(GSearchQueryNode::tOperator op);

	/**
	 * Clear a string. In practice, the leading and ending spaces are removed. If
	 * the string is an expression, the corresponding parentheses are removed.
    * @param str            String to clear.
    * @return the string cleared.
    */
	R::RString ClearString(const R::RString& str);

	/**
	 * Print a node to the console. A number of spaces corresponding to its
	 * depth in the tree is inserted before. After that, the method is called for
	 * its children.
    * @param node           Node to print. If null, the top node is taken.
    */
	void Print(GSearchQueryNode* node);

	/**
	 * Get the list of tokens.
	 * @return a R;;RCursor.
	 */
	R::RCursor<R::RString> GetTokens(void) const;

	/**
    * @return the number of tokens.
    */
	size_t GetNbTokens(void) const {return(Tokens.GetNb());}

	/**
	 * Get the list of concepts used in the query.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate
	 *                       (included max). If SZE_MAX, iterate until the end
	 *                       of the container.
	 * @return a R;;RCursor.
	 */
	R::RCursor<GConcept> GetConcepts(size_t min=0,size_t max=SIZE_MAX) const;

	/**
    * @return the number of concepts.
    */
	size_t GetNbConcepts(void) const {return(Concepts.GetNb());}

	/**
	 * Look if a given concept was used to formulate the query.
	 * @param concept        Concept to search for.
	 * @return true if the concept was used.
	 */
	bool IsIn(GConcept* concept) const;

	/**
	 * Destructor.
	 */
	~GSearchQuery(void);

	friend class GSearchQueryNode;
	friend class R::RNode<GSearchQuery,GSearchQueryNode,true>;
	friend class R::RTree<GSearchQuery,GSearchQueryNode,true>;
	friend class R::RNodeCursor<GSearchQuery,GSearchQueryNode>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif