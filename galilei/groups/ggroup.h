/*

	GALILEI Research Project

	ggroup.h

	Basic Information - Implementation.

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
#ifndef GGroupH
#define GGroupH


//-----------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <ggroups/ggroups.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------
// forward classes declaration
class GGroups;
class GSubProfile;


//-----------------------------------------------------------------------------
/**
* This class represent a group of sub-profiles. In fact, it is implemented as a
* container of GSubProfile.
* @author Pascal Francq
* @short Group class.
*/
class GGroup : public RStd::RContainer<GSubProfile,unsigned int,false,true>
{
public:
	
	/**
	* identifier
	*/
	unsigned int Id;
	
	GGroups *Owner;	

	/**
	* Construct a group.
	*/
	GGroup(GGroups *owner) throw(bad_alloc);

	/**
	* Construct a group with a specific identificator.
	* @param id		The identificator.
	*/
	GGroup(GGroups *owner,unsigned int id) throw(bad_alloc);

	/**
	* comparaison function.
	*/
	int Compare(const unsigned int &id);

	/**
	* comparaison function.
	*/
	int Compare(const GGroup &group);
	
	/**
	* comparaison function.
	*/
	int Compare(const GGroup *group);

	/**
	* virtual load function.
	*/
	//virtual void Load(void)=0;
	
    /**
	* virtual save function.
	*/
	//virtual void Save(void)=0;
	
	/**
	* can the group attache the subprofile ?
	* @param sub            subprofile to attach.
	*/
	bool CanAttach(GSubProfile *sub);

};


}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif

