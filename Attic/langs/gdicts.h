/*

	GALILEI Research Project

	GDicts.h

	List of dictionnaries - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDictsH
#define GDictsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/gdict.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDicts class provides a representation for a list of dictionnaries. These
* can be either stoplists or "real" dictionnaries.
* @author Pascal Francq
* @short List of dictionnaries.
*/
class GDicts : public RStd::RContainer<GDict,unsigned,true,true>
{
public:

	/**
	* Constructor.
	* @param nb             Number of dictinnaries contained.
	*/
	GDicts(unsigned nb) throw(bad_alloc);

	/**
	* Clear all the dictionnaries.
	*/
	void Clear(void);

	/**
	* Destrcutor.
	*/
	virtual ~GDicts(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
