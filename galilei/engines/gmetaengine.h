/*

	GALILEI Research Project

	GMetaEngine.h

	Meta Engine for extraction of results from different search engines - Header.

	Copyright 2003-2012 by Pascal Francq.
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
#include <gdocretrieved.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMetaEngine class provides a representation for a generic results extractor
* for different Search Engines
* @author Valery Vandaele
* @short Generic results extractor for different search engines
*/
class GMetaEngine : public GPlugIn
{
protected:

	/**
	* Container of results (instance of class GDocRetrieved).
	*/
	R::RContainer<GDocRetrieved,true,true> Results;

public:

	/**
	* Construct a meta-engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GMetaEngine(GSession* session,GPlugInFactory* fac);

	/**
	* Add a known document as result to the meta-engine. In practice, it adds an
	* entry to the container of results.
	* @param docid           Identifier of the document.
	* @param desc            Description of the document (such as an extract).
	* @param ranking         Ranking of the document given by the engine
	*                        (\f$0\leq ranking \leq 1\f$).
	* @param engine          Engine from which the result come.
	*/
	virtual void AddResult(size_t DocId,const R::RString desc,double ranking,const GEngine* engine);

	/**
	* Add an unknown document as result to the meta-engine. In practice, it adds
	* an entry to the container of results.
	* @param uri             URI of the document.
	* @param title           Title of the document.
	* @param desc            Description of the document (such as an extract).
	* @param ranking         Ranking of the document given by the engine.
	*                        (\f$0\leq ranking \leq 1\f$).
	* @param engine          Engine from which the result come.
	*/
	virtual void AddResult(const R::RString& uri,const R::RString& title,const R::RString desc,double ranking,const GEngine* engine);

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
	void SetRanking(GDocRetrieved* doc,double ranking);

	/**
	* Set the global ranking for a document.
	* @param docid                 Identifier of the document retrieved.
	* @param ranking               Global ranking.
	*/
	void SetRanking(size_t docid,double ranking);

	/**
	* Set the global ranking for a document.
	* @param uri                   URI of the document retrieved.
	* @param ranking               Global ranking.
	*/
	void SetRanking(const R::RString& uri,double ranking);

	/**
	* @return a cursor of the documents retrieved by the meta-search engine.
	*/
	virtual R::RCursor<GDocRetrieved> GetDocs(void);

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
