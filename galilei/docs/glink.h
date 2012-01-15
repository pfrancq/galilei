/*

	GALILEI Research Project

	GLink.h

	Link - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GLinkH
#define GLinkH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GLink class provides a representation of a hyperlink. The link is maintain
* by a pointer to a GDoc (document pointed by the link).
* @author Vandaele valery
* @short Link.
*/
class GLink
{
private:

	/**
	* Document pointed by the link.
	*/
	const GDoc* DocTO;

	/**
	* Number of occurrence of the link.
	*/
	size_t Occurs;

public :

	/**
	* Constructor of a link.
	* @param doc            Pointer to the document.
	*/
	GLink(const GDoc* doc);

	/**
	* Compare two links by comparing the identifiers of the corresponding
	* documents.
	* @see R::RContainer
	* @param link           Pointer to a link.
	* @return int
	*/
	int Compare(const GLink* link) const;

	/**
	* Compare two links by comparing the identifiers of the corresponding
	* documents.
	* @see R::RContainer
	* @param link           Link.
	* @return int
	*/
	int Compare(const GLink& link) const;

	/**
	* Compare an identifier of the document of a link with an identifier.
	* @see R::RContainer
	* @param id             Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Compare an identificator of the document of a link with another document.
	* documents.
	* @see R::RContainer
	* @param doc            Pointer to a document.
	* @return int
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare an identificator of the document of a link with another document.
	* documents.
	* @see R::RContainer
	* @param doc            Document.
	* @return int
	*/
	int Compare(const GDoc& doc) const;

	/**
	* Increase the number of occurrences of the link.
	*/
	void IncOccurs(void) {Occurs++;}

	/**
	* Set the number of occurrence of the link.
	* @param nbO            The Number of occurrences.
	*/
	void SetOccurs(size_t nbO) {Occurs = nbO;}

	/**
	* Get the number of occurrences of the link.
	*/
	size_t GetOccurs(void) const {return(Occurs);}

	/**
	* @return the URI of the document corresponding to the link.
	*/
	R::RURI GetURI(void) const {return(DocTO->GetURI());}

	/**
	* Get the document pointed by the link.
	* @return Pointer to GDoc.
	*/
	const GDoc* GetDoc(void) const {return(DocTO);}

	/**
	* Get the identifier of the document pointed by the link.
	*/
	size_t GetId(void) const {return DocTO->GetId();}

	/**
	* Destruct of a link.
	*/
	virtual ~GLink(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
