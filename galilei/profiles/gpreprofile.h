/*

	GALILEI Research Project

	GPreProfile.h

	Generic Pre-Profiling Computing Method - Header.

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
#ifndef GPreProfileH
#define GPreProfileH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_PREPROFILE_VERSION "2.0"



//------------------------------------------------------------------------------
/**
* The GPreProfile class provides a representation for a generic method to compute
* the pre-profiling methods.
* @author Wartel David
* @short Generic Pre-Profiling Computing Method.
*/
class GPreProfile : public GPlugin<GFactoryPreProfile>
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
	GPreProfile(GFactoryPreProfile* fac) throw(std::bad_alloc);

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
	* Run the pre-group method.
	*/
	virtual void Run(void) throw(GException)=0 ;

	/**
	* Destructor.
	*/
	virtual ~GPreProfile(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryPreProfile represent a factory for a given pre grouping method.
* @author Pascal Francq
* @short Generic Pre Grouping Factory.
*/
class GFactoryPreProfile : public GFactoryPlugin<GFactoryPreProfile,GPreProfile,GPreProfileManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPreProfile(GPreProfileManager* mng,const char* n,const char* f);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GFactoryPreProfile& f) const;

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GFactoryPreProfile* f) const;

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
	virtual ~GFactoryPreProfile(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a pre profiling factory.
*/
typedef GFactoryPreProfile* GFactoryPreProfileInit(GPreProfileManager*,const char*);

//------------------------------------------------------------------------------
#define CREATE_PREPROFILE_FACTORY(name,C)                                                 \
class TheFactory : public GFactoryPreProfile                                              \
{                                                                                         \
private:                                                                                  \
	static GFactoryPreProfile* Inst;                                                      \
	TheFactory(GPreProfileManager* mng,const char* l) : GFactoryPreProfile(mng,name,l)    \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryPreProfile* CreateInst(GPreProfileManager* mng,const char* l)          \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_PREPROFILE_VERSION);}       \
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
GFactoryPreProfile* TheFactory::Inst = 0;                                                 \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryPreProfile* FactoryCreate(GPreProfileManager* mng,const char* l)              \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
	const char* LibType(void)                                                             \
	{                                                                                     \
		return("PreProfile");                                                             \
	}                                                                                     \
}



}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
