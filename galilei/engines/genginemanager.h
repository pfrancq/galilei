/*

	GALILEI Research Project

	GEngine.h

	Generic Engine Manager- Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#ifndef GEngineManagerH
#define GEngineManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>

//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GEngineManager class provides a representation for a manager
* responsible to manage all the search engines.
* @author Vandaele Valery
* @short search engine Manager.
*/
class GEngineManager : public R::RContainer<GFactoryEngine,true,true> , public GPluginManager
{
public:

	/**
	* Constructor of a manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GEngineManager(const char* path,bool dlg=true) throw(std::bad_alloc,GException);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	void Disconnect(GSession* session) throw(GException);

	/**
	* Get a cursor over the factories handled by the manager.
	* @return GFactoryEngineCursor
	*/
	R::RCursor<GFactoryEngine> GetEnginesCursor(void);

	/**
	* Get the engine corresponding to the "name"
	* @param name           The name of the engine
	* @return GEngine       A pointer to the engine corresponding to the name
	*/
	GEngine* GetEngine(R::RString name);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);
	
	/**
	* Store config of the manager
	*/	
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of the manager.
	*/
	virtual ~GEngineManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
