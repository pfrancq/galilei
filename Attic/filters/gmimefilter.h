/*

	GALILEI Research Project

	GMIMEFilter
  .h

	Represents a couple file extention / filter - Header.

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
#ifndef GMIMEFilterH
#define GMIMEFilterH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GFilter;


//-----------------------------------------------------------------------------
/**
* The GMIMEFilter class provides a representation of a link between a MIME-Type
* and a filter.
* @author Pascal Francq
*/
class GMIMEFilter
{	
protected:
	
	/**
	* File extension.
	*/
	R::RString Type;

	/**
	* Filter corresponding.
	*/
	GFilter* Filter;

public:

	/**
	* Construct the mime.
	* @param t              MIME-Type.
	* @param f              Filter corresponding.
	*/
	GMIMEFilter(const char* t,GFilter* f);

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const GMIMEFilter* f) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const GMIMEFilter& f) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const R::RString& t) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const char* t) const;

	/**
	* Get the MIME type.
	* @returns Pointer to a MIME type.
	*/
	const char* GetName(void) const;

	/**
	* Get a filter able to manipulate this type of files.
	* @returns Pointer to a Filter.
	*/
	GFilter* GetFilter(void) const;

	/**
	* Destructor of the filter.
	*/
	virtual ~GMIMEFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
