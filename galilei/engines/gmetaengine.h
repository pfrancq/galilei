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
* It is the role of inheriting classes to ensure that the query submitted to the
* engines respects their constraints.
*
* See the documentation related to GPlugIn for more general information.
* @short Meta-search Engine
*/
class GMetaEngine : public GPlugIn, R::RDownloadFile
{
protected:

	/**
	* All fragments classified by documents.
	*/
	R::RContainer<GDocRef,true,true> ResultsByDocs;

	/**
	* All document fragments.
	*/
	R::RContainer<GDocFragment,false,false> Results;

	/**
	 * All the rankins for each document fragments.
	 */
	R::RContainer<GDocFragmentRanks,true,true> Rankings;

public:

	/**
	* Construct a meta-engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GMetaEngine(GSession* session,GPlugInFactory* fac);

	/**
	* Add a fragment from a known document as result to the meta-engine. In
	* practice, it adds an entry to the container of results.
	* @param doc             Document.
	* @param pos             Position to the fragment to extract.
	* @param first           First concept found.
	* @param last            Last concept found.
	* @param ranking         Ranking of the document given by the engine
	*                        (\f$0\leq ranking \leq 1\f$).
	* @param engine          Engine from which the result come.
	*/
	void AddResult(GDoc* doc,size_t pos,size_t first,size_t last,double ranking,GEngine* engine);

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
	void AddResult(size_t docid,size_t pos,size_t first,size_t last,double ranking,GEngine* engine);

	/**
	* Send a query to the meta-search engine. It call GMetaEngine::PerformRequest
	* and order then all the document fragments.
	* @param query           Query.
	*/
	void Request(const R::RString query);

private:

	/**
	 * method called each time an engine  add a given document fragment as a
	 * result. By default, the method does nothing.
    * @param rank           Document fragment added.
	 * @param engine         Engine from which the result come.
    */
	virtual void FragmentRankAdded(GDocFragmentRank* rank,GEngine* engine);

	/**
	* Send a query to the meta-search engine. It should:
	* -# Analyse the query to identify the keywords and (eventually) operators.
	* -# Call the different engines (or at least the most relevant ones). The
	 *   GMetaEngine::RequestEngines can be used.
	* -# Produce a global ranking of all the documents retrieved by all engines.
	* @param query           Query.
	*/
	virtual void PerformRequest(const R::RString query)=0;

public:

	/**
	* Send a query to the different engines. By default, it is send to all
	* enabled engines.
	* @param query           Query.
	*
	*/
	virtual void RequestEngines(const R::RString& query);

	/**
	* Get all the fragments retrieved by documents.
	* @return a cursor on GDocRef.
	*/
	R::RCursor<GDocRef> GetResultsByDocs(void);

	/**
	* Get all the fragments retrieved by documents.
	* @return a cursor on GDocRef.
	*/
	R::RCursor<GDocFragment> GetResults(void);

	/**
	* @return a cursor of the rankings retrieved by the meta-search engine.
	*/
	R::RCursor<GDocFragmentRanks> GetRankings(void);

	/**
	 * @return the number of results.
    */
	size_t GetNbResults(void) const;

	/**
	* Destructor of the meta-engine.
	*/
	virtual ~GMetaEngine(void);
};


//-------------------------------------------------------------------------------
#define CREATE_METAENGINE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GMetaEngine,plugin,"MetaEngine",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
