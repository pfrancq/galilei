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
GALILEI::GMIMEFilter::GMIMEFilter(const RString& ext,GFilter* f)
	: Ext(ext), Filter(f)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter* f) const
{
	return(Ext.Compare(f->Ext));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter& f) const
{
	return(Ext.Compare(f.Ext));
}
//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const RString& ext) const
{
	return(Ext.Compare(ext));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const char* ext) const
{
	return(Ext.Compare(ext));
}


//-----------------------------------------------------------------------------
GALILEI::GMIMEFilter::~GMIMEFilter(void)
{
}
