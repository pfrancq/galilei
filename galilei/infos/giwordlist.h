/*

	GALILEI Research Project

	GIWordList.h

	Represents a list of words' references - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GIWordListH
#define GIWordListH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/ginfo.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GIWord;


//-----------------------------------------------------------------------------
/**
* The GIWordList class provides a representation for a list of words'
*references. In particular, the list has an identifier, which can be used for
* storing it.
* @author Pascal Francq
* @short List of words' references.
*/
class GIWordList : public GInfo,public RStd::RContainer<GIWord,unsigned,true,true>
{
	/**
	* Identifier of the list.
	*/
	unsigned Id;

public:

	/**
	* Constructor of the list.
	*/
	GIWordList(void) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Get the identifier of the list.
	*  @returns Identifier.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier of the list.
	* @param id             Identifier to assign.
	*/
	void SetId(const unsigned int id) {Id=id;}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
