/*

	GALILEI Research Project

	GStatsCalc.h

	Generic Statistics- Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GStatsCalcH
#define GStatsCalcH


//-----------------------------------------------------------------------------
// include file for LibTool--
#include <ltmm/loader.hh>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gplugin.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// API VERSION
#define API_STATSCALC_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GStatsCalc class provides a representation for a set of statistics to
* compute.
* @author Pascal Francq
* @short Generic Statistics.
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
	* Construct the computing method.
	* @param fac             Factory of the plugin.
	*/
	GStatsCalc(GFactoryStatsCalc* fac) throw(bad_alloc);

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
	* Compute a profile.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLTag& res)=0;

	/**
	* Destructor.
	*/
	virtual ~GStatsCalc(void);
};


//-----------------------------------------------------------------------------
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
		 : GFactoryPlugin<GFactoryStatsCalc,GStatsCalc,GStatsCalcManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryStatsCalc(void) {}
};


//-----------------------------------------------------------------------------
typedef GFactoryStatsCalc*(*GFactoryStatsCalcInit)(GStatsCalcManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_STATSCALC_FACTORY(name,C)                                                        \
class TheFactory : public GFactoryStatsCalc                                                     \
{                                                                                               \
private:                                                                                        \
	static GFactoryStatsCalc* Inst;                                                             \
	TheFactory(GStatsCalcManager* mng,const char* l) : GFactoryStatsCalc(mng,name,l)            \
	{                                                                                           \
		C::CreateParams(this);                                                                  \
	}                                                                                           \
	virtual ~TheFactory(void) {}                                                                \
public:                                                                                         \
	static GFactoryStatsCalc* CreateInst(GStatsCalcManager* mng,const char* l)                  \
	{                                                                                           \
		if(!Inst)                                                                               \
			Inst = new TheFactory(mng,l);                                                       \
		return(Inst);                                                                           \
	}                                                                                           \
	virtual const char* GetAPIVersion(void) const {return(API_STATSCALC_VERSION);}              \
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
GFactoryStatsCalc* TheFactory::Inst = 0;                                                        \
                                                                                                \
extern "C"                                                                                      \
{                                                                                               \
	GFactoryStatsCalc* FactoryCreate(GStatsCalcManager* mng,const char* l)                      \
	{                                                                                           \
		return(TheFactory::CreateInst(mng,l));                                                  \
	}                                                                                           \
}



//-----------------------------------------------------------------------------
/**
* The GStatsCalcCursor class provides a way to go trough a set of computing
* method for the profiles.
* @short Statss Computing Methods Cursor
*/
CLASSCURSOR(GStatsCalcCursor,GStatsCalc,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryStatsCalcCursor class provides a way to go trough a set of
* factories.
* @short Statss Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryStatsCalcCursor,GFactoryStatsCalc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

