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
// include files for GALILEI
#include <glangs/glang.h>
#include <ginfos/giwordlist.h>
#include <gprofiles/gprofdoc.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class GProfile;


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
	* Owner profile of the subprofile.
	*/
	GProfile* Owner;

	/**
	* Identifier of the subprofile.
	*/
	unsigned Id;

	/**
	* Language of the subprofile.
	*/
	GLang* Lang;

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
	* Default Constructor.
	*/
	GSubProfile(void) throw(bad_alloc);

	/**
	* Constructor of the subprofile.
	* @param owner          owner profile
	* @param id             identifier
	* @param lang           Language of the subprofile
	*/
	GSubProfile(GProfile* owner,unsigned int id,GLang* lang) throw(bad_alloc);

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
	GProfile* GetProfile(void) const {return(Owner);}

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

