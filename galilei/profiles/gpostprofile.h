/*

	GALILEI Research Project

	GPostProfile.h

	Generic Post-Profiling Computing Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel(dwartel@ulb.ac.be).

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
#ifndef GPostProfileH
#define GPostProfileH


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
* The GPostProfile class provides a representation for a generic method to compute
* the post-profiling methods.
* @author Wartel David
* @short Generic Post-Profiling Computing Method.
*/
class GPostProfile : public GPlugin<GFactoryPostProfile>
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
	GPostProfile(GFactoryPostProfile* fac) throw(std::bad_alloc);

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
	virtual ~GPostProfile(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryPostProfile represent a factory for a given post grouping method.
* @author Pascal Francq
* @short Generic Post Grouping Factory.
*/
class GFactoryPostProfile : public GFactoryPlugin<GFactoryPostProfile,GPostProfile,GPostProfileManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostProfile(GPostProfileManager* mng,const char* n,const char* f);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GFactoryPostProfile& f) const;

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GFactoryPostProfile* f) const;

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(unsigned int level) const;

	/**
	* Static function used to order the factory by level.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int sortOrder(const void* a,const void* b);

	/**
	* Destructor.
	*/
	virtual ~GFactoryPostProfile(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a post profiling factory.
*/
typedef GFactoryPostProfile* GFactoryPostProfileInit(GPostProfileManager*,const char*);

//------------------------------------------------------------------------------
#define CREATE_POSTPROFILE_FACTORY(name,C)                                                  \
class TheFactory : public GFactoryPostProfile                                               \
{                                                                                         \
private:                                                                                  \
	static GFactoryPostProfile* Inst;                                                       \
	TheFactory(GPostProfileManager* mng,const char* l) : GFactoryPostProfile(mng,name,l)      \
	{                                                                                     \
		C::CreateParams(this);                                                \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryPostProfile* CreateInst(GPostProfileManager* mng,const char* l)            \
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
GFactoryPostProfile* TheFactory::Inst = 0;                                                  \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryPostProfile* FactoryCreate(GPostProfileManager* mng,const char* l)                \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}



}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
