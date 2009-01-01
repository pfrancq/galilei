/*

	GALILEI Research Project

	GDebugObject.h

	Object that can provide debugging information - Header.

	Copyright 2007 by the Université Libre de Bruxelles.

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
#ifndef GDebugObject_H
#define GDebugObject_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rdebug.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * Each object that should provide some debugging information must inherit this
 * class and register itself to the session. Each such object must have an
 * unique name.
 * @author Pascal Francq
 * @short Debugging Object
 */
class GDebugObject
{
protected:

	/**
	 * Name of the object.
	 */
	R::RString Name;

	/**
	 * File where the information should be store.
	 */
	R::RDebug* Debug;

public:

	/**
	 * Constructor.
	 * @param name           Name of the object.
	 */
	GDebugObject(const R::RString& name);

	/**
	 * Compare an object with another one.
	 * @param obj            Object.
	 * @see R::RContainer.
	 */
	int Compare(const GDebugObject& obj) const;

	/**
	* Compare an object with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the name.
	*/
	inline R::RString GetName(void) const {return(Name);}

	/**
	 * @return Pointer to the output where the debugging information should be
	 * stored. If null, no information should be build for the object.
	 * @see GSession::SetActiveDebugObjects(const R::RString&,bool)
	 */
	inline R::RDebug* GetDebug(void) const {return(Debug);}

	/**
	 * Build some debugging information and put the results in the debugging
	 * files of GALILEI. A string is given as parameter to allow the building
	 * of several outputs.
	 * @remark GetDebug() can be used since it status is managed by GSession.
	 * @param info           Description of the information needed.
	 */
	virtual void DebugInfo(const R::RString& info);

	/**
	 * Destruct the debug object.
	 */
	virtual ~GDebugObject(void);

	friend class GSession;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
