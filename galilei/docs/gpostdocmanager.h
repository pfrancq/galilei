/*

	GALILEI Research Project

	GPostDocManager.h

	Documents Post-Analysis Methods Manager - Header.

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
#ifndef GPostDocManagerH
#define GPostDocManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>

//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPostDocManager class provides a representation for a manager
* responsible to manage all the documents post-analysis methods.
* @author Pascal Francq
* @short Documents Post-Analysis Methods Manager.
*/
class GPostDocManager : public R::RContainer<GFactoryPostDoc,true,true>, public GPluginManager
{
public:

	/**
	* Constructor of a manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GPostDocManager(R::RContainer<RString, true, false>* paths,bool dlg=true) throw(GException);

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
	* Get a cursor on the factories handled by the manager.
	*/
	R::RCursor<GFactoryPostDoc> GetPostDocsCursor(void);

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
	virtual ~GPostDocManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
