/*

	GALILEI Research Project

	GSubProfileDescVector.h

	Sub-Profile Description using a vector - Header.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GSubProfileDescVectorH
#define GSubProfileDescVectorH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gsubprofiledesc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSubProfile;
class GGroup;
class GIWordsWeights;


//-----------------------------------------------------------------------------
/**
* The GSubProfileDescVector provides a representation for a sub-profile
* described as a vector corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Sub-Profile Vector Description.
*/
class GSubProfileDescVector : public GSubProfileDesc
{
	/**
	* List representing the sub-profile.
	*/
	GIWordsWeights* Vector;

public:

	/**
	* Constructor of the subprofile.
	* @param sub            Sub-Profile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileDescVector(GSubProfile* sub,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Compute similarity between SubProfiles.
	*/
	virtual double Similarity(const GSubProfileDesc* desc) const;

	/**
	*Destructor.
	*/
	~GSubProfileDescVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

