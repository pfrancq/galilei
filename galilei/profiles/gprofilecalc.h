/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile Computing Method - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#ifndef GProfileCalcH
#define GProfileCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_PROFILECALC_VERSION "2.0"


//------------------------------------------------------------------------------
// forward declaration
class GSubProfileRef;


//------------------------------------------------------------------------------
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

	/**
	* Current subprofile to compute.
	*/
	GSubProfile* SubProfile;

public:

	/**
	* Construct the computing method.
	* @param fac             Factory of the plugin.
	*/
	GProfileCalc(GFactoryProfileCalc* fac) throw(std::bad_alloc);

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
	* Compute a subprofile.
	* @param subprofile      Reference to the subprofile to compute.
	*/
	virtual void Compute(GSubProfile* subprofile) throw(GException)=0;

	/**
	* Destructor of the computing method.
	*/
	virtual ~GProfileCalc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryProfileCalc represent a factory for a given profile computing
* method.
* @author Pascal Francq
* @short Generic Profile Computing Factory.
*/
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


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a profile computing factory.
*/
typedef GFactoryProfileCalc* GFactoryProfileCalcInit(GProfileCalcManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_PROFILECALC_FACTORY(name,C)                                                \
class TheFactory : public GFactoryProfileCalc                                             \
{                                                                                         \
private:                                                                                  \
	static GFactoryProfileCalc* Inst;                                                     \
	TheFactory(GProfileCalcManager* mng,const char* l) : GFactoryProfileCalc(mng,name,l)  \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryProfileCalc* CreateInst(GProfileCalcManager* mng,const char* l)        \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_PROFILECALC_VERSION);}      \
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
GFactoryProfileCalc* TheFactory::Inst = 0;                                                \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryProfileCalc* FactoryCreate(GProfileCalcManager* mng,const char* l)            \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryProfileCalcCursor class provides a way to go trough a set of
* factories.
* @short Profiles Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryProfileCalcCursor,GFactoryProfileCalc);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
