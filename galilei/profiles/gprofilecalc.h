/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile' Computing Method - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GProfileCalcH
#define GProfileCalcH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gprofile.h>
#include <gsessions/gsession.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile, i.e. its sub-profiles.
* @author Pascal Francq
* @short Generic Profile' Computing Method.
*/
class GProfileCalc
{
protected:

	/**
	* Session.
	*/
	GSession *Session;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalc(GSession* session) throw(bad_alloc);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GProfile* profile)=0;

	/**
	* Destructor.
	*/
	virtual ~GProfileCalc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

