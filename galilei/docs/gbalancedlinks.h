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



//-----------------------------------------------------------------------------
#ifndef GBalancedLinksH
#define GBalancedLinksH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <sessions/galilei.h>
#include <docs/glinks.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class provides a representation of a set of links with each a weight.
* @author  Vandaele Valery
* @short List of Links whith a weight.
*/
class GBalancedLinks : public GLinks
{
private :

	/**
	* The weight
	*/
	float Weight;

public :

	/**
	* constructor of GBalancedLinks
	* @param doc               the document containing all the links.
	* @param weight           the weight associated whith the link
	*/
	GBalancedLinks(GDoc* doc,float weight);

	/**
	* constructor of GBalancedLinks
	* @param doc               the document containing all the links.
	*/
	GBalancedLinks(GDoc* doc);


	/**
	* Get the id of the doc
	* @return id            the id of the document.
	*/
	unsigned int GetId(void) const {return Doc->GetId() ; }

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
	* Compare function needed by R::RContainer.
	* @param lnk            Pointer to the links used for the comparaison.
	*/
	int Compare(const GBalancedLinks* lnk) const {return(Doc->GetId()- lnk->GetDoc()->GetId());}

	/**
	* Compare function needed by R::RContainer.
	* @param lnk            Links used for the comparaison.
	*/
	int Compare(GBalancedLinks& lnk) const {return(Doc->GetId()- lnk.GetDoc()->GetId());}

	/**
	* Compare function needed by R::RContainer.
	* @param ID             ID used for the comparaison.
	*/
	int Compare(const unsigned int id) const {return(Doc->GetId()-id);}

	/**
	* destructor of GLinks
	*/
	virtual ~GBalancedLinks(void);
};

//-----------------------------------------------------------------------------
/**
* The GLinksCursor class provides a way to go trough a set of links.
* @short Links Cursor
*/
CLASSCURSOR(GBalancedLinksCursor,GBalancedLinks,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------



//-----------------------------------------------------------------------------
#endif
