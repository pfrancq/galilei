/*

	GALILEI Research Project

	GConceptType.h

	Concept Type - Header.

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
#ifndef GConceptType_H
#define GConceptType_H


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
* The GConceptType provides a representation for a given type of concepts.
* @param author Pascal Francq
* @param short Concept Type.
*/
class GConceptType
{
	/**
	* Identifier of the concept type.
	*/
	unsigned int Id;

	/**
	* Name of the concept type.
	*/
	R::RString Name;

public:

	/**
	* Construct a concept type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	*/
	GConceptType(unsigned int id,const R::RString& name);

	/**
	* Compare two concepts types.
	* @param type            Concept type used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptType& type) const;

	/**
	* Compare a concept with an identifier.
	* @param id              Identifier used.
	* @see R::RContainer.
	*/
	int Compare(unsigned int id) const;

	/**
	* Compare a concept with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
