/*

	GALILEI Research Project

	GBalancedLinks.h

	List of Links with an associated weight - Header.

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



//------------------------------------------------------------------------------
#ifndef GBalancedLinksH
#define GBalancedLinksH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <docs/glinks.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a weighted list of links.
* @author Vandaele Valery
* @short Weighted List of Links.
*/
class GBalancedLinks : public GLinks
{
protected:

	/**
	* The weight associated with the list.
	*/
	float Weight;

public :

	/**
	* Constructor of a weighted list of links.
	* @param doc             Document containing all the links.
	* @param weight          Weight associated with the link.
	*/
	GBalancedLinks(GDoc* doc,float weight) throw(bad_alloc, GException);

	/**
	* Constructor of GBalancedLinks of a weighted list of links.
	* @param doc               the document containing all the links.
	*/
	GBalancedLinks(GDoc* doc) throw(bad_alloc, GException);

	/**
	* Get the id of the doc
	* @return id            the id of the document.
	*/
	unsigned int GetId(void) const {return Doc->GetId() ; }

	/**
	* Get the weight of the links.
	* return                  float containing the weight.
	*/
	float GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of a links.
	* @param w                the weight.
	*/
	void SetWeight(float w);

	/**
	* Compare two weighted lists by comparing their identificator.
	* @see R::RContainer
	* @param lnk             Pointer to a weighted list.
	* @return int
	*/
	int Compare(const GBalancedLinks* lnk) const;

	/**
	* Compare two weighted lists by comparing their identificator.
	* @see R::RContainer
	* @param lnk             Weighted list.
	* @return int
	*/
	int Compare(GBalancedLinks& lnk) const;

	/**
	* Compare the idenfiticator of a weighted list with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Destructor of a weighted list of links.
	*/
	virtual ~GBalancedLinks(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinksCursor class provides a way to go trough a set of a weighted list
* of links.
* @short Weighted List of Links Cursor
*/
CLASSCURSOR(GBalancedLinksCursor,GBalancedLinks,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
