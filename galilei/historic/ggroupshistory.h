/*

	GALILEI Research Project

	GGroups.h

	Groups for a given language - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GGroupsHistoryH
#define GGroupsHistoryH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <historic/ggrouphistory.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupsHistory class provides a representation for all the historic groups of a given
* language. The GGroupsHistory are ordered by IDs.
* @author David Wartel
* @short Languages GroupsHistory.
*/
class GGroupsHistory : public RStd::RContainer<GGroupHistory,unsigned int,true,true>
{

private :

	/**
	*historic  identificator
	*/
	unsigned int Id;

	/**
	* lang
	*/
	GLang* Lang;

public :

	/**
	* constructor
	*/
	GGroupsHistory(GLang* lang, unsigned int id);

	/**
	* get the id
	*/
	unsigned int GetId(void);

	/**
	*  returns the subprofiles with Id=id;
	* @param id             id to found.
	*/
	GIWordsWeightsHistory* GetSubProfile(unsigned int id);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupsHistory& groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupsHistory* groups) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(unsigned int id) const ;

	/**
	*   Destructor
	*/
	~GGroupsHistory(void);
	};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif