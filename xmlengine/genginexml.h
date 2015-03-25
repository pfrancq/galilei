/*

	GALILEI Research Project

	GEngineXML.h

	XML Search Engine - Header.

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
#ifndef GEngineXMLH
#define GEngineXMLH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <robject.h>
#include <rstack.h>
#include <gengine.h>
#include <gsearchquery.h>
#include <gsearchquerynode.h>
#include <gsearchtoken.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// forward declaration
class GQuery;
class GQueryRes;


//------------------------------------------------------------------------------
/**
* The GEngineXML class provides a search engine over XML documents
* @short XML Search Engine
*/
class GEngineXML : public RObject, public GEngine
{
	class cTreeRef;

	/**
	* Number of results
	*/
	size_t NbResults;

	/**
	 * Retrieve only documents.
	 */
	bool OnlyDocs;

	/**
	 * Container of the most used document trees.
	 */
	RContainer<cTreeRef,true,true> Trees;

	/**
	 * Beginning synaptic position of a window (<=0).
	 */
	int BeginWindowPos;

	/**
	 * Ending synaptic position of a window (>=0).
	 */
	int EndWindowPos;

	/**
	 * The last query executed.
	 */
	GSearchQuery* Query;

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
	* Construct XML search engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GEngineXML(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Request a query.
	* @param query           Query.
	*/
	virtual void PerformRequest(GSearchQuery* query);

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
	void ApplyOperator(GSearchQueryNode::tOperator op,GDocRef* left,GDocRef* right,GDocRef* res);

	/**
	 * Apply a given operator. In practice, it takes the two most results on the
	 * stack as respectively the right and the left operands.
    * @param op             Operator to apply.
    */
	void ApplyOperator(GSearchQueryNode::tOperator op);

	/**
	 * Adapt the list of results with a current node. in practice, if the node
	 * corresponds to a concept, a list is filled with all its occurrences and
	 * pushed if not empty. If it corresponds to an operator, two lists are
	 * popped and a result is pushed.
	 * @param node           Node to analyze. If null, the whole query is
	 *                       performed.
    */
	void Perform(GSearchQueryNode* node);

	/**
	 * Get the last query executed.
    * @return
    */
//	GQuery* GetQuery(void) const {return(Query);}

	/**
	 * Method used to ordered the blocks by descending order of accesses.
	 * @param a              Pointer to a block.
	 * @param b              Pointer to the second block.
	 */
	static int sortOrderAccess(const void* a,const void* b);

	/**
	 * Get a concept tree for a given document. If it is not loaded into memory,
	 * the engine loads it.
	 * @param docid          Identifier of the document.
	 * @return a pointer to the tree.
	 */
	const GConceptTree* GetTree(size_t docid);

	/**
	 * Destructor.
    */
	~GEngineXML(void);


	friend class GQueryRes;
};


//-----------------------------------------------------------------------------
#endif
