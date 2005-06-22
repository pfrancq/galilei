/*

	GALILEI Research Project

	GStatsCalc.h

	Generic Statistics Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GStatsCalcH
#define GStatsCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_STATSCALC_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GStatsCalc class provides a representation for a set of statistics to
* compute.
* @author Pascal Francq
* @short Generic Statistics Method.
*/
class GStatsCalc : public GPlugin<GFactoryStatsCalc>
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Construct the statistics method.
	* @param fac             Factory of the plugin.
	*/
	GStatsCalc(GFactoryStatsCalc* fac) throw(std::bad_alloc);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session) throw(GException);

	/**
	* Compute the statistics.
	* @param xml             XML Structure.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLStruct* xml,R::RXMLTag& res) throw(GException)=0;

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	*/
	void AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,double val);

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	*/
	void AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,R::RString val);

	/**
	* Destructor of the statistics method.
	*/
	virtual ~GStatsCalc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryStatsCalc represent a factory for a given statistics method.
* @author Pascal Francq
* @short Generic Statistics Factory.
*/
class GFactoryStatsCalc : public GFactoryPlugin<GFactoryStatsCalc,GStatsCalc,GStatsCalcManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryStatsCalc(GStatsCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryStatsCalc,GStatsCalc,GStatsCalcManager>(mng,n,f,"GFactoryStatsCalc") {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryStatsCalc(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a statistics factory.
*/
typedef GFactoryStatsCalc* GFactoryStatsCalcInit(GStatsCalcManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_STATSCALC_FACTORY(name,C)                                                  \
class TheFactory : public GFactoryStatsCalc                                               \
{                                                                                         \
private:                                                                                  \
	static GFactoryStatsCalc* Inst;                                                       \
	TheFactory(GStatsCalcManager* mng,const char* l) : GFactoryStatsCalc(mng,name,l)      \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryStatsCalc* CreateInst(GStatsCalcManager* mng,const char* l)            \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_STATSCALC_VERSION);}        \
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
GFactoryStatsCalc* TheFactory::Inst = 0;                                                  \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryStatsCalc* FactoryCreate(GStatsCalcManager* mng,const char* l)                \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
