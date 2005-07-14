/*

	GALILEI Research Project

	GPostGroup.h

	Generic Post-Group Computing Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Va�ery(vavdaele@ulb.ac.be).

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
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTGROUP_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GPostGroup class provides a representation for a generic method to compute
* the post-groupment.
* @author Vandaele Valery
* @short Generic Post-Group Computing Method.
*/
class GPostGroup : public GPlugin<GFactoryPostGroup>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GPostGroup(GFactoryPostGroup* fac) throw(std::bad_alloc);

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
/*
* The GFactoryPostGroup represent a factory for a given post grouping method.
* @author Pascal Francq
* @short Generic Post Grouping Factory.
*/
class GFactoryPostGroup : public GFactoryPlugin<GFactoryPostGroup,GPostGroup,GPostGroupManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostGroup(GPostGroupManager* mng,const char* n,const char* f)
	 : GFactoryPlugin<GFactoryPostGroup,GPostGroup,GPostGroupManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPostGroupManager class provides a representation for a manager
* responsible to manage all the postgroup computing methods.
* @author Vandaele Valery
* @short Post-Group Computing Methods Manager.
*/
class GPostGroupManager : public GPluginManager<GPostGroupManager,GFactoryPostGroup,GPostGroup>
{
public:

	/**
	* Construct the post-group computing methods manager.
	*/
	GPostGroupManager(void);

	/**
	* Destruct the post-group computing methods manager.
	*/
	virtual ~GPostGroupManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_POSTGROUP_FACTORY(name,plugin)\
	CREATE_FACTORY(GPostGroupManager,GFactoryPostGroup,GPostGroup,plugin,"PostGroup",API_POSTGROUP_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
