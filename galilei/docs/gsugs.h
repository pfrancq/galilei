/*

	GALILEI Research Project

	GSugs.h

	Suggestions - Header.

	Copyright 2005 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rvectorint.h>


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a list of suggestion for a given
* @author Pascal Francq
* @short Suggestions
*/
class GSugs
{
	/**
	* Type of the destinator of the suggestions (otProfile or otGroup).
	*/
	tObjType Type;

	/**
	* Identicator of the destinator of the suggestions.
	*/
	unsigned int Id;

	/**
	* Identificators of the documents suggested.
	*/
	R::RVectorInt<false> Docs;

public:

	/**
	* Constructor of the suggestion.
	* @param type           Type of the destinator.
	* @param id             Identificator of the destinator.
	* @param max            Maximum number of suggestions.
	*/
	GSugs(tObjType type,unsigned int id,unsigned int max);

	/**
	* Compare two destinators by comparing their identificator.
	* @see R::RContainer
	* @param doc             Document.
	* @return int
	*/
	int Compare(const GSugs& sugs) const;

	/**
	* Compare two destinators by comparing their identificator.
	* @see R::RContainer
	* @param doc             Pointer to the document.
	* @return int
	*/
	int Compare(const GSugs* sugs) const;

	/**
	* Compare the identificator of a destinator with a given identificator.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned id) const;

	/**
	* Add a suggestion to the list.
	* @param id              Identificator of the document.
	*/
	void AddSugs(unsigned int id);

	/**
	* Add a suggestion to the list.
	* @param doc             Pointer to the document.
	*/
	void AddSugs(const GDoc* doc);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

