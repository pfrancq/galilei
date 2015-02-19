/*

	GALILEI Research Project

	GMetaEngineSum.h

	Meta-engine based on a Weighted Sum - Header.

   Copyright 2003-2015 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GMetaEngineSumH
#define GMetaEngineSumH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gmetaengine.h>
#include <gengine.h>
#include <gdocfragment.h>
using namespace std;
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
/**
* The GMetaEngineSum class provides a representation for a results extractor
* for different Search Engines
* @short Meta-engine based on a Weighted Sum
*/
class GMetaEngineSum : public GMetaEngine
{
public:

	/**
	 * Query type.
	 */
	enum tType
	{
		Single              /** All keywords formed one query.*/,
		kCombinations       /** Create k-combinations of terms.*/
	};

private:

	/**
	 * Query.
	 */
	RString Query;

	/**
	* The set of keywords
	*/
	R::RContainer<R::RString,true,false> Keywords;

	/**
	* The set of words to create the query
	*/
	R::RContainer<R::RString,false,false> QueryWords;

	/**
	 * Query type.
	 */
	tType Type;

	/**
	* specify if the meta engine must use weight associated to the engines.
	*/
	bool UseWeight;

	/**
	* Specifies if the values for the parameters are percentages of the value or
	* absolute values.
	*/
	bool PercentValue;

	/**
	* Minimum value of the "k" parameter.
	*/
	size_t kMin;

	/**
	* Maximal value of the "k" parameter.
	*/
	size_t kMax;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GMetaEngineSum(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Add a fragment from a known document as result to the meta-engine. In
	* practice, it adds an entry to the container of results.
	* @param docid           Identifier of the document.
	* @param pos             Position to the fragment to extract.
	* @param first           First concept found.
	* @param last            Last concept found.
	* @param ranking         Ranking of the document given by the engine
	*                        (\f$0\leq ranking \leq 1\f$).
	* @param engine          Engine from which the result come.
	*/
	virtual void AddResult(size_t docid,size_t pos,size_t first,size_t last,double ranking,const GEngine* engine);

	/**
	* Send a query to the meta-search engine.
	* @param query           Query.
	*/
	virtual void Request(const R::RString query);

	/**
	* Build all the queries based on Keywords depending of the type.
	*/
	void BuildRequests(void);

	/**
	* A recursive method that combines all the words of the query. In practice,
	* the first call launches a Combination (k n).
	* @param pos             Position of the last inserted keywords (used by the
	 *                       recursion).
	* @param k               The k parameter.
	*/
	void CombineKeywords(size_t pos,size_t k);

	/**
	* Send a query to the different engines.
	* @param query           Query.
	*
	*/
	void RequestEngines(const R::RString& query);

	/**
	* This function is used to compute the global ranking
	* for documents extracted from different search engines.
	* Results are then sort using this global ranking
	*/
	void ComputeGlobalRanking(void);

	/**
	* Function used by "qsort" to sort the results
	*/
	static int sortOrder(const void* a, const void* b);
};


//-----------------------------------------------------------------------------
#endif
