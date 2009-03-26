/*

	GALILEI Research Project

	GPostProfile.h

	Generic Post-Profiling Computing Method - Header.

	Copyright 2003 by David Wartel.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTPROFILE_VERSION "2.0"



//------------------------------------------------------------------------------
/**
* The GPostProfile class provides a representation for a generic method to compute
* the post-profiling methods.
* @author Wartel David
* @short Generic Post-Profiling Computing Method.
*/
class GPostProfile : public GPlugin<GFactoryPostProfile>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GPostProfile(GFactoryPostProfile* fac);

	/**
	* Run the post-group method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GPostProfile(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryPostProfile represent a factory for a given post grouping method.
* @author Pascal Francq
* @short Generic Post Grouping Factory.
*/
class GFactoryPostProfile : public GFactoryPlugin<GFactoryPostProfile,GPostProfile,GPostProfileManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostProfile(GPostProfileManager* mng,const char* n,const char* f)
		: GFactoryPlugin<GFactoryPostProfile,GPostProfile,GPostProfileManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPostProfileManager class provides a representation for a manager
* responsible to manage all the postprofile computing methods.
* @author David Wartel
* @short Post-Profile Computing Methods Manager.
*/
class GPostProfileManager : public GPluginManager<GPostProfileManager,GFactoryPostProfile,GPostProfile>
{
public:

	/**
	* Construct the post-group computing methods manager.
	*/
	GPostProfileManager(void);

	/**
	* Destruct the post-group computing methods manager.
	*/
	virtual ~GPostProfileManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_POSTPROFILE_FACTORY(name,plugin)\
	CREATE_FACTORY(GPostProfileManager,GFactoryPostProfile,GPostProfile,plugin,"PostProfile",API_POSTPROFILE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
