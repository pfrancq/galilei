/*

	GALILEI Research Project

	GEngine.h

	Generic Engine for extraction of results from the a search engine - Header.

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
#include <genginedoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------
// API VERSION
#define API_ENGINE_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GEngine class provides a representation for a results extractor
* for Search Engine
* @author Valery Vandaele
* @short Results extractor for search engine
*/
class GEngine : public GPlugin<GFactoryEngine>
{
protected:
	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

	/**
	* The weigth associated to the engine
	*/
	float Weight;

public:

	/**
	* Construct the extractor for the Yahoo engine.
	*/
	GEngine(GFactoryEngine* fac);

	/**
	* Process the Engine extraction. All results will be extracted, the url of the next page...
	* @param keyWords        The set of keywords to be searched
	*/
	virtual void Process(R::RContainer<R::RString,false,false>& keyWords)=0;

	/**
	* Get the weight associated to this engine
	*@return double         The weight.
	*/
	virtual double GetEngineWeight(void)=0;

	/**
	* Get the weight associated to the engine
	* @return float          The weight associated to the engine
	*/
	float GetEngineWeight(void) const {return Weight;};

	/**
	* Associate a new weight to the engine
	* @param w               The weight to associate to the engine
	*/
	void SetEngineWeight(float w) {Weight=w;};

	/**
	* Destructor of the search engine.
	*/
	virtual ~GEngine(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryEngine represent a factory for a given link method.
* @author Vandaele Valery
* @short Generic Search engine Factory.
*/
class GFactoryEngine : public GFactoryPlugin<GFactoryEngine,GEngine,GEngineManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryEngine(GEngineManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryEngine,GEngine,GEngineManager>(mng,n,f) {}
};




//------------------------------------------------------------------------------
/**
* The GEngineManager class provides a representation for a manager
* responsible to manage all the search engines.
* @author Vandaele Valery
* @short search engine Manager.
*/
class GEngineManager : public GPluginManager<GEngineManager,GFactoryEngine,GEngine>
{
public:

	/**
	* Constructor of a manager.
	*/
	GEngineManager(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GEngineManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_ENGINE_FACTORY(name,plugin)\
	CREATE_FACTORY(GEngineManager,GFactoryEngine,GEngine,plugin,"Engine",API_ENGINE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
