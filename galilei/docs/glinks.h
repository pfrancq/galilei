/*
	GALILEI Research Project

	GLinks.h

	List of Links - Header.

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
#ifndef GLinksH
#define GLinksH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/gdoc.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

class GLinkCursor;

//class GLink

//-----------------------------------------------------------------------------
/**
* This class provides a representation of a set of links.
* @author  Vandaele Valery
* @short List of Links.
*/
class GLinks : public RStd::RContainer<GLink,unsigned,false,true>
{
private :

	/**
	* The document containing a set of link.
	*/
	GDoc* Doc;

	/**
	* The weight
	*/
	float Weight;

public :

	/**
	* constructor of GLinks
	* @param doc               the document containing all the links.
	*/
	GLinks(GDoc* doc);

	/**
	* Get the document containing all the links.
	* return doc              the document.
	*/
	GDoc* GetDoc(void) const {return Doc;}

	/**
	* Get the weight of the links.
	* return                  float containing the weight.
	*/
	float GetWeight(void) {return Weight;}

	/**
	* Set the weight of a links.
	* @param w                the weight.
	*/
	void SetWeight(float w){Weight=w;}

	/**
	* Get a cursor on the Links of the document.
	* @return GLinkCursor.
	*/
	GLinkCursor& GetLinkCursor(void);

	/**
	* Compare function needed by RStd::RContainer.
	* @param lnk            Pointer to the links used for the comparaison.
	*/
	int Compare(const GLinks* lnk) const {return(Doc->GetId()- lnk->GetDoc()->GetId());}

  /**
	* Compare function needed by RStd::RContainer.
	* @param lnk            Links used for the comparaison.
	*/
	int Compare(GLinks& lnk) const {return(Doc->GetId()- lnk.GetDoc()->GetId());}
	
	/**
	* Compare function needed by RStd::RContainer.
	* @param ID             ID used for the comparaison.
	*/
	int Compare(const unsigned int id) const {return(Doc->GetId()-id);}
	
	/**
	* destructor of GLinks
	*/
	~GLinks(void);
};

//-----------------------------------------------------------------------------
/**
* The GLinksCursor class provides a way to go trough a set of links.
* @short Links Cursor
*/
CLASSCURSOR(GLinksCursor,GLinks,unsigned int)

}  //-------- End of namespace GALILEI ----------------------------------



//-----------------------------------------------------------------------------
#endif
