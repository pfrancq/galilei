/*

	GALILEI Research Project

	GLinkCalc.h

	Generic Link Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

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
#ifndef GLinkCalcH
#define GLinkCalcH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_LINKCALC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GLinkCalc provides a representation for a generic method to compute links.
* @author Vandaele Valery
* @short Generic Link Method.
*/
class GLinkCalc : public GPlugin<GFactoryLinkCalc>
{
protected :

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Container of outgoing links.
	*/
	static R::RContainer<GLinks,true,true> Links_Out;

	/**
	* Status of the graph.
	*/
	static bool Inited;

public :

	/**
	* Construct the computing method.
	* @param fac            Factory of the plugin.
	*/
	GLinkCalc(GFactoryLinkCalc* fac) throw(std::bad_alloc);

	/**
	* Compute the links for a given subprofile.
	* @param subprof        Reference to the subprofile to compute.
	*/
	virtual void Compute(GSubProfile* subprof) throw(GException)=0;

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
	* Init the algorithm for link computation
	*/
	void InitGraph(void) throw(GException);

	/**
	* Add a new doc (not present in the initAlgo phase).
	* @param doc            Document.
	*/
	void AddDoc(GDoc* doc) throw(GException);

	/**
	* destructor of GLinkCalc
	*/
	virtual ~GLinkCalc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryLinkCalc represent a factory for a given link method.
* @author Vandaele Valery
* @short Generic Link Factory.
*/
class GFactoryLinkCalc : public GFactoryPlugin<GFactoryLinkCalc,GLinkCalc,GLinkCalcManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryLinkCalc(GLinkCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryLinkCalc,GLinkCalc,GLinkCalcManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryLinkCalc(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a link factory.
*/
typedef GFactoryLinkCalc* GFactoryLinkCalcInit(GLinkCalcManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_LINKCALC_FACTORY(name,C)                                                   \
class TheFactory : public GFactoryLinkCalc                                                \
{                                                                                         \
private:                                                                                  \
	static GFactoryLinkCalc* Inst;                                                        \
	TheFactory(GLinkCalcManager* mng,const char* l) : GFactoryLinkCalc(mng,name,l)        \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryLinkCalc* CreateInst(GLinkCalcManager* mng,const char* l)              \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_LINKCALC_VERSION);}         \
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
GFactoryLinkCalc* TheFactory::Inst = 0;                                                   \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryLinkCalc* FactoryCreate(GLinkCalcManager* mng,const char* l)                  \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
