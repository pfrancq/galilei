/*

	GALILEI Research Project

	GEngine.h

	Generic Engine for extraction of results from the a search engine - Header.

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
class GEngineManager : public R::RContainer<GFactoryMetaEngine,true,true>, public R::RContainer<GFactoryEngine,true,true>
{
	/**
	* Method currently selected.
	*/
	GMetaEngine* Current;

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
	* Get a cursor over the meta engine factories handled by the manager.
	* @return GFactoryEngineCursor
	*/
	R::RCursor<GFactoryMetaEngine> GetMetaEnginesCursor(void);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name) throw(GException);

	/**
	* Get the current method.
	* @return Pointer to GDocAnalyse.
	*/
	GMetaEngine* GetCurrentMethod(void);

	/**
	* Get the engine corresponding to the "name"
	* @param name           The name of the engine
	* @return GEngine       A pointer to the engine corresponding to the name
	*/
	GEngine* GetEngine(R::RString name);

	/**
	* Insert a meta engine factory in the maanger
	* @param ptr              Pointer to the meta engine factory
	*/
	void InsertPtr(GFactoryMetaEngine* ptr) throw(std::bad_alloc);

	/**
	* Insert a engine factory in the maanger
	* @param ptr              Pointer to the engine factory
	*/
	void InsertPtr(GFactoryEngine* ptr) throw(std::bad_alloc);

	/**
	* Destructor of the manager.
	*/
	virtual ~GEngineManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
