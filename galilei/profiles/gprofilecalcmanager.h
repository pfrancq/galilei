/*

	GALILEI Research Project

	GProfileCalcManager.h

	Profile Computing Methods Manager - Header.

	Copyright 2001-2003 by the Universit� Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GProfileCalcManagerH
#define GProfileCalcManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GProfileCalcManager class provides a representation for a manager
* responsible to manage all the profile computing methods.
* @author Pascal Francq
* @short Profile Computing Methods Manager.
*/
class GProfileCalcManager : public R::RContainer<GFactoryProfileCalc,unsigned int,true,true>
{
	/**
	* Method currently selected.
	*/
	GProfileCalc* Current;

public:

	/**
	* Construct the profile computing methods manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GProfileCalcManager(const char* path,bool dlg=true) throw(bad_alloc,GException);

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
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name) throw(GException);

	/**
	* Get the current method.
	* @return Pointer to GProfileCalc.
	*/
	GProfileCalc* GetCurrentMethod(void);

	/**
	* Get a cursor over the profile computing methods managed.
	*/
	GFactoryProfileCalcCursor& GetProfileCalcsCursor(void);

	/**
	* Destructor of the profile computing methods manager.
	*/
	virtual ~GProfileCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
