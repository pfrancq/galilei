/*

	GALILEI Research Project

	GClasses.h

	Classes - Header.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GClassesH
#define GClassesH


//------------------------------------------------------------------------------
// include files for R
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gobjects.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GClasses provides just a tree of GClass.
 * @author Pascal Francq.
 * @short Classes
 */
class GClasses : public R::RTree<GClasses,GClass,false>, public GObjects<GClass>
{
	using GObjects<GClass>::InsertObj;
	using GObjects<GClass>::DeleteObj;

public:

	/**
	* Construct a tree of classes.
	* @param max             Initial size of the container.
	*/
	GClasses(size_t max);

	/**
	 * @return Pointer to the class.
	 */
	GClasses* GetClassTree(void);

	/**
	* Insert a class.
   * @param parent          Parent class.
	* @param obj             object to insert.
	*/
	virtual void InsertObj(GClass* parent,GClass* obj);

	/**
	* Delete an object.
	* @param obj             object to delete.
	*/
	virtual void DeleteObj(GClass* obj);

   /**
	* Clear the classes.
	*/
  virtual void Clear(void);

   /**
	* Clear the classes.
   * @param obj             pseudo-parameter.
	*/
  virtual void Clear(const GClass* obj);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
