/*

	GALILEI Research Project

	GObject.h

	Generic GALILEI Object - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef GObjectH
#define GObjectH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GObject provides a class for a generic object in GALILEI. In particular,
 * each object can be part of a statement.
 * @author Pascal Francq
 * @short Generic Object
 */
class GObject
{
protected:

	/**
	 * Identifier of the object.
	 */
	size_t Id;

	/**
	 * Name of the object.
	 */
	R::RString Name;

	/**
	* Type of the object.
	*/
	tObjType ObjType;

public:

	/**
	 * Construct a object.
	 * @param id             Identifier.
	 * @param name           Name.
	 * @param objtype        Type.
	 */
	GObject(size_t id,const R::RString& name,tObjType objtype);

	/**
	 * Copy constructor.
	 * @param obj            Original object.
	 */
	GObject(const GObject* obj);

	/**
	* @return the identifier of the object.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	 * Set the identifier of the object. It can only be called if the current
	 * identifier is undefined.
	 * @param id             Identifier.
	 */
	void SetId(size_t id);

	/**
	* @return the name of the object.
	*/
	inline R::RString GetName(void) const {return(Name);}

	/**
	* @return the type of the object.
	*/
	inline tObjType GetObjType(void) const {return(ObjType);}

	/**
	 * Emit a notification concerning the change of an object.
	 */
	inline void Emit(GEvent::tEvent event) const
	{
		GEvent Event(event,this);
		R::NotificationCenter.PostNotification<GEvent&>("ObjectChanged",Event);
	}

	/**
	 * Destruct the object.
	 */
	virtual ~GObject(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
