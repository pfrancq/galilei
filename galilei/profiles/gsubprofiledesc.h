/*

	GALILEI Research Project

	GSubProfileDesc.h

	Generic Sub-Profile Description - Header.

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
#ifndef GSubProfileDescH
#define GSubProfileDescH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GLang;
class GSubProfile;
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GSubProfileDesc class provides a representation for description of a
* sub-profile.
* @author Pascal Francq.
* @short Genertic Sub-Profile Description.
*/
class GSubProfileDesc
{
protected:

	/**
	* Owner of the description.
	*/
	GSubProfile* Owner;

	/**
	* Corresponding Group.
	*/
	GGroup* Group;

	/**
	* Date of attachment to the group.
	*/
	RTimeDate::RDate Attached;

public:

	/**
	* Constructor of the subprofile.
	* @param sub            Owner.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileDesc(GSubProfile* sub,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfileDesc& desc) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfileDesc* desc) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const tSubProfileDesc t) const;

	/**
	* Get the type of the description.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const=0;

	/**
	* Get the identifier of the subprofile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const;

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const;

	/**
	* Get the corresponding sub-profile.
	* @return Pointer to the sub-profile.
	*/
	GSubProfile* GetSubProfile(void) const {return(Owner);}

	/**
	* Get the corresponding group.
	* @returns Pointer to GGroup.
	*/
	GGroup* GetGroup(void) const {return(Group);}

	/**
	* Set the group for the subprofile.
	* @params grp           Group where to attached.
	*/
	void SetGroup(GGroup* grp);

	/**
	* Get the date of the last attachment.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetAttached(void) const {return(&Attached);}

	/**
	* See if the subprofiles was updated until the last attachment to a group.
	* @returns Boolean.
	*/
	bool IsUpdated(void) const;

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Compute similarity between SubProfiles.
	*/
	virtual double Similarity(const GSubProfileDesc* desc) const=0;

	/**
	*Destructor
	*/
	virtual ~GSubProfileDesc(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfileDescCursor class provides a way to go trough a set of
* sub-profile descriptions.
* @short SubProfile Descriptions Cursor
*/
CLASSCURSOR(GSubProfileDescCursor,GSubProfileDesc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

