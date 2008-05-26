/*

	GALILEI Research Project

	GPostCommunity.h

	Generic Post-Community Computing Method - Header.

	Copyright 2003-2008 by the Université Libre de Bruxelles.

	Authors:
		Francq Pascal (pfrancq@ulb.ac.be)
		Vandaele Valéry(vavdaele@ulb.ac.be).

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
#ifndef GPostCommunityH
#define GPostCommunityH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTCOMMUNITY_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GPostCommunity class provides a representation for a generic method to
* compute the post-groupment of profiles.
* @author Pascal Francq
* @short Generic Post-Community Computing Method.
*/
class GPostCommunity : public GPlugin<GFactoryPostCommunity>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GPostCommunity(GFactoryPostCommunity* fac);

	/**
	* Run the post-community method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GPostCommunity(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryPostCommunity represent a factory for a given post-community
* method.
* @author Pascal Francq
* @short Generic Post Community Factory.
*/
class GFactoryPostCommunity : public GFactoryPlugin<GFactoryPostCommunity,GPostCommunity,GPostCommunityManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostCommunity(GPostCommunityManager* mng,const char* n,const char* f)
	 : GFactoryPlugin<GFactoryPostCommunity,GPostCommunity,GPostCommunityManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPostCommunityManager class provides a representation for a manager
* responsible to manage all the post-community computing methods.
* @author Pascal Francq
* @short Post-Community Computing Methods Manager.
*/
class GPostCommunityManager : public GPluginManager<GPostCommunityManager,GFactoryPostCommunity,GPostCommunity>
{
public:

	/**
	* Construct the post-community computing methods manager.
	*/
	GPostCommunityManager(void);

	/**
	* Destruct the post-community computing methods manager.
	*/
	virtual ~GPostCommunityManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_POSTCOMMUNITY_FACTORY(name,plugin)\
	CREATE_FACTORY(GALILEI::GPostCommunityManager,GALILEI::GFactoryPostCommunity,GALILEI::GPostCommunity,plugin,"PostCommunity",API_POSTCOMMUNITY_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
