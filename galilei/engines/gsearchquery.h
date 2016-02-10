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
 * A query may have multiple options as defined with tQueryOption.
 * @short Search Query.
 */
class GSearchQuery : protected R::RTree<GSearchQuery,GSearchQueryNode,true>
{
	class cExpressions;

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
	 * Options of the query.
	 */
	int Options;

	/**
	 * Temporary concepts.
	 */
	R::RContainer<GConcept,false,true> Stems;

	/**
	 * Languages used for the stemming if necessary.
    */
	R::RContainer<GLang,false,false>* Langs;

public:

	/**
	 * Constructor of the query.
	 * @param session        Session.
	 * @param options        Options of the query.
    */
	GSearchQuery(GSession* session,int options=qoStems|qoKeywords|qoExpressions,R::RContainer<GLang,false,false>* langs=0);

	/**
	 * Get the top node of a query.
    * @return a pointer to a GSearchQueryNode.
    */
	GSearchQueryNode* GetTop(void) {return(R::RTree<GSearchQuery,GSearchQueryNode,true>::GetTop());}

	/**
	 * Get the top node of a query.
    * @return a pointer to a GSearchQueryNode.
    */
	const GSearchQueryNode* GetTop(void) const {return(R::RTree<GSearchQuery,GSearchQueryNode,true>::GetTop());}

	/**
	 * Look if only the AND operator is used.
	 * @return true if it is the case.
	 */
	bool UseOnlyAND(void) const {return(OnlyAND);}

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
	 * Build the query based on a string. By default, it is analyses the string
	 * in order to create the tree. In practice, the query is parsed from left
	 * to right.
    * @param query          Query submitted.
    */
	virtual void Build(const R::RString& query);

	/**
	 * Insert a token in the query tree.
    * @param parent         Parent token.
    * @param node           Token to add.
    */
	void InsertNode(GSearchQueryNode* parent,GSearchQueryNode* node);

	/**
	 * Move a token in the query tree.
    * @param to             Future parent token.
    * @param node           Token to add.
    */
	void MoveNode(GSearchQueryNode* to,GSearchQueryNode* node);

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
	 * Create the nodes appearing in a given string. The method is called
	 * recursively to obtain the whole query.
    * @param parent         Parent node (if null, it is the top node).
    * @param str            String to analyze.
    * @return true if a node was created for the string passed.
    */
	bool CreateToken(GSearchQueryNode* parent,const R::RString& str);

	/**
	 * Create a token and insert it in the query tree. Eventually, if the
	 * token is a keyword, stems are used as alternatives and combine with the
	 *  OR operator.
    * @param parent         Parent token.
    * @param token          String representing the keyword.
    * @param type           Type of the token
	 * @param multiple       In the case of a keyword, are there multiple terms.
    * @return a pointer to the node  was inserted in the query tree.
    */
	GSearchQueryNode* CreateToken(GSearchQueryNode* parent,const R::RString& token,GSearchToken::tType type,bool multiple);

private:

	/**
	 * Find all the expressions for which a given concept is a part of.
	 * @param concept        Concept which must be contained in the expression.
	 * @param exprs          Container of expressions.
	 */
	void FindExpressions(GConcept* concept,cExpressions& exprs);

	/**
	 * Treat a node to find an AND that can create an expression.
    * @param node           Node to treat.
    */
	void TreatNode(GSearchQueryNode* node);

	/**
	 * Build all the possible expressions that results from the combinations of
	 * the keywords linked by AND operators.
    * @param node           Node to treat.
    * @param exprs          Expression that represent a concept.
    */
	void TreatANDNode(GSearchQueryNode* node,cExpressions& exprs);

public:

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
namespace std{
//------------------------------------------------------------------------------


#ifndef __APPLE__
extern "C++"
{
#endif

//------------------------------------------------------------------------------
/**
 * Print a query.
 * @param os                Output stream.
 * @param query             Query to print.
 * @return the stream.
 */
extern std::ostream& operator<<(std::ostream& os,const GALILEI::GSearchQuery& query);

#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
