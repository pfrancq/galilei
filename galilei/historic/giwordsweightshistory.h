/*

	GALILEI Research Project

	GIWordsWeightsHistory.h

	Weights of a list of words - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef GIWordsWeightsHistoryH
#define GIWordsWeightsHistoryH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <profiles/gsubprofile.h>
#include <infos/ginfo.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupsHistory class provides a representation for all the historic subprofile 
* @author David Wartel
* @short historic subprofile.
*/
class GIWordsWeightsHistory : public GIWordsWeights
{

private :

	/**
	* SubProfile.
	*/
	 GSubProfile* SubProfile;

	 /**
	 * parent
	 */
	 GGroupHistory* Parent;

	 /**
	 * is the subprofile well grouped?
	 */
	 bool WellGrouped;

	 /**
	 * is this subprofile a new one?
	 */
	 bool NewSubProfile;

public :

	/**
	* constructor
	*/
	GIWordsWeightsHistory(GSubProfile* sub, unsigned int nb) ;

	/**
	* get the subprofile.
	*/
	GSubProfile* GetSubProfile(void);

	/**
	*  get the id.
	*/
	unsigned int GetId(void);

	/**
	*  set the parent.
	*/
	void SetParent(GGroupHistory* p) ;

	/**
	*  return true if the subprofile is well-grouped.
	*/
	bool IsWellGrouped(void) ;

	/**
	*  set tthe stauts (well grouped- not well grouped) of the subprofile..
	*/
	void SetWellGrouped(bool b);

	/**
	*  return true if the subprofile is a new one.
	*/
	bool IsNewSubProfile(void) ;

	/**
	*  set tthe stauts (new one, old one) of the subprofile..
	*/
	void SetNewSubProfile(bool b);

	/**
	*  get the parent.
	*/
	GGroupHistory* GetParent(void);

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(const GIWordsWeightsHistory& giwwh) const ;

	/**
	* Compare method needed by R::RContainer.
	*/
	 int Compare(const GIWordsWeightsHistory* giwwh) const ;

	/**
	* Compare method needed by R::RContainer.
	*/
	int Compare(unsigned int id) const ;

	/**
	*   Destructor
	*/
	~GIWordsWeightsHistory(void);

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
