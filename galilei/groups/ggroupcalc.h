/*

	GALILEI Research Project

	GGroupCalc.h

	Generic Group Computing Method - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GGroupCalcH
#define GGroupCalcH


//------------------------------------------------------------------------------
// include file for LibTool--
#include <ltmm/loader.hh>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_GROUPCALC_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GGroupCalc class provides a representation for a generic method to compute
* the description of a specific group.
* @author Pascal Francq
* @short Generic Group Computing Method.
*/
class GGroupCalc : public GPlugin<GFactoryGroupCalc>
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GGroupCalc(GFactoryGroupCalc* fac) throw(std::bad_alloc);

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
	* Compute a group.
	* @param grp            Group to compute.
	*/
	virtual void Compute(GGroup* grp) throw(GException)=0;

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const=0;

	/**
	* Destructor.
	*/
	virtual ~GGroupCalc(void);
};


//------------------------------------------------------------------------------
class GFactoryGroupCalc : public GFactoryPlugin<GFactoryGroupCalc,GGroupCalc,GGroupCalcManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGroupCalc(GGroupCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGroupCalc,GGroupCalc,GGroupCalcManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryGroupCalc(void) {}
};


//------------------------------------------------------------------------------
typedef GFactoryGroupCalc*(*GFactoryGroupCalcInit)(GGroupCalcManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_GROUPCALC_FACTORY(name,C)                                                  \
class TheFactory : public GFactoryGroupCalc                                               \
{                                                                                         \
private:                                                                                  \
	static GFactoryGroupCalc* Inst;                                                       \
	TheFactory(GGroupCalcManager* mng,const char* l) : GFactoryGroupCalc(mng,name,l)      \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryGroupCalc* CreateInst(GGroupCalcManager* mng,const char* l)            \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_GROUPCALC_VERSION);}        \
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
GFactoryGroupCalc* TheFactory::Inst = 0;                                                  \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryGroupCalc* FactoryCreate(GGroupCalcManager* mng,const char* l)                \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryGroupCalcCursor class provides a way to go trough a set of
* factories.
* @short Groups Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryGroupCalcCursor,GFactoryGroupCalc,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
