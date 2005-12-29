/*

	GALILEI Research Project

	GMetaEngine.h

	Meta Engine for extraction of results from different search engines - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#include <genginedoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_METAENGINE_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GMetaEngine class provides a representation for a generic results extractor
* for differents Search Engines
* @author Valery Vandaele
* @short Generic results extractor for different search engines
*/
class GMetaEngine : public GPlugin<GFactoryMetaEngine>
{
public:

	/**
	* Construct the extractor for the Yahoo engine.
	*/
	GMetaEngine(GFactoryMetaEngine* fac);

	/**
	* Send a query to the meta Search engine
	* all the available search engines are processed with the query.
	* @param keyWords        The set of keywords on witch the query will be based
	* @param useAllkwds      Use all keywords?
	*/
	virtual void Query(R::RContainer<R::RString,true,false> &keyWords, bool useAllKwds)=0;

	/**
	* Retreive the results from the searches.
	* The results are first sorted (using the gobal ranking)
	* and then stored in the RContainer
	*/
	virtual void Process(void)=0;

	/**
	* Add a result from the engine
	* @param url             The url of the document
	* @param title           The title of the document
	* @param desc            The description of the document
	* @param rank            The ranking of the document
	* @param engine          The name of the engine from which the result come
	*/
	virtual void AddResult(R::RString url,R::RString title, R::RString desc, int rank,R::RString engine)=0;

	/**
	* Get a cursor of the documents from the search engines
	* @return GEngineDocCursor
	*/
	virtual R::RCursor<GEngineDoc> GetEngineDocs(void)=0;

	/**
	* return the number of results that must be used
	*/
	virtual unsigned int GetNbResUsed(void)=0;

public:

	/**
	* Destructor of the Yahoo engine.
	*/
	virtual ~GMetaEngine(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryMetaEngine represent a factory for a given meta engine.
* @author Vandaele Valery
* @short Generic meta engine Factory.
*/
class GFactoryMetaEngine : public GFactoryPlugin<GFactoryMetaEngine,GMetaEngine,GMetaEngineManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryMetaEngine(GMetaEngineManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryMetaEngine,GMetaEngine,GMetaEngineManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GMetaEngineManager class provides a representation for a manager
* responsible to manage all the search engines.
* @author Vandaele Valery
* @short search engine Manager.
*/
class GMetaEngineManager : public GPluginManager<GMetaEngineManager,GFactoryMetaEngine,GMetaEngine>
{
public:

	/**
	* Constructor of a manager.
	*/
	GMetaEngineManager(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GMetaEngineManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_METAENGINE_FACTORY(name,plugin)\
	CREATE_FACTORY(GMetaEngineManager,GFactoryMetaEngine,GMetaEngine,plugin,"MetaEngine",API_METAENGINE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
