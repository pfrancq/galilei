/*

	GALILEI Research Project

	GRelationType.h

	Relation Type - Header.

	Copyright 2006 by the Université Libre de Bruxelles.

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
#ifndef GRelationType_H
#define GRelationType_H


//------------------------------------------------------------------------------
// include files for R


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GRelationType provides a representation for a given type of relations.
* @param author Pascal Francq
* @param short Relation Type.
*/
class GRelationType
{
	/**
	* Identifier of the relation type.
	*/
	unsigned int Id;

	/**
	* Name of the relation type.
	*/
	R::RString Name;

public:

	/**
	* Construct a relation type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	*/
	GRelationType(unsigned int id,const R::RString& name);

	/**
	* Compare two relations types.
	* @param type            Relation type used.
	* @see R::RContainer.
	*/
	int Compare(const GRelationType& type) const;

	/**
	* Compare a relation with an identifier.
	* @param id              Identifier used.
	* @see R::RContainer.
	*/
	int Compare(unsigned int id) const;

	/**
	* Compare a relation with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
