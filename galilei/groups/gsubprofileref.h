/*

	GALILEI Research Project

	GSubProfileRef.h

	Reference of a SubProfile - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GSubProfileRefH
#define GSubProfileRefH


//-----------------------------------------------------------------------------
// include files for R
#include <rtimedate/rdate.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GProfile;
class GSubProfile;
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GSubProfileRef class provides a representation for a reference to a
*subprofile attached to a given group. It is composed by a GSubProfile pointer
* and a date when the subprofile was attached to its group.
* @author Pascal Francq
* @short SubProfile Ref.
*/
class GSubProfileRef
{
	/**
	* SubProfile.
	*/
	GSubProfile* SubProfile;

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
	* Constructor.
	* @param sub            SubProfile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileRef(GSubProfile* sub,GGroup* grp,const char* a);

	/**
	* Constructor.
	* @param sub            SubProfile.
	*/
	GSubProfileRef(GSubProfile* sub);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GSubProfileRef& gp) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GSubProfileRef* gp) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GSubProfile* s) const;

	/**
	* Get the identificator of the subprofile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const;

	/**
	* Get the subprofile.
	* @return Pointer to GALILEI::GSubProfile.
	*/
	GSubProfile* GetSubProfile(void) const {return(SubProfile);}

	/**
	* Get the profile.
	* @return Pointer to GALILEI::GProfile.
	*/
	GProfile* GetProfile(void) const;

	/**
	* Get the group.
	* @return Pointer to GALILEI::GGroup.
	*/
	GGroup* GetGroup(void) const {return(Group);}

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
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
