/*

	GALILEI Research Project

	GGroupCalcManager.h

	Manager to handle group computing method - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GGroupCalcManagerH
#define GGroupCalcManagerH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupCalcManager class provides a representation for a manager
* responsible to manage all the group computing methods.
* @author Pascal Francq
* @short Groups Computing Method Manager.
*/
class GGroupCalcManager : public R::RContainer<GFactoryGroupCalc,unsigned int,true,true>
{
	/**
	* Method currently selected.
	*/
	GGroupCalc* Current;

public:

	/**
	* Construct a URL manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GGroupCalcManager(const char* path,bool dlg=true) throw(GException);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	void Disconnect(GSession* session);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name) throw(GException);

	/**
	* Get the current method.
	* @return Pointer to GGroupCalc.
	*/
	GGroupCalc* GetCurrentMethod(void);

	/**
	* Get a cursor over the filters of the system.
	*/
	GFactoryGroupCalcCursor& GetGroupCalcsCursor(void);

	/**
	* Destructor of URL manager.
	*/
	virtual ~GGroupCalcManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
