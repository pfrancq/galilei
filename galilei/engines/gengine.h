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
* -# Make a direct search in the actual corpus of the session.
* -# Call an external engine (for example Google Search) and process the
*    results.
*
* Each time a engine wants to add a results, it must call the AddResult methods
* of GMetaEngine calling the engine. One of the method adds an document already
* known by the current session, the other one an unknown document.
* @warning The ranking, \f$ranking\f$, associated by a given search engine must
* respect the constraint: \f$0\leq ranking \leq 1\f$. Otherwise, an exception
* is generated. This constraint ensures that each engine normalizes its
* rankings. Without this normalization, rankings from different engines cannot
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
	 * Clear the engine before a new request is send.
    */
	virtual void Clear(void);

	/**
	* Request a query. It is the responsibility of the meta-search engine to
	* build a syntactically correct query for the particular search engine.
	* @param caller          Meta-engine calling the engine.
	* @param query           Query.
	*/
	virtual void Request(GMetaEngine* caller,const R::RString& query)=0;

	/**
	* @return the weight associated to the engine
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Get all the fragments retrieved by documents.
	* @return a cursor on GDocRef.
	*/
	R::RCursor<GDocFragment> GetResults(void);

	/**
	 * Get the number of document fragments selected by the engine.
	 * @return the number of document fragments.
    */
	size_t GetNbResults(void) const;

	/**
	 * Get an array of the document fragments selected by the engine.
    * @param tab            Array that will hold the results.
    * @return the number of document fragments.
    */
	size_t GetTabResults(const GDocFragment** tab) const;

	/**
	 * Get an array of the document fragments selected by the engine.
    * @param tab            Array that will hold the results.
    * @return the number of document fragments.
    */
	size_t GetTabResults(GDocFragment** tab);

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
