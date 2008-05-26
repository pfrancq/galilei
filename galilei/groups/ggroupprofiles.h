/*

	GALILEI Research Project

	GGroupProfiles.h

	Generic Profiles Grouping Method - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#ifndef GGroupProfilesH
#define GGroupProfilesH


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
#define API_GROUPPROFILES_VERSION "2"


//------------------------------------------------------------------------------
/**
* The GGroupProfiles provides a representation for a generic method to group some
* profiles.
* @author Pascal Francq
* @short Generic Profiles Grouping Method.
*/
class GGroupProfiles : public GPlugin<GFactoryGroupProfiles>
{
protected:

	/**
	* Profiles that must be grouped.
	*/
	R::RContainer<GProfile,false,false> Profiles;

public:

	/**
	* Constructor of the profiles grouping method.
	* @param fac             Factory of the plugin.
	*/
	GGroupProfiles(GFactoryGroupProfiles* fac);

protected:

	/**
	* Make the grouping for the defined profiles.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Group the Profiles.
	* @param rec            Receiver of the signals.
	* @param save           Save modified elements.
	*/
	void Grouping(GSlot* rec,bool save);

	/**
	* Destructor of the profiles grouping method.
	*/
	virtual ~GGroupProfiles(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryGroupProfiles represent a factory for a given profiles grouping
* method.
* @author Pascal Francq
* @short Generic Profiles Grouping Factory.
*/
class GFactoryGroupProfiles : public GFactoryPlugin<GFactoryGroupProfiles,GGroupProfiles,GGroupProfilesManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGroupProfiles(GGroupProfilesManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGroupProfiles,GGroupProfiles,GGroupProfilesManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GGroupProfilesManager class provides a representation for a manager
* responsible to manage all the profiles grouping methods.
* @author Pascal Francq
* @short Profiles Grouping Methods Manager.
*/
class GGroupProfilesManager : public GPluginManager<GGroupProfilesManager,GFactoryGroupProfiles,GGroupProfiles>
{
public:

	/**
	* Construct the profiles grouping methods manager.
	*/
	GGroupProfilesManager(void);

	/**
	* Destructor of a profiles grouping methods manager.
	*/
	virtual ~GGroupProfilesManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_GROUPPROFILES_FACTORY(name,plugin)\
	CREATE_FACTORY(GGroupProfilesManager,GFactoryGroupProfiles,GGroupProfiles,plugin,"GroupProfiles",API_GROUPPROFILES_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
