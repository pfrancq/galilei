/*

	GALILEI Research Project

	GPreProfile.h

	Generic Pre-Profiling Computing Method - Header.

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
#ifndef GPreProfileH
#define GPreProfileH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


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
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GPreProfile(GFactoryPreProfile* fac);

	/**
	* Run the pre-group method.
	*/
	virtual void Run(void)=0;

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
	GFactoryPreProfile(GPreProfileManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPreProfile,GPreProfile,GPreProfileManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPreProfileManager class provides a representation for a manager
* responsible to manage all the preprofile computing methods.
* @author David Wartel
* @short Post-Profile Computing Methods Manager.
*/
class GPreProfileManager : public GPluginManager<GPreProfileManager,GFactoryPreProfile,GPreProfile>
{
public:

	/**
	* Construct the pre-group computing methods manager.
	*/
	GPreProfileManager(void);

	/**
	* Destruct the pre-group computing methods manager.
	*/
	virtual ~GPreProfileManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_PREPROFILE_FACTORY(name,plugin)\
	CREATE_FACTORY(GPreProfileManager,GFactoryPreProfile,GPreProfile,plugin,"PreProfile",API_PREPROFILE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
