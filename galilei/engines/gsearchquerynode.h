/*

	GALILEI Research Project

	GSearchQueryNode.h

	Search Query Node - Header.

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
#ifndef GSearchQueryNodeH
#define GSearchQueryNodeH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rnode.h>
#include <rstring.h>
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/**
 * The GSearchQueryNode class provides a representation of a query node. In practice,
 * it maintains either an operator or a pointer to a concept.
 * @short Query Node
 * @warning Actually, it is supposes that the word submitted is in English and
 * is stemmed.
 */
class GSearchQueryNode : public R::RNode<GSearchQuery,GSearchQueryNode,true>
{
public:

	/**
	 * A query is a tree composed from nodes which are either operators or concepts.
	 * @short Query Token Type.
	 */
	enum tNode
	{
		nOperator,              /** The token is an operator.*/
		nConcept                /** The token is a concept.*/
	};

	/**
	 * Several operators can be used in a structured query. Besides the classical
	 * Boolean operators, some operators allow hierarchical relations.
	 */
	enum tOperator
	{
		oNOP,                   /** Invalid operator.*/
		oNOT,                   /** NOT operator.*/
		oOR,                    /** OR operator.*/
		oAND,                   /** AND operator.*/
		oINC,                   /** INCLUSION operator. "a INC b" supposes that 'b'
											  is a concept that must appear in a child node of
											  ones containing 'a'.*/
		oSIB,                  /** SIBLING operator. "a SIB b" supposes that 'a' and
											 'b' must appear in the same node.*/
		oNAND,                 /** NOT AND operator.*/
		oNINC,                 /** NOT INCLUSION operator.*/
		oNSIB                  /** NOT SUBLING operator.*/
	};

private:

	/**
	 * Type of the node.
	 */
	tNode Type;

	/**
	 * Value of the node.
	 */
	union
	{
		/**
		 * The node corresponds to an operator.
		 */
		tOperator Operator;

		/**
		 * The node corresponds to a concept.
		 */
		GConcept* Concept;
	}
	Value;

public :

	/**
	 * Construct an operator node.
    * @param op             Operator.
    */
	GSearchQueryNode(tOperator op);

	/**
	 * Construct a concept node.
    * @param session        Session needed to search for the concept
	 *                       corresponding to a string.
    * @param str            String that is a concept.
	 * @param type           String defines a type.
    */
	GSearchQueryNode(GSession* session,const R::RString& str,bool type);

	/**
    * @return the type of the node.
    */
	tNode GetType(void) const {return(Type);}

	/**
    * @return the operator of the node.
	 * @exception an exception is generated if the node doesn't contain an
	 * operator.
    */
	tOperator GetOperator(void) const;

	/**
    * @return a pointer to a concept.
	 * @exception an exception is generated if the node doesn't contain a
	 * concept.
    */
	GConcept* GetConcept(void) const;

	/**
	 * Look if a character correspond to a given operator.
	 * @param c                 Character to look for.
	 * @return the operator or toNOP if the character is an invalid one.
	 */
	static tOperator GetOperator(const R::RChar c);

	/**
	 * Look if a given character correspond to a operator that can be inverted.
	 * @param c                 Character to look for.
	 * @return the operator or toNOP if the character is an invalid one or has no
	 * inverted version.
	 */
	static tOperator GetNotOperator(const R::RChar c);

	friend class R::RTree<GSearchQuery,GSearchQueryNode,true>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
