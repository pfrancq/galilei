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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GFilter;


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
	RStd::RString Type;

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
	GMIMEFilter(const char* t,GFilter* f);

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
	int Compare(const RStd::RString& t) const;

	/**
	* Compare function used by RContainer.
	*/
	int Compare(const char* t) const;

	/**
	* Get the MIME type.
	* @returns Pointer to a MIME type.
	*/
	const char* GetName(void) const {return(Type());}

	/**
	* Get a filter able to manipulate this type of files.
	* @returns Pointer to a Filter.
	*/
	GFilter* GetFilter(void) const {return(Filter);}

	/**
	* Destructor of the filter.
	*/
	virtual ~GMIMEFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
