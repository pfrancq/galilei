/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GGroupingH
#define GGroupingH


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
#define API_GROUPING_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GGrouping provides a representation for a generic method to group some
* subprofiles.
* @author Pascal Francq
* @short Generic Grouping.
*/
class GGrouping : public GPlugin<GFactoryGrouping>
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Pointer to the language corresponding to the groups to form.
	*/
	GLang* Lang;

	/**
	* Pointer to groups to form.
	*/
	GGroups* Groups;

	/**
	* SubProfiles that must be grouped again.
	*/
	R::RContainer<GSubProfile,unsigned int,false,true> SubProfiles;

	/**
	* Type of the description used to group.
	*/
	tSubProfileDesc SubProfileDesc;

	/**
	* Groups to destruct after the grouping.
	*/
	R::RContainer<GGroups,unsigned int,true,true> DeletedGroups;

	/**
	* Must the groups be save in the database.
	*/
	bool SaveGroups;

	/**
	* Must the groups be used without reconstruction.
	*/
	bool Modified;

	/**
	* Ideal Groups.
	*/
	R::RContainer<GGroups,unsigned int,true,true>* IdealGroups;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GGrouping(GFactoryGrouping* fac) throw(bad_alloc);

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
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Clear method.
	*/
	virtual void Clear(void) throw(bad_alloc);

	/**
	* Set the ideal groups.
	* @param ideal          Pointer to the ideal groups.
	*/
	void SetIdealGroups(R::RContainer<GGroups,unsigned int,true,true>* ideal) {IdealGroups=ideal;}

protected:

	/**
	* Make the grouping for a specific Language. Groups is a pointer to the
	* groups to form and SubProfiles contains all the subprofiles for a given
	* language. This variables must be set before calling this function. This
	* is done by the Grouping method.
	*/
	virtual void Run(void) throw(GException)=0;

	/**
	* Create a new group. In practice a list of all removed groups are hold
	* and an existing group not more necessary is used before creating a new
	* one.
	* @param lang       Language of the group to create.
	*/
	GGroup* NewGroup(GLang* lang);

	/**
	* Delete an existing group. In practice a list of all removed groups are
	* hold and an existing group not more necessary is first put in a container
	* before being destructed.
	*/
	void DeleteGroup(GGroup* grp);

public:

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp)=0;

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void Grouping(GSlot* rec,bool modified,bool save);

	/**
	* Destructor.
	*/
	virtual ~GGrouping(void);
};


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
typedef GFactoryGrouping*(*GFactoryGroupingInit)(GGroupingManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_GROUPING_FACTORY(name,C)                                                         \
class TheFactory : public GFactoryGrouping                                                      \
{                                                                                               \
private:                                                                                        \
	static GFactoryGrouping* Inst;                                                              \
	TheFactory(GGroupingManager* mng,const char* l) : GFactoryGrouping(mng,name,l)              \
	{                                                                                           \
		C::CreateParams(this);                                                                  \
	}                                                                                           \
	virtual ~TheFactory(void) {}                                                                \
public:                                                                                         \
	static GFactoryGrouping* CreateInst(GGroupingManager* mng,const char* l)                    \
	{                                                                                           \
		if(!Inst)                                                                               \
			Inst = new TheFactory(mng,l);                                                       \
		return(Inst);                                                                           \
	}                                                                                           \
	virtual const char* GetAPIVersion(void) const {return(API_GROUPING_VERSION);}               \
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
GFactoryGrouping* TheFactory::Inst = 0;                                                         \
                                                                                                \
extern "C"                                                                                      \
{                                                                                               \
	GFactoryGrouping* FactoryCreate(GGroupingManager* mng,const char* l)                        \
	{                                                                                           \
		return(TheFactory::CreateInst(mng,l));                                                  \
	}                                                                                           \
}


//-----------------------------------------------------------------------------
/**
* The GGroupingCursor class provides a way to go trough a set of grouping
* method for the profiles.
* @short Profiles Grouping Methods Cursor
*/
CLASSCURSOR(GGroupingCursor,GGrouping,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryGroupingCursor class provides a way to go trough a set of
* factories.
* @short Grouping Methods Factories Cursor
*/
CLASSCURSOR(GFactoryGroupingCursor,GFactoryGrouping,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
