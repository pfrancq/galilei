/*

	GALILEI Research Project

	GWeightInfosHistory.h

	List of weighted information entities for history - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GWeightInfosHistoryH
#define GWeightInfosHistoryH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWeightInfosHistory class provides a representation for a history of a
* list of weighted information entities used to hold a history of a subprofile.
* @author David Wartel
* @short History of a Weighted Information Entity List.
*/
class GWeightInfosHistory : public GWeightInfos
{
protected:

	/**
	* Subprofile.
	*/
	GSubProfile* SubProfile;

	/**
	* Group containing the subprofile.
	*/
	GGroupHistory* Parent;

	/**
	* Is the subprofile well grouped?
	*/
	bool WellGrouped;

	/**
	* Is this subprofile a new one?
	*/
	bool NewSubProfile;

public :

	/**
	* Constructor.
	* @param sub             Subprofile.
	* @param max             Maximal number of word created at initialisation.
	*/
	GWeightInfosHistory(GSubProfile* sub, unsigned int max) throw(std::bad_alloc);

	/**
	* Compare two lists by comparing the identificator of their subprofile.
	* @see R::RContainer
	* @param giwwh           List.
	* @return int
	*/
	int Compare(const GWeightInfosHistory& giwwh) const;

	/**
	* Compare two lists by comparing the identificator of their subprofile.
	* @see R::RContainer
	* @param giwwh           POinter to the list.
	* @return int
	*/
	 int Compare(const GWeightInfosHistory* giwwh) const;

	/**
	* Compare the identificator of the subprofile of a list with another
	* identificator.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(unsigned int id) const;

	/**
	* Get the subprofile.
	* @return Pointer to a GSubProfile.
	*/
	GSubProfile* GetSubProfile(void) const {return(SubProfile);}

	/**
	* Get the identificator of the subprofile.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const;

	/**
	* Get the parent of the subprofile.
	* @return Pointer to a GGroupHistory.
	*/
	GGroupHistory* GetParent(void) const {return(Parent);}

	/**
	* Set the parent of a list.
	* @param p               Pointer to the parent.
	*/
	void SetParent(GGroupHistory* p);

	/**
	* Look if the subprofile is well-grouped.
	* @return bool.
	*/
	bool IsWellGrouped(void) const {return(WellGrouped);}

	/**
	* Set if the subprofile is correctly grouped or not.
	* @param b               Correctly grouped?
	*/
	void SetWellGrouped(bool b);

	/**
	* Look if the subprofile is a new one.
	* @return bool.
	*/
	bool IsNewSubProfile(void) const {return(NewSubProfile);}

	/**
	* Set if the subprofile is a new one or not.
	* @param b               New one?
	*/
	void SetNewSubProfile(bool b);

	/**
	*  Destructor.
	*/
	~GWeightInfosHistory(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
