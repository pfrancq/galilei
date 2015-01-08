/*

	GALILEI Research Project

	GObject.h

	Generic GALILEI Object - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
// include files for R
#include <robject.h>


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
class GObject : public R::RObject
{
protected:

	/**
	 * Session of the object.
	 */
	GSession* Session;

	/**
	 * Identifier of the object.
	 */
	size_t Id;

	/**
	* Type of the object.
	*/
	tObjType ObjType;

public:

	/**
	 * Construct a object.
	 * @param session        Session.
	 * @param id             Identifier.
	 * @param name           Name.
	 * @param type           Type.
	 */
	GObject(GSession* session,size_t id,const R::RString& name,tObjType type);

	/**
	 * Copy constructor.
	 * @param obj            Original object.
	 */
	GObject(const GObject* obj);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GObject");}

	/**
	 * Defines if the objects have a description.
	 * @return false.
	 */
	static inline bool HasDesc(void) {return(false);}

	/**
	 * Defines of the objects have a concept tree.
	 * @return false.
	 */
	static inline bool HasTree(void){return(false);}

	/**
	* @return the session of the object.
	*/
	inline GSession* GetSession(void) const {return(Session);}

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
	 * Get a string that represents the object when a search has to be
	 * performed in the list. By default, it is its name.
	 * @return the string used for searching an object.
	 */
	virtual R::RString GetSearchStr(void) const;

	/**
	* @return the type of the object.
	*/
	inline tObjType GetObjType(void) const {return(ObjType);}

	/**
	 * Handler called when a notification posted is not caught. In GALILEI,
	 * this function does nothing.
	 * @param notification    Notification posted.
	 */
	virtual void HandlerNotFound(const R::RNotification& notification);

	/**
	 * Destruct the object.
	 */
	virtual ~GObject(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
