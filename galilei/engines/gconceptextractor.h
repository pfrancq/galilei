/*

	GALILEI Research Project

	GConceptExtractor.h

	Concept Extractor - Header.

	Copyright 2010-2016 by Pascal Francq (pascal@francq.info).

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
#ifndef GConceptExtractorH
#define GConceptExtractorH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rthread.h>
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GConceptExtractorData provides a data structure that is internally used.
 */
class GConceptExtractorData
{
	/**
	 * Tokens extracted from the search string.
	 */
	R::RContainer<R::RString,true,false> Tokens;

	/**
	 * Stems extracted from the search string.
	 */
	R::RContainer<R::RString,true,false> Stems;

	R::RString Aggregate;

public:

	/**
	 * Constructor.
	 */
	GConceptExtractorData(void);

	/**
	 * Reset the structure before a search is performed.
	 */
	void Reset(void);

	/**
	 * Destructor.
	 **/
	~GConceptExtractorData(void);

	friend class GConceptExtractor;
};


//------------------------------------------------------------------------------
/**
 * The GConceptExtractor class provides a mechanism to search for a set of
 * concepts related to a given string.
 *
 * @remark It will be used by GSearchQuery in the future to build a query.
 *
 * @short Concept Extractor.
 */
class GConceptExtractor
{
	/**
	 * Session.
	 */
	GSession* Session;

	/**
	 * Internal data used by the extractor.
	 */
	R::RThreadLocalStorage<GConceptExtractorData> Datas;

public:

	/**
	 * Construct a concept extractor.
	 * @param session        Session.
	 */
	GConceptExtractor(GSession* session);

private:

	/**
	 * Add a result in the container by verifying that it is not already there.
	 * Moreover, the highest weight is retain.
	 * @param concept        Concept to add.
	 * @param results        Container to which concepts must be added.
	 * @param weight         Weight associated to each concept found.
	 */
	void AddResult(GConcept* concept,R::RContainer<GConceptRef,true,true>& results,double weight);

	/**
	 * Look for an expression for each concept type that doesn't contains only
	 * single token.
	 * @param expr           Expression to search for.
	 * @param results        Container to which concepts must be added.
	 * @param weight         Weight associated to each concept found.
	 */
	void AddExpression(const R::RString& expr,R::RContainer<GConceptRef,true,true>& results,double weight);

public:

	/**
	 * Find a set of concepts related to a given set of tokens. For each possible
	 * concept, a weight is associated (through the GConceptRef class) that
	 * represents a sort of probability.
	 *
	 * When there is only one token, all the different existing stems are added
	 * with a weight of 1.0.
	 *
	 * For expressions, the extractor searches for a matching once stopwords are
	 * removed and stemming is done.
	 * @param tokens         Tokens to search for.
	 * @param results        Container of concept references representing the
	 *                       possible concepts. It is cleared by the method.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @return the number of concepts found.
	 */
	virtual size_t Search(R::RContainer<R::RString,true,false>& tokens,R::RContainer<GConceptRef,true,true>& results,size_t caller);
};

}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
