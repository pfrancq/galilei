/*

	GALILEI Research Project

	GEngine.h

	Generic Engine for extraction of results from the a search engine - Header.

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
#ifndef GEngineH
#define GEngineH


//------------------------------------------------------------------------------
// incldue files for CUL library
#include <curl/curl.h>


//------------------------------------------------------------------------------
//include file for GALILEI
#include <sessions/gplugin.h>
#include <engines/genginedoc.h>


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
	* Session.
	*/
	GSession* Session;

	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

private:

	/**
	* The name of the engine
	*/
	R::RString Name;

	/**
	* The weigth associated to the engine
	*/
	float Weight;

public:

	/**
	* Construct the extractor for the Yahoo engine.
	*/
	GEngine(GFactoryEngine* fac) throw(std::bad_alloc,R::RIOException);

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
	* Process the Engine extraction. All results will be extracted, the url of the next page...
	* @param keyWords        The set of keywords to be searched
	*/
	virtual void Process(R::RContainer<R::RString,false,false>& keyWords) throw(GException)=0;

	/**
	* Get the weight associated to this engine
	*@return double         The weight.
	*/
	virtual double GetEngineWeight(void)=0;

	/**
	* Get the name of the engine
	*/
	R::RString GetName(void) const {return Name;}

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
	* Compare this object with another one
	*/
	int Compare(const GEngine* eng) const;

	/**
	* Compare this object with another one
	*/
	int Compare(const GEngine& eng) const;

	/**
	* Compare this object with another one
	*/
	int Compare(const R::RString& name) const;

	/**
	* Destructor of the search engine.
	*/
	virtual ~GEngine(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryEngine represent a factory for a given link method.
* @author Vandaele Valery
* @short Generic Search engine Factory.
*/
class GFactoryEngine : public GFactoryPlugin<GFactoryEngine,GEngine,GEngineManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryEngine(GEngineManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryEngine,GEngine,GEngineManager>(mng,n,f) {}


	/**
	* Destructor.
	*/
	virtual ~GFactoryEngine(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a link factory.
*/
typedef GFactoryEngine* GFactoryEngineInit(GEngineManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_ENGINE_FACTORY(name,C)                                                     \
class TheFactory : public GFactoryEngine                                                  \
{                                                                                         \
private:                                                                                  \
	static GFactoryEngine* Inst;                                                          \
	TheFactory(GEngineManager* mng,const char* l) : GFactoryEngine(mng,name,l)            \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryEngine* CreateInst(GEngineManager* mng,const char* l)                  \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_ENGINE_VERSION);}           \
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
GFactoryEngine* TheFactory::Inst = 0;                                                     \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryEngine* FactoryCreate(GEngineManager* mng,const char* l)                      \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
	const char* LibType(void)                                                             \
	{                                                                                     \
		return("Engine");                                                                 \
	}                                                                                     \
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
