/*

	GALILEI Research Project

	GBalancedLinks.h

	List of Links with an associated weight - Header.

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
#ifndef GBalancedLinksH
#define GBalancedLinksH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <glinks.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a weighted list of links associated
* with a document.
* @author Vandaele Valery
* @short Weighted List of Links.
*/
class GBalancedLinks : public GLinks
{
protected:

	/**
	* Weight associated with the list.
	*/
	double Weight;

public :

	/**
	* Constructor of a weighted list of links.
	* @param doc             Document containing the links.
	* @param weight          Weight associated with the link.
	*/
	GBalancedLinks(GDoc* doc,double weight);

	/**
	* Constructor of a weighted list of links.
	* @param doc             Document containing the links.
	*/
	GBalancedLinks(GDoc* doc);

	/**
	* Get the identificator of the document.
	*/
	size_t GetId(void) const {return(Doc->GetId());}

	/**
	* Get the weight of the links.
	* return double.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of a links.
	* @param w                the weight.
	*/
	void SetWeight(double w);

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
	int Compare(const GBalancedLinks& lnk) const;

	/**
	* Compare the idenfiticator of a weighted list with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Destructor of a weighted list of links.
	*/
	virtual ~GBalancedLinks(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
