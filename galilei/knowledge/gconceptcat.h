/*

	GALILEI Research Project

	GConceptCat.h

	Concept Category - Header.

	Copyright 20011-2011 by Pascal Francq (pascal@francq.info).

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
#ifndef GConceptCat_H
#define GConceptCat_H


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gobject.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GConceptCat provides a representation of a category of concept types. For
 * example, it serves to identify all concept types that should be considered as
 * metadata.
 * @author Pascal Francq
 * @short Concept Category
 */
class GConceptCat : public GObject
{
    /**
     * Types associated with the category.
     */
    R::RContainer<GConceptType,false,true> Types;

public:

	/**
	* Construct a concept category.
	* @param session         Session.
	* @param id              Identifier of the category.
	* @param name            Name of the category.
	*/
	GConceptCat(GSession* session,size_t id,const R::RString& name);

   	/**
	* Compare two concepts categories.
	* @param cat             Concept category used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptCat& cat) const;

	/**
	* Compare two concepts categories.
	* @param cat             Concept category used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptCat* cat) const;

	/**
	* Compare a concept category with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;

   /**
	* Get a given concept type.
	* @param name            Name of the concept type to look for.
	* @return Pointer to the concept type.
	*/
	GConceptType* GetConceptType(const R::RString& name) const;

	/**
	* Insert a concept type .
	* @param type             Concept type.
	*/
	void InsertConceptType(GConceptType* type);

	/**
	* Delete a concept type.
	* @param type            Concept type.
	*/
	void DeleteConceptType(GConceptType* type);

	/**
	* Get the total number of concept types.
	*/
	size_t GetNbConceptTypes(void) const {return(Types.GetNb());}

   /**
    * @return a cursor over the concept typpes.
    */
   R::RCursor<GConceptType> GetConceptTypes(void) const {return(Types);}

   friend class GSession;
	friend class GKB;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
