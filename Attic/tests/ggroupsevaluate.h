/*

	GALILEI Research Project

	GGroupsEvaluate.h

	Groups for a given language - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GGroupsEvaluateH
#define GGroupsEvaluateH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <tests/ggroupevaluate.h>
#include <rstd/rcursor.h>
using namespace RStd;



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupsEvaluate class provides a representation for all the GroupEvaluates of a given
* language. The GGroups are ordered by languages.
* @author Lamoral Julien
* @short Languages GroupsEvaluate.
*/
class GGroupsEvaluate:public RStd::RContainer<GGroupEvaluate,unsigned int,false,true>
{
	/**
	* Language corresponding to the set of groups.
	*/
	GLang* Lang;

public:

	/**
	* Constructor.
	* @param lang           Pointer to the corresponding language.
	*/
	GGroupsEvaluate(GLang* lang) throw(bad_alloc);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupsEvaluate& groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupsEvaluate* groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Get the language of the set of groups.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get a cursor over the groups.
	*/
	GGroupEvaluateCursor& GetGroupEvaluateCursor(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupsEvaluateCursor class provides a way to go trough a set of
* GroupsEvaluate.
* @short GroupsEvaluate Cursor.
*/
CLASSCURSOR(GGroupsEvaluateCursor,GGroupsEvaluate,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
