/*

	GALILEI Research Project

	GSugs.h

	Suggestions - Header.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GSugsH
#define GSugsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a list of suggestion for a given
* addressee.
* @author Pascal Francq
* @short Suggestions
*/
class GSugs
{
	/**
	* Type of the addressee of the suggestions (otProfile or otGroup).
	*/
	tObjType Type;

	/**
	* Identifier of the addressee of the suggestions.
	*/
	size_t Id;

	/**
	* Identifiers of the documents suggested.
	*/
	R::RNumContainer<size_t,false> Docs;

public:

	/**
	* Constructor of the suggestion.
	* @param type           Type of the addressee.
	* @param id             Identifier of the addressee.
	* @param max            Maximum number of suggestions.
	*/
	GSugs(tObjType type,size_t id,size_t max);

	/**
	* Compare two addressees by comparing their identifier.
	* @see R::RContainer
	* @param sugs            Suggestion.
	* @return int
	*/
	int Compare(const GSugs& sugs) const;

	/**
	* Compare two addressees by comparing their identifier.
	* @see R::RContainer
	* @param sugs            Suggestion.
	* @return int
	*/
	int Compare(const GSugs* sugs) const;

	/**
	* Compare the identifier of a addressee with a given identifier.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Add a suggestion to the list.
	* @param id              Identifier of the document.
	*/
	void AddSugs(size_t id);

	/**
	* Add a suggestion to the list.
	* @param doc             Pointer to the document.
	*/
	void AddSugs(const GDoc* doc);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

