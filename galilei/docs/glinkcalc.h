/*
	GALILEI Research Project

	GLinkCalc.h

	Generic Link Method - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GLinkCalcH
#define GLinkCalcH

//-----------------------------------------------------------------------------
// include file for LibTool--
#include <ltmm/loader.hh>


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/glinks.h>
#include <sessions/gplugin.h>
#include <docs/glinkcalcmanager.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API VERSION
#define API_LINKCALC_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GLinkCalc provides a representation for a generic method to compute links.
* @author  Vandaele Valery
* @short generic link computation.
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
	R::RContainer<GLinks,unsigned int,true,true>* Links_Out;

	/**
	* remain status of the graph
	*/
	bool Inited;

public :

	/**
	* Construct the computing method.
	* @param fac             Factory of the plugin.
	*/
	GLinkCalc(GFactoryLinkCalc* fac) throw(bad_alloc);

	/**
	* Compute a profile using link method
	* @params Prof       The profile to compute.
	*/
	virtual void Compute(GSubProfile* subprof)=0;

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Init the algorithm for link computation
	*/
	void InitGraph(void);

	/**
	* Add a new doc (not present in the initAlgo phase).
	*/
	void AddDoc(GDoc* doc);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*) {}

	/**
	* destructor of GLinkCalc
	*/
	virtual ~GLinkCalc(void);
};


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
typedef GFactoryLinkCalc*(*GFactoryLinkCalcInit)(GLinkCalcManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_LINKCALC_FACTORY(name,C)                                                         \
class TheFactory : public GFactoryLinkCalc                                                      \
{                                                                                               \
private:                                                                                        \
	static GFactoryLinkCalc* Inst;                                                              \
	TheFactory(GLinkCalcManager* mng,const char* l) : GFactoryLinkCalc(mng,name,l)              \
	{                                                                                           \
		C::CreateParams(this);                                                                  \
	}                                                                                           \
	virtual ~TheFactory(void) {}                                                                \
public:                                                                                         \
	static GFactoryLinkCalc* CreateInst(GLinkCalcManager* mng,const char* l)                    \
	{                                                                                           \
		if(!Inst)                                                                               \
			Inst = new TheFactory(mng,l);                                                       \
		return(Inst);                                                                           \
	}                                                                                           \
	virtual const char* GetAPIVersion(void) const {return(API_LINKCALC_VERSION);}               \
	virtual void Create(void) throw(GException)                                                 \
	{                                                                                           \
		if(Plugin) return;                                                                      \
		Plugin=new C(this);                                                                     \
		Plugin->ApplyConfig();                                                                  \
	}                                                                                           \
	virtual void Delete(void) throw(GException)                                                 \
	{                                                                                           \
		if(!Plugin) return;                                                                     \
		delete Plugin;                                                                          \
		Plugin=0;                                                                               \
	}                                                                                           \
	virtual void Create(GSession* ses) throw(GException)                                        \
	{                                                                                           \
		if(!Plugin)                                                                             \
		{                                                                                       \
			Plugin=new C(this);                                                                 \
			Plugin->ApplyConfig();                                                              \
		}                                                                                       \
		if(ses)                                                                                 \
			Plugin->Connect(ses);                                                               \
	}                                                                                           \
	virtual void Delete(GSession* ses) throw(GException)                                        \
	{                                                                                           \
		if(!Plugin) return;                                                                     \
		if(ses)                                                                                 \
			Plugin->Disconnect(ses);                                                            \
		delete Plugin;                                                                          \
		Plugin=0;                                                                               \
	}                                                                                           \
};                                                                                              \
                                                                                                \
GFactoryLinkCalc* TheFactory::Inst = 0;                                                         \
                                                                                                \
extern "C"                                                                                      \
{                                                                                               \
	GFactoryLinkCalc* FactoryCreate(GLinkCalcManager* mng,const char* l)                        \
	{                                                                                           \
		return(TheFactory::CreateInst(mng,l));                                                  \
	}                                                                                           \
}



//-----------------------------------------------------------------------------
/**
* The GLinkCalcCursor class provides a way to go trough a set of Link
* description method.
* @short Link Description Methods Cursor
*/
CLASSCURSOR(GLinkCalcCursor,GLinkCalc,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryLinkCalcCursor class provides a way to go trough a set of
* factories.
* @short Links Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryLinkCalcCursor,GFactoryLinkCalc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
