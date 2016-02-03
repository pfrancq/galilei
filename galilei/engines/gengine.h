/*

	GALILEI Research Project

	GEngine.h

	Search Engine - Header.

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
#ifndef GEngineH
#define GEngineH


//------------------------------------------------------------------------------
// include files for CUL library
#include <curl/curl.h>


//------------------------------------------------------------------------------
//include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gdocfragment.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GEngine class provides a representation for a generic search engine. An
 * engine plug-in may :
 * - Make a direct search in the actual corpus of the session.
 * - Call an external engine (for example Google Search), create a document in
 *    the session and process the results.
 * - Call a ranking algorithm to compute the rankings of the results.
 *
 * In practice, a search engine performs two steps:
 * -# It calls the PerformRequest(GSearchQuery*) method. It is a pure virtual
 *    one that must be written by inheriting classes.
 * -# If a ranking algorithm is associated to the engine, it is called after the
 *    query is treated through the GComputeRank::Rank(GEngine*) method. In
 *    practice, the ranking algorithm is supposed to modify the rankings of the
 *    fragments added as results through the 'AddResults' methods.
 *
 * Each time a engine wants to add a results, it must call the one of the
 * AddResult methods.
 *
 * It is the role of inheriting classes to ensure that the query submitted to
 * an external search engine respects its constraints. In particular, it must
 * probably formulate a new compatible query based on the one used in GALILEI.
 *
 * @warning The ranking, \f$ranking\f$, associated by a given search engine must
 * respect the constraint: \f$0\leq ranking \leq 1\f$. Otherwise, an exception
 * is generated. This constraint ensures that each engine normalises its
 * rankings. Without this normalisation, rankings from different engines cannot
 * be compared.
 *
 * See the documentation related to GPlugIn for more general information.
 * @short Generic Search Engine
 */
class GEngine : public GPlugIn
{
	/**
	* All document fragments.
	*/
	R::RContainer<GDocFragment,false,false> Results;

	/**
	 * Ranking method associated to the engine.
	 */
	GComputeRank* Ranking;

	/**
	 * Name of the ranking method used.
	 */
	R::RString RankingMethod;

	/**
	 * Meta engine that calls the engine.
	 */
	GMetaEngine* MetaEngine;

protected:

	/**
	* Weight associated to the engine.
	*/
	double Weight;

public:

	/**
	* Construct an engine plug-in.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GEngine(GSession* session,GPlugInFactory* fac);

	/**
	* Configuration parameters were changed (or loaded).
	*/
	virtual void ApplyConfig(void);

	/**
	 * Clear the engine before a new request is send. By default, the results are
	 * cleared and the MetaEngine variable is set.
	 * @param metaengine     Meta-engine that will query the engine.
	 * @param query          Query.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	virtual void Clear(GMetaEngine* metaengine,GSearchQuery* query,size_t caller);

	/**
	 * Add a fragment from a known document as result to the meta-engine. In
	 * practice, it adds an entry to the container of results.
	 * @param doc            Document.
	 * @param rec            Concept record.
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
	GDocFragment* AddResult(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,size_t caller);

	/**
 	 * Add a fragment from a known document as result to the meta-engine. In
	 * practice, it adds an entry to the container of results.
	 * @param docid          Identifier of the document.
	 * @param rec            Concept record.
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
	GDocFragment* AddResult(size_t docid,const GConceptRecord* rec,size_t spos,size_t pos,size_t first,size_t last,double ranking,size_t caller);

private:

	/**
	* Request a query.
	* @param query           Query.
	* @param caller         Identifier of the caller (for example a thread).
	*/
	virtual void PerformRequest(GSearchQuery* query,size_t caller)=0;

public:

	/**
	 * Treat a request. It is the responsibility of the meta-search engine to
	 * build a syntactically correct query for the particular search engine.
	 * @param query          Query.
	 * @param caller         Identifier of the caller (for example a thread).
    */
	void Request(GSearchQuery* query,size_t caller);

	/**
	 * Get the meta-engine that calls the engine (may be null if no meta-engine
	 * is runned).
    * @return a pointer to a GMetaEngine.
    */
	GMetaEngine* GetMetaEngine(void) const {return(MetaEngine);}

	/**
	* @return the weight associated to the engine
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Get all the fragments retrieved by documents.
	* @param caller         Identifier of the caller (for example a thread).
	* @return a cursor on GDocRef.
	*/
	R::RCursor<GDocFragment> GetResults(size_t caller);

	/**
	 * Get the number of document fragments selected by the engine.
	 * @param caller         Identifier of the caller (for example a thread).
	 * @return the number of document fragments.
    */
	size_t GetNbResults(size_t caller) const;

	/**
	 * Get an array of the document fragments selected by the engine.
    * @param tab            Array that will hold the results.
	 * @param caller         Identifier of the caller (for example a thread).
    * @return the number of document fragments.
    */
	size_t GetTabResults(const GDocFragment** tab,size_t caller) const;

	/**
	 * Get an array of the document fragments selected by the engine.
    * @param tab            Array that will hold the results.
	 * @param caller         Identifier of the caller (for example a thread).
    * @return the number of document fragments.
    */
	size_t GetTabResults(GDocFragment** tab,size_t caller);

	/**
	 * Create the configuration (attached to the session).
	 */
	virtual void CreateConfig(void);

	/**
	* Destructor of the search engine.
	*/
	virtual ~GEngine(void);

	friend class GMetaEngine;
};


//-------------------------------------------------------------------------------
#define CREATE_ENGINE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GEngine,plugin,"Engine",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
