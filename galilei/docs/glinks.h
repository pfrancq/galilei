/*
	GALILEI Research Project

	GLinks.h

	List of Links - Header.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

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



//------------------------------------------------------------------------------
#ifndef GLinksH
#define GLinksH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <docs/gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a set of links. The list is not
* responsible for the desalocation of the list.
* @author Vandaele Valery
* @short List of Links.
*/
class GLinks : public R::RContainer<GLink,unsigned,false,true>
{
protected :

	/**
	* The document containing a set of link.
	*/
	GDoc* Doc;

public :

	/**
	* Constructor of a list of links.
	* @param doc             Document containing all the links.
	*/
	GLinks(GDoc* doc) throw(bad_alloc, GException);

	/**
	* Get the document containing all the links.
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
	* @return GLinkCursor.
	*/
	GLinkCursor& GetLinkCursor(void);

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
	int Compare(GLinks& lnk) const;

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


//------------------------------------------------------------------------------
/**
* The GLinksCursor class provides a way to go trough a set of links.
* @short Links Cursor
*/
CLASSCURSOR(GLinksCursor,GLinks,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
