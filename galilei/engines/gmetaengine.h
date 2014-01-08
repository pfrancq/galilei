/*

	GALILEI Research Project

	GMetaEngine.h

	Meta Engine for extraction of results from different search engines - Header.

	Copyright 2003-2014 by Pascal Francq.
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
#include <gdocfragment.h>


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
* @short Meta-search Engine
*/
class GMetaEngine : public GPlugIn, R::RDownload
{
protected:

	/**
	* Container of results (instance of class GDocFragment).
	*/
	R::RContainer<GDocFragment,true,true> Results;

public:

	/**
	* Construct a meta-engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GMetaEngine(GSession* session,GPlugInFactory* fac);

	/**
	 * Extract a text fragment from a document. In practice, it calls the
	 * corresponding method of the right filter.
	 * @param fragment       Fragment to extract.
    * @return a string containing the text fragment.
    */
	R::RString GetTextFragment(GDocFragment* fragment);

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
	* Add a fragment from an unknown document as result to the meta-engine. In
	* practice, it adds an entry to the container of results.
	* @param uri             URI of the document.
	* @param title           Title of the document.
	* @param fragment        Fragment from the document.
	* @param ranking         Ranking of the document given by the engine.
	*                        (\f$0\leq ranking \leq 1\f$).
	* @param engine          Engine from which the result come.
	*/
	virtual void AddResult(const R::RString& uri,const R::RString& title,const R::RString fragment,double ranking,const GEngine* engine);

	/**
	* Send a query to the meta-search engine. It should:
	* -# Analyse the query to identify the keywords and (eventually) operators.
	* -# Call the different engines (or at least the most relevant ones).
	* -# Produce a global ranking of all the documents retrieved by all engines.
	* @param query           Query.
	*/
	virtual void Request(const R::RString query)=0;

	/**
	* Set the global ranking for a document.
	* @param doc                   Document retrieved.
	* @param ranking               Global ranking.
	*/
	void SetRanking(GDocFragment* doc,double ranking);

	/**
	* @return a cursor of the documents retrieved by the meta-search engine.
	*/
	virtual R::RCursor<GDocFragment> GetResults(void);

	/**
	 * @return the number of results.
    */
	virtual size_t GetNbResults(void) const;

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
