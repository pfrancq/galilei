/*

	GALILEI Research Project

	GSubProfileDesc.h

	Generic Sub-Profile Description - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
public:
	/**
	* Type of the description.
	*/
	enum DescType {Vector /** Vector Representation.*/};

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

