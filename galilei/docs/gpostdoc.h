/*
	GALILEI Research Project

	GPostDoc.h

	Generic Link Method - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Kumps Nicolas (nkumps@ulb.ac.be)

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
#ifndef GPostDocH
#define GPostDocH

//-----------------------------------------------------------------------------
// include file for LibTool--
#include <ltmm/loader.hh>


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <sessions/gplugin.h>
#include <docs/gpostdocmanager.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API VERSION
#define API_POSTDOC_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GPostDoc provides a representation for a generic method to compute links.
* @author  Vandaele Valery
* @short generic link computation.
*/
class GPostDoc : public GPlugin<GFactoryPostDoc>
{
protected :

	/**
	* Session.
	*/
	GSession* Session;

public :

	/**
	* Construct the computing method.
	* @param fac             Factory of the plugin.
	*/
	GPostDoc(GFactoryPostDoc* fac) throw(bad_alloc);

	/**
	* Compute a profile using link method
	* @params Prof       The profile to compute.
	*/
	virtual void Compute(bool save)=0;

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
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*) {}

	/**
	* destructor of GPostDoc
	*/
	virtual ~GPostDoc(void);
};


//-----------------------------------------------------------------------------
class GFactoryPostDoc : public GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostDoc(GPostDocManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPostDoc(void) {}
};


//-----------------------------------------------------------------------------
typedef GFactoryPostDoc*(*GFactoryPostDocInit)(GPostDocManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_POSTDOC_FACTORY(name,C)                                                         \
class TheFactory : public GFactoryPostDoc                                                      \
{                                                                                               \
private:                                                                                        \
	static GFactoryPostDoc* Inst;                                                              \
	TheFactory(GPostDocManager* mng,const char* l) : GFactoryPostDoc(mng,name,l)              \
	{                                                                                           \
		C::CreateParams(this);                                                                  \
	}                                                                                           \
	virtual ~TheFactory(void) {}                                                                \
public:                                                                                         \
	static GFactoryPostDoc* CreateInst(GPostDocManager* mng,const char* l)                    \
	{                                                                                           \
		if(!Inst)                                                                               \
			Inst = new TheFactory(mng,l);                                                       \
		return(Inst);                                                                           \
	}                                                                                           \
	virtual const char* GetAPIVersion(void) const {return(API_POSTDOC_VERSION);}               \
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
GFactoryPostDoc* TheFactory::Inst = 0;                                                         \
                                                                                                \
extern "C"                                                                                      \
{                                                                                               \
	GFactoryPostDoc* FactoryCreate(GPostDocManager* mng,const char* l)                        \
	{                                                                                           \
		return(TheFactory::CreateInst(mng,l));                                                  \
	}                                                                                           \
}



//-----------------------------------------------------------------------------
/**
* The GPostDocCursor class provides a way to go trough a set of Link
* description method.
* @short Link Description Methods Cursor
*/
CLASSCURSOR(GPostDocCursor,GPostDoc,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryPostDocCursor class provides a way to go trough a set of
* factories.
* @short Links Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryPostDocCursor,GFactoryPostDoc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
