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
#include <rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * Each object that should provide some debugging information must inherit this
 * class and rergister itself to the session. Each such object must have an
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
	 * Compare an object with another one.
	 * @param obj            Object.
	 * @see R::RContainer.
	 */
	int Compare(const GDebugObject* obj) const;
	
	/**
	* Compare an object with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;
	
	/**
	* Get the name.
	*/
	R::RString GetName(void) const {return(Name);}
	
	/**
	 * Build some debugging information as a string. A string is given as
	 * parameter to allow the building of several outputs.
	 * @param info           Description of the information needed.
	 */
	virtual R::RString GetDebugInfo(const R::RString& info);
	
	/**
	 * Put debugging information in a text file. This method calls the
	 * CreateInfo method.
	 * @param file           RTextFile where to write.
	 * @param info           Description of the information needed.
	 */
	void PutDebugInfo(R::RTextFile& file,const R::RString& info);
	
	/**
	 * Destructor.
	 */
	virtual ~GDebugObject(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
