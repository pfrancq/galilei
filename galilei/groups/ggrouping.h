/*

	GALILEI Research Project

	GGrouping.h

	Generic Grouping Method - Header.

	(C) 2001 by P. Francq.

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
#ifndef GGroupingH
#define GGroupingH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGrouping provides a representation for a generic method to group some
* subprofiles.
* @author Pascal Francq
* @short Generic Grouping.
*/
class GGrouping
{
	/**
	* Name of the grouping.
	*/
	RStd::RString GroupingName;

protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Pointer to groups to form.
	*/
	GGroups* Groups;

	/**
	* SubProfiles that where changed and must be grouped again.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,false> SubProfiles;

	/**
	* Type of the description used to group.
	*/
	tSubProfileDesc SubProfileDesc;

public:

	/**
	* Constructor.
	* @param n              Name of the grouping.
	* @param s              Session.
	*/
	GGrouping(const char* n,GSession* s) throw(bad_alloc);

	/**
	* Get the name of the grouping method.
	* @returns Pointer to a C string.
	*/
	const char* GetGroupingName(void) const {return(GroupingName());}

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void) {return("");}

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*) {}

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const GGrouping* grp);

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const GGrouping& grp);

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const char* name);

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

protected:

	/**
	* Make the grouping for a specific Language. Groups is a pointer to the
	* groups to form and SubProfiles contains all the subprofiles for a given
	* language. This variables must be set before calling this function. This
	* is done by the Grouping method.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp)=0;

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified       Recompute only modified elements or all.
	*/
	void Grouping(GSlot* rec=0,bool modified=true);

	/**
	* Destructor.
	*/
	virtual ~GGrouping(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupingCursor class provides a way to go trough a set of grouping
* method for the profiles.
* @short Profiles Grouping Methods Cursor
*/
CLASSCURSOR(GGroupingCursor,GGrouping,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
