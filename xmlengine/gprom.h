/*

	GALILEI Research Project

	GProm.h

	Document Fragment PROMETHEE Ranking - Header.

	Copyright 2004-2012 by Pascal Francq.
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
#ifndef GPromH
#define GPromH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <galilei.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// forward declaration
class GEngineXML;
class GResNode;
class GQuery;


//-----------------------------------------------------------------------------
class GPromSol : public RPromSol
{
	/**
	 * Fragment to rank.
	 */
	GResNode* Node;

public:

	/**
	 * Constructor.
    * @param id             Identifier of the solution.
    * @param node           Node.
    */
	GPromSol(const size_t id,GResNode* node);

	friend class GEngineXML;
};


//-----------------------------------------------------------------------------
class GProm : RPromKernel
{
	class cDocTfIdf;
	class cOccur;

	/**
	* Engine.
	*/
	GEngineXML* Engine;

	/**
	 * Query.
	 */
	GQuery* Query;

	/**
	* The tf-idf criteria.
	*/
	RPromCriterion* TfIdf;

	/**
	* The tf-iff Criteria.
	**/
	RPromCriterion* TfIff;

	/**
	* The specificity criteria.
	*/
	RPromCriterion* Specificity;

	/**
	* The distance criteria.
	*/
	RPromCriterion* Distance;

	/**
	* The type criteria.
	*/
	RPromCriterion* Type;

	/**
	 * Tf-idf factors of documents.
	 */
	RContainer<cDocTfIdf,true,true> TfIdfs;

	/**
	 * Temporary container of occurs.
	 */
	RContainer<cOccur,true,true> TmpOccurs;

public:

	/**
	* Constructor.
	* @param engine          Engine.
	* @param query           Query.
	*/
	GProm(GEngineXML* engine,GQuery* query);

	/**
	 * Add a node (document fragment) to the PROMETHEE kernel.
	 */
	void Add(GResNode* node);

	/**
	 * @return the number of solutions managed by the kernel.
	 */
	size_t GetNbSols(void) const {return(RPromKernel::GetNbSols());}

	/**
	 * Compute the solutions.
    * @param sols           Container to fill.
    */
	RCursor<RPromSol> Compute(void);

	/**
    * @return the Fi of the worst solution.
    */
	double GetMinFi(void) {return(RPromKernel::GetMinFi());}

	/**
    * @return the Fi of the best solution.
    */
	double GetMaxFi(void) {return(RPromKernel::GetMaxFi());}

private:

	/**
	 * Compute the average idf factor for all keywords of the query contained in
	 * a fragment.
    * @param node           Fragment.
    * @return the idf factor.
    */
	double ComputeTfIdf(GResNode* node);

	/**
	 * Collect the occurrences of all concepts appearing in a node (and all its
	 * child nodes).
    * @param node           Node to scan.
    */
	void CollectOccurs(GConceptNode* node);

	/**
	 * Compute the average iff factor for all keywords of the query contained in
	 * a fragment.
    * @param node           Fragment.
    * @return the iff factor.
    */
	double ComputeTfIff(GResNode* node);

	/**
	 * Compute the specificity of a given node. Normally, it is defined as the
	 * inverse of the total number of child nodes and the criteria must be
	 * maximized. For efficency, we minimize the total number of child nodes.
    * @param node           Fragment.
    * @return the specificity factor.
    */
	double ComputeSpecificity(GResNode* node);

	/**
	 * Compute the average distance between all pairs of keywords of the querying
	 * in the fragment. The distance between two keywords in a fragment is the
	 * size of the text window including the node corresponding to the fragment
	 * and all keywords contained.
    * @param node           Fragment.
    * @return the distance factor.
    */
	double ComputeDistance(GResNode* node);

	/**
	 * Compute the importance of a given fragment depending of the type of its
	 * node.
    * @param node           Fragment.
    * @return the type factor.
    */
	double ComputeType(GResNode* node);

public:

	~GProm(void);
};


//-----------------------------------------------------------------------------
#endif
