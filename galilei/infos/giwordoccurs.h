/*

	GALILEI Research Project

	giwordoccurs.h

	Container of word occurencies - Header.

	(C) 2001 by P. Francq.

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
#ifndef GIWordOccursH
#define GIWordOccursH


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
class GIWordOccur;


//-----------------------------------------------------------------------------
/**
* The GWordOccurs class provides a representation for a list of words with
* their occurences.
*/
class GIWordOccurs : public GInfo,public RStd::RContainer<GIWordOccur,unsigned,true,true>
{
public:
	/**
	* Constructor.
	* @param nb             Number of words in the container.
	*/
	GIWordOccurs(unsigned nb) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Return a pointer to a word's occurence. If the word doesn't exist, it is
	* created and inserted.
	* @param id             Identificator of the word.
	* @returns Pointer to the word.
	*/
	GIWordOccur* GetPtr(unsigned id) throw(bad_alloc);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
