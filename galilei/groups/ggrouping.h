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
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_GROUPING_VERSION "2"


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
	* Current language treated.
	*/
	GLang* Lang;

	/**
	* SubProfiles that must be grouped.
	*/
	R::RContainer<GSubProfile,false,true> SubProfiles;

public:

	/**
	* Constructor of the grouping method.
	* @param fac             Factory of the plugin.
	*/
	GGrouping(GFactoryGrouping* fac) throw(std::bad_alloc);

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
	* @param save           Save modified elements.
	*/
	void Grouping(GSlot* rec,bool save) throw(GException);

	/**
	* Destructor of tghe grouping method.
	*/
	virtual ~GGrouping(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryGrouping represent a factory for a given grouping method.
* @author Pascal Francq
* @short Generic Grouping Factory.
*/
class GFactoryGrouping : public GFactoryPlugin<GFactoryGrouping,GGrouping,GGroupingManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGrouping(GGroupingManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGrouping,GGrouping,GGroupingManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GGroupingManager class provides a representation for a manager
* responsible to manage all the grouping methods.
* @author Pascal Francq
* @short Grouping Methods Manager.
*/
class GGroupingManager : public GPluginManager<GGroupingManager,GFactoryGrouping,GGrouping>
{
public:

	/**
	* Construct the grouping methods manager.
	*/
	GGroupingManager(void);

	/**
	* Destructor of a grouping methods manager.
	*/
	virtual ~GGroupingManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_GROUPING_FACTORY(name,plugin)\
	CREATE_FACTORY(GGroupingManager,GFactoryGrouping,GGrouping,plugin,"Grouping",API_GROUPING_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
