/*

	GALILEI Research Project

	GMIMEFilter.h

	Represents a couple file extention / filter - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gmimefilter.h>



//-----------------------------------------------------------------------------
//
// class GMIMEFilter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GMIMEFilter::GMIMEFilter(GMIMEType* t,GFilter* f)
	: Type(t), Filter(f)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter* f) const
{
	return(Type->Compare(f->Type));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter& f) const
{
	return(Type->Compare(f.Type));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const RString& t) const
{
	return(Type->Compare(t));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const char* t) const
{
	return(Type->Compare(t));
}


//-----------------------------------------------------------------------------
GALILEI::GMIMEFilter::~GMIMEFilter(void)
{
}
