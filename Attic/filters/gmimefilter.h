/*

	GALILEI Research Project

	GMIMEFilter.h

	Represents a couple file extention / filter - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GMIMEFilterH
#define GMIMEFilterH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gmimetype.h>
#include <filters/gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GMIMEFilter class provides a representation of a link between a MIME-Type
* and a filter.
* @author Pascal Francq
*/
class GMIMEFilter
{	
protected:
	
	/**
	* File extension.
	*/
	GMIMEType* Type;

	/**
	* Filter corresponding.
	*/
	GFilter* Filter;

public:

	/**
	* Construct the mime.
	* @param t              MIME-Type.
	* @param f              Filter corresponding.
	*/
	GMIMEFilter(GMIMEType* t,GFilter* f);

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const GMIMEFilter* f) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const GMIMEFilter& f) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const RString& t) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const char* t) const;

	/**
	* Destructor of the filter.
	*/
	virtual ~GMIMEFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
