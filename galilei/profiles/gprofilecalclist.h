/*

	GALILEI Research Project

	GProfileCalcList.h

	"Lists" Profile Computing Method - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GProfileCalcListH
#define GProfileCalcListH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gprofilecalc.h>
#include <ginfos/giwordcalcs.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// class GUsers
//-----------------------------------------------------------------------------
/**
* The GProfileCalcList class provides a representation for a method to compute
* a specific profile, i.e. its sub-profiles by using the "lists" method base on
* frequential appearence.
* @author Pascal Francq
* @short "Lists" Profile' Computing Method.
*/
class GProfileCalcList : public GProfileCalc
{
	/**
	* List of words' frequences in the "OK" and "N" documents for the different
	* languages.
	*/
	RStd::RContainer<GIWordCalcs,unsigned int,true,true>* OK;

	/**
	* List of words' frequences in the "KO" documents for the different
	* languages.
	*/
	RStd::RContainer<GIWordCalcs,unsigned int,true,true>* KO;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcList(GSession* session) throw(bad_alloc);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GProfile* profile);

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcList(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

