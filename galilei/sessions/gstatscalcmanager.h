/*

	GALILEI Research Project

	GStatsCalcManager.h

	Sstatistics Methods Manager - Header.

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
#ifndef GStatsCalcManagerH
#define GStatsCalcManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GStatsCalcManager class provides a representation for a manager
* responsible to manage all the statistics.
* @author Pascal Francq
* @short Statistics Methods Manager.
*/
class GStatsCalcManager : public R::RContainer<GFactoryStatsCalc,true,true>
{
public:

	/**
	* Construct the statistics methods manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GStatsCalcManager(const char* path,bool dlg=true) throw(std::bad_alloc,GException);

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
	* Get a particular statistics method.
	* @param name            Name of the method.
	* @return Pointer to GStatsCalc.
	*/
	GStatsCalc* Get(const char* name);

	/**
	* Get a cursor over the statistics methods managed.
	* return GFactoryStatsCalcCursor.
	*/
	R::RCursor<GFactoryStatsCalc> GetStatsCalcsCursor(void);

	/**
	* Destructor of the statistics methods manager.
	*/
	virtual ~GStatsCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
