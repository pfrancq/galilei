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
#include <sessions/gplugin.h>
#include <engines/genginedoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_METAENGINE_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GMetaEngine class provides a representation for a generic results extractor
* for differents Search Engines
* @author Valery Vandaele
* @short Generic results extractor for different search engines
*/
class GMetaEngine : public GPlugin<GFactoryMetaEngine>
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Construct the extractor for the Yahoo engine.
	*/
	GMetaEngine(GFactoryMetaEngine* fac) throw(std::bad_alloc);

	/**
	* Connect to a Session.
	* @param session        Session.
	*/
	virtual void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session        Session.
	*/
	virtual void Disconnect(GSession* session) throw(GException);

	/**
	* Send a query to the meta Search engine
	* all the available search engines are processed with the query.
	* @param keyWords        The set of keywords on witch the query will be based
	* @param NbResuls        The number of results to retreive from each search engine
	*/
	virtual void Query(R::RContainer<R::RString,true,false> &keyWords, bool useAllKwds)=0;

	/**
	* Retreive the results from the searches.
	* The results are first sorted (using the gobal ranking)
	* and then stored in the RContainer
	*/
	virtual void Process() throw(R::RIOException)=0;

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
	virtual R::RCursor<GEngineDoc> GetEngineDocsCursor(void) throw(GException)=0;

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
/**
* The GFactoryMetaEngine represent a factory for a given meta engine.
* @author Vandaele Valery
* @short Generic meta engine Factory.
*/
class GFactoryMetaEngine : public GFactoryPlugin<GFactoryMetaEngine,GMetaEngine,GMetaEngineManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryMetaEngine(GMetaEngineManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryMetaEngine,GMetaEngine,GMetaEngineManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryMetaEngine(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a meta engine factory.
*/
typedef GFactoryMetaEngine* GFactoryMetaEngineInit(GMetaEngineManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_METAENGINE_FACTORY(name,C)                                                 \
class TheFactory : public GFactoryMetaEngine                                              \
{                                                                                         \
private:                                                                                  \
	static GFactoryMetaEngine* Inst;                                                      \
	TheFactory(GMetaEngineManager* mng,const char* l) : GFactoryMetaEngine(mng,name,l)        \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryMetaEngine* CreateInst(GMetaEngineManager* mng,const char* l)              \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_METAENGINE_VERSION);}       \
	virtual void Create(void) throw(GException)                                           \
	{                                                                                     \
		if(Plugin) return;                                                                \
		Plugin=new C(this);                                                               \
		Plugin->ApplyConfig();                                                            \
	}                                                                                     \
	virtual void Delete(void) throw(GException)                                           \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
	virtual void Create(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin)                                                                       \
		{                                                                                 \
			Plugin=new C(this);                                                           \
			Plugin->ApplyConfig();                                                        \
		}                                                                                 \
		if(ses)                                                                           \
			Plugin->Connect(ses);                                                         \
	}                                                                                     \
	virtual void Delete(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		if(ses)                                                                           \
			Plugin->Disconnect(ses);                                                      \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
};                                                                                        \
                                                                                          \
GFactoryMetaEngine* TheFactory::Inst = 0;                                                 \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryMetaEngine* FactoryCreate(GMetaEngineManager* mng,const char* l)                  \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
