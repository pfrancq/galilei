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
// include files for GALILEI
#include <gprofiles/gsubprofile.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


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
	* Date of attachment to the group.
	*/
	RTimeDate::RDate Attached;

public:

	/**
	* Constructor.
	* @param sub            SubProfile.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileRef(GSubProfile* sub,const char* a);

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
	unsigned int GetId(void) const {return(SubProfile->GetId());}

	/**
	* Get the the subprofile.
	* @return Pointer to GALILEI::SubProfile.
	*/
	GSubProfile* GetSubProfile(void) const {return(SubProfile);}

	/**
	* Get the the profile.
	* @return Pointer to GALILEI::Profile.
	*/
	GProfile* GetProfile(void) const {return(SubProfile->GetProfile());}

	/**
	* Get the date of the last attachment.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetAttached(void) const {return(&Attached);}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
