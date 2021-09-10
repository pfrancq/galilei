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
	class cComplexConcept;

	/**
	 * Tokens extracted from the search string.
	 */
	R::RContainer<R::RString,true,false> Tokens;

	/**
	 * Stems extracted from the search string.
	 */
	R::RContainer<R::RString,true,false> Stems;

	/**
	 * Complex concepts found.
	 */
	R::RContainer<cComplexConcept,true,true> Complex;

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
 * concepts related to a given string or concept.
 *
 * It is be used by GSearchQuery in the future to build a query.
 *
 * @short Concept Extractor.
 */
class GConceptExtractor
{
protected:

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


	/***
	 * Search all complex concepts containing a given token.
	 * @param token          Token to search for.
	 * @param data           Data containing the results.
	 */
	void SearchComplexConcept(const R::RString& token,GConceptExtractorData* data);

public:

	/**
	 * Find a set of concepts related to a given string. For each possible
	 * concept, a weight is associated (through the GConceptRef class) that
	 * represents a sort of probability.
	 *
	 * When the string contains only one token, all the different existing
	 * stems are added with a weight of 1.0.
	 *
	 * For expressions, the extractor searches for a matching once stopwords are
	 * removed and stemming is done. A weight corresponds to the amount of
	 * tokens defining the corresponding expression that are contained in the
	 * string.
	 * @param str            String to search for.
	 * @param results        Container of concept references representing the
	 *                       possible concepts. It is cleared by the method.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @return the number of concepts found.
	 */
	virtual size_t Search(const R::RString& str,R::RContainer<GConceptRef,true,true>& results,size_t caller);

	/**
	 * Look if a query node can be expanded with better options (or a set of
	 * better options). For example, one may decide that a given thesaurus 
	 * should be preferred, and that other concepts should be replace if 
	 * possible.
	 *
	 * By default, this methods does nothing.
	 * @param query          Query.
	 * @param node           Node to analyse.
	 * @param caller         Identifier of the caller (for example a thread).
	 */
	virtual void Expand(GSearchQuery* query,GSearchQueryNode* node,size_t caller);

	/**
	 * Destructor.
	 */
	virtual ~GConceptExtractor(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
