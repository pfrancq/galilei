/*
	GALILEI Research Project

	GLinks.h

	List of Links - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

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
#ifndef GLinksH
#define GLinksH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a list of links contained in a
* document. The list is not responsible for the desallocation of the links.
* @author Vandaele Valery
* @short List of Links.
*/
class GLinks : public R::RContainer<GLink,false,true>
{
protected:

	/**
	* Document containing the set of links.
	*/
	GDoc* Doc;

public:

	/**
	* Constructor of a list of links.
	* @param doc             Document containing the links.
	*/
	GLinks(GDoc* doc) throw(std::bad_alloc, GException);

	/**
	* Get the document containing the links.
	* @return Pointer to a GDoc.
	*/
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	* Get the Identificator of the document.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Doc->GetId());}

	/**
	* Get a cursor on the list of links.
	*/
	R::RCursor<GLink> GetLinks(void);

	/**
	* Compare two lists by comparing their identificator.
	* @see R::RContainer
	* @param lnk             Pointer to a list.
	* @return int
	*/
	int Compare(const GLinks* lnk) const;

	/**
	* Compare two lists by comparing their identificator.
	* @see R::RContainer
	* @param lnk             List.
	* @return int
	*/
	int Compare(const GLinks& lnk) const;

	/**
	* Compare the idenfiticator of a weighted list with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Destructor of a list of links.
	*/
	virtual ~GLinks(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
