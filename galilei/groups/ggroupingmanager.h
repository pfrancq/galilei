/*

	GALILEI Research Project

	GGroupingManager.h

	Grouping Methods Manager - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
#ifndef GGroupingManagerH
#define GGroupingManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupingManager class provides a representation for a manager
* responsible to manage all the grouping methods.
* @author Pascal Francq
* @short Grouping Methods Manager.
*/
class GGroupingManager : public R::RContainer<GFactoryGrouping,true,true>,public GPluginManager
{
	/**
	* Method currently selected.
	*/
	GGrouping* Current;

public:

	/**
	* Construct the grouping methods manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GGroupingManager(const char* path,bool dlg=true) throw(std::bad_alloc,GException);

	/**
	* Connect to a Session.
	* @param session         Session.
	*/
	void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         Session.
	*/
	void Disconnect(GSession* session) throw(GException);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name) throw(GException);

	/**
	* Get the current method.
	* @return Pointer to GProfileCalc.
	*/
	GGrouping* GetCurrentMethod(void);

	/**
	* Get a cursor over the grouping methods.
	*/
	R::RCursor<GFactoryGrouping> GetGroupingsCursor(void);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);
	
	/**
	* Store config of the manager
	*/	
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of a grouping methods manager.
	*/
	virtual ~GGroupingManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
