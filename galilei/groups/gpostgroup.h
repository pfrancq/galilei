/*

	GALILEI Research Project

	GPostGroup.h

	Generic Post-Group Computing Method - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Vaélery(vavdaele@ulb.ac.be).

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
#ifndef GPostGroupH
#define GPostGroupH


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
#define API_POSTGROUP_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GPostGroup class provides a representation for a generic method to compute
* the post-groupment.
* @author Vandaele Valery
* @short Generic Post-Group Computing Method.
*/
class GPostGroup : public GPlugin<GFactoryPostGroup>
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
	GPostGroup(GFactoryPostGroup* fac) throw(bad_alloc);

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
	* Run the post-group method.
	*/
	virtual void Run(void) throw(GException)=0 ;

	/**
	* Destructor.
	*/
	virtual ~GPostGroup(void);
};


//------------------------------------------------------------------------------
class GFactoryPostGroup : public GFactoryPlugin<GFactoryPostGroup,GPostGroup,GPostGroupManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostGroup(GPostGroupManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPostGroup,GPostGroup,GPostGroupManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPostGroup(void) {}
};


//------------------------------------------------------------------------------
typedef GFactoryPostGroup*(*GFactoryPostGroupInit)(GPostGroupManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_POSTGROUP_FACTORY(name,C)                                                  \
class TheFactory : public GFactoryPostGroup                                               \
{                                                                                         \
private:                                                                                  \
	static GFactoryPostGroup* Inst;                                                       \
	TheFactory(GPostGroupManager* mng,const char* l) : GFactoryPostGroup(mng,name,l)      \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryPostGroup* CreateInst(GPostGroupManager* mng,const char* l)            \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_POSTGROUP_VERSION);}        \
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
GFactoryPostGroup* TheFactory::Inst = 0;                                                  \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryPostGroup* FactoryCreate(GPostGroupManager* mng,const char* l)                \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryPostGroupCursor class provides a way to go trough a set of
* factories.
* @short Post Groups Computing Methods Factories Cursor
*/
CLASSCURSOR(GFactoryPostGroupCursor,GFactoryPostGroup,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
