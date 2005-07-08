/*

	GALILEI Research Project

	GPlugins.h

	Generic Plug-In Managers - Header.

	Copyright 2005 by the Université libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GPluginsH
#define GPluginsH


//------------------------------------------------------------------------------
// include file for R
#include <rstd/rxmltag.h>
#include <rstd/rxmlstruct.h>

//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>
using namespace std;
using namespace R;


//-----------------------------------------------------------------------------
// include file for dlopen
#include <dlfcn.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/galilei.h>
#include <sessions/gparams.h>
#include <sessions/gplugin.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGPluginManagers::PluginManagers class provides a manager for all plug-ins Managers.
* @author Pascal Francq
* @short Manager of Plug-ins managers.
*/
class GPluginManagers : public R::RContainer<GGenericPluginManager,true,true>
{
public:
	static GPluginManagers PluginManagers;

	/**
	* Constructor for the manager of plug-ins manager
	*/
	GPluginManagers(void);

	/**
	* Connect to the session.
	*/
	void Connect(GSession* sess);

	/**
	* Disconnect to the session.
	*/
	void Disconnect(GSession* sess);

protected:

	/**
	* Starting from a directory, this function looks for all shared libraries in the
	* sub-dirs and add them in the containers.
	* @param dir                 Root directory to scan.
	* @param plugins             Strings for each main plug-ins.
	* @param dlgs                Strings for each dialog plug-ins.
	*/
	void FindPlugins(const R::RString dir,R::RContainer<R::RString,true,false>& plugins,R::RContainer<R::RString,true,false>& dlgs);

public:

	void Load(const R::RContainer<R::RString,true,false>& dirs,bool dlg=true);

	/**
	* Get the manager associated to the "name".
	* @param name         The name of the manager to be found
	* @return GPluginManager The plug-ins manager.
	*/
	GGenericPluginManager* GetManager(const R::RString& name);

	/**
	* Get a cursor over all the managers.
	* @return GPluginManager A cursor on the managers
	*/
	R::RCursor<GGenericPluginManager> GetCursor(void);

	/**
	* The destructor.
	*/
	virtual ~GPluginManagers(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
