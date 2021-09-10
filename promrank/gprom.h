/*

	GALILEI Research Project

	GProm.h

	Document Fragment PROMETHEE Ranking - Header.

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
#ifndef GPromH
#define GPromH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <galilei.h>
#include <gdocfragment.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// forward declaration
class GPromRank;


//------------------------------------------------------------------------------
//
// class GPromSol
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GPromSol : public RPromSol
{
public:

	/**
	 * Fragment to rank.
	 */
	GDocFragmentRank* Fragment;

	GPromSol(const size_t id,size_t pos,GDocFragmentRank* fragment)
		: RPromSol(id,RString::Number(pos),5), Fragment(fragment)
	{}
};



//-----------------------------------------------------------------------------
class GProm : RPromKernel
{
	class cDocTfIdf;
	class cOccur;
	class Iterator;
	class Iterator2;

	/**
	* Engine.
	*/
	GPromRank* Rank;

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

	/**
	 * Weighting method used.
    */
	GMeasure* Weighting;

public:

	/**
	* Constructor.
	* @param rank            Ranking method.
	* @param weighting       Weighting method.
	*/
	GProm(GPromRank* rank,GMeasure* weighting);

	/**
	 * Add a node (document fragment) to the PROMETHEE kernel.
	 */
	void Add(GDocFragmentRank* node);

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
	 * a document.
    * @param node           Fragment.
    * @return the idf factor.
    */
	double ComputeTfIdf(GDocFragment* node);

	/**
	 * Collect the occurrences of all concepts appearing in a record (and all its
	 * child nodes).
	 * @paral doc            Document.
    * @param rec            Record to scan.
    */
	void CollectOccurs(GDoc* doc,const GConceptRecord* rec);

	/**
	 * Compute the average iff factor for all keywords of the query contained in
	 * a fragment.
    * @param node           Fragment.
    * @return the iff factor.
    */
	double ComputeTfIff(GDocFragment* node);

	/**
	 * Compute the specificity of a given node. Normally, it is defined as the
	 * inverse of the total number of child nodes and the criteria must be
	 * maximized. For efficency, we minimize the total number of child nodes.
    * @param node           Fragment.
    * @return the specificity factor.
    */
	double ComputeSpecificity(GDocFragment* node);

	/**
	 * Compute the average distance between all pairs of keywords of the query
	 * and in the fragment. The distance between two keywords in a fragment is
	 * the size of the text window including the node corresponding to the
	 * fragment and all keywords contained.
    * @param node           Fragment.
    * @return the distance factor.
    */
	double ComputeDistance(GDocFragment* node);

	/**
	 * Compute the importance of a given fragment depending of the type of its
	 * node.
    * @param node           Fragment.
    * @return the type factor.
    */
	double ComputeType(GDocFragment* node);

public:

	/**
	 * Destructor.
	 */
	~GProm(void);
};


//-----------------------------------------------------------------------------
#endif
