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


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GSearchQuery class provides a representation for a structured query, that is a
 * tree of query nodes, where each node is either an operator or a word.
 * @short Structured Query.
 */
class GSearchQuery : public R::RTree<GSearchQuery,GSearchQueryNode,true>
{
	/**
	 * Session launching the query.
	 */
	GSession* Session;

	/**
	 * Container of concepts.
	 */
	R::RContainer<GConcept,false,false> Concepts;

public:

	/**
	 * Constructor of the query.
	 * @param session        Session.
	 * @param query          Query submitted.
    */
	GSearchQuery(GSession* session,const R::RString& query);

	/**
	 * Set the query. It is analyzes the query in order to create the tree. In
	 * practice, the query is parsed from left to right.
    * @param query          Query submitted.
    */
	void Set(const R::RString& query);

private:

	/**
	 * Create the nodes appearing in a given string. The method is called
	 * recursively to obtain the whole query.
    * @param parent         Parent node (if null, it is the top node).
    * @param str            String to analyze.
    * @return true if a node was created for the string passed.
    */
	bool CreateToken(GSearchQueryNode* parent,const R::RString& str);

	/**
	 * Create a new node corresponding to a string.
    * @param str            String
	 * @param define         String contains a type.
    * @return a pointer that will be inserted in the query tree.
    */
	GSearchQueryNode* NewNode(const R::RString& str,bool type);

	/**
	 * Create a new node corresponding to an operator.
    * @param op             Operator.
    * @return a pointer that will be inserted in the query tree.
    */
	GSearchQueryNode* NewNode(GSearchQueryNode::tOperator op);

	/**
	 * Clear a string. In practice, the leading and ending spaces are removed. If
	 * the string is an expression, the corresponding parentheses are removed.
    * @param str            String to clear.
    * @return the string cleared.
    */
	R::RString ClearString(const R::RString& str);

public:

	/**
	 * Print a node to the console. A number of spaces corresponding to its
	 * depth in the tree is inserted before. After that, the method is called for
	 * its children.
    * @param node           Node to print. If null, the top node is taken.
    */
	void Print(GSearchQueryNode* node);

	/**
	 * Look if a given concept is present in the query.
	 * @return true if a given concept is used in the query.
	 */
	bool IsIn(const GConcept& concept) const;

	/**
	 * Get concepts that must be present in the documents.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SZE_MAX, iterate until the end of the container.
	 * @return a R;;RCursor.
	 */
	R::RCursor<GConcept> GetConcepts(size_t min=0,size_t max=SIZE_MAX) const;

	/**
	 * Destructor.
	 */
	~GSearchQuery(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
