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
#include <filters/gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GMIMEFilter class provides a representation of a file extension
* representing a specific filter.
* @author Pascal Francq
*/
class GMIMEFilter
{	
protected:
	
	/**
	* File extension.
	*/
	RString Ext;

	/**
	* Filter corresponding.
	*/
	GFilter* Filter;

public:

	/**
	* Construct the mime.
	* @param ext            Extension.
	* @param f              Filter corresponding.
	*/
	GMIMEFilter(const RString& ext,GFilter* f);

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
	int Compare(const RString& ext) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const char* ext) const;

	/**
	* Destructor of the filter.
	*/
	virtual ~GMIMEFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
