/*

	GALILEI Research Project

	GMetaEngine.h

	Meta-Engine - Header.

	Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef GMetaEngineH
#define GMetaEngineH


//------------------------------------------------------------------------------
// include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMetaEngine class provides a representation for a generic meta-search
* engine. In practice, it manages a set of results (document fragments) for a
* given query passed by the user.
*
* In practice, a meta-engine does the following thinks
* -# It calls the method PrepareRequest(GSearchQuery*). By default, it
*    clears the old results and call GEngine::Clear(GMetaEngine*,GSearchQuery*)
 *   for each enabled engine.
* -# It calls the method RequestEngines(GSearchQuery*). By default, the
*    method forwards the query passed to each enabled engine. Inheriting classes
*    may redefine this method to adapt the query to each engine or to select
*    specific engines. Each time a engine add a fragment as a result, the
*    method FragmentRankAdded(GDocFragmentRank*,GEngine*) is called.
* -# It calls the method ComputeGlobalRanking(void) to compute a global ranking.
*    This method should be overloaded by inheriting classes.
* -# The results are ranked by ascending ranking.
* -# It calls the method PostRequest(void). By default, this method does
*    nothing.
*
* See the documentation related to GPlugIn for more general information.
* @short Meta-search Engine
*/
class GMetaEngine : public GPlugIn, R::RDownloadFile
{
	/*
	 * Internal structure for the results.
	 */
	class Internal
	{
	public:
		/**
		* All fragments classified by documents.
		*/
		R::RContainer<GDocRef,true,true> ResultsByDocs;

		/**
		* All document fragments.
		*/
		R::RContainer<GDocFragment,false,false> Results;

		/**
		 * All the rankings for each document fragments.
		 */
		R::RContainer<GDocFragmentRanks,true,true> Rankings;

		/**
		 * Constructor.
       */
		Internal(void);
	};

	/**
	 * Data used to store the results.
	 */
	R::RThreadLocalStorage<Internal> Data;

	/**
	 * Extractor used.
	 */
	GConceptExtractor* Extractor;

public:

	/**
	* Construct a meta-engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GMetaEngine(GSession* session,GPlugInFactory* fac);

	/**
	 * Initialize the plug-in.
	 */
	virtual void Init(void);

private:

	/**
	 * Add a fragment from a known document as result to the meta-engine. In
	 * practice, it adds an entry to the container of results.
	 * @param doc            Document.
	 * @param rec            Concept record
	 * @param pos            Position to the fragment to extract.
	 * @param spos           Syntactic position of the fragment centre.
	 * @param first          First concept found.
	 * @param last           Last concept found.
	 * @param ranking        Ranking of the document given by the engine
	 *                       (\f$0\leq ranking \leq 1\f$). When a ranking
	 *                       algorithm is associated to the engine, it will
	 *                       overwrite this ranking.
	 * @param engine         Engine from which the result come.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @warning The deallocation of the record must be managed by the caller.
	 * @return a pointer to a GDocFragment.
	 */
	GDocFragment* AddResult(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,GEngine* engine,size_t caller);

	/**
	 * Add a fragment from a known document as result to the meta-engine. In
	 * practice, it adds an entry to the container of results.
	 * @param docid          Identifier of the document.
	 * @param rec            Concept record
	 * @param pos            Position to the fragment to extract.
	 * @param spos           Syntactic position of the fragment centre.
	 * @param first          First concept found.
	 * @param last           Last concept found.
	 * @param ranking        Ranking of the document given by the engine
	 *                       (\f$0\leq ranking \leq 1\f$). When a ranking
	 *                       algorithm is associated to the engine, it will
	 *                       overwrite this ranking.
	 * @param engine         Engine from which the result come.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @warning The deallocation of the record must be managed by the caller.
	 * @return a pointer to a GDocFragment.
	 */
	GDocFragment* AddResult(size_t docid,const GConceptRecord* rec,size_t spos,size_t pos,size_t first,size_t last,double ranking,GEngine* engine,size_t caller);

protected:

	/**
	 * Create an extractor.
	 * @return a pointer to an extractor.
	 */
	virtual GConceptExtractor* CreateConceptExtractor(void);

	/**
	 * Get the extractor used.
	 * @return a pointer to an extractor.
	 */
	GConceptExtractor* GetExtractor(void) const {return(Extractor);}

	/**
	 * method called each time an engine  add a given document fragment as a
	 * result. By default, the method does nothing.
    * @param rank           Document fragment added.
	 * @param engine         Engine from which the result come.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	virtual void FragmentRankAdded(GDocFragmentRank* rank,GEngine* engine,size_t caller);

	/**
	 * Initialise the query. The containers are cleared and the method
	 * GEngine::Clear(GMetaEngine*) is called for each enabled engine.
    * @param query          Query.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	virtual void PrepareRequest(GSearchQuery* query,size_t caller);

	/**
	* Send a query to the engines. By default, it is send to all
	* enabled engines.
	* @param query           Query.
	* @param caller         Identifier of the caller (for example a thread).
	*/
	virtual void RequestEngines(GSearchQuery* query,size_t caller);

	/**
	* This function is used to compute the global ranking
	* for documents extracted from different search engines.
	* Results are then sort using this global ranking
	* @param caller         Identifier of the caller (for example a thread).
	*/
	virtual void ComputeGlobalRanking(size_t caller);

	/**
	 * Perform some tasks once the request was treated.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	virtual void PostRequest(size_t caller);

public:

	/**
	 * Build a search query. By default, it creates an object of the class
	 * QSearchQuery and build it. Moreover, the stems are used to expand the
	 * query.
    * @param query          String representing the query.
	 * @param caller         Identifier of the caller (for example a thread).
    * @return a pointer to a QSearchQuery.
    */
	virtual GSearchQuery* BuildQuery(const R::RString query,size_t caller);

	/**
	 * Send a query to the meta-search engine. It call GMetaEngine::PerformRequest
	 * and order then all the document fragments.
	 * @param query          Query.
	 * @param caller         Identifier of the caller (for example a thread).
	 */
	void Request(GSearchQuery* query,size_t caller);

	/**
	 * Get all the fragments retrieved by documents.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @return a cursor on GDocRef.
	 */
	R::RCursor<GDocRef> GetResultsByDocs(size_t caller);

	/**
	 * Get all the fragments retrieved by documents.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @return a cursor on GDocRef.
	 */
	R::RCursor<GDocFragment> GetResults(size_t caller);

	/**
	 * @return a cursor of the rankings retrieved by the meta-search engine.
	 * @param caller         Identifier of the caller (for example a thread).
	 */
	R::RCursor<GDocFragmentRanks> GetRankings(size_t caller);

	/**
	 * @return the number of results.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	size_t GetNbResults(size_t caller);

	/**
	 * The method is called each time a session is closed.
	 */
	virtual void Done(void);

	/**
	* Destructor of the meta-engine.
	*/
	virtual ~GMetaEngine(void);

	friend class GEngine;
	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_METAENGINE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GMetaEngine,plugin,"MetaEngine",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
