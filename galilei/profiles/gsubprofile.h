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
// include files for ANSI C/C++
#include <time.h>


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
* This class represents a sub-profile for a specific language.
* @author Pascal Francq.
* @short Sub-Profile.
*/
class GSubProfile
{
public:

	/**
	* Owner profile of the subprofile.
	*/
	GProfile *Owner;

	/**
	* Identifier of the subprofile.
	*/
	unsigned Id;

	/**
	* Language of the subprofile.
	*/
	GLang *Lang;

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
	
	/**
	* Must Attach to group ? (T/F).
	*/
	bool bToAttach;

	time_t Attached;

	/**
	* Default Constructor.
	*/
	GSubProfile(void) throw(bad_alloc);


	/** Constructor of the subprofile.
	* @param owner          owner profile
	* @param id             identifier
	* @param lang           Language of the subprofile
	*/
	GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc);

	/** boolean functions for calculation of OK, KO and Common lists.
	*/
	bool NeedOK(unsigned int NbOK);
	bool NeedKO(unsigned int NbKO);

	/**
	* Comparaison functions
	*/
	int Compare(const unsigned int id);
	int Compare(const GLang* lang);
	int Compare(const GSubProfile &subprofile);
	int Compare(const GSubProfile *subprofile);	

	/**
	* Analyse of the subprofile
	*/
	void Analyse(GProfDoc *profdoc);
	void Analyse(void);

	/**
	* Boolean function for calculation of similarity between SubProfiles.
	*/
	bool Similar(GSubProfile *sub);

	/**
	*Destructor
	*/
	~GSubProfile(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

