/*
	GALILEI Research Project

	GDocRefUrl.h

	Document descibe by URL  - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GDocRefUrlH
#define GDocRefUrlH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/gdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* [Desccription of the class]
* @author 
* @short [short description].
*/
class GDocRefUrl
{
public :

	/**
	* the document.
	*/
	GDoc* Doc;

private :

	/**
	* [function description]
	* @param1 [param1 name]    [param1 description]
	*/
//	[type] [function name]([type] param1,...);

protected :

	/**
	* [function description]
	* @param1 [param1 name]    [param1 description]
	*/
//	[type] [function name]([type] param1,...);

public :

	/**
	* constructor of GDocRefUrl
	* @doc                the pointer to the document
	*/
	GDocRefUrl(GDoc* doc);

	/**              
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	const char* GetURL(void) const {return(Doc->GetURL());}

	/**
	* @return Identificator of the document.
	*/
	unsigned int GetId(void) const {return(Doc->GetId());}

	/**
	* @return the document .
	*/
	GDoc* GetDoc(void) const {return Doc; }

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDocRefUrl* doc) const;

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Document used for the comparaison.
	*/
	int Compare(const  GDocRefUrl& doc) const;

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Document used for the comparaison.
	*/
	int Compare(const GDoc& doc) const;

		/**
	* Compare function needed by R::RContainer.
	* @param url            the url used for the comparaison.
	*/
	int Compare(const char* url) const;


	
	/**
	* destructor of GDocRefUrl
	*/
	~GDocRefUrl(void);
};


}  //-------- End of namespace GALILEI ----------------------------------


//-----------------------------------------------------------------------------
#endif
