/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile Computing Method - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#ifndef GProfileCalcH
#define GProfileCalcH


//-----------------------------------------------------------------------------
// include file for LibTool--
#include <ltmm/loader.hh>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gplugin.h>
#include <profiles/gprofilecalcmanager.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// API VERSION
#define API_PROFILECALC_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile, i.e. its sub-profiles.
* @author Pascal Francq
* @short Generic Profile Computing Method.
*/
class GProfileCalc : public GPlugin<GFactoryProfileCalc>
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
	GProfileCalc(GFactoryProfileCalc* fac) throw(bad_alloc);

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
	* @param subprofile      Profile to compute.
	*/
	virtual void Compute(GSubProfile* subprofile)=0;

	/**
	* Destructor.
	*/
	virtual ~GProfileCalc(void);
};


//-----------------------------------------------------------------------------
class GFactoryProfileCalc : public GFactoryPlugin<GFactoryProfileCalc,GProfileCalc,GProfileCalcManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryProfileCalc(GProfileCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryProfileCalc,GProfileCalc,GProfileCalcManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryProfileCalc(void) {}
};


//-----------------------------------------------------------------------------
typedef GFactoryProfileCalc*(*GFactoryProfileCalcInit)(GProfileCalcManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_PROFILECALC_FACTORY(name,C,about,config)                                         \
class TheFactory : public GFactoryProfileCalc                                                   \
{                                                                                               \
private:                                                                                        \
	static GFactoryProfileCalc* Inst;                                                           \
	TheFactory(GProfileCalcManager* mng,const char* l) : GFactoryProfileCalc(mng,name,l)        \
	{                                                                                           \
		C::CreateParams(this);                                                                  \
	}                                                                                           \
	virtual ~TheFactory(void) {}                                                                \
public:                                                                                         \
	static GFactoryProfileCalc* CreateInst(GProfileCalcManager* mng,const char* l)              \
	{                                                                                           \
		if(!Inst)                                                                               \
			Inst = new TheFactory(mng,l);                                                       \
		return(Inst);                                                                           \
	}                                                                                           \
	virtual void About(void) {C::About();}                                                      \
	virtual bool HasAbout(void) const {return(about);}                                          \
	virtual void Configure(void) {C::Configure(this);}                                          \
	virtual bool HasConfigure(void) const {return(config);}                                     \
	virtual const char* GetAPIVersion(void) const {return(API_PROFILECALC_VERSION);}            \
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
};                                                                                              \
                                                                                                \
GFactoryProfileCalc* TheFactory::Inst = 0;                                                      \
                                                                                                \
extern "C"                                                                                      \
{                                                                                               \
	GFactoryProfileCalc* FactoryCreate(GProfileCalcManager* mng,const char* l)                  \
	{                                                                                           \
		return(TheFactory::CreateInst(mng,l));                                                  \
	}                                                                                           \
}



//-----------------------------------------------------------------------------
/**
* The GProfileCalcCursor class provides a way to go trough a set of computing
* method for the profiles.
* @short Profiles Computing Methods Cursor
*/
CLASSCURSOR(GProfileCalcCursor,GProfileCalc,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryProfileCalcCursor class provides a way to go trough a set of
* factories.
* @short Profiles Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryProfileCalcCursor,GFactoryProfileCalc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

