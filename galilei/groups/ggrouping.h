/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GGroupingH
#define GGroupingH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_GROUPING_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GGrouping provides a representation for a generic method to group some
* subprofiles.
* @author Pascal Francq
* @short Generic Grouping Method.
*/
class GGrouping : public GPlugin<GFactoryGrouping>
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Current language treated.
	*/
	GLang* Lang;

	/**
	* Pointer to groups to form.
	*/
	GGroups* Groups;

	/**
	* SubProfiles that must be grouped.
	*/
	R::RContainer<GSubProfile,false,true> SubProfiles;

	/**
	* Should the method starts from the current clustering.
	*/
	bool Modified;

	/**
	* Ideal clustering.
	*/
	GGroups* IdealGroups;

public:

	/**
	* Constructor of the grouping method.
	* @param fac             Factory of the plugin.
	*/
	GGrouping(GFactoryGrouping* fac) throw(std::bad_alloc);

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
	* Set the ideal groups.
	* @param ideal          Pointer to the ideal groups.
	*/
	void SetIdealGroups(GGroups* ideal){IdealGroups=ideal;}

protected:

	/**
	* Make the grouping for a specific Language. SubProfiles contains all the
	* subprofiles for a given language. This variables must be set before
	* calling this function. This is done by the Grouping method.
	*/
	virtual void Run(void) throw(GException)=0;

public:

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified       Should the method restart from the current
	*                       clustering (true) or start from scratch (false).
	* @param save           Save modified elements.
	* @param savehistory    Save history of grouping.
	*/
	void Grouping(GSlot* rec,bool modified,bool save, bool savehistory) throw(GException);

	/**
	* Destructor of tghe grouping method.
	*/
	virtual ~GGrouping(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryGrouping represent a factory for a given grouping method.
* @author Pascal Francq
* @short Generic Grouping Factory.
*/
class GFactoryGrouping : public GFactoryPlugin<GFactoryGrouping,GGrouping,GGroupingManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGrouping(GGroupingManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGrouping,GGrouping,GGroupingManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryGrouping(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a grouping factory.
*/
typedef GFactoryGrouping* GFactoryGroupingInit(GGroupingManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_GROUPING_FACTORY(name,C)                                                   \
class TheFactory : public GFactoryGrouping                                                \
{                                                                                         \
private:                                                                                  \
	static GFactoryGrouping* Inst;                                                        \
	TheFactory(GGroupingManager* mng,const char* l) : GFactoryGrouping(mng,name,l)        \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryGrouping* CreateInst(GGroupingManager* mng,const char* l)              \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_GROUPING_VERSION);}         \
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
GFactoryGrouping* TheFactory::Inst = 0;                                                   \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryGrouping* FactoryCreate(GGroupingManager* mng,const char* l)                  \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryGroupingCursor class provides a way to go trough a set of
* factories.
* @short Grouping Methods Factories Cursor
*/
CLASSCURSOR(GFactoryGroupingCursor,GFactoryGrouping);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
