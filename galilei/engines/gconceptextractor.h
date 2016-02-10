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
	R::RContainer<GConcept,false,false> Stems;

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
	 * Compute the stems corresponding to a set of tokens for a given language.
	 * @param data           Data.
	 * @param lang           Language.
	 * @return true if a stem exists in the dictionaries for all tokens.
	 */
	bool ComputeStems(GConceptExtractorData* data,GLang* lang);

public:

	/**
	 * Find a set of concepts related to a given string.
	 * @param str            String to search for.
	 * @param multiple       Can the string contains several parts (such as
	 *                       multiple term).
	 * @param results        Container of vectors representing the possible
	 *                       concepts. A given vector stores equivalent or
	 *                       similar concepts, while each vector represents a
	 *                       different meaning. Each vector is cleared, but no
	 *                       the container (i.e. some vectors may be empty).
	 * @param caller         Identifier of the caller (for example a thread).
	 */
	virtual void Search(R::RString& str,bool multiple,R::RContainer<GVector,true,false>& results,size_t caller);
};

}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
