/*

	GALILEI Research Project

	GSubProfile.h

	Sub-Profile - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GSubProfileH
#define GSubProfileH



//-----------------------------------------------------------------------------
// include files for R Project
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
class GIWordList;
class GProfDoc;
class GProfile;
class GSubProfileRef;
class GGroup;


//-----------------------------------------------------------------------------
/**
* This class represents a sub-profile for a specific language. The sub-profiles
* are ordered by language.
* @author Pascal Francq.
* @short Sub-Profile.
*/
class GSubProfile
{
	/**
	* Identifier of the subprofile.
	*/
	unsigned Id;

	/**
	* Owner profile of the subprofile.
	*/
	GProfile* Profile;

	/**
	* Language of the subprofile.
	*/
	GLang* Lang;

	/**
	* Corresponding Group.
	*/
	GGroup* Group;

	/**
	* Date of attachment to the group.
	*/
	RTimeDate::RDate Attached;

	/**
	* List OK.
	*/
	GIWordList* OK;

	/**
	* List KO.
	*/
	GIWordList* KO;

	/**
	* List Common.
	*/
	GIWordList* Common;

public:

	/**
	* Constructor of the subprofile.
	* @param prod           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfile& subprofile) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfile* subprofile) const;
	
	/**
	* Get the identifier of the subprofile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the corresponding profile.
	* @return Pointer to the profile.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

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
	* Get the list of word for OK.
	* @return Pointer to the list.
	*/
	GIWordList* GetOK(void) const {return(OK);}

	/**
	* Get the list of word for KO.
	* @return Pointer to the list.
	*/
	GIWordList* GetKO(void) const {return(KO);}

	/**
	* Get the list of word for Common.
	* @return Pointer to the list.
	*/
	GIWordList* GetCommon(void) const {return(Common);}

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
	bool IsDefined(void) const;

	/**
	* Compute similarity between SubProfiles.
	*/
	double Similarity(const GSubProfile *sub) const;

	/**
	*Destructor
	*/
	~GSubProfile(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

