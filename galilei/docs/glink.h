/*
	GALILEI Research Project

	GLink.h

	Link - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele valery(vavdaele@ulb.ac.be)

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
#ifndef GLinkH
#define GLinkH


//-----------------------------------------------------------------------------
//include file for ....
#include <galilei.h>
#include <docs/gdoc.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLink class provides a representation of a hyperlink. The link is maintain by
* a pointer to a GDoc (document pointed by the link)
* @author  Vandaele valery
* @short Link.
*/
class GLink
{
private:

	/**
	* The Document pointed by the link.
	*/
	const GDoc* DocTO;
	
	/**
	* The number of occurence of the link.
	*/
	unsigned int Occurs;

	/**
	* The mimetype of the link.
	*/
	//R::RString Format;

	/**
	* The type of the link.
	*/
	//R::RString  Type;

public :

	/**
	* constructor of GLink
	* @url                    The url of the link.
	*/
	GLink(const GDoc* doc);

	/**
	* Compare two links.
	* @param link            Link used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GLink* link) const { return(DocTO->GetId() - link->GetDoc()->GetId());}

	/**
	* Compare two links.
	* @param link            Link used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GLink& link) const{ return(DocTO->GetId()-link.GetDoc()->GetId() ); }

	/**
	* Compare two links.
	* @param id             The id of the document used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare( const unsigned int id) const{return (DocTO->GetId()- id );};

	/**
	* Compare two links.
	* @param doc            Pointer to a Gdoc used for the comparaison.
	* @return 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GDoc* doc) const{ return( DocTO->GetId()- doc->GetId() );}

	/**
	* Compare two links.
	* @param doc            Gdoc id used for the comparaison.
	* @return 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GDoc& doc) const {return (DocTO->GetId() - doc.GetId() );}

	/**
	* increase the number of same link.
	*/
	void AddOccurence() {Occurs++;}

	/**
	* Set the number of occurence of the link.
	* @param nbO          The Number of occurences.
	*/
	void SetOccurs(unsigned int nbO) {Occurs = nbO;}

	/**
	* Get the number of occurences of the link.
	* @return int         The number of links.
	*/
	unsigned int GetOccurs() {return Occurs;}

	/**
	* Get the url of the link.
	* @return RString     the url of the link.
	*/
	R::RString GetUrl(void) {return DocTO->GetURL();}

	/**
	* Get the format of the link.
	* @return RString     the format of the link.
	*/
	//R::RString GetFormat(void) {return Format;};

	/**
	* Get the type of the link.
	* @return RString     the type of the link.
	*/
	//R::RString GetType(void) {return Type;};

	/**
	* Set the format of the link.
	* @param format       The format to assign to the link.
	*/
	//void SetFormat(char* format);

	/**
	* Set the type of the link.
	* @param type         The type to assign to the link.
	*/
	//void SetType(char* type);

	/**
	* Get the doc pointed by the link
	* @return GDoc        the pointer to the doc Pointed.
	*/
	const GDoc* GetDoc(void) const {return DocTO;}

	/**
	* Get the id of the document.
	* @return int         the id of the link.
	*/
	unsigned int GetId(void) {return DocTO->GetId();}

	/**
	* destructor of GLink
	*/
	virtual ~GLink(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinkCursor class provides a way to go trough a set of links.
* @short Link Cursor
*/
CLASSCURSOR(GLinkCursor,GLink,unsigned int)


}  //-------- End of namespace [GALILEI] ----------------------------------


//-----------------------------------------------------------------------------
#endif
